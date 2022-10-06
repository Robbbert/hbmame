// license:CC BY-NC-ND 3.0 (copying this code into another project is not permitted)
// copyright-holders:Robbbert
/***************************************************************************************
Some sounds are missing
- Wind
- Screech
- Player Shot
***************************************************************************************/
#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/eepromser.h"
#include "machine/timer.h"
#include "machine/watchdog.h"
#include "sound/discrete.h"
#include "sound/sn76477.h"
#include "screen.h"
#include "speaker.h"
#include "spcenctr.lh"

#define MW8080BW_MASTER_CLOCK             (19968000.0)
#define MW8080BW_CPU_CLOCK                (MW8080BW_MASTER_CLOCK / 10)
#define MW8080BW_PIXEL_CLOCK              (MW8080BW_MASTER_CLOCK / 4)
#define MW8080BW_HTOTAL                   (0x140)
#define MW8080BW_HBEND                    (0x000)
#define MW8080BW_HBSTART                  (0x100)
#define MW8080BW_VTOTAL                   (0x106)
#define MW8080BW_VBEND                    (0x000)
#define MW8080BW_VBSTART                  (0x0e0)
#define MW8080BW_VCOUNTER_START_NO_VBLANK (0x020)
#define MW8080BW_VCOUNTER_START_VBLANK    (0x0da)
#define MW8080BW_INT_TRIGGER_COUNT_1      (0x080)
#define MW8080BW_INT_TRIGGER_VBLANK_1     (0)
#define MW8080BW_INT_TRIGGER_COUNT_2      MW8080BW_VCOUNTER_START_VBLANK
#define MW8080BW_INT_TRIGGER_VBLANK_2     (1)
#define MW8080BW_60HZ                     (MW8080BW_PIXEL_CLOCK / MW8080BW_HTOTAL / MW8080BW_VTOTAL)

// +4 is added to HBSTART because the hardware displays that many pixels after setting HBLANK
#define MW8080BW_HPIXCOUNT                (MW8080BW_HBSTART + 4)

class spcenctr_audio_device : public device_t
{
public:
	spcenctr_audio_device(machine_config const &mconfig, char const *tag, device_t *owner, u32 clock = 0);

	void p1_w(u8 data);
	void p2_w(u8 data);
	void p3_w(u8 data);

protected:
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;

private:
	TIMER_CALLBACK_MEMBER(strobe_callback);

	required_device<sn76477_device> m_sn;
	required_device<discrete_sound_device> m_discrete;
	output_finder<> m_lamp;
	output_finder<> m_strobe;
	emu_timer *m_strobe_timer = nullptr;
	u8 m_strobe_enable = 0;
};

class spcenctr_state : public driver_device
{
public:
	spcenctr_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this,"maincpu")
		, m_watchdog(*this, "watchdog")
		, m_main_ram(*this, "main_ram")
		, m_screen(*this, "screen")
		, m_soundboard(*this, "soundboard")
	{ }

	void spcenctr(machine_config &config);

	IRQ_CALLBACK_MEMBER(interrupt_vector);

private:
	virtual void machine_start() override;
	virtual void machine_reset() override;

	DECLARE_WRITE_LINE_MEMBER(int_enable_w);

	// device/memory pointers
	required_device<cpu_device> m_maincpu;
	required_device<watchdog_timer_device> m_watchdog;
	required_shared_ptr<uint8_t> m_main_ram;
	required_device<screen_device> m_screen;
	required_device<spcenctr_audio_device> m_soundboard;

	void main_map(address_map &map);
	void io_map(address_map &map);

	attotime m_interrupt_time;
	emu_timer   *m_interrupt_timer = nullptr;

	bool m_int_enable = 0;

	TIMER_CALLBACK_MEMBER(interrupt_trigger);
	uint8_t vpos_to_vysnc_chain_counter(int vpos);
	int vysnc_chain_counter_to_vpos(uint8_t counter, int vblank);

	void io_w(offs_t offset, u8 data);

	u32 screen_update(screen_device &screen, bitmap_rgb32 &bitmap, rectangle const &cliprect);

	u8 m_trench_width = 0;
	u8 m_trench_center = 0;
	u8 m_trench_slope[16];  // 16x4 bit RAM
	u8 m_bright_control = 0;
	u8 m_brightness = 0;
};


/*************************************
 *
 *  Interrupt generation
 *
 *************************************/

uint8_t spcenctr_state::vpos_to_vysnc_chain_counter( int vpos )
{
	/* convert from a vertical position to the actual values on the vertical sync counters */
	uint8_t counter;
	int vblank = (vpos >= MW8080BW_VBSTART);

	if (vblank)
		counter = vpos - MW8080BW_VBSTART + MW8080BW_VCOUNTER_START_VBLANK;
	else
		counter = vpos + MW8080BW_VCOUNTER_START_NO_VBLANK;

	return counter;
}

int spcenctr_state::vysnc_chain_counter_to_vpos( uint8_t counter, int vblank )
{
	/* convert from the vertical sync counters to an actual vertical position */
	int vpos;

	if (vblank)
		vpos = counter - MW8080BW_VCOUNTER_START_VBLANK + MW8080BW_VBSTART;
	else
		vpos = counter - MW8080BW_VCOUNTER_START_NO_VBLANK;

	return vpos;
}

TIMER_CALLBACK_MEMBER(spcenctr_state::interrupt_trigger)
{
	int const vpos = m_screen->vpos();
	uint8_t const counter = vpos_to_vysnc_chain_counter(vpos);

	if (m_int_enable)
	{
		m_maincpu->set_input_line(0, ASSERT_LINE);
		m_interrupt_time = machine().time();
	}
	else
		m_maincpu->set_input_line(0, CLEAR_LINE);

	// set up for next interrupt
	uint8_t next_counter;
	int next_vblank;
	if (counter == MW8080BW_INT_TRIGGER_COUNT_1)
	{
		next_counter = MW8080BW_INT_TRIGGER_COUNT_2;
		next_vblank = MW8080BW_INT_TRIGGER_VBLANK_2;
	}
	else
	{
		next_counter = MW8080BW_INT_TRIGGER_COUNT_1;
		next_vblank = MW8080BW_INT_TRIGGER_VBLANK_1;
	}

	int const next_vpos = vysnc_chain_counter_to_vpos(next_counter, next_vblank);
	m_interrupt_timer->adjust(m_screen->time_until_pos(next_vpos));
}

