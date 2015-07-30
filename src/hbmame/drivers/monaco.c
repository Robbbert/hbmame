// license:BSD-3-Clause
// copyright-holders:Robbbert
/*
Monaco GP Simulator v0.1
http://www.jps.net/camilty/monman.htm
Phil Stroffolino

To Do:
- abstract and externalize in-game parameters (speeds,accel,timing)
- fix distribution of computer car
- computer cars should avoid rescue car lane when present
- computer cars should move to center in bridge zone
- player lives not hooked up
- high score tracking
- extended play colors

General:
	Score Increases as a function of the player's speed.
	Speed of Terrain scrolling is independent.

Computer Cars:
	The bridge section is different from the others (besides all the cars going
	faster after so many points(6K and 8K).

Display Panel:
	The cockpit model displays TIME (2 digit), top 5 scores (4 digits each),
	your Score (4 digits), Players to Date (3 digits)(daily), and
	Ranking (3 digits) (this is final score ranking, not position during the game).

	The top 999 high scores are internally recorded in a RAM.
	The player's ranking is updated only after gamplay is finished (during the
	fanfare).

Timed Play:
	X000-X295	A-Smooth Zone
	X295-X450	B-Slip Zone
	X450-X795	A-Smooth Zone
	X795-X000	C-Tunnel

Extended Play:
	X000-X095	A-Smooth Zone
	X095-X150	D-Gravel Zone
	X150-X295	A-Smooth Zone
	X295-X475	B-Slip Zone
	X475-X595	A-Smooth Zone
	X595-X635	E-Bridge Zone
	X635-X795	A-Smooth Zone
	X795-X000	C-Tunnel Zone

Oil Slicks (pool):
	X185		(during zone A)
	X685		(during zone A)
	After 8000 points, two pools come out in succession
	(the second about 20-30 points from the first).

Rescue Car (firetruck)
	The rescue car only appears during Extended play.  Tolerance -50 (not+)
	X000
	X075
	X125
	X250
	X500

	The rescue car appears in a mid-right lane.  It does not move side to side,
	it just moves slowly up the screen until it goes off the screen.

	In extended play, the rescue car was observed to come out AT LEAST ONCE
	during each 1000 points cycle.  I have observed the rescue car coming
	out as much a 3 TIMES in one 1000 point cycle.
	I could not find a pattern in the time or count interval between the
	rescue cars.  However, 72% were between 750 and 1250 point intervals
	(most around 875) while the remaining 28% were 500 point intervals or
	lower.  Therfore, we can narrow down WHERE the rescue cars appear, but
	IF it is going to appear seems to have some randomness to it.


notes:
	when the player first appears (game start, after a spinout, after
	a crash), the player's car shows up on the extreme right of the
	screen and is totally invulnerable (even if over water on the
	bridge scene!).  The road doesn't scroll at all until you give the
	car some gas.  Note that once you start moving, you can never
	completely come to a stop.

	Once you move to left onto the center part of the
	road, normal collision behavior takes over.  You earn no points
	while in the invulnerable state.  The player can remain in this
	state forever during timed play (until time runs out).  In extended
	play, the player's car will explode after several seconds of driving.

---------------------------------------------------------------------------

Board 96598-P is an oscillator board that generates the game sounds.
It is activated by outputs from Board Assy A (96577X).

Board Assy's A (96577X) and B (96578X) are the main guts of the gameplay
and contain the sprite roms.

Board Assy A
- accepts all of the game inputs
- sends the signals for the sounds to the Oscillator board
- outputs to the L.E.D. score display board
- directly interacts with Board Assy B
	Components:
		Start Timer Control
		Player Control (pedal, steering, shifter)
		My Car light control, explosion, and pool (oil slick) skid
		Other Car Control

Board Assy B
- outputs the video
- outputs to the L.E.D. score display board
- directly interacts with Board Assy A.
	Components:
		Display of Letter
		Road Movement Horizontal
		Road Movement Vertical (Right and Left)
		Background (sprite) coloring
		Road Object (tunnel, bridge, water) control
		Signal Sync
		Rescue Car, Player Car, Bridge--Video and Control

	(On the second set, boards A and B have different part numbers
	(97091X and 97092X) and two less roms.  These boards were made
	later then the first set.  I have interchanged these boards with
	each other, and they seem to be interchangable, the only
	difference seems to be the rom size, which only 2 of the roms
	utilize the full 1024 bytes (second set).)

Oscillator Board 96598
ROM ID		IC#
PRm-40		IC21			PRm-40		<both type 6331-1, 16 pin>
	engine (constant; pitch varies with speed)
	passing (two variations)
	bonus (chimes)
	fanfare
	explosion
	slip zone
	siren (rescue car)
*/

/* The following changes were done by Robbbert on 2007-Dec-08:

1. Add siren sound for ambulance
2. Add slip sound for puddle
3. Save and display high scores, score, number of games played, and ranking
4. Play fanfare sound if high score exceeded
5. Fix the gears so that each has its own max speed and acceleration rates
6. Keep track of extra lives (er, cars).
7. In extended play, you lose a life. When they are all gone, the game ends
8. When game ends, all sounds are turned off, except Crash sound is allowed to finish, and fanfare can play.
9. When you crash, you nearly stop, instead of continuing at full speed.
10. Once you start moving, you cannot completely stop (as documented above)
11. Sound volume adjustment confined to the machine driver.
12. Volume adjusted so the sound is no longer grossly overdriven.
13. Add the overlay file (it was always compiled but never used), and activated the LEDs.
14. Add support for Mr.Do artwork.
15. Add speedometer

There is a bunch more things to be done

1. Don't get points while not on the track
2. Fall in the water if you don't line up for the bridge
3. Show the bridge warning before you reach the bridge
4. Different engine sounds depending on the speed
5. Add sounds when passing other cars
6. Plus some of what's listed above by the original author
*/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "sound/samples.h"
#include "monaco.lh"


#define kRESCUE_CAR_SPEED (1.75)
#define SCREEN_WIDTH	384 /* 12 car lengths */
#define SCREEN_HEIGHT	240 /* 15 car widths */
#define BRIDGE_YPOS	((SCREEN_HEIGHT-16)/2-8)
#define PAGE_SIZE		(14*32)
#define NUM_COMPUTER_CARS 4

/* red */
#define TEXT_GAMEOVER		2
#define TEXT_EXTENDEDPLAY	0

/* blue */
#define TEXT_DEPOSITCOIN	3
#define TEXT_START		1

enum
{
	GFX_UNKNOWN,GFX_EXPLOSION,GFX_PLAYER,GFX_SHAKE,GFX_SPINOUT1,GFX_SPINOUT2,
	GFX_SPRAY,GFX_COMPUTER,GFX_TEXT,
	GFX_TREE,GFX_GRASS,GFX_SHRUB,GFX_HOUSE,
	GFX_TUNNEL,GFX_POOL,GFX_BELT,
	GFX_RESCUE_CAR,GFX_SIGNAL,GFX_DUMMY,
	GFX_BRIDGE1,GFX_BRIDGE2
};

enum
{
	CYAN_PEN	= 0x91,
	BLACK_PEN	= 0x94,
	YELLOW_PEN	= 0x95,
	GREY_PEN	= 0x96
};

enum
{
	RESCUE_CLUT	= 0x50,
	HOUSE_CLUT	= 0x60,
	WATER_CLUT	= 0x70,
	PLAYER_CLUT	= 0x80,
	TREE_CLUT	= 0x84,
	SHRUB_CLUT	= 0x88,
	GRASS_CLUT	= 0x8c,
	CYAN_CLUT	= 0x90,
	RED_CLUT	= 0x92,
	YELLOW_CLUT	= 0x94
};

enum
{
	GREEN_CAR, YELLOW_CAR, /* slow */
	CYAN_CAR, PURPLE_CAR, /* medium */
	BLUE_CAR, /* fast */
	NUM_COMPUTER_CAR_TYPES
};

enum
{
	PAGE_SMOOTH,
	PAGE_SLIP,
	PAGE_TUNNEL,
	PAGE_GRAVEL,
	PAGE_BRIDGE
};

enum
{
	COLLISION_CRASH,
	COLLISION_POOL
};

enum monaco_mode
{
	MODE_ATTRACT,
	/*	cars zoom up the screen (max 2 on the screen at a time)
	 *	player's car is not visible
	 *	"GAME OVER" is centered near the middle-top
	 *	"DEPOSIT COIN" is centered near the bottom
	 */
	MODE_START,
	MODE_INVULNERABLE,
	MODE_NORMAL,	/* driving */
	MODE_GRAVEL,
	MODE_SLIP1,		/* skidding */
	MODE_SLIP2,
	MODE_SPINOUT1,
	MODE_SPINOUT2,
	MODE_SPINOUT3,
	MODE_SPINOUT4,
	MODE_CRASH1,
	MODE_CRASH2,
	MODE_CRASH3,
	MODE_CRASH4
};

class monaco_state : public driver_device
{
public:
	monaco_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_samples(*this, "samples")
		, m_p_ram(*this, "ram")
		, m_gfxdecode(*this, "gfxdecode")
		, m_palette(*this, "palette")
		{ }