WRITE_LINE_MEMBER(spcenctr_state::int_enable_w)
{
	m_int_enable = state;
}

IRQ_CALLBACK_MEMBER(spcenctr_state::interrupt_vector)
{
	int vpos = m_screen->vpos();
	// MAME scheduling quirks cause this to happen more often than you might think, in fact far too often
	if (machine().time() < m_interrupt_time)
		vpos++;
	uint8_t counter = vpos_to_vysnc_chain_counter(vpos);
	uint8_t vector = 0xc7 | ((counter & 0x40) >> 2) | ((~counter & 0x40) >> 3);

	m_maincpu->set_input_line(0, CLEAR_LINE);
	return vector;
}

/*************************************
 *
 *  Machine setup
 *
 *************************************/

void spcenctr_state::machine_reset()
{
	int vpos = vysnc_chain_counter_to_vpos(MW8080BW_INT_TRIGGER_COUNT_1, MW8080BW_INT_TRIGGER_VBLANK_1);
	m_interrupt_timer->adjust(m_screen->time_until_pos(vpos));

	m_interrupt_time = attotime::zero;
}

//************************************************** VIDEO *************************************************
/*************************************
 *
 *  Space Encounters
 *
 *************************************/

/* video signals mixed via R2 open collector nand gates and 'brite' RC circuit

    * when /BRITE lo, cap C23 discharge rapidly, disabling Q2, overpowering all other video signals
    * when /BRITE hi, cap C23 charge through 10k res, brite voltage decrease to 0 over approx 0.4 sec

    * inverted video data is fed into R2 nand gates:
        * when /VIDEO lo, all gates open for max brightness
            * max V = (5 - 0.7) * 470 / (470 + 100) = 3.5 V
        * when /video hi, pin 5 always gnd, max = 3 V, min = 1 V
        * (guess) pin 11 state controls trench color
        * (guess) pin 3 low for trench side
        * (guess) pin 8 low for trench floor
        * thus, trench side: 1.4 or 2.2 V
        * trench floor: 1.3 or 2.0 V
        * trech top: 1.8 or 3 V
        * scaled to 3.2 V = 255, 1.2 V = 0 (arbitrary values chosen to match video)
*/

#define SPCENCTR_TOP_TRENCH_DARK_RGB32_PEN       rgb_t(0x4d, 0x4d, 0x4d)
#define SPCENCTR_TOP_TRENCH_LIGHT_RGB32_PEN      rgb_t(0xe6, 0xe6, 0xe6)
#define SPCENCTR_SIDE_TRENCH_DARK_RGB32_PEN      rgb_t(0x1a, 0x1a, 0x1a)
#define SPCENCTR_SIDE_TRENCH_LIGHT_RGB32_PEN     rgb_t(0x80, 0x80, 0x80)
#define SPCENCTR_BOTTOM_TRENCH_DARK_RGB32_PEN    rgb_t(0x0d, 0x0d, 0x0d)
#define SPCENCTR_BOTTOM_TRENCH_LIGHT_RGB32_PEN   rgb_t(0x66, 0x66, 0x66)
#define SPCENCTR_BRIGHTNESS_DECAY                10


uint32_t spcenctr_state::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	uint8_t line_buf[256]; /* 256x1 bit RAM */

	uint8_t x = 0;
	uint8_t y = MW8080BW_VCOUNTER_START_NO_VBLANK;
	uint8_t video_data = 0;
	uint8_t draw_line = 0;
	uint8_t draw_trench = 0;
	uint8_t draw_floor = 0;
	uint8_t width = m_trench_width;
	uint8_t floor_width = width;
	uint8_t center = m_trench_center;

	memset(line_buf, 0, 256);

	if (m_bright_control)
		m_brightness = 255;
	else if (m_brightness > SPCENCTR_BRIGHTNESS_DECAY)
		m_brightness -= SPCENCTR_BRIGHTNESS_DECAY;
	else
		m_brightness = 0;

	while (1)
	{
		// plot the current pixel
		uint8_t bit = video_data & 0x01;
		pen_t pen = bit ? rgb_t::white() : rgb_t::black();

		if (!(width & 0x80) && draw_trench) // possibly draw trench in the background, top of trench first
		{
			line_buf[x] = draw_line;

			if (!bit)
				pen = draw_line ? SPCENCTR_TOP_TRENCH_LIGHT_RGB32_PEN : SPCENCTR_TOP_TRENCH_DARK_RGB32_PEN;
		}
		else if (!(floor_width & 0x80) && (draw_trench || draw_floor)) // sides of trench?
		{
			if (!bit)
				pen = line_buf[x] ? SPCENCTR_SIDE_TRENCH_LIGHT_RGB32_PEN : SPCENCTR_SIDE_TRENCH_DARK_RGB32_PEN;
		}
		else if (draw_floor) // bottom of trench?
		{
			line_buf[x] = line_buf[x - 1];

			if (!bit)
				pen = line_buf[x] ? SPCENCTR_BOTTOM_TRENCH_LIGHT_RGB32_PEN : SPCENCTR_BOTTOM_TRENCH_DARK_RGB32_PEN;
		}

		if (m_brightness > (pen & 0xff))
			pen = rgb_t(m_brightness, m_brightness, m_brightness);

		bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, x) = pen;

		center = center + 1;
		width = width + ((center & 0x80) ? -1 : 1);
		floor_width = floor_width + ((center & 0x80) ? -1 : 1);

		// next pixel
		video_data = video_data >> 1;
		x = x + 1;

		if (x == 0) // end of line?
		{
			// yes, flush out the shift register
			for (int i = 0; i < 4; i++)
			{
				if (video_data & 0x01)
					pen = rgb_t::white();
				else if(m_brightness)
					pen = rgb_t(m_brightness, m_brightness, m_brightness);
				else
					pen = rgb_t::black();

				bitmap.pix(y - MW8080BW_VCOUNTER_START_NO_VBLANK, 256 + i) = pen;

				video_data = video_data >> 1;
			}

			/* update the trench control for the next line */
			offs_t const offs = ((offs_t)y << 5) | 0x1f;
			uint8_t const trench_control = m_main_ram[offs];

			if (trench_control & 0x40)
				draw_trench = 1;

			if (trench_control & 0x20)
				draw_trench = 0;

			if (trench_control & 0x10)
				draw_floor = 1;

			if (trench_control & 0x08)
				draw_floor = 0;

			draw_line = (trench_control & 0x80) >> 7;

			// add the lower 2 bits stored in the slope array to width
			if (draw_trench)
				width = width + (m_trench_slope[y & 0x0f] & 0x03);

			// add the higher 2 bits stored in the slope array to floor width
			if (draw_floor)
				floor_width = floor_width + ((m_trench_slope[y & 0x0f] & 0x0c) >> 2);

			// next row, video_data is now 0, so the next line will start with 4 blank pixels
			y = y + 1;

			// end of screen?
			if (y == 0)
				break;
		}
		else if ((x & 0x07) == 0x04) // the video RAM is read at every 8 pixels starting with pixel 4
		{
			offs_t const offs = ((offs_t)y << 5) | (x >> 3);
			video_data = m_main_ram[offs];
		}
	}

	return 0;
}

//********************************************* SOUND ************************************************************
DEFINE_DEVICE_TYPE(SPCENCTR_AUDIO, spcenctr_audio_device, "spcenctr_audio", "Midway Space Encounters Audio")

/*************************************
 *
 *  Space Encounters
 *
 *  Discrete sound emulation:
 *  Apr 2007, D.R.
 *************************************/

/*************************************
 *
 *  Implementation of the common
 *  noise circuits
 *
 *************************************/

discrete_lfsr_desc const midway_lfsr =
{
	DISC_CLK_IS_FREQ,
	17,                 // bit length
						// the RC network fed into pin 4 has the effect of presetting all bits high at power up
	0x1ffff,            // reset value
	4,                  // use bit 4 as XOR input 0
	16,                 // use bit 16 as XOR input 1
	DISC_LFSR_XOR,      // feedback stage1 is XOR
	DISC_LFSR_OR,       // feedback stage2 is just stage 1 output OR with external feed
	DISC_LFSR_REPLACE,  // feedback stage3 replaces the shifted register contents
	0x000001,           // everything is shifted into the first bit only
	0,                  // output is not inverted
	12                  // output bit
};

// nodes - inputs
#define SPCENCTR_ENEMY_SHIP_SHOT_EN       NODE_01
#define SPCENCTR_PLAYER_SHOT_EN           NODE_02
#define SPCENCTR_SCREECH_EN               NODE_03
#define SPCENCTR_CRASH_EN                 NODE_04
#define SPCENCTR_EXPLOSION_EN             NODE_05
#define SPCENCTR_BONUS_EN                 NODE_06
#define SPCENCTR_WIND_DATA                NODE_07

// nodes - sounds
#define SPCENCTR_NOISE                    NODE_10
#define SPCENCTR_ENEMY_SHIP_SHOT_SND      NODE_11
#define SPCENCTR_PLAYER_SHOT_SND          NODE_12
#define SPCENCTR_SCREECH_SND              NODE_13
#define SPCENCTR_CRASH_SND                NODE_14
#define SPCENCTR_EXPLOSION_SND            NODE_15
#define SPCENCTR_BONUS_SND                NODE_16
#define SPCENCTR_WIND_SND                 NODE_17

static discrete_op_amp_info const spcenctr_enemy_ship_shot_op_amp_E1 =
{
	DISC_OP_AMP_IS_NORTON,
	0,                      // no r1
	RES_K(510),             // R100
	RES_M(2.2),             // R101
	RES_M(2.2),             // R102
	CAP_U(0.1),             // C100
	0,                      // vN
	12                      // vP
};

static discrete_op_amp_osc_info const spcenctr_enemy_ship_shot_op_amp_osc =
{
	DISC_OP_AMP_OSCILLATOR_VCO_3 | DISC_OP_AMP_IS_NORTON | DISC_OP_AMP_OSCILLATOR_OUT_SQW,
	RES_K(560),             // R103
	RES_K(7.5),             // R118
	RES_K(22),              // R104
	RES_K(47),              // R106
	RES_K(100),             // R105
	0,                      // no r6
	0,                      // no r7
	0,                      // no r8
	CAP_U(0.0022),          // C101
	12,                     // vP
};


static discrete_op_amp_info const spcenctr_enemy_ship_shot_op_amp_D1 =
{
	DISC_OP_AMP_IS_NORTON,
	RES_K(100),             // R107
	RES_K(100),             // R109
	RES_M(2.7),             // R108
	RES_K(100),             // R110
	0,                      // no c
	0,                      // vN
	12                      // vP
};

static discrete_op_amp_filt_info const spcenctr_enemy_ship_shot_filt =
{
	RES_K(100),             // R112
	RES_K(10),              // R113
	RES_M(4.3),             // r3
	0,                      // no r4
	RES_M(2.2),             // R114
	CAP_U(0.001),           // c1
	CAP_U(0.001),           // c2
	0,                      // no c3
	0,                      // vRef
	12,                     // vP
	0                       // vN
};

static discrete_op_amp_1sht_info const spcenctr_player_shot_1sht =
{
	DISC_OP_AMP_1SHT_1 | DISC_OP_AMP_IS_NORTON,
	RES_M(4.7),             // R500
	RES_K(100),             // R502
	RES_M(1),               // R501
	RES_M(1),               // R503
	RES_M(2.2),             // R504
	CAP_U(1),               // C500
	CAP_P(470),             // C501
	0,                      // vN
	12                      // vP
};