	DECLARE_READ8_MEMBER(monaco_ram_r);
	DECLARE_WRITE8_MEMBER(monaco_ram_w);
	DECLARE_DRIVER_INIT(monaco);
	INTERRUPT_GEN_MEMBER(monaco_interrupt);
	DECLARE_MACHINE_RESET(monaco);
	DECLARE_VIDEO_START(monaco);
	UINT32 screen_update_monaco(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
private:
	struct monaco_gfx *m_monaco_gfx;
	enum monaco_mode m_monaco_mode;
	UINT32 monaco_word_r (UINT16 offset);
	void monaco_word_w (UINT16 offset, UINT32 data);
	int get_player_xpos( void );
	void handle_collision( int sx, int sy, int width, int height, int type );
	int read_coin();
	void update_player_speed();
	double get_player_delta( void );
	void HandlePlayer();
	void update_computer_cars( void );
	void HandleRescue( void );
	void HandlePool( void );
	void GameOver();
	void HandleEvents();
	void draw_computer( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_pool( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_player( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_strip( bitmap_ind16 &bitmap, int sy, int x0, int x1, int xpos, int pen);
	void DrawSmoothZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos);
	void DrawSlipZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos);
	void DrawGravelZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos);
	void DrawBridgeZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos);
	void DrawTunnelZone( bitmap_ind16 &bitmap, int xpos);
	void DrawTunnelWall( bitmap_ind16 &bitmap, const rectangle clip, int xpos);
	void draw_light_helper( bitmap_ind16 &bitmap, int xpos);
	void draw_wall( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_light( bitmap_ind16 &bitmap);
	void draw_page( bitmap_ind16 &bitmap, const rectangle clip, int which, int xpos);
	void draw_background( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_text( bitmap_ind16 &bitmap, const rectangle clip);
	void draw_leds( bitmap_ind16 &bitmap);
	void draw_signal( bitmap_ind16 &bitmap);
	int m_anim_timer;

	/* LED display */
	UINT16 m_plays;
	UINT16 m_rank;
	UINT16 m_rank_display;	// shows 0 until game ends
	UINT32 m_score;
	UINT32 m_bonus_score;
	UINT8 m_in_ext_play;
	UINT8 m_gear;
	UINT16 m_time;
	UINT8 m_lives;
	int m_bShaking;
	double m_speed;
	double m_player_ypos;
	double m_computer_speed;
	double m_xpos[NUM_COMPUTER_CARS];
	double m_ypos[NUM_COMPUTER_CARS];
	double m_dy[NUM_COMPUTER_CARS];
	double m_rescue_xpos;
	double m_pool_xpos;
	double m_scroll;
	double m_distance;
	int m_ticks;
	int m_page_current;
	int m_page_next;
	int m_page_next2;

	double m_track_bottom_inset;
	double m_track_top_inset;
	double m_track_bottom_delta;
	double m_track_top_delta;
	int m_bSignalVisible;
	int m_left_text;
	int m_right_text;
	int m_bExtendedPlay;
	int m_left_page, m_right_page;
	int m_top_inset, m_bottom_inset;
	int m_player_x, m_player_y, m_player_tile, m_player_splash;
	int m_pool_x, m_pool_y;
	int m_rescue_x, m_rescue_y, m_rescue_tile;
	int m_x[NUM_COMPUTER_CARS];
	int m_y[NUM_COMPUTER_CARS];
	int m_tile[NUM_COMPUTER_CARS];
	int m_color[NUM_COMPUTER_CARS];
	UINT32 m_led_high1;
	UINT32 m_led_high2;
	UINT32 m_led_high3;
	UINT32 m_led_high4;
	UINT32 m_led_high5;
	UINT32 m_led_score;
	UINT16 m_led_time;
	UINT16 m_led_rank;
	UINT16 m_led_plays;
	UINT16 m_led_lives;
	UINT16 m_led_gear;
	UINT16 m_led_speed;
	required_device<samples_device> m_samples;
	required_shared_ptr<UINT8> m_p_ram;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<palette_device> m_palette;
};


enum
{
	eSAMPLE_PASS1,
	eSAMPLE_PASS2,
	eSAMPLE_CRASH,
	eSAMPLE_ENGINE,
	eSAMPLE_EXTEND,
	eSAMPLE_FANFARE,
	eSAMPLE_SIREN,
	eSAMPLE_SLIP,
//	eSAMPLE_GRAVEL
};

#define DOWN	0x01
#define UP	0x02
#define RIGHT	0x04
#define LEFT	0x08
#define COIN	0x10
#define ACCEL1	0x20
#define ACCEL2	0x40
#define ACCEL3	0x80

#define IS_PRESSED(key)		(ioport("IN0")->read() & key)

#define EXT_SCORE (2000<<8)			// number of points needed to get extended play, 2000
#define FIRST_TIME (90<<8)			// number of seconds when coin inserted, 90


READ8_MEMBER( monaco_state::monaco_ram_r )
{
	return m_p_ram[offset];
}

WRITE8_MEMBER( monaco_state::monaco_ram_w )
{
	m_p_ram[offset] = data;
}

UINT32 monaco_state::monaco_word_r (UINT16 offset)
{
	address_space &space = machine().device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 byte0 = monaco_ram_r (space, (offset << 2));
	UINT8 byte1 = monaco_ram_r (space, (offset << 2) + 1);
	UINT8 byte2 = monaco_ram_r (space, (offset << 2) + 2);
	UINT8 byte3 = monaco_ram_r (space, (offset << 2) + 3);
	return (byte3<<24) | (byte2<<16) | (byte1<<8) | byte0;
}

void monaco_state::monaco_word_w (UINT16 offset, UINT32 data)
{
	address_space &space = machine().device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 byte0 = data & 0xff;
	UINT8 byte1 = (data & 0xff00) >> 8;
	UINT8 byte2 = (data & 0xff0000) >> 16;
	UINT8 byte3 = (data & 0xff000000) >> 24;
	monaco_ram_w (space, (offset << 2), byte0);
	monaco_ram_w (space, (offset << 2) + 1, byte1);
	monaco_ram_w (space, (offset << 2) + 2, byte2);
	monaco_ram_w (space, (offset << 2) + 3, byte3);
}

int monaco_state::get_player_xpos( void )
{
	return SCREEN_WIDTH-32-m_speed;
}

void monaco_state::handle_collision( int sx, int sy, int width, int height, int type )
{
	int px, py;

	if( m_monaco_mode == MODE_NORMAL )
	{
		px = get_player_xpos();
		py = m_player_ypos + 8;

		if( px < sx + width &&
			sx < px + 32 &&
			py < sy + height &&
			sy < py + 16 )
		{
			m_samples->start( 3, eSAMPLE_CRASH );
			m_anim_timer = 0;
			m_speed = 15;		/* you almost stop when you crash */
			if( type == COLLISION_CRASH )
			{
				if( !m_in_ext_play )
				{
					m_monaco_mode = MODE_SPINOUT1;
				}
				else
				{
					m_monaco_mode = MODE_CRASH1;
					m_lives--;
				}
			}
			else if( type == COLLISION_POOL )
			{
				m_pool_xpos = 512;
				m_monaco_mode = MODE_SLIP1;
			}
		}
	}
}

/*****************************************************************/

int monaco_state::read_coin()
{
	static int old_trigger;
	if( IS_PRESSED(COIN) )
	{
		old_trigger = 1;
	}
	else {
		if( old_trigger ){
			old_trigger = 0;
			return 1;
		}
		old_trigger = 0;
	}
	return 0;
}

void monaco_state::update_player_speed()
{
	double desired_speed, min_speed, max_speed = 44;
	int accel = 0;

	if( IS_PRESSED(ACCEL1) ) accel = 1;
	if( IS_PRESSED(ACCEL2) ) accel = 2;
	if( IS_PRESSED(ACCEL3) ) accel = 3;

	/* gearstick is a mechanical device which can be moved anytime */
	if( IS_PRESSED(UP) )
		m_gear = 1;
	else
	if( IS_PRESSED(DOWN) )
		m_gear = 0;

	if (m_monaco_mode == MODE_ATTRACT)
	{
		m_speed = 0;
		m_in_ext_play = 0;
		return;
	}
	else
	if( m_monaco_mode == MODE_START )
	{
		if( accel ) m_monaco_mode = MODE_INVULNERABLE;
		else return;
	}
	
	if (m_speed)
		min_speed = m_gear?0.5:0.25;
	else
		min_speed = 0;

	if (m_gear)
	{
		max_speed = 125;
		if (m_score > 6000<<8) max_speed = 152;
		if (m_score > 8000<<8) max_speed = 180;
	}

	/* min: 0; max: 6 */
	/* 30 is derived by absolute maximum speed (above) divided by 6 */
	desired_speed = 30*(accel?(m_gear*3+accel):min_speed);

	/* 1st gear */
	if (!m_gear)
	{
		if (desired_speed > m_speed) m_speed += 1;

		if (desired_speed < m_speed) m_speed -= 4;
	}
	else
	{
		if (desired_speed > m_speed) m_speed += 0.4;

		if (desired_speed < m_speed) m_speed -= 2;
	}

	if (m_speed < min_speed) m_speed = min_speed;

	if (m_speed > max_speed) m_speed = max_speed;
}

double monaco_state::get_player_delta( void )
{
	if( m_monaco_mode == MODE_ATTRACT || m_monaco_mode == MODE_START ) return 0;
	return m_speed/16.0;
}

void monaco_state::HandlePlayer()
{
	double dy = 0;
	int sy = m_player_ypos;
	update_player_speed();
	if( IS_PRESSED(RIGHT) && sy>0 ) dy-=1.0;
	if( IS_PRESSED(LEFT) && sy<SCREEN_HEIGHT-32 ) dy+=1.0;
	if( m_page_current == PAGE_SLIP ) dy = dy*0.5;
	m_player_ypos += dy;
}

/*****************************************************************/

void monaco_state::update_computer_cars( void )
{
	int i;
	int delta = get_player_delta();
	for( i=0; i<NUM_COMPUTER_CARS; i++ )
	{
		int top_inset = m_track_top_inset;
		int bottom_inset = m_track_bottom_inset;
		int sx,sy;
		m_xpos[i] += delta-m_computer_speed;
		if( m_xpos[i] < -256 )
			m_xpos[i] += SCREEN_WIDTH*2;
		else
		if( m_xpos[i] > SCREEN_WIDTH + 256 )
			m_xpos[i] -= SCREEN_WIDTH*2;

		m_ypos[i] += m_dy[i];

		sx = m_xpos[i];
		sy = m_ypos[i];

		if( ( m_dy[i]<0 && sy<top_inset ) ||
		    (	m_dy[i]>0 && sy+16>SCREEN_HEIGHT-bottom_inset ) )
		{
			m_dy[i] *= -1;
		}

		handle_collision( sx-32, sy, 32, 16, COLLISION_CRASH );
	}
}

#define DELTA_TIME (256.0/60)

void monaco_state::HandleRescue( void )
{
	if( m_rescue_xpos > -64 )
	{
		m_rescue_xpos -= kRESCUE_CAR_SPEED;
		handle_collision(
			m_rescue_xpos,
			(SCREEN_HEIGHT-32)/2,
			64,
			32,
			COLLISION_CRASH );
	}
}

void monaco_state::HandlePool( void )
{
	if( m_pool_xpos < SCREEN_WIDTH )
	{
		m_pool_xpos += get_player_delta();
		handle_collision(
			m_pool_xpos,
			(SCREEN_HEIGHT-32)/2,
			32,
			32,
			COLLISION_POOL );
	}
}

void monaco_state::GameOver()
{
	m_monaco_mode = MODE_ATTRACT;			/* this causes speed to be zero */
	m_rank_display = m_rank;		/* show rank on led display */
	monaco_word_w (m_rank, m_score);	/* save players score in RAM */
	m_led_high1 = monaco_word_r(1);
	m_led_high2 = monaco_word_r(2);
	m_led_high3 = monaco_word_r(3);	/* show the 5 highest scores on led display */
	m_led_high4 = monaco_word_r(4);
	m_led_high5 = monaco_word_r(5);
	m_samples->stop(0);				/* turn off engine sounds */
	m_samples->stop(1);
	m_samples->stop(2);
}

void monaco_state::HandleEvents()
{
	int score = m_score>>8;
	int event = (score)%1000;

	if (m_monaco_mode == MODE_ATTRACT) return;

	if( m_page_current == PAGE_SMOOTH &&
		(score == 8000 || score == 8030 || event == 185 || event == 685)  )
	{
		m_pool_xpos = -32<<8;
	}

	if( m_in_ext_play )
	{
		/* Extended Play */
		if(	m_rescue_xpos<= -64 &&
			(machine().rand()&3) == 0 &&
			(event == 0 || event == 75 || event == 125 || event == 250 || event == 500) )
		{
			m_rescue_xpos = SCREEN_WIDTH;
			m_samples->start( 1, eSAMPLE_SIREN );
		}

		if( event == 0 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 95 ) m_page_next2 = PAGE_GRAVEL;
		else if( event == 150 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 295 ) m_page_next2 = PAGE_SLIP;
		else if( event == 475 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 595 ) m_page_next2 = PAGE_BRIDGE;
		else if( event == 635 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 795 ) m_page_next2 = PAGE_TUNNEL;
	}
	else
	{
		/* Timed Play */
		if( event == 0 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 295 ) m_page_next2 = PAGE_SLIP;
		else if( event == 450 ) m_page_next2 = PAGE_SMOOTH;
		else if( event == 795 ) m_page_next2 = PAGE_TUNNEL;
	}

	if ((!m_time) && (!m_in_ext_play))
	{
		if (m_score > EXT_SCORE)
		{
			/* Turn On Extended Play */
			m_in_ext_play = 1;
//			m_samples->start( 2, eSAMPLE_EXTEND );
		}
		else
			m_lives = 0;
	}

	if ((!m_lives) && (m_monaco_mode != MODE_ATTRACT))
	{
		/* Game Over */
		if (m_rank == 1) m_samples->start (4, eSAMPLE_FANFARE);

		GameOver();
	}
}

INTERRUPT_GEN_MEMBER( monaco_state::monaco_interrupt )
{
	int i;
	double delta = get_player_delta();

	HandleRescue();
	HandlePool();
	HandlePlayer();
	/* increase score */
	m_score += m_speed;
	/* cap the score at 9999 */
	if ( m_score > (9999<<8 | 255)) m_score = (9999<<8 | 255);

	/* Bonus car at 2000, 4000, 6000, 8000 points */

	if ( m_score < 9000<<8) m_bonus_score += m_speed;

	if ( m_bonus_score > 2000<<8 )
	{
		m_lives++;
		m_bonus_score -= 2000<<8;
		m_samples->start( 2, eSAMPLE_EXTEND );
	}
	/* decrease time remaining */
	if( m_time>0 )
	{
		if( m_monaco_mode != MODE_ATTRACT && m_monaco_mode != MODE_START )
		{
			if( m_time<DELTA_TIME ) m_time = 0;
			else m_time -= DELTA_TIME;
		}
	}
	/* adjust ranking if a score is passed */
	if (m_rank > 1)
	{
		if (m_score > monaco_word_r (m_rank - 1))
		{
			monaco_word_w ( m_rank , monaco_word_r (m_rank - 1));
			m_rank--;
		}
	}

	HandleEvents();

	switch( m_monaco_mode )
	{
	case MODE_ATTRACT:
		m_pool_xpos = SCREEN_WIDTH;
		m_rescue_xpos = -64;

		if( read_coin() )
		{
			m_samples->start( 0, eSAMPLE_ENGINE, 1/*loop*/ );
			m_score = 0;
			m_bonus_score = 0;
			m_rank_display = 0xffff;	/* rank is blank till game ends */
			m_lives = 1;
			if (m_plays < 999) m_plays++;
			m_rank = m_plays;
			m_time = FIRST_TIME;
			m_monaco_mode = MODE_START;
			m_anim_timer = 0;
			m_player_ypos = 0;//32;
		}
	default:
		break;
	}

	m_scroll += delta;
	if( m_scroll>=14*32 )
	{
		m_scroll -= 14*32;
		m_page_current = m_page_next;
		m_page_next = m_page_next2;
	}

	m_ticks++; /* for animation */

	if(	m_page_next != PAGE_TUNNEL && m_page_current != PAGE_TUNNEL &&
		m_page_next != PAGE_BRIDGE && m_page_current != PAGE_BRIDGE )
	{
		/* delta: player speed */
		m_track_bottom_inset += m_track_bottom_delta*delta/8;
		m_track_top_inset += m_track_top_delta*delta/8;

		if( m_track_bottom_inset <= 32 )
		{
			m_track_bottom_delta = 1;
		}
		else if( m_track_bottom_inset >= 96 )
		{
			m_track_bottom_delta = -1;
		}

		if( m_track_top_inset <= 32 )
		{
			m_track_top_delta = 1;
		}
		else if( m_track_top_inset >= 64 )
		{
			m_track_top_delta = -1;
		}
	}

	switch( m_monaco_mode )
	{
	case MODE_ATTRACT:
		break;
	case MODE_START:
		if( m_speed )
		{
			m_monaco_mode = MODE_INVULNERABLE;
			m_anim_timer = 0;
		}
		break;

	case MODE_INVULNERABLE:
		m_anim_timer++;
		if( m_anim_timer > 60*2 )
		{
			m_monaco_mode = MODE_NORMAL;
			m_anim_timer = 0;
		}
		break;

	case MODE_NORMAL:
		break;

	case MODE_GRAVEL:
//		m_samples->start (1, eSAMPLE_GRAVEL);  There is no code to make this mode occur
		break;

	case MODE_SLIP1:
	case MODE_SLIP2:
		m_anim_timer++;
		if( m_anim_timer>16 )
		{
			m_anim_timer = 0;
			if( m_monaco_mode!=MODE_SLIP2 )
			{
				m_monaco_mode = MODE_SLIP2;
			}
			else
			{
				m_monaco_mode = MODE_NORMAL;
			}
		}
		m_samples->start( 1, eSAMPLE_SLIP );
		break;

	case MODE_SPINOUT1:
	case MODE_SPINOUT2:
	case MODE_SPINOUT3:
	case MODE_SPINOUT4:
		if( m_player_ypos > 0 )
		{
			UINT8 temp = (m_anim_timer/2)&3;
			if (temp == 0) m_monaco_mode = MODE_SPINOUT1;
			else if (temp == 1) m_monaco_mode = MODE_SPINOUT2;
			else if (temp == 2) m_monaco_mode = MODE_SPINOUT3;
			else if (temp == 3) m_monaco_mode = MODE_SPINOUT4;
			m_anim_timer++;
			m_player_ypos-=2.0;
		}
		else
		{
			m_player_ypos = 0;
			m_monaco_mode = MODE_INVULNERABLE;
			m_anim_timer = 0;
		}
		if( m_speed > 40 )
		{
			m_speed-= 1.0;
		}
		break;

	case MODE_CRASH1:
	case MODE_CRASH2:
	case MODE_CRASH3:
	case MODE_CRASH4:
		m_anim_timer++;
		if( m_anim_timer>8 )
		{
			m_anim_timer = 0;
			if( m_monaco_mode == MODE_CRASH1 ) m_monaco_mode = MODE_CRASH2;
			else
			if( m_monaco_mode == MODE_CRASH2 ) m_monaco_mode = MODE_CRASH3;
			else
			if( m_monaco_mode == MODE_CRASH3 ) m_monaco_mode = MODE_CRASH4;
			else
			{
				m_monaco_mode = MODE_INVULNERABLE;
				m_player_ypos = 0;
			}
		}
		break;
	default:
		break;
	}

	if( m_monaco_mode == MODE_NORMAL )
	{
		if( m_player_ypos+16 > m_track_top_inset+8 &&
			m_player_ypos+16 < SCREEN_HEIGHT - (m_track_bottom_inset+8) )
		{
			m_bShaking = 0;
		}
		else if( m_player_ypos+16 > m_track_top_inset+8 - 16 &&
			m_player_ypos+16 < SCREEN_HEIGHT - (m_track_bottom_inset+8 - 16) )
		{
			/* brush roadside */
			m_bShaking = 1;
		}
		else
		{
			/* crash wall */
			m_monaco_mode = MODE_SPINOUT1;
			m_anim_timer = 0;
			m_bShaking = 1;
		}
	}
	else
	{
		m_bShaking = 0;
	}

	update_computer_cars();

	for( i=0; i<NUM_COMPUTER_CARS; i++ )
	{
		m_x[i] = m_xpos[i] - 32;
		m_y[i] = m_ypos[i];
		m_tile[i] = (m_ticks&2)>>1;
		m_color[i] = i; /* hack */
	}
	m_rescue_x = m_rescue_xpos;
	m_rescue_y = BRIDGE_YPOS;
	m_rescue_tile = (m_ticks&2)>>1;
	m_pool_x = m_pool_xpos;
	m_pool_y = (SCREEN_HEIGHT-32)/2;
	m_top_inset = m_track_top_inset;
	m_bottom_inset = m_track_bottom_inset;
	m_bSignalVisible = (m_page_next == PAGE_BRIDGE || m_page_next == PAGE_BRIDGE );
	m_led_score = m_score;
	m_led_gear = m_gear;
	m_led_lives = m_lives;
	m_led_time = m_time;
	m_led_rank = m_rank_display;
	//	m_led_rank = m_rank;	/* test to display real-time ranking */
	m_led_plays = m_plays;
	m_led_speed = m_speed;
	m_right_page = m_page_current;
	m_left_page = m_page_next;
	m_left_text = -1;
	m_right_text = -1;

	/* we stretch the text so that it is proportioned correctly
	** compared to the rest of the in-game graphics
	*/

	switch( m_monaco_mode )
	{
		case MODE_ATTRACT:
			m_left_text = TEXT_GAMEOVER;
			m_right_text = TEXT_DEPOSITCOIN;
			break;

		case MODE_START:
			m_right_text = TEXT_START;
			break;

		default:
			break;
	}

	if (m_in_ext_play) m_left_text = TEXT_EXTENDEDPLAY;

	m_player_x = get_player_xpos();
	m_player_y = m_player_ypos;
	m_player_tile = -1;
	m_player_splash = -1;
	switch( m_monaco_mode )
	{
	case MODE_ATTRACT:
		break;

	case MODE_SLIP1:
		m_player_splash = 0;
		m_player_tile = (m_ticks&2)>>1;
		break;

	case MODE_SLIP2:
		m_player_splash = 1;
		m_player_tile = (m_ticks&2)>>1;
		break;

	case MODE_SPINOUT1:
		m_player_tile = 2;
		break;

	case MODE_SPINOUT2:
		m_player_tile = 3;
		break;

	case MODE_SPINOUT3:
		m_player_tile = 4;
		break;
	case MODE_SPINOUT4:
		m_player_tile = 5;
		break;

	case MODE_CRASH1:
	case MODE_CRASH2:
	case MODE_CRASH3:
	case MODE_CRASH4:
		m_player_tile = m_monaco_mode-MODE_CRASH1 + 6;
		break;

	default:
		if( m_bShaking )
		{
			m_player_tile = 10+((m_ticks&4)>>2);
		}
		else
		{
			m_player_tile = (m_ticks&2)>>1;
		}
		break;
	}

	return;
}

/*********************************************/

static ADDRESS_MAP_START( monaco_map, AS_PROGRAM, 8, monaco_state )/* fake */
	AM_RANGE(0x0000, 0xefff) AM_ROM
	AM_RANGE(0xf000, 0xffff) AM_READWRITE(monaco_ram_r,monaco_ram_w) AM_SHARE("ram") /* scores need 4000 bytes */
ADDRESS_MAP_END

static INPUT_PORTS_START( monaco )
	PORT_START("IN0") /* fake */
	PORT_BIT( DOWN,   IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN  ) PORT_8WAY
	PORT_BIT( UP,     IP_ACTIVE_HIGH, IPT_JOYSTICK_UP    ) PORT_8WAY
	PORT_BIT( RIGHT,  IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_8WAY
	PORT_BIT( LEFT,   IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  ) PORT_8WAY
	PORT_BIT( COIN,   IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( ACCEL1, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( ACCEL2, IP_ACTIVE_HIGH, IPT_BUTTON2 )
	PORT_BIT( ACCEL3, IP_ACTIVE_HIGH, IPT_BUTTON3 )
INPUT_PORTS_END

/***************************************************************************/

static gfx_layout tree_layout =
{
	32,32,
	1, /* number of characters */
	2, /* number of bitplanes */
	{ 0,4 },
	{
		0x003,0x002,0x001,0x000,0x103,0x102,0x101,0x100,
		0x203,0x202,0x201,0x200,0x303,0x302,0x301,0x300,
		0x403,0x402,0x401,0x400,0x503,0x502,0x501,0x500,
		0x603,0x602,0x601,0x600,0x703,0x702,0x701,0x700
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8
	},
	0
};

static gfx_layout pool_layout =
{
	32,32,
	1, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x003,0x002,0x001,0x000,0x103,0x102,0x101,0x100,
		0x203,0x202,0x201,0x200,0x303,0x302,0x301,0x300,
		0x403,0x402,0x401,0x400,0x503,0x502,0x501,0x500,
		0x603,0x602,0x601,0x600,0x703,0x702,0x701,0x700
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8
	},
	4
};

static gfx_layout player_layout =
{
	32,32,
	2, /* number of characters */
	2, /* number of bitplanes */
	{ 0,4 },
	{
		0x003,0x002,0x001,0x000,0x103,0x102,0x101,0x100,
		0x203,0x202,0x201,0x200,0x303,0x302,0x301,0x300,
		0x403,0x402,0x401,0x400,0x503,0x502,0x501,0x500,
		0x603,0x602,0x601,0x600,0x703,0x702,0x701,0x700
	},
	{
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8,
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x800
};

static gfx_layout text_layout =
{
	16,64,
	4, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x007,0x006,0x005,0x004,0x003,0x002,0x001,0x000,
		0x207,0x206,0x205,0x204,0x203,0x202,0x201,0x200,
	},
	{
		0x100,0x108,0x110,0x118,0x120,0x128,0x130,0x138,
		0x140,0x148,0x150,0x158,0x160,0x168,0x170,0x178,
		0x180,0x188,0x190,0x198,0x1a0,0x1a8,0x1b0,0x1b8,
		0x1c0,0x1c8,0x1d0,0x1d8,0x1e0,0x1e8,0x1f0,0x1f8,
		0x000,0x008,0x010,0x018,0x020,0x028,0x030,0x038,
		0x040,0x048,0x050,0x058,0x060,0x068,0x070,0x078,
		0x080,0x088,0x090,0x098,0x0a0,0x0a8,0x0b0,0x0b8,
		0x0c0,0x0c8,0x0d0,0x0d8,0x0e0,0x0e8,0x0f0,0x0f8,
	},
	0x400
};

static gfx_layout house_layout =
{
	32,32,
	1, /* number of characters */
	4, /* number of bitplanes */
	{ 0,2,4,6 },
	{
		0x001,0x000,0x101,0x100,0x201,0x200,0x301,0x300,
		0x401,0x400,0x501,0x500,0x601,0x600,0x701,0x700,
		0x801,0x800,0x901,0x900,0xa01,0xa00,0xb01,0xb00,
		0xc01,0xc00,0xd01,0xd00,0xe01,0xe00,0xf01,0xf00
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8
	},
	4
};

static gfx_layout signal_layout =
{
	32,32,
	1, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x001,0x000,0x101,0x100,0x201,0x200,0x301,0x300,
		0x401,0x400,0x501,0x500,0x601,0x600,0x701,0x700,
		0x801,0x800,0x901,0x900,0xa01,0xa00,0xb01,0xb00,
		0xc01,0xc00,0xd01,0xd00,0xe01,0xe00,0xf01,0xf00
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8,
	},
	0
};

static gfx_layout dummy_layout =
{
	32,16,
	2, /* number of characters */
	2, /* number of bitplanes */
	{ 4,6 },
	{
		0x001,0x000,0x101,0x100,0x201,0x200,0x301,0x300,
		0x401,0x400,0x501,0x500,0x601,0x600,0x701,0x700,
		0x801,0x800,0x901,0x900,0xa01,0xa00,0xb01,0xb00,
		0xc01,0xc00,0xd01,0xd00,0xe01,0xe00,0xf01,0xf00
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x80
};

static gfx_layout computer_layout =
{
	32,16,
	2, /* number of characters */
	4, /* number of bitplanes */
	{ 0,2,4,6 },
	{
		0x001,0x000,0x081,0x080,0x101,0x100,0x181,0x180,
		0x201,0x200,0x281,0x280,0x301,0x300,0x381,0x380,
		0x401,0x400,0x481,0x480,0x501,0x500,0x581,0x580,
		0x601,0x600,0x681,0x680,0x701,0x700,0x781,0x780
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x800
};

static gfx_layout rescue_layout =
{
	64,16,
	2, /* number of characters */
	4, /* number of bitplanes */
	{ 0,2,4,6 },
	{
		0x001,0x001,0x000,0x000,0x081,0x081,0x080,0x080,
		0x101,0x101,0x100,0x100,0x181,0x181,0x180,0x180,
		0x201,0x201,0x200,0x200,0x281,0x281,0x280,0x280,
		0x301,0x301,0x300,0x300,0x381,0x381,0x380,0x380,
		0x401,0x401,0x400,0x400,0x481,0x481,0x480,0x480,
		0x501,0x501,0x500,0x500,0x581,0x581,0x580,0x580,
		0x601,0x601,0x600,0x600,0x681,0x681,0x680,0x680,
		0x701,0x701,0x700,0x700,0x781,0x781,0x780,0x780
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x800
};

static gfx_layout explode_layout =
{
	32,32,
	4, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x007,0x006,0x005,0x004,0x003,0x002,0x001,0x000,
		0x107,0x106,0x105,0x104,0x103,0x102,0x101,0x100,
		0x207,0x206,0x205,0x204,0x203,0x202,0x201,0x200,
		0x307,0x306,0x305,0x304,0x303,0x302,0x301,0x300,
	},
	{
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8,
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x400
};

static gfx_layout bridge_layout =
{
	32,16,
	2, /* number of characters */
	4, /* number of bitplanes */
	{ 0,1,2,3 },
	{
		0x004,0x000,0x104,0x100,0x204,0x200,0x304,0x300,
		0x404,0x400,0x504,0x500,0x604,0x600,0x704,0x700,
		0x804,0x800,0x904,0x900,0xa04,0xa00,0xb04,0xb00,
		0xc04,0xc00,0xd04,0xd00,0xe04,0xe00,0xf04,0xf00
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x80
};

static gfx_layout unknown_layout =
{ /* police car? */
	32,32,
	2, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x307,0x306,0x305,0x304,0x303,0x302,0x301,0x300,
		0x207,0x206,0x205,0x204,0x203,0x202,0x201,0x200,
		0x107,0x106,0x105,0x104,0x103,0x102,0x101,0x100,
		0x007,0x006,0x005,0x004,0x003,0x002,0x001,0x000,
	},
	{
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8,
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78
	},
	0x400
};


static gfx_layout tunnel_layout =
{
	32,32,
	2, /* number of characters */
	1, /* number of bitplanes */
	{ 0 },
	{
		0x003,0x002,0x001,0x000,0x103,0x102,0x101,0x100,
		0x203,0x202,0x201,0x200,0x303,0x302,0x301,0x300,
		0x403,0x402,0x401,0x400,0x503,0x502,0x501,0x500,
		0x603,0x602,0x601,0x600,0x703,0x702,0x701,0x700
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8
	},
	4
};

static gfx_layout belt_layout = {
	32,16,
	2, /* number of characters */
	1, /* number of bitplanes */
	{ 4 },
	{
		0x003,0x002,0x001,0x000,0x103,0x102,0x101,0x100,
		0x203,0x202,0x201,0x200,0x303,0x302,0x301,0x300,
		0x403,0x402,0x401,0x400,0x503,0x502,0x501,0x500,
		0x603,0x602,0x601,0x600,0x703,0x702,0x701,0x700
	},
	{
		0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,
		0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,
		0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,
		0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8
	},
	0x80
};

static GFXDECODE_START( monaco )
	GFXDECODE_ENTRY( "gfx1", 0x0100, unknown_layout,	0x00, 1)	/* PR125: unused; police car? */
	GFXDECODE_ENTRY( "gfx1", 0x0200, explode_layout,	RED_CLUT, 2)	/* PR126: explosion */
	GFXDECODE_ENTRY( "gfx1", 0x0400, player_layout,	PLAYER_CLUT, 1)	/* PR127: player's car */
	GFXDECODE_ENTRY( "gfx1", 0x0600, player_layout,	PLAYER_CLUT, 1)	/* PR128: swerving (rough road) */
	GFXDECODE_ENTRY( "gfx1", 0x0800, player_layout,	PLAYER_CLUT, 1)	/* PR129: spinout1 */
	GFXDECODE_ENTRY( "gfx1", 0x0a00, player_layout,	PLAYER_CLUT, 1)	/* PR130: spinout2 */
	GFXDECODE_ENTRY( "gfx1", 0x0c00, explode_layout,	CYAN_CLUT, 1)	/* PR131: spray */
	GFXDECODE_ENTRY( "gfx1", 0x0e00, computer_layout,	0x00, 5)	/* PR132: computer car A,B */
	GFXDECODE_ENTRY( "gfx1", 0x1000, text_layout,	CYAN_CLUT, 2)	/* PR133: text */
	GFXDECODE_ENTRY( "gfx1", 0x1200, tree_layout,	TREE_CLUT, 1)	/* PR134: tree */
	GFXDECODE_ENTRY( "gfx1", 0x1300, tree_layout,	TREE_CLUT, 1)	/* PR134: grass */
	GFXDECODE_ENTRY( "gfx1", 0x1400, tree_layout,	SHRUB_CLUT, 1)	/* PR135: shrub */
	GFXDECODE_ENTRY( "gfx1", 0x1600, house_layout,	HOUSE_CLUT, 1)	/* PR136: house */
	GFXDECODE_ENTRY( "gfx1", 0x1800, tunnel_layout,	RED_CLUT, 1)	/* PR137: tunnel */
	GFXDECODE_ENTRY( "gfx1", 0x1900, pool_layout,	CYAN_CLUT, 1)	/* PR137: pool (oil slick) */
	GFXDECODE_ENTRY( "gfx1", 0x1900, belt_layout,	GRASS_CLUT, 2)	/* PR137: red/green belt */
	GFXDECODE_ENTRY( "gfx1", 0x1a00, rescue_layout,	RESCUE_CLUT, 1)	/* PR138: rescue car */
	GFXDECODE_ENTRY( "gfx1", 0x1c00, signal_layout,	YELLOW_CLUT, 1)	/* PR139: bridge signal (yellow on black)*/
	GFXDECODE_ENTRY( "gfx1", 0x1c00, dummy_layout,	PLAYER_CLUT, 1)	/* PR139: dummy car */
	GFXDECODE_ENTRY( "gfx1", 0x1e00, bridge_layout,	WATER_CLUT, 1)	/* PR140: bridge-water */
	GFXDECODE_ENTRY( "gfx1", 0x2000, bridge_layout,	WATER_CLUT, 1)	/* PR141: bridge-pillar */
GFXDECODE_END

static const char *const monaco_sample_names[] =
{
	"*monaco",
	"6car1",
	"6car2",
	"6crash",
	"6engine",
	"6extend",
	"6fanfare",
	"6siren",
	"6slip",
//	"gravel",
	0
};

/* ch 0 = engine; ch 1 = ambulance, puddle; ch 2 = extra car; ch 3 = crash; ch 4 = fanfare */

MACHINE_RESET_MEMBER( monaco_state, monaco )
{
	UINT16 i;
	m_time = 0;			/* time remaining = 0 */
	m_score = 0;		/* player score = 0 */
	m_gear = 0;			/* low gear */
	m_plays = 0;		/* no games played */
	for (i = 0;i < 1000;i++)
		monaco_word_w (i, 0);	/* delete all scores from RAM */
	GameOver();
}

static MACHINE_CONFIG_START( monaco, monaco_state )

	/* basic machine hardware */
	MCFG_CPU_ADD ("maincpu", Z80, 200) /* fake */
	MCFG_CPU_PROGRAM_MAP(monaco_map)
	MCFG_CPU_VBLANK_INT_DRIVER("screen", monaco_state, monaco_interrupt)
	MCFG_MACHINE_RESET_OVERRIDE(monaco_state, monaco)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(4368))
	MCFG_SCREEN_SIZE(SCREEN_WIDTH, SCREEN_HEIGHT)
	MCFG_SCREEN_VISIBLE_AREA(0, SCREEN_WIDTH-1, 0, SCREEN_HEIGHT-1)
	MCFG_VIDEO_START_OVERRIDE(monaco_state, monaco)
	MCFG_SCREEN_UPDATE_DRIVER(monaco_state, screen_update_monaco)
	MCFG_SCREEN_PALETTE("palette")
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", monaco)
	MCFG_PALETTE_ADD("palette", 160)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("samples", SAMPLES, 0)
	MCFG_SAMPLES_CHANNELS(5)
	MCFG_SAMPLES_NAMES(monaco_sample_names)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.90)
MACHINE_CONFIG_END

/*****************************************************************/

ROM_START( monaco )
	ROM_REGION( 0x10000, "maincpu", ROMREGION_ERASE00 )	/* fake */

	ROM_REGION( 0x3000, "gfx1", 0 )
	ROM_LOAD( "pr125", 512*0,  512, CRC(7a66ed4c) SHA1(514e129c334a551b931c90b063b073a9b4bdffc3) ) /* light data */
	ROM_LOAD( "pr126", 512*1,  512, CRC(5d7a8f12) SHA1(b4f0d21b91a7cf7002f99c08788669c7c38be51d) ) /* explosion */
	ROM_LOAD( "pr127", 512*2,  512, CRC(8ffdc2f0) SHA1(05cc3330c067965b8b90b5d27119fe9f26580a13) ) /* car(2)main */
	ROM_LOAD( "pr128", 512*3,  512, CRC(dde29dea) SHA1(34c413edff991297471bd0bc193c4bd8ede4e468) ) /* car(2)rotated */
	ROM_LOAD( "pr129", 512*4,  512, CRC(7b18af26) SHA1(3d1ff2610813544c3b9b65182f081272a9537640) ) /* car(2)rotated */
	ROM_LOAD( "pr130", 512*5,  512, CRC(9ef1913b) SHA1(58830121781b8a13532eaf8ea13ec07f10522320) ) /* car(2) spinout */
	ROM_LOAD( "pr131", 512*6,  512, CRC(ff31eb01) SHA1(fd6bcd92c4bd919bb1a96ca97688d46cb310b39d) ) /* splash */
	ROM_LOAD( "pr132", 512*7,  512, CRC(6b8ad9bc) SHA1(be36e3b6b647d3a9565bc45903027c791dc889e5) ) /* car(2)(other) */
	ROM_LOAD( "pr133", 512*8,  512, CRC(d50641d9) SHA1(bf399e9830c88e4d8f8fb386305f54ef766946d9) ) /* text(4) */
	ROM_LOAD( "pr134", 512*9,  512, CRC(8ebd50bb) SHA1(98d51f503753d4d7191a09b509d26c1e049e981a) ) /* tree, grass */
	ROM_LOAD( "pr135", 512*10, 512, CRC(986eda32) SHA1(73fa539d4c83748952d9339985208520fec955f3) ) /* shrub */
	ROM_LOAD( "pr136", 512*11, 512, CRC(ecc5d1a2) SHA1(33bff7381785557a85e4c8bdd74679b59e0ed9d5) ) /* house */
	ROM_LOAD( "pr137", 512*12, 512, CRC(ddd9004e) SHA1(5229c34578e66d9c51a05439a516513946ba69ed) ) /* tunnel, oil slip */
	ROM_LOAD( "pr138", 512*13, 512, CRC(058e53cf) SHA1(7c3aaaca5a9e9ce3a3badd0dcc8360342673a397) ) /* firetruck */
	ROM_LOAD( "pr139", 512*14, 512, CRC(e8ba0794) SHA1(eadd7425134f26b1c126bbcd3d3dabf4b2e1fe70) ) /* car, bridge symbol */
	ROM_LOAD( "pr140", 512*15, 512, CRC(48e9971b) SHA1(c0c265cdc08727e3caaf49cdfe728a91c4c46ba2) ) /* bridge-water */
	ROM_LOAD( "pr141", 512*16, 512, CRC(99934236) SHA1(ec271f3e690d5c57ead9132b22b9b1b966e4d170) ) /* bridge-pillar */

	ROM_REGION( 32*3, "proms", 0 )
	ROM_LOAD( "prm38",	0*32, 32, CRC(82dd0a0f) SHA1(3e7e475c3270853d70c1fe90a773172532b60cfb) )	/* acceleration related */
	ROM_LOAD( "prm39",	1*32, 32, CRC(6acfa0da) SHA1(1e56da4cdf71a095eac29878969b831babac222b) )	/* regulates opponent car speed */

//	ROM_LOAD( "prm-40",	2*32, 32, CRC(8030dac8) )
/*	PR40 is in the Fanfare sound circuit and seems to access the particular
 *	notes for the fanfare sound (so PR40 may contain timing and pointer info
 *	on the melody).  The switch (SW1) I mentioned before that helped in tuning
 *	the fanfare sound with the 6 pots seems to help in making the tuning of each
 *	pot for output of one of three audio frequencies (262, 330, 392 Hz),
 *	instead of having to tune to 6 different frequencies (a production/test
 *	equipment issue).
 *	In any case, if we get a good sample of this fanfare sound, we will not
 *	need to bother with this circuit or PR40.  As far a I have seen, the
 *	fanfare sound only comes up at the end of the game if you have a top five
 *	score and possibly when you plug in the game.
 */
ROM_END

DRIVER_INIT_MEMBER( monaco_state, monaco )
{
	int i;
	const double dy_table[5] =
	{
		0.75,
		1.25,
		1.75,
		2.00,
		2.50
	};

	m_computer_speed = 52*16/256.0;
	m_track_bottom_inset = 32;
	m_track_top_inset = 32;
	m_track_bottom_delta = 1;
	m_track_top_delta = 1;

	/* computer car */
	for( i=0; i<NUM_COMPUTER_CARS; i++ )
	{
		m_xpos[i] = i*32*3 + ((i>1)?192:0);
		m_ypos[i] = (SCREEN_HEIGHT-16)/2;
		m_dy[i] = dy_table[i];
	}
}

/*          rom     parent  machine inp     init */
GAMEL( 1979, monaco, 0, monaco, monaco, monaco_state, monaco, ROT90, "Sega", "Monaco GP", 0, layout_monaco )




/* Monaco GP video hardware simulation */


#define plot_pixel(bitmap,x,y,col)	do { bitmap.pix16(y, x) = col; } while (0)

void monaco_state::draw_computer( bitmap_ind16 &bitmap, const rectangle clip )
{
	int i;

	for( i=0; i<NUM_COMPUTER_CARS; i++ )
	{
		m_gfxdecode->gfx(GFX_COMPUTER)->transpen(
			bitmap, clip,
			m_tile[i],
			m_color[i],
			0,0,
			m_x[i],
			m_y[i],
			0 );
	}

	m_gfxdecode->gfx(GFX_RESCUE_CAR)->transpen(
		bitmap, clip,
		m_rescue_tile,
		0, /* color */
		0,0,
		m_rescue_x,
		m_rescue_y,
		0 );
}

void monaco_state::draw_pool( bitmap_ind16 &bitmap, const rectangle clip )
{
	m_gfxdecode->gfx(GFX_POOL)->transpen(
		bitmap, clip,
		0,0, /* tile,color */
		0,0, /* flip */
		m_pool_x,
		m_pool_y,
		0 );
}

void monaco_state::draw_player( bitmap_ind16 &bitmap, const rectangle clip )
{
	int gfx;
	int tile;

	switch( m_player_splash )
	{
	case 0:
		m_gfxdecode->gfx(GFX_SPRAY)->transpen( bitmap, clip,
			2,0,0,0,
			m_player_x,
			m_player_y+32-8,
			0);
		m_gfxdecode->gfx(GFX_SPRAY)->transpen( bitmap, clip,
			0,0,0,0,
			m_player_x,
			m_player_y-32+8,
			0);
		break;
	case 1:
		m_gfxdecode->gfx(GFX_SPRAY)->transpen( bitmap, clip,
			3,0,0,0,
			m_player_x,
			m_player_y+32-8,
			0);
		m_gfxdecode->gfx(GFX_SPRAY)->transpen( bitmap, clip,
			1,0,0,0,
			m_player_x,
			m_player_y-32+8,
			0);
		break;
	}

	tile = m_player_tile;
	gfx = 0;
	if( tile>=0 )
	{
		switch( tile )
		{
		case 0:
		case 1: gfx = GFX_PLAYER; break;

		case 2:
		case 3: gfx = GFX_SPINOUT1; tile -= 2; break;

		case 4:
		case 5: gfx = GFX_SPINOUT2; tile -= 4; break;

		case 6:
		case 7:
		case 8:
		case 9: gfx = GFX_EXPLOSION; tile -= 6; break;

		case 10:
		case 11: gfx = GFX_SHAKE; tile -= 10; break;
		}

		m_gfxdecode->gfx(gfx)->transpen( bitmap, clip,
			tile,0,
			0,0,
			m_player_x,
			m_player_y,
			0);
	}
}

/*****************************************************************/

void monaco_state::draw_strip( bitmap_ind16 &bitmap, int sy, int x0, int x1, int xpos, int pen )
{
	int sx;
	if( x0<xpos ) x0 = xpos;
	if( x1>xpos+PAGE_SIZE ) x1 = xpos+PAGE_SIZE;
	if( x0<0 ) x0 = 0;
	if( x1>SCREEN_WIDTH ) x1 = SCREEN_WIDTH;
	for( sx=x0; sx<x1; sx++ ) plot_pixel( bitmap,sx,sy,pen );
}

void monaco_state::DrawSmoothZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos )
{
	const UINT8 data[14] =
	{
		GFX_GRASS,GFX_GRASS,GFX_GRASS,
		GFX_TREE,GFX_GRASS,GFX_TREE,
		GFX_GRASS,GFX_HOUSE,GFX_GRASS,
		GFX_SHRUB,GFX_SHRUB,
		GFX_GRASS,GFX_GRASS,GFX_GRASS
	};
	int top_inset = m_top_inset;
	int bottom_inset = m_bottom_inset;
	int i;

	draw_strip( bitmap, top_inset, xpos, xpos+PAGE_SIZE, xpos, YELLOW_PEN );
	draw_strip( bitmap, SCREEN_HEIGHT-1 - bottom_inset, xpos, xpos+PAGE_SIZE, xpos, YELLOW_PEN );

	for( i=0; i<14; i++ )
	{
		int code = data[i];
		gfx_element *gfx = m_gfxdecode->gfx(code);
		gfx_element *belt = m_gfxdecode->gfx((code==GFX_HOUSE)?GFX_DUMMY:GFX_BELT);
		int j;

		for( j=0; j<3; j++ )
		{
			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos, m_top_inset-32-16-j*32 );

			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos,SCREEN_HEIGHT-m_bottom_inset+j*32+16-8 );
		}
		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos, m_top_inset-16 );

		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos,SCREEN_HEIGHT-m_bottom_inset );