static discrete_op_amp_info const spcenctr_player_shot_op_amp_E1 =
{
	DISC_OP_AMP_IS_NORTON,
	0,                      // no r1
	RES_K(10),              // R505
	RES_M(1.5),             // R506
	0,                      // no r4
	CAP_U(0.22),            // C502
	0,                      // vN
	12                      // vP
};

static discrete_op_amp_osc_info const spcenctr_player_shot_op_amp_osc =
{
	DISC_OP_AMP_OSCILLATOR_VCO_3 | DISC_OP_AMP_IS_NORTON | DISC_OP_AMP_OSCILLATOR_OUT_SQW,
	1.0 / (1.0 / RES_M(1) + 1.0 / RES_K(330)) + RES_M(1.5),     // R507||R509 + R508
	RES_M(1),               // R513
	RES_K(560),             // R512
	RES_M(2.7),             // R516
	RES_M(1),               // R515
	RES_M(4.7),             // R510
	RES_M(3.3),             // R511
	0,                      // no r8
	CAP_P(330),             // C504
	12,                     // vP
};

static discrete_op_amp_info const spcenctr_player_shot_op_amp_C1 =
{
	DISC_OP_AMP_IS_NORTON,
	RES_K(560),             // R517
	RES_K(470),             // R514
	RES_M(2.7),             // R518
	RES_K(560),             // R524
	0,                      // no c
	0,                      // vN
	12                      // vP
};

static discrete_op_amp_tvca_info const spcenctr_player_shot_tvca =
{
	RES_M(2.7),                         // R522
	RES_K(560),                         // R521
	0,                                  // no r3
	RES_K(560),                         // R560
	RES_K(1),                           // R42
	0,                                  // no r6
	RES_K(560),                         // R523
	0,                                  // no r8
	0,                                  // no r9
	0,                                  // no r10
	0,                                  // no r11
	CAP_U(1),                           // C506
	0,                                  // no c2
	0, 0,                               // no c3, c4
	12,                                 // v1
	0,                                  // no v2
	0,                                  // no v3
	12,                                 // vP
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f0
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f1
	DISC_OP_AMP_TRIGGER_FUNCTION_TRG0,  // f2
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f3
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f4
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE   // no f5
};

static discrete_op_amp_tvca_info const spcenctr_crash_tvca =
{
	RES_M(2.7),                         // R302
	RES_K(470),                         // R300
	0,                                  // no r3
	RES_K(470),                         // R303
	RES_K(1),                           // R56
	0,                                  // no r6
	RES_K(470),                         // R301
	0,                                  // no r8
	0,                                  // no r9
	0,                                  // no r10
	0,                                  // no r11
	CAP_U(2.2),                         // C304
	0,                                  // no c2
	0, 0,                               // no c3, c4
	5,                                  // v1
	0,                                  // no v2
	0,                                  // no v3
	12,                                 // vP
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f0
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f1
	DISC_OP_AMP_TRIGGER_FUNCTION_TRG0,  // f2
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f3
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f4
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE   // no f5
};

static discrete_op_amp_tvca_info const spcenctr_explosion_tvca =
{
	RES_M(2.7),                         // R402
	RES_K(680),                         // R400
	0,                                  // no r3
	RES_K(680),                         // R403
	RES_K(1),                           // R41
	0,                                  // no r6
	RES_K(680),                         // R401
	0,                                  // no r8
	0,                                  // no r9
	0,                                  // no r10
	0,                                  // no r11
	CAP_U(2.2),                         // C400
	0,                                  // no c2
	0, 0,                               // no c3, c4
	12,                                 // v1
	0,                                  // no v2
	0,                                  // no v3
	12,                                 // vP
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f0
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f1
	DISC_OP_AMP_TRIGGER_FUNCTION_TRG0,  // f2
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f3
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE,  // no f4
	DISC_OP_AMP_TRIGGER_FUNCTION_NONE   // no f5
};

static const discrete_555_desc spcenctr_555_bonus =
{
	DISC_555_OUT_SQW | DISC_555_OUT_DC,
	5,                      // B+ voltage of 555
	DEFAULT_555_VALUES
};

static const discrete_mixer_desc spcenctr_mixer =
{
	DISC_MIXER_IS_RESISTOR, // type
	{ RES_K(15),            // R117
		RES_K(15),          // R526
		RES_K(22),          // R211
		RES_K(3.6),         // R309
		RES_K(1.8) +  RES_K(3.6) + RES_K(4.7),  // R405 + R406 + R407
		RES_K(27),          // R715
		RES_K(27)},         // R51
	{0},                    // no rNode{}
	{ 0,
		CAP_U(0.001),       // C505
		CAP_U(0.1),         // C202
		CAP_U(1),           // C303
		0,
		0,
		CAP_U(10)},         // C16
	0,                      // no rI
	0,                      // no rF
	0,                      // no cF
	CAP_U(1),               // C900
	0,                      // vRef = ground
	1                       // gain
};

static DISCRETE_SOUND_START(spcenctr_discrete)

	/************************************************
	 * Input register mapping
	 ************************************************/
	DISCRETE_INPUTX_LOGIC(SPCENCTR_ENEMY_SHIP_SHOT_EN, 12, 0, 0)
	DISCRETE_INPUTX_LOGIC(SPCENCTR_PLAYER_SHOT_EN, 12, 0, 0)
	DISCRETE_INPUT_LOGIC (SPCENCTR_SCREECH_EN)
	DISCRETE_INPUT_LOGIC (SPCENCTR_CRASH_EN)
	DISCRETE_INPUT_LOGIC (SPCENCTR_EXPLOSION_EN)
	DISCRETE_INPUT_LOGIC (SPCENCTR_BONUS_EN)
	DISCRETE_INPUT_DATA  (SPCENCTR_WIND_DATA)

	/************************************************
	 * Noise Generator
	 ************************************************/
	// Noise clock was breadboarded and measured at 7515
	DISCRETE_LFSR_NOISE(SPCENCTR_NOISE,                 // IC A0, pin 10
			1,                                          // ENAB
			1,                                          // no RESET
			7515,                                       // CLK in Hz
			12,                                         // p-p AMPL
			0,                                          // no FEED input
			12.0/2,                                     // dc BIAS
			&midway_lfsr)

	/************************************************
	 * Enemy Ship Shot
	 ************************************************/
	DISCRETE_OP_AMP(NODE_20,                            // IC E1, pin 10
			1,                                          // ENAB
			0,                                          // no IN0
			SPCENCTR_ENEMY_SHIP_SHOT_EN,                // IN1
			&spcenctr_enemy_ship_shot_op_amp_E1)
	DISCRETE_OP_AMP_VCO1(NODE_21,                       // IC D1, pin 5
			1,                                          // ENAB
			NODE_20,                                    // VMOD1
			&spcenctr_enemy_ship_shot_op_amp_osc)
	DISCRETE_OP_AMP(NODE_22,                            // IC D1, pin 9
			1,                                          // ENAB
			NODE_21,                                    // IN0
			NODE_20,                                    // IN1
			&spcenctr_enemy_ship_shot_op_amp_D1)
	DISCRETE_OP_AMP_FILTER(NODE_23,                     // IC D1, pin 10
			1,                                          // ENAB
			NODE_22,                                    // INP0
			0,                                          // no INP1
			DISC_OP_AMP_FILTER_IS_BAND_PASS_1M | DISC_OP_AMP_IS_NORTON,
			&spcenctr_enemy_ship_shot_filt)
	DISCRETE_CRFILTER(SPCENCTR_ENEMY_SHIP_SHOT_SND,
			NODE_23,                                    // IN0
			RES_K(1.8),                                 // R116
			CAP_U(0.1) )                                // C104

	/************************************************
	 * Player Shot
	 ************************************************/
	DISCRETE_OP_AMP_ONESHOT(NODE_30,                    // IC E1, pin 4
			SPCENCTR_PLAYER_SHOT_EN,                    // TRIG
			&spcenctr_player_shot_1sht)                 // breadboarded and scoped at 325mS
	DISCRETE_OP_AMP(NODE_31,                            // IC E1, pin 5
			1,                                          // ENAB
			0,                                          // no IN0
			NODE_30,                                    // IN1
			&spcenctr_player_shot_op_amp_E1)
	// next 2 modules simulate the D502 voltage drop
	DISCRETE_ADDER2(NODE_32,
			1,                                          // ENAB
			NODE_31,                                    // IN0
			-0.5)                                       // IN1
	DISCRETE_CLAMP(NODE_33,
			NODE_32,                                    // IN0
			0,                                          // MIN
			12)                                         // MAX
	DISCRETE_CRFILTER(NODE_34,
			SPCENCTR_NOISE,                             // IN0
			RES_M(1) + RES_K(330),                      // R507, R509
			CAP_U(0.1) )                                // C503
	DISCRETE_GAIN(NODE_35,
			NODE_34,                                    // IN0
			RES_K(330)/(RES_M(1) + RES_K(330)))         // GAIN - R507 : R509
	DISCRETE_OP_AMP_VCO2(NODE_36,                       // IC C1, pin 4
			1,                                          // ENAB
			NODE_35,                                    // VMOD1
			NODE_33,                                    // VMOD2
					&spcenctr_player_shot_op_amp_osc)
	DISCRETE_OP_AMP(NODE_37,                            // IC C1, pin 9
			1,                                          // ENAB
			NODE_36,                                    // IN0
			NODE_33,                                    // IN1
			&spcenctr_player_shot_op_amp_C1)
	DISCRETE_OP_AMP_TRIG_VCA(SPCENCTR_PLAYER_SHOT_SND,  // IC C1, pin 10
			SPCENCTR_PLAYER_SHOT_EN,                    // TRG0
			0,                                          // no TRG1
			0,                                          // no TRG2
			NODE_37,                                    // IN0
			0,                                          // no IN1
			&spcenctr_player_shot_tvca)

	/************************************************
	 *Screech - unemulated
	 ************************************************/
	DISCRETE_CONSTANT(SPCENCTR_SCREECH_SND, 0)

	/************************************************
	 * Crash
	 ************************************************/
	DISCRETE_OP_AMP_TRIG_VCA(NODE_60,                   // IC C2, pin 4
			SPCENCTR_CRASH_EN,                          // TRG0
			0,                                          // no TRG1
			0,                                          // no TRG2
			SPCENCTR_NOISE,                             // IN0
			0,                                          // no IN1
			&spcenctr_crash_tvca)
	// The next 5 modules emulate the filter.
	// The DC level was breadboarded and the frequency response was SPICEd
	DISCRETE_ADDER2(NODE_61,                            // center on filter DC level
			1,                                          // ENAB
			NODE_60,                                    // IN0
			-6.8)                                       // IN1
	DISCRETE_FILTER2(NODE_62,
			1,                                          // ENAB
			NODE_61,                                    // INP0
			130,                                        // FREQ
			1.0 / 8,                                    // DAMP
			DISC_FILTER_BANDPASS)
	DISCRETE_GAIN(NODE_63,
			NODE_62,                                    // IN0
			6)                                          // GAIN
	DISCRETE_ADDER2(NODE_64,                                    // center on filter DC level
			1,                                          // ENAB
			NODE_63,                                    // IN0
			6.8)                                        // IN1
	DISCRETE_CLAMP(SPCENCTR_CRASH_SND,                          // IC C2, pin 5
			NODE_64,                                    // IN0
			0,                                          // MIN
			12.0 - OP_AMP_NORTON_VBE)                   // MAX

	/************************************************
	 * Explosion
	 ************************************************/
	DISCRETE_OP_AMP_TRIG_VCA(NODE_70,                   // IC D2, pin 10
			SPCENCTR_EXPLOSION_EN,                      // TRG0
			0,                                          // no TRG1
			0,                                          // no TRG2
			SPCENCTR_NOISE,                             // IN0
			0,                                          // no IN1
			&spcenctr_explosion_tvca)
	DISCRETE_RCFILTER(NODE_71,
			NODE_70,                                    // IN0
			RES_K(1.8),                                 // R405
			CAP_U(0.22) )                               // C401
	DISCRETE_RCFILTER(SPCENCTR_EXPLOSION_SND,
			NODE_71,                                    // IN0
			RES_K(1.8) + RES_K(3.6),                    // R405 + R406
			CAP_U(0.22) )                               // C402

	/************************************************
	 *Bonus
	 ************************************************/
	DISCRETE_555_ASTABLE(NODE_80,                       // pin 5
			// the pin 4 reset is not connected in schematic, but should be
			SPCENCTR_BONUS_EN,                          // RESET
			RES_K(1),                                   // R710
			RES_K(27),                                  // R711
			CAP_U(0.047),                               // C710
			&spcenctr_555_bonus)
	DISCRETE_555_ASTABLE(NODE_81,                       // pin 9
			SPCENCTR_BONUS_EN,                          // RESET pin 10
			RES_K(100),                                 // R713
			RES_K(47),                                  // R714
			CAP_U(1),                                   // C713
			&spcenctr_555_bonus)
	DISCRETE_LOGIC_AND3(NODE_82,                        // IC C-D, pin 6
			NODE_80,                                    // INP0
			NODE_81,                                    // INP1
			SPCENCTR_BONUS_EN)                          // INP2
	DISCRETE_GAIN(SPCENCTR_BONUS_SND,                   // adjust from logic to TTL voltage level
			NODE_82,                                    // IN0
			DEFAULT_TTL_V_LOGIC_1)                      // GAIN

	/************************************************
	 *Wind - unemulated
	 ************************************************/
	DISCRETE_CONSTANT(SPCENCTR_WIND_SND, 0)

	/************************************************
	 * Final mix
	 ************************************************/
	DISCRETE_MIXER7(NODE_91,
			1,                                          // ENAB
			SPCENCTR_ENEMY_SHIP_SHOT_SND,               // IN0
			SPCENCTR_PLAYER_SHOT_SND,                   // IN1
			SPCENCTR_SCREECH_SND,                       // IN2
			SPCENCTR_CRASH_SND,                         // IN3
			SPCENCTR_EXPLOSION_SND,                     // IN4
			SPCENCTR_BONUS_SND,                         // IN5
			SPCENCTR_WIND_SND,                          // IN6
			&spcenctr_mixer)

	DISCRETE_OUTPUT(NODE_91, 20000)