		xpos += 32;
	}
}

void monaco_state::DrawSlipZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos )
{
	const UINT8 data[14] =
	{
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,GFX_HOUSE,
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,/* start  */GFX_SHRUB,GFX_SHRUB
	};

	int top_inset = m_top_inset;
	int bottom_inset = m_bottom_inset;
	int i;

	draw_strip( bitmap, top_inset, xpos, xpos+PAGE_SIZE, xpos, YELLOW_PEN );
	draw_strip( bitmap, SCREEN_HEIGHT-1 - bottom_inset, xpos, xpos+PAGE_SIZE, xpos, YELLOW_PEN );

	for( i=0; i<14; i++ ){
		int code = data[i];
		gfx_element *gfx = m_gfxdecode->gfx(code);
		gfx_element *belt = m_gfxdecode->gfx((code==GFX_HOUSE)?GFX_DUMMY:GFX_BELT);
		int j;

		for( j=0; j<3; j++ ){
			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos, top_inset-32-j*32-16 );

			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos,SCREEN_HEIGHT-bottom_inset+j*32+16-8 );
		}

		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos, top_inset-16 );

		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos,SCREEN_HEIGHT-bottom_inset );

		xpos += 32;
	}
}

void monaco_state::DrawGravelZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos )
{
	const UINT8 data[14] = {
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,GFX_HOUSE,
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB,
		GFX_SHRUB,GFX_SHRUB,GFX_SHRUB
	};
	int top_inset = m_top_inset;
	int bottom_inset = m_bottom_inset;
	int i;
	int xpos0 = xpos;

	for( i=0; i<14; i++ )
	{
		int code = data[i];
		gfx_element *gfx = m_gfxdecode->gfx(code);
		gfx_element *belt = m_gfxdecode->gfx((code==GFX_HOUSE)?GFX_DUMMY:GFX_BELT);
		int j;
		int ypos;

		/* draw gravel */
		if( data[i]!=GFX_HOUSE )
		{
			ypos = SCREEN_HEIGHT-bottom_inset-32;
			m_gfxdecode->gfx(GFX_BELT)->opaque( bitmap, clip,
				1,1, /* number, color */
				0,0, /* no flip */
				xpos, ypos+32-8 );
			ypos-=24;
			while( ypos>0 ){
				m_gfxdecode->gfx(GFX_BELT)->opaque( bitmap, clip,
					1,1, /* number, color */
					0,0, /* no flip */
					xpos, ypos );
				ypos -= 16;
			}
		}

		for( j=0; j<3; j++ )
		{
			ypos = SCREEN_HEIGHT-bottom_inset+16+j*32-8;
			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos, ypos );
		}

		for( j=0; j<3; j++ )
		{
			ypos = top_inset-32-16-j*32;
			gfx->opaque( bitmap, clip,
				0,0, /* number, color */
				0,0, /* no flip */
				xpos, ypos );
		}

		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos, top_inset-16 );

		belt->opaque( bitmap, clip,
			0,0, /* number, color */
			0,0, /* no flip */
			xpos,SCREEN_HEIGHT-bottom_inset );


		xpos += 32;
	}
	draw_strip( bitmap, top_inset, xpos0, xpos0+PAGE_SIZE, xpos0, YELLOW_PEN );
	draw_strip( bitmap, SCREEN_HEIGHT-1 - bottom_inset, xpos0, xpos0+PAGE_SIZE, xpos0, YELLOW_PEN );
}