DISCRETE_SOUND_END

spcenctr_audio_device::spcenctr_audio_device(machine_config const &mconfig, char const *tag, device_t *owner, u32 clock) :
	device_t(mconfig, SPCENCTR_AUDIO, tag, owner, clock),
	m_sn(*this, "snsnd"),
	m_discrete(*this, "discrete"),
	m_lamp(*this, "LAMP"),
	m_strobe(*this, "STROBE"),
	m_strobe_timer(nullptr),
	m_strobe_enable(0U)
{
}

void spcenctr_audio_device::p1_w(u8 data)
{
	machine().sound().system_mute(!BIT(data, 0));

	// D1 is marked as 'OPTIONAL SWITCH VIDEO FOR COCKTAIL', but it is never set by the software

	m_discrete->write(SPCENCTR_CRASH_EN, BIT(data, 2));

	// D3-D7 are not connected
}

void spcenctr_audio_device::p2_w(u8 data)
{
	// set WIND SOUND FREQ(data & 0x0f)  0, if no wind

	m_discrete->write(SPCENCTR_EXPLOSION_EN, BIT(data, 4));
	m_discrete->write(SPCENCTR_PLAYER_SHOT_EN, BIT(data, 5));

	// D6 and D7 are not connected
}


void spcenctr_audio_device::p3_w(u8 data)
{
	// if (data & 0x01)  enable SCREECH (hit the sides) sound

	m_discrete->write(SPCENCTR_ENEMY_SHIP_SHOT_EN, BIT(data, 1));

	m_strobe_enable = BIT(data, 2);

	m_lamp = BIT(data, 3);

	m_discrete->write(SPCENCTR_BONUS_EN, BIT(data, 4));

	m_sn->enable_w(BIT(data, 5));

	// D6 and D7 are not connected
}

void spcenctr_audio_device::device_add_mconfig(machine_config &config)
{
	SPEAKER(config, "mono").front_center();

	SN76477(config, m_sn);
	m_sn->set_noise_params(0, 0, 0);
	m_sn->set_decay_res(0);
	m_sn->set_attack_params(0, RES_K(100));
	m_sn->set_amp_res(RES_K(56));
	m_sn->set_feedback_res(RES_K(10));
	m_sn->set_vco_params(0, CAP_U(0.047), RES_K(56));
	m_sn->set_pitch_voltage(5.0);
	m_sn->set_slf_params(CAP_U(1.0), RES_K(150));
	m_sn->set_oneshot_params(0, 0);
	m_sn->set_vco_mode(1);
	m_sn->set_mixer_params(0, 0, 0);
	m_sn->set_envelope_params(1, 0);
	m_sn->set_enable(1);
	m_sn->add_route(ALL_OUTPUTS, "mono", 0.20);

	DISCRETE(config, m_discrete, spcenctr_discrete);
	m_discrete->add_route(ALL_OUTPUTS, "mono", 0.45);
}

void spcenctr_audio_device::device_start()
{
	m_lamp.resolve();
	m_strobe.resolve();

	m_strobe_timer = timer_alloc(FUNC(spcenctr_audio_device::strobe_callback), this);

	m_strobe_enable = 0U;

	save_item(NAME(m_strobe_enable));

	strobe_callback(0);
}

TIMER_CALLBACK_MEMBER(spcenctr_audio_device::strobe_callback)
{
	constexpr double STROBE_FREQ = 9.00;    // Hz - calculated from the 555 timer
	constexpr u32 STROBE_DUTY_CYCLE = 95;   // %

	m_strobe = (param && m_strobe_enable) ? 1 : 0;
	m_strobe_timer->adjust(
			attotime::from_hz(STROBE_FREQ) * (param ? (100 - STROBE_DUTY_CYCLE) : STROBE_DUTY_CYCLE) / 100,
			param ? 0 : 1);
}