void monaco_state::DrawBridgeZone( bitmap_ind16 &bitmap, const rectangle clip, int xpos )
{
	gfx_element *gfx1 = m_gfxdecode->gfx(GFX_BRIDGE1);
	gfx_element *gfx2 = m_gfxdecode->gfx(GFX_BRIDGE2);

	int i;
	for( i=0; i<14; i++ )
	{
		int j;
		gfx_element *gfx = (i==0)?gfx2:gfx1;
		for( j=0; j<7; j++ )
		{
			int flip;
			for( flip=0; flip<=1; flip++ )
			{
				int ypos = flip?(SCREEN_HEIGHT-16-j*16)+8:j*16-8;
				if( j<5 )
				{ /* water */
					gfx1->opaque( bitmap, clip,
						0,0, /* number, color */
						0,flip,
						xpos, ypos );
				}
				else
				{ /* edge of bridge */
					gfx->opaque( bitmap, clip,
						j-5,0, /* number, color */
						0,flip,
						xpos, ypos );
				}
			}
		}
		xpos += 32;
	}
}

void monaco_state::DrawTunnelZone( bitmap_ind16 &bitmap, int xpos )
{
	int pen = YELLOW_PEN;
	int top_inset = m_top_inset;
	int bottom_inset = m_bottom_inset;
	draw_strip( bitmap, top_inset, xpos, xpos+PAGE_SIZE, xpos, pen );
	draw_strip( bitmap, SCREEN_HEIGHT-1 - bottom_inset, xpos, xpos+PAGE_SIZE, xpos, pen );
}

void monaco_state::DrawTunnelWall( bitmap_ind16 &bitmap, const rectangle clip, int xpos )
{
	rectangle clip2;
	gfx_element *gfx = m_gfxdecode->gfx(GFX_TUNNEL);
	int top_inset = m_top_inset - 16;
	int bottom_inset = m_bottom_inset - 16;
	int i;

	clip2.min_x = clip.min_x;
	clip2.max_x = clip.max_x;
	clip2.max_y = clip.max_y;
	clip2.min_y = SCREEN_HEIGHT-bottom_inset;

	for( i=0; i<14; i++ )
	{
		int j;
		for( j=0; j<2; j++ )
		{
			gfx->transpen( bitmap, clip,
				1,0, /* number, color */
				0,0, /* no flip */
				xpos, top_inset-32-j*32, 0 );

			gfx->transpen( bitmap, clip,
				1,0, /* number, color */
				0,0, /* no flip */
				xpos, SCREEN_HEIGHT-bottom_inset+j*32 - 8, 0 );
		}
		xpos += 32;
	}
}

void monaco_state::draw_light_helper( bitmap_ind16 &bitmap, int xpos )
{
	int pen = BLACK_PEN;
	const unsigned char *source = memregion( "gfx1" )->base();
	int x0 = m_player_x-128;
	int y0 = m_player_y-48;
	int sy;
	for( sy=0; sy<SCREEN_HEIGHT; sy++ )
	{
		int i = sy-y0;
		if( i<0 || i>=128 )
		{
			draw_strip( bitmap, sy, xpos, xpos+PAGE_SIZE, xpos, pen );
		}
		else
		{
			int left = x0+source[i];
			int right = x0+source[i+128];
			draw_strip( bitmap, sy, xpos, left, xpos, pen );
			draw_strip( bitmap, sy, right, xpos+PAGE_SIZE, xpos, pen );
		}
	}
}