/*************************************
 *
 *  Space Encounters (PCB #645)
 *
 *************************************/

void spcenctr_state::machine_start()
{
	m_interrupt_timer = timer_alloc(FUNC(spcenctr_state::interrupt_trigger), this);

	m_trench_width = 0U;
	m_trench_center = 0U;
	std::fill(std::begin(m_trench_slope), std::end(m_trench_slope), 0U);
	m_bright_control = 0U;
	m_brightness = 0U;

	save_item(NAME(m_trench_width));
	save_item(NAME(m_trench_center));
	save_item(NAME(m_trench_slope));
	save_item(NAME(m_bright_control));
	save_item(NAME(m_brightness));
}


void spcenctr_state::io_w(offs_t offset, u8 data)
{                                           // A7 A6 A5 A4 A3 A2 A1 A0
	if ((offset & 0x07) == 0x00)
		// hex flip-flop B5
		// bit 3: /BRITE
		// bit 2: /NO_PLANET
		// bit 1: /SET_WSL
		// bit 0: COIN_COUNTER
		m_bright_control = BIT(~data, 3);   //  -  -  -  -  -  0  0  0
	else if ((offset & 0x5f) == 0x01)
		m_soundboard->p1_w(data);           //  -  0  -  0  0  0  0  1
	else if ((offset & 0x5f) == 0x09)
		m_soundboard->p2_w(data);           //  -  0  -  0  1  0  0  1
	else if ((offset & 0x5f) == 0x11)
		m_soundboard->p3_w(data);           //  -  0  -  1  0  0  0  1
	else if ((offset & 0x07) == 0x02)
		m_watchdog->watchdog_reset();       //  -  -  -  -  -  0  1  0
	else if ((offset & 0x07) == 0x03)
	{                                       //  -  -  -  -  -  0  1  1
		m_trench_slope[bitswap<4>(offset, 7, 6, 4, 3)] = data;
	}
	else if ((offset & 0x07) == 0x04)
		m_trench_center = data;             //  -  -  -  -  -  1  0  0
	else if ((offset & 0x07) == 0x07)
		m_trench_width = data;              //  -  -  -  -  -  1  1  1
	else
		logerror("%s:  Unmapped I/O port write to %02x = %02x\n", machine().describe_context(), offset, data);
}


void spcenctr_state::main_map(address_map &map)
{
	map.global_mask(0x7fff);
	map(0x0000, 0x1fff).rom().nopw();
	map(0x2000, 0x3fff).mirror(0x4000).ram().share("main_ram");
	map(0x4000, 0x5fff).rom().nopw();
}

void spcenctr_state::io_map(address_map &map)
{
	map.global_mask(0xff);
	map(0x00, 0x00).mirror(0xfc).portr("IN0");
	map(0x01, 0x01).mirror(0xfc).portr("IN1");
	map(0x02, 0x02).mirror(0xfc).portr("IN2");
	map(0x03, 0x03).mirror(0xfc).nopr();

	map(0x00, 0xff).w(FUNC(spcenctr_state::io_w)); // complicated addressing logic
}


static const ioport_value spcenctr_controller_table[] =
{
	0x3f, 0x3e, 0x3c, 0x3d, 0x39, 0x38, 0x3a, 0x3b,
	0x33, 0x32, 0x30, 0x31, 0x35, 0x34, 0x36, 0x37,
	0x27, 0x26, 0x24, 0x25, 0x21, 0x20, 0x22, 0x23,
	0x2b, 0x2a, 0x28, 0x29, 0x2d, 0x2c, 0x2e, 0x2f,
	0x0f, 0x0e, 0x0c, 0x0d, 0x09, 0x08, 0x0a, 0x0b,
	0x03, 0x02, 0x00, 0x01, 0x05, 0x04, 0x06, 0x07,
	0x17, 0x16, 0x14, 0x15, 0x11, 0x10, 0x12, 0x13,
	0x1b, 0x1a, 0x18, 0x19, 0x1d, 0x1c, 0x1e, 0x1f
};


static INPUT_PORTS_START( spcenctr )
	PORT_START("IN0")
	/* horizontal range is limited to 12 - 46 by stoppers on the control for 35 positions */
	PORT_BIT( 0x3f, 17, IPT_POSITIONAL ) PORT_POSITIONS(35) PORT_REMAP_TABLE(spcenctr_controller_table+12) PORT_SENSITIVITY(5) PORT_KEYDELTA(10) PORT_CENTERDELTA(0) PORT_REVERSE /* 6 bit horiz encoder */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 )

	PORT_START("IN1")
	/* vertical range is limited to 22 - 41 by stoppers on the control for 20 positions */
	PORT_BIT( 0x3f, 19, IPT_POSITIONAL_V ) PORT_POSITIONS(20) PORT_REMAP_TABLE(spcenctr_controller_table+22) PORT_SENSITIVITY(5) PORT_KEYDELTA(10) PORT_CENTERDELTA(0) PORT_REVERSE /* 6 bit vert encoder - pushing control in makes ship move faster */
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNUSED )  /* not connected */
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )  /* marked as COIN #2, but the software never reads it */

	PORT_START("IN2")
	PORT_DIPNAME( 0x03, 0x02, DEF_STR( Bonus_Life ) ) PORT_CONDITION("IN2", 0x30, EQUALS, 0x00) PORT_DIPLOCATION("F3:1,2")
	PORT_DIPSETTING(    0x00, "2000 4000 8000" )
	PORT_DIPSETTING(    0x01, "3000 6000 12000" )
	PORT_DIPSETTING(    0x02, "4000 8000 16000" )
	PORT_DIPSETTING(    0x03, "5000 10000 20000" )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Coinage ) ) PORT_CONDITION("IN2", 0x30, NOTEQUALS, 0x10) PORT_DIPLOCATION("F3:3,4")
	PORT_DIPSETTING(    0x04, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x0c, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 1C_3C ) )
	PORT_DIPNAME( 0x30, 0x00, "Bonus/Test Mode" ) PORT_DIPLOCATION("F3:5,6")
	PORT_DIPSETTING(    0x00, "Bonus On" )
	PORT_DIPSETTING(    0x30, "Bonus Off" )
	PORT_DIPSETTING(    0x20, "Cross Hatch" )
	PORT_DIPSETTING(    0x10, "Test Mode" )
	PORT_DIPNAME( 0xc0, 0x40, "Time" ) PORT_CONDITION("IN2", 0x30, NOTEQUALS, 0x10) PORT_DIPLOCATION("F3:7,8")
	PORT_DIPSETTING(    0x00, "45" )
	PORT_DIPSETTING(    0x40, "60" )
	PORT_DIPSETTING(    0x80, "75" )
	PORT_DIPSETTING(    0xc0, "90" )