void monaco_state::draw_wall( bitmap_ind16 &bitmap, const rectangle clip )
{
	if( m_right_page == PAGE_TUNNEL )
		DrawTunnelWall( bitmap, clip, SCREEN_WIDTH-14*32+m_scroll );

	if( m_left_page == PAGE_TUNNEL )
		DrawTunnelWall( bitmap, clip, SCREEN_WIDTH-14*32*2+m_scroll );
}

void monaco_state::draw_light( bitmap_ind16 &bitmap )
{
	if( m_right_page == PAGE_TUNNEL )
		draw_light_helper( bitmap, SCREEN_WIDTH-14*32+m_scroll );

	if( m_left_page == PAGE_TUNNEL )
		draw_light_helper( bitmap, SCREEN_WIDTH-14*32*2+m_scroll );
}

void monaco_state::draw_page( bitmap_ind16 &bitmap, const rectangle clip, int which, int xpos )
{
	rectangle r;
	r.min_x = xpos;
	r.max_x = xpos+PAGE_SIZE-1;
	r.min_y = 0;
	r.max_y = SCREEN_HEIGHT-1;
	bitmap.fill( (which!=PAGE_SLIP)?GREY_PEN:CYAN_PEN,r );
	switch( which )
	{
	case PAGE_SMOOTH:
		DrawSmoothZone( bitmap, clip, xpos );
		break;

	case PAGE_SLIP:
		DrawSlipZone( bitmap, clip, xpos );
		break;

	case PAGE_TUNNEL:
		DrawTunnelZone( bitmap, xpos );
		break;

	case PAGE_GRAVEL:
		DrawGravelZone( bitmap, clip, xpos );
		break;

	case PAGE_BRIDGE:
		DrawBridgeZone( bitmap, clip, xpos );
		break;
	}
}