INPUT_PORTS_END


void spcenctr_state::spcenctr(machine_config &config)
{
	/* basic machine hardware */
	i8080_cpu_device &maincpu(I8080(config, m_maincpu, MW8080BW_CPU_CLOCK));
	maincpu.set_addrmap(AS_PROGRAM, &spcenctr_state::main_map);
	maincpu.set_addrmap(AS_IO, &spcenctr_state::io_map);
	maincpu.set_irq_acknowledge_callback(FUNC(spcenctr_state::interrupt_vector));
	maincpu.out_inte_func().set(FUNC(spcenctr_state::int_enable_w));

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(MW8080BW_PIXEL_CLOCK, MW8080BW_HTOTAL, MW8080BW_HBEND, MW8080BW_HPIXCOUNT, MW8080BW_VTOTAL, MW8080BW_VBEND, MW8080BW_VBSTART);
	m_screen->set_screen_update(FUNC(spcenctr_state::screen_update));

	WATCHDOG_TIMER(config, m_watchdog).set_time(255 * attotime::from_hz(MW8080BW_60HZ));

	// audio hardware
	SPCENCTR_AUDIO(config, m_soundboard);
}


ROM_START( spcenctr )
	ROM_REGION( 0x6000, "maincpu", 0 )
	ROM_LOAD( "m645h-4m33.h1", 0x0000, 0x0800, CRC(7458b2db) SHA1(c4f41efb8a35fd8bebc75bff0111476affe2b34d) )
	ROM_LOAD( "m645g-4m32.g1", 0x0800, 0x0800, CRC(1b873788) SHA1(6cdf0d602a65c7efcf8abe149c6172b4c7ab87a1) )
	ROM_LOAD( "m645f-4m31.f1", 0x1000, 0x0800, CRC(d4319c91) SHA1(30830595c220f490fe150ad018fbf4671bb71e02) )
	ROM_LOAD( "m645e-4m30.e1", 0x1800, 0x0800, CRC(9b9a1a45) SHA1(8023a05c13e8b541f9e2fe4d389e6a2dcd4766ea) )
	ROM_LOAD( "m645d-4m29.d1", 0x4000, 0x0800, CRC(294d52ce) SHA1(0ee63413c5caf60d45ae8bef08f6c07099d30f79) )
	ROM_LOAD( "m645c-4m28.c1", 0x4800, 0x0800, CRC(ce44c923) SHA1(9d35908de3194c5fe6fc8495ae413fa722018744) )
	ROM_LOAD( "m645b-4m27.b1", 0x5000, 0x0800, CRC(098070ab) SHA1(72ae344591df0174353dc2e3d22daf5a70e2261f) )
	ROM_LOAD( "m645a-4m26.a1", 0x5800, 0x0800, CRC(7f1d1f44) SHA1(2f4951171a55e7ac072742fa24eceeee6aca7e39) )
ROM_END

ROM_START( senctest )
	ROM_REGION( 0x6000, "maincpu", 0 )
	ROM_LOAD( "senctest.h1",   0x0000, 0x0800, CRC(6e1a77e7) SHA1(2f920679eadccef389205869f7fa60389a682599) )
	ROM_LOAD( "m645g-4m32.g1", 0x0800, 0x0800, CRC(1b873788) SHA1(6cdf0d602a65c7efcf8abe149c6172b4c7ab87a1) )
	ROM_LOAD( "m645f-4m31.f1", 0x1000, 0x0800, CRC(d4319c91) SHA1(30830595c220f490fe150ad018fbf4671bb71e02) )
	ROM_LOAD( "m645e-4m30.e1", 0x1800, 0x0800, CRC(9b9a1a45) SHA1(8023a05c13e8b541f9e2fe4d389e6a2dcd4766ea) )
	ROM_LOAD( "m645d-4m29.d1", 0x4000, 0x0800, CRC(294d52ce) SHA1(0ee63413c5caf60d45ae8bef08f6c07099d30f79) )
	ROM_LOAD( "m645c-4m28.c1", 0x4800, 0x0800, CRC(ce44c923) SHA1(9d35908de3194c5fe6fc8495ae413fa722018744) )
	ROM_LOAD( "m645b-4m27.b1", 0x5000, 0x0800, CRC(098070ab) SHA1(72ae344591df0174353dc2e3d22daf5a70e2261f) )
	ROM_LOAD( "m645a-4m26.a1", 0x5800, 0x0800, CRC(7f1d1f44) SHA1(2f4951171a55e7ac072742fa24eceeee6aca7e39) )
ROM_END

GAMEL( 1980, spcenctr,   0,        spcenctr, spcenctr, spcenctr_state, empty_init, ROT0,   "Midway", "Space Encounters", MACHINE_SUPPORTS_SAVE, layout_spcenctr )
GAMEL( 2008, senctest, spcenctr,   spcenctr, spcenctr, spcenctr_state, empty_init, ROT0,   "Timothy Shiels", "Space Encounters Test Rom", MACHINE_SUPPORTS_SAVE, layout_spcenctr )