void monaco_state::draw_background( bitmap_ind16 &bitmap, const rectangle clip )
{
	draw_page(bitmap, clip,
		m_right_page,
		SCREEN_WIDTH-14*32+m_scroll );

	draw_page(bitmap, clip,
		m_left_page,
		SCREEN_WIDTH-14*32*2+m_scroll );
}

void monaco_state::draw_text( bitmap_ind16 &bitmap, const rectangle clip )
{
	int sx = (SCREEN_WIDTH-8)/2;
	int sy = (SCREEN_HEIGHT-128)/2;

	if( m_left_text != -1 )
	{
		m_gfxdecode->gfx(GFX_TEXT)->zoom_transpen( bitmap, clip,
			m_left_text,0,
			0,0, /* flip */
			sx-96,sy,
			1<<16, 2<<16, 0
		);
	}

	if( m_right_text != -1 )
	{
		m_gfxdecode->gfx(GFX_TEXT)->zoom_transpen( bitmap, clip,
			m_right_text,1,
			0,0, /* flip */
			SCREEN_WIDTH-32,sy,
			1<<16, 2<<16, 0
		);
	}
}

/* last two values are L and H used to display in gear field (not supported officially)*/
static const UINT8 led_map[12] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x38,0x76 };

/* following outputs LED's for bezel. also displays time,score&speed text versions (text version disabled here) */

void monaco_state::draw_leds( bitmap_ind16 &bitmap )
{
	int i;
	UINT32 data;

//	data = m_speed;
//	for( i=2; i>=0; i-- )
//	{
//		drawgfx_transpen( bitmap, led_font,
//			data%10,0, /* number, color */
//			0,1, /* no flip */
//			9,SCREEN_HEIGHT-6-i*6,
//			NULL, TRANSPARENCY_NONE,0 );
//
//		data = data/10;
//	}

	data = m_led_high1>>8;
	for( i=3; i>=0; i-- )
	{
		output_set_digit_value(i+10, led_map[data%10]);
		data = data/10;
	}

	data = m_led_high2>>8;
	for( i=3; i>=0; i-- )
	{
		output_set_digit_value(i+20, led_map[data%10]);
		data = data/10;
	}

	data = m_led_high3>>8;
	for( i=3; i>=0; i-- )
	{
		output_set_digit_value(i+30, led_map[data%10]);
		data = data/10;
	}

	data = m_led_high4>>8;
	for( i=3; i>=0; i-- )
	{
		output_set_digit_value(i+40, led_map[data%10]);
		data = data/10;
	}

	data = m_led_high5>>8;
	for( i=3; i>=0; i-- )
	{
		output_set_digit_value(i+50, led_map[data%10]);
		data = data/10;
	}

	data = m_led_score>>8;
	for( i=3; i>=0; i-- )
	{
//		drawgfx_transpen( bitmap, led_font,
//			data%10,0, /* number, color */
//			0,1, /* no flip */
//			0,SCREEN_HEIGHT-6-i*6,
//			NULL, TRANSPARENCY_NONE,0 );

		output_set_digit_value(i, led_map[data%10]);

		data = data/10;
	}

	data = m_led_plays;
	for( i=2; i>=0; i-- )
	{
		output_set_digit_value(i+60, led_map[data%10]);
		data = data/10;
	}

	data = m_led_rank;
	for( i=2; i>=0; i-- )
	{
		if (data == 0xffff)
		{
			output_set_digit_value(i+70, 0);		/* make it blank */
		}
		else
		{
			output_set_digit_value(i+70, led_map[data%10]);
			data = data/10;
		}
	}

	data = m_led_speed;
	for( i=2; i>=0; i-- )
	{
//		drawgfx_transpen( bitmap, led_font,
//			data%10,0, /* number, color */
//			0,1, /* no flip */
//			9,SCREEN_HEIGHT-6-i*6,
//			NULL, TRANSPARENCY_NONE,0 );
//
		output_set_digit_value(i+80, led_map[data%10]);
		data = data/10;
	}

	data = m_led_gear;
	for( i=0; i>=0; i-- )
	{
		output_set_digit_value(i+90, led_map[10+data%10]);
		data = data/10;
	}

	data = m_led_lives;
	for( i=0; i>=0; i-- )
	{
		output_set_digit_value(i+100, led_map[data%10]);
		data = data/10;
	}

	data = m_led_time>>8;
	for( i=1; i>=0; i-- )
	{
//		drawgfx_opaque( bitmap, NULL, led_font,
//			data%10,0, /* number, color */
//			0,1, /* no flip */
//			18,SCREEN_HEIGHT-6-i*6 );

		output_set_digit_value(i+4, led_map[data%10]);

		data = data/10;
	}
}

void monaco_state::draw_signal( bitmap_ind16 &bitmap )
{
	if( m_bSignalVisible )
	{
//		drawgfx_opaque(
//			bitmap, NULL, machine().gfx[GFX_SIGNAL],
//			0,0, /* number, color */
//			0,0, /* no flip */
//			32,(SCREEN_HEIGHT - 32)/2 );
	}
}

UINT32 monaco_state::screen_update_monaco(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	const rectangle &visarea = screen.visible_area();
	draw_background( bitmap, visarea );
	draw_pool( bitmap, visarea );
	draw_computer( bitmap, visarea );
	draw_light( bitmap );
	draw_wall( bitmap, visarea );
	draw_text( bitmap, visarea );
	draw_player( bitmap, visarea );
	draw_signal( bitmap );
	draw_leds( bitmap );
	return 0;
}

VIDEO_START_MEMBER( monaco_state, monaco )
{
	int i;

	m_palette->set_pen_color( 0, 0x00,0x00,0x00 ); /* black (tire) */
	m_palette->set_pen_color( 1, 0xff,0xff,0xff ); /* white (trim) */
	/* computer car */
	for( i=0; i<NUM_COMPUTER_CAR_TYPES; i++ )
	{
		const unsigned char clut[3*NUM_COMPUTER_CAR_TYPES] =
		{
			0x00,0xff,0x00, /* green car */
			0xff,0xff,0x00, /* yellow car */
			0x00,0xff,0xff, /* cyan car */
			0xff,0x00,0xff, /* purple car */
			0x00,0x00,0xff  /* blue car */
		};

		m_palette->set_pen_color( 16*i+0x8, 0x00,0x00,0x00 ); /* black (tire) */
		m_palette->set_pen_color( 16*i+0xa, 0xff,0xff,0xff ); /* white (trim) */
		m_palette->set_pen_color( 16*i+0xc, clut[i*3+0],clut[i*3+1],clut[i*3+2] );
	}
	/* rescue car */
	m_palette->set_pen_color( 0x50+0x8, 0x00,0x00,0x00 ); // black
	m_palette->set_pen_color( 0x50+0x9, 0xff,0xff,0x00 ); // yellow
	m_palette->set_pen_color( 0x50+0xa, 0xff,0xff,0xff ); // white
	m_palette->set_pen_color( 0x50+0xc, 0xff,0x00,0x00 ); // red (light)
	/* house */
	m_palette->set_pen_color( 0x60+0x0, 0x00,0x00,0x00 ); // ground
	m_palette->set_pen_color( 0x60+0x9, 0xff,0x00,0x00 ); // right roof
	m_palette->set_pen_color( 0x60+0xa, 0xff,0x00,231 ); // left roof
	m_palette->set_pen_color( 0x60+0xb, 0x00,198,255 ); // front
	m_palette->set_pen_color( 0x60+0xc, 0xff,0xff,0xff ); // trim
	/* water */
	m_palette->set_pen_color( 0x70, 0x00,0x00,0xff );//l.blue
	m_palette->set_pen_color( 0x71, 0xff,0x00,0x00 );//red
	m_palette->set_pen_color( 0x72, 0xff,0xff,0x00 );//yellow
	m_palette->set_pen_color( 0x73, 0x00,0x00,0x00 );//black
	m_palette->set_pen_color( 0x74, 0x00,0x00,0x00 );//N/A
	m_palette->set_pen_color( 0x75, 0xff,0xff,0xff );//white
	m_palette->set_pen_color( 0x76, 0x9f,0x9f,0x9f );//grey (road)
	m_palette->set_pen_color( 0x77, 0x00,0x00,0x9f );//d.blue
	/* player car */
	m_palette->set_pen_color( 0x80, 0x00,0x00,0x00 );
	m_palette->set_pen_color( 0x81, 0xff,0x00,0x00 ); // red (car body)
	m_palette->set_pen_color( 0x82, 0xff,0xff,0xff ); // white (trim)
	m_palette->set_pen_color( 0x83, 0x00,0x00,0x00 ); // black (tire)
	/* tree */
	m_palette->set_pen_color( 0x84, 0xff,0xff,0x00 ); // yellow
	m_palette->set_pen_color( 0x85, 0x00,0x00,0x00 ); // dark green
	m_palette->set_pen_color( 0x86, 0x00,165,0x00 ); // light green
	m_palette->set_pen_color( 0x87, 0x00,0x00,0x00 ); // black?
	/* shrub */
	m_palette->set_pen_color( 0x88, 0x00,0x00,0x00 );
	m_palette->set_pen_color( 0x89, 0x00,0xff,0x00 );
	m_palette->set_pen_color( 0x8a, 0x00,0x00,0xff );
	m_palette->set_pen_color( 0x8b, 0xff,0xff,0xff );
	/* grass */
	m_palette->set_pen_color( 0x8c, 0x00,0x00,0x00 );
	m_palette->set_pen_color( 0x8d, 0x00,0x9f,0x00 ); // dark green
	m_palette->set_pen_color( 0x8e, 0x00,0x00,0x00 );
	m_palette->set_pen_color( 0x8f, 0x00,0x00,0x9f ); // dark blue
	m_palette->set_pen_color( 0x90, 0x00,0x00,0x00 ); // black
	m_palette->set_pen_color( 0x91, 0x00,0xff,0xff ); // cyan
	m_palette->set_pen_color( 0x92, 0x00,0x00,0x00 ); // grey
	m_palette->set_pen_color( 0x93, 0xff,0x00,0x00 ); // red
	m_palette->set_pen_color( 0x94, 0x00,0x00,0x00 ); // black
	m_palette->set_pen_color( 0x95, 255,215,0 ); // yellow trim
	m_palette->set_pen_color( 0x96, 132,132,132 ); // grey (road)
// 0,198,255: wet road
// 255,215,0: yellow trim
}
