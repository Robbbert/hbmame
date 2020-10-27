// license:BSD-3-Clause
// copyright-holders:Bryan McPhail,Ernesto Corvi,Andrew Prime,Zsolt Vasvari
// thanks-to:Fuzz
/***************************************************************************

    Neo-Geo hardware

    Credits:
        * This driver was made possible by the research done by
          Charles MacDonald.  For a detailed description of the Neo-Geo
          hardware, please visit his page at:
          http://cgfm2.emuviews.com/txt/mvstech.txt
        * Presented to you by the Shin Emu Keikaku team.
        * The following people have all spent probably far
          too much time on this:
          AVDB
          Bryan McPhail
          Fuzz
          Ernesto Corvi
          Andrew Prime
          Zsolt Vasvari


    Known driver issues/to-do's:
    ============================

        * Fatal Fury 3 crashes during the ending - this doesn't occur if
          the language is set to Japanese, maybe the English endings
          are incomplete / buggy?
        * Graphical Glitches caused by incorrect timing?
          - Some raster effects are imperfect (off by a couple of lines)
        * Multi-cart support not implemented - the MVS can take up to
          6 cartridges depending on the board being used
        * 68000 waitstates on ROM region access, determined by jumpers on cart
          (garou train stage 3 background bug is probably related to this)


    Confirmed non-bugs:

        * Bad zooming in the Kof2003 bootlegs - this is what happens
          if you try and use the normal bios with a pcb set, it
          looks like the bootleggers didn't care.
        * Glitches at the edges of the screen - the real hardware
          can display 320x224 but most of the games seem designed
          to work with a width of 304, some less.
        * Distorted jumping sound in Nightmare in the Dark
        * Ninja Combat sometimes glitches


*****************************************************************************

    The Neo-Geo Multi Video System (MVS), is an arcade system board, being
    the first product in the Neo-Geo family, designed by Alpha Denshi(ADK)
    and released in 1990 by SNK. It was known to the coin-op industry, and
    offered arcade operators the ability to put up to 6 different arcade
    titles into a single cabinet, a key economic consideration for operators
    with limited floorspace (games for the Neo-Geo are cartridge based and are
    easily exchangeable). It comes in many different cabinets but basically
    consists of an add on board that can be linked to a standard Jamma system.
    The system was discontinued in 2004.
    Source (modified): http://en.wikipedia.org/wiki/Neo_Geo


    MVS motherboards were produced in 1 / 2 / 4 and 6 Slot versions.

    Known motherboards:
    ===================

    1 Slot:
    NEO-MVH MV1
    NEO-MVH MV1-1
    NEO-MVH MV1A
     . NEO-MVH MV1A CHX ??
    NEO-MVH MV1B (1996.1.19)
     . NEO-MVH MV1B CHX (1996.1.19) ??
    NEO-MVH MV1B1 (1998.6.17)
    NEO-MVH MV1C (1999.4.30)
    NEO-MVH MV1F
    NEO-MVH MV1FS
    NEO-MVH MV1FT
    NEO-MVH MV1FZ
    NEO-MVH MV1FZS

    2 Slot:
    NEO-MVH MV2
    NEO-MVH MV2F
    NEO-MVH MV2F-01

    4 Slot:
    NEO-MVH MV4
    NEO-MVH MV4F
    NEO-MVH MV4FS
    NEO-MVH MV4FT
    NEO-MVH MV4FT2

    6 Slot:
    NEO-MVH MV6
    NEO-MVH MV6F


    Neo-Geo Motherboard (info - courtesy of Guru):

          NEO-MVH MV1
          |---------------------------------------------------------------------|
          |       4558                                                          |
          |                                          HC04  HC32                 |
          |                      SP-S2.SP1  NEO-E0   000-L0.L0   LS244  AS04    |
          |             YM2610                                                  |
          | 4558                                                                |
          |       4558                        5814  HC259   SFIX.SFIX           |
          |                                                             NEO-I0  |
          | HA13001 YM3016                    5814                              |
          --|                                                                   |
            |     4558                                                          |
          --|                                                 SM1.SM1   LS32    |
          |                                                                     |
          |                           LSPC-A0         PRO-C0            LS244   |
          |                                                                     |
          |J              68000                                                 |
          |A                                                                    |
          |M                                                                    |
          |M                                                      NEO-ZMC2      |
          |A                                                                    |
          |   LS273  NEO-G0                          58256  58256     Z80A      |
          |                           58256  58256   58256  58256     6116      |
          |   LS273 5864                                                        |
          --| LS05  5864  PRO-B0                                                |
            |                                                                   |
          --|             LS06   HC32           D4990A    NEO-F0   24.000MHz    |
          |                      DSW1    BATT3.6V 32.768kHz       NEO-D0        |
          |                                           2003  2003                |
          |---------------------------------------------------------------------|


*****************************************************************************

    Neo-Geo game PCB infos:
    =======================

    The Neo-Geo games for AES (home) and MVS (arcade) systems are cartridge based.

    Each cartridge consists of two PCBs: CHA and PROG.
    .CHA PCB contains gfx data ('C' - rom), text layer data ('S' - rom) and sound driver ('M' - rom).
    .PROG PCB contains sample data ('V' - rom) and program code ('P' - rom).

    On most PCBs various custom/protection chips can also be found:
    (Custom chip detail information (modified) from: http://wiki.neogeodev.org)

    CHA:
    . NEO-273  (C and S-ROM address latch)
    . NEO-CMC 90G06CF7042 (NEO-273 logic / NEO-ZMC logic / C-ROM decryption / C and S-ROM multiplexer / S-ROM bankswitching)
    . NEO-CMC 90G06CF7050 (NEO-273 logic / NEO-ZMC logic / C-ROM decryption / M-ROM decryption / C and S-ROM multiplexer / S-ROM bankswitching)
    . NEO-ZMC  (Z80 memory controller)
    . NEO-ZMC2 (Z80 memory controller / Tile serializer)
    . PRO-CT0  (C-ROM serializer and multiplexer?; used on early AES-CHA boards)
    . SNK-9201 (C-ROM serializer and multiplexer?; used on early AES-CHA boards)

    PROG:
    . 0103 (QFP144) (Only found on Metal Slug X NEO-MVS PROGEOP board; function unknown)
    . ALTERA   (EPM7128SQC100-15) (P-ROM protection chip used for KOF98 NEO-MVS PROGSF1 board and Metal Slug X NEO-MVS PROGEOP board)
    . NEO-COMA (Microcontroller; used for MULTI PLAY MODE, boards and sets see below)
    . NEO-PCM2 (SNK 1999) (PCM functionality / V-ROM decryption / P-ROM decoding and bankswitching)
    . NEO-PCM2 (PLAYMORE 2002) (PCM functionality / V-ROM decryption / P-ROM decoding and bankswitching)
    . NEO-PVC  (P-ROM decryption and bankswitching) / RAM
    . NEO-SMA  (P-ROM decryption and bankswitching / RNG / Storage of 256kb game data)
    . PCM      (ADPCM bus latches / V-ROM multiplexer)
    . PRO-CT0  (On PROG board used for P-ROM protection -> Fatal Fury 2)
    . SNK-9201 (On PROG board used for P-ROM protection -> Fatal Fury 2)



    Known PCBs:
    ============

    MVS CHA:
    -- SNK --
    . NEO-MVS CHA-32
    . NEO-MVS CHA-8M
    . NEO-MVS CHA42G
    . NEO-MVS CHA42G-1
    . NEO-MVS CHA 42G-2
    . NEO-MVS CHA 42G-3
    . NEO-MVS CHA42G-3B
    . NEO-MVS CHA256
    . NEO-MVS CHA256B
    . NEO-MVS CHA512Y
    . NEO-MVS CHAFIO (1999.6.14) - used with NEO-CMC 90G06C7042 or NEO-CMC 90G06C7050
    . MVS CHAFIO REV1.0 (KOF-2001)
    . NEO-MVS CHAFIO (SNK 2002) - MADE IN KOREA
    -- SNKPLAYMORE --
    . NEO-MVS CHAFIO (2003.7.24) - used only with NEO-CMC 90G06C7050

    -- SNK development boards --
    . NEO-MVS CHAMC2

    MVS PROG:
    -- SNK --
    . NEO-MVS PROG-NAM
    . NEO-MVS PROG-HERO
    . NEO-MVS PROG-EP
    . NEO-MVS PROG-8MB
    . NEO-MVS PROGEP8M
    . NEO-MVS PROG8M42
    . NEO-MVS PROG16
    . NEO-MVS PROG42G
    . NEO-MVS PROG42G-COM
    . NEO-MVS PROG42G-1
    . NEO-MVS PROG-G2
    . NEO-MVS PROG 4096
    . NEO-MVS PROG 4096 B
    . NEO-MVS PROGGSC
    . NEO-MVS PROGSM
    . NEO-MVS PROGSS3
    . NEO-MVS PROGTOP
    . NEO-MVS PROGSF1 (1998.6.17)
    . NEO-MVS PROGSF1E (1998.6.18)
    . NEO-MVS PROGEOP (1999.2.2)
    . NEO-MVS PROGLBA (1999.4.12) - LBA-SUB (2000.2.24)
    . NEO-MVS PROGBK1 (1994)
    . NEO-MVS PROGBK1 (2001)
    . NEO-MVS PROGBK2 (2000.3.21) - used with NEO-PCM2 (1999 SNK) or NEO-PCM2 (2002 PLAYMORE)
    . MVS PROGBK2 REV1.0 (KOF-2001)
    . NEO-MVS PROGBK2 (SNK 2002) - MADE IN KOREA
    -- SNKPLAYMORE --
    . NEO-MVS PROGBK2R (2003.8.26) - NEO-HYCS (2003.9.29)
    . NEO-MVS PROGBK3R (2003.9.2) - NEO-HYCS (2003.9.29)
    . NEO-MVS PROGBK3S (2003.10.1)
    . NEO-MVS PROGBK2S (2003.10.18)

    -- SNK development boards --
    . NEO-MVS PROGMC2


    AES CHA:
    -- SNK --
    . NEO-AEG CHA-32
    . NEO-AEG CHA-8M
    . NEO-AEG CHA42G
    . NEO-AEG CHA42G-1
    . NEO-AEG CHA42G-2B
    . NEO-AEG CHA42G-3
    . NEO-AEG CHA42G-4
    . NEO-AEG CHA256
    . NEO-AEG CHA256 B
    . NEO-AEG CHA256[B]
    . NEO-AEG CHA256BY
    . NEO-AEG CHA256RY
    . NEO-AEG CHA512Y
    . NEO-AEG CHAFIO (1999.8.10) - used with NEO-CMC 90G06C7042 or NEO-CMC 90G06C7050
    -- SNKPLAYMORE --
    . NEO-AEG CHAFIO (2003.7.24) - used only with NEO-CMC 90G06C7050

    AES PROG:
    -- SNK --
    . NEO-AEG PROG-NAM
    . NEO-AEG PROG-HERO
    . NEO-AEG PROG-4A
    . NEO-AEG PROG-4B
    . NEO-AEG PROG 8M42
    . NEO-AEG PROG B
    . NEO-AEG PROG16
    . NEO-AEG PROG42G
    . NEO-AEG PROG42G-COM
    . NEO-AEG PROG42G-1
    . NEO-AEG PROG-G2
    . NEO-AEG PROG4096 B
    . NEO-AEG PROGGS
    . NEO-AEG PROGTOP2
    . NEO-AEG PROGTOP2Y
    . NEO-AEG PROGEOP (1999.4.2)
    . NEO-AEG PROGLBA (1999.7.6)
    . NEO-AEG PROGRK
    . NEO-AEG PROGRKB
    . NEO-AEG PROGBK1Y
    . NEO-AEG PROGBK1F
    -- PLAYMORE --
    . NEO-AEG PROGBK2 (2002.4.1) - used with NEO-PCM2 (1999 SNK) or NEO-PCM2 (2002 PLAYMORE)
    -- SNKPLAYMORE --
    . NEO-AEG PROGBK3R (2003.8.29) - NEO-HYCS (2003.9.29)
    . NEO-AEG PROGBK3S (2003.10.6)
    . NEO-AEG PROGBK2S (2003.10.16)



    Cartridge colours:
    ==================

    MVS cartridges were produced in different colours.

    Known cartridge colours:
    . Black
    . Blue
    . Green
    . Grey
    . Red
    . Transparent
    . Transparent Blue
    . Transparent Green
    . White
    . Yellow

    The above listed only covers SNK / PLAYMORE / SNKPLAYMORE PCBs. There also exists a
    wide range of 'bootleg' PCBs.


    Unofficial pcb's from NG:DEV.TEAM:

    MVS CHA:
    GIGA CHAR Board 1.0 Rev. A
    GIGA CHAR Board 1.5 Rev. 0
    GIGA CHAR Board 1.5 Rev. C

    MVS PROG:
    GIGA PROG Board 1.0 Rev. B
    GIGA PROG Board 1.5 Rev. A
    GIGA PROG Board 1.5 Rev. C


    Unofficial pcb's from NEOBITZ:

    MVS CHA:
    CHARBITZ1 2013.12.01

    MVS PROG:
    PROGBITZ1 2013.12.01


    Neo-Geo game PCB infos by Johnboy



    MVS cart pinout:
    ================

    Kindly submitted by Apollo69 (apollo69@columbus.rr.com)
    =================================================================
                CTRG1                            CTRG2
    =================================================================
         GND = 01A | 01B = GND            GND = 01A | 01B = GND
         GND = 02A | 02B = GND            GND = 02A | 02B = GND
          P0 = 03A | 03B = P1             GND = 03A | 03B = GND
          P2 = 04A | 04B = P3             GND = 04A | 04B = GND
          P4 = 05A | 05B = P5              D0 = 05A | 05B = A1
          P6 = 06A | 06B = P7              D1 = 06A | 06B = A2
          P8 = 07A | 07B = P9              D2 = 07A | 07B = A3
         P10 = 08A | 08B = P11             D3 = 08A | 08B = A4
         P12 = 09A | 09B = P13             D4 = 09A | 09B = A5
         P14 = 10A | 10B = P15             D5 = 10A | 10B = A6
         P16 = 11A | 11B = P17             D6 = 11A | 11B = A7
         P18 = 12A | 12B = P19             D7 = 12A | 12B = A8
         P20 = 13A | 13B = P21             D8 = 13A | 13B = A9
         P22 = 14A | 14B = P23             D9 = 14A | 14B = A10
       PCK1B = 15A | 15B = 24M            D10 = 15A | 15B = A11
       PCK2B = 16A | 16B = 12M            D11 = 16A | 16B = A12
         2H1 = 17A | 17B = 8M             D12 = 17A | 17B = A13
         CA4 = 18A | 18B = RESET          D13 = 18A | 18B = A14
         CR0 = 19A | 19B = CR1            D14 = 19A | 19B = A15
         CR2 = 20A | 20B = CR3            D15 = 20A | 20B = A16
         CR4 = 21A | 21B = CR5            R/W = 21A | 21B = A17
         CR6 = 22A | 22B = CR7             AS = 22A | 22B = A18
         CR8 = 23A | 23B = CR9         ROMOEU = 23A | 23B = A19
        CR10 = 24A | 24B = CR11        ROMOEL = 24A | 24B = 68KCLKB
        CR12 = 25A | 25B = CR13       PORTOEU = 25A | 25B = ROMWAIT
        CR14 = 26A | 26B = CR15       PORTOEL = 26A | 26B = PWAIT0
        CR16 = 27A | 27B = CR17       PORTWEU = 27A | 27B = PWAIT1
        CR18 = 28A | 28B = CR19       PORTWEL = 28A | 28B = PDTACT
         VCC = 29A | 29B = VCC            VCC = 29A | 29B = VCC
         VCC = 30A | 30B = VCC            VCC = 30A | 30B = VCC
         VCC = 31A | 31B = VCC            VCC = 31A | 31B = VCC
         VCC = 32A | 32B = VCC            VCC = 32A | 32B = VCC
        CR20 = 33A | 33B = CR21      PORTADRS = 33A | 33B = 4MB
        CR22 = 34A | 34B = CR23            NC = 34A | 34B = ROMOE
        CR24 = 35A | 35B = CR25            NC = 35A | 35B = RESET
        CR26 = 36A | 36B = CR27            NC = 36A | 36B = NC
        CR28 = 37A | 37B = CR29            NC = 37A | 37B = NC
        CR30 = 38A | 38B = CR31            NC = 38A | 38B = NC
          NC = 39A | 39B = FIX00           NC = 39A | 39B = NC
          NC = 40A | 40B = FIX01           NC = 40A | 40B = NC
          NC = 41A | 41B = FIX02           NC = 41A | 41B = SDPAD0
     SYSTEMB = 42A | 42B = FIX03      SYSTEMB = 42A | 42B = SDPAD1
        SDA0 = 43A | 43B = FIX04        SDPA8 = 43A | 43B = SDPAD2
        SDA1 = 44A | 44B = FIX05        SDPA9 = 44A | 44B = SDPAD3
        SDA2 = 45A | 45B = FIX06       SDPA10 = 45A | 45B = SDPAD4
        SDA3 = 46A | 46B = FIX07       SDPA11 = 46A | 46B = SDPAD5
        SDA4 = 47A | 47B = SDRD0       SDPMPX = 47A | 47B = SDPAD6
        SDA5 = 48A | 48B = SDRD1        SDPOE = 48A | 48B = SDPAD7
        SDA6 = 49A | 49B = SDROM        SDRA8 = 49A | 49B = SDRA00
        SDA7 = 50A | 50B = SDMRD        SDRA9 = 50A | 50B = SDRA01
        SDA8 = 51A | 51B = SDDO        SDRA20 = 51A | 51B = SDRA02
        SDA9 = 52A | 52B = SDD1        SDRA21 = 52A | 52B = SDRA03
       SDA10 = 53A | 53B = SDD2        SDRA22 = 53A | 53B = SDRA04
       SDA11 = 54A | 54B = SDD3        SDRA23 = 54A | 54B = SDRA05
       SDA12 = 55A | 55B = SDD4        SDRMPX = 55A | 55B = SDRA06
       SDA13 = 56A | 56B = SDD5         SDROE = 56A | 56B = SDRA07
       SDA14 = 57A | 57B = SDD6           GND = 57A | 57B = GND
       SDA15 = 58A | 58B = SDD7           GND = 58A | 58B = GND
         GND = 59A | 59B = GND            GND = 59A | 59B = GND
         GND = 60A | 60B = GND            GND = 60A | 60B = GND

    CTRG1 (CHA)  = Contains gfx data ('C' - rom), text layer data ('S' - rom) and sound driver ('M' - rom)
    CTRG2 (PROG) = Contains sample data ('V' - rom) and program code ('P' - rom)

    NOTE: On CTRG2-B, The "A" lines start at "A1". If you trace this on an
    actual cart, you will see that this is actually "A0" (A0 - A18).

    These are from a very hard to read copy of the schematics, so
    I hope that I got the pin names correct.

    Apollo69 10/19/99


*****************************************************************************

    Watchdog:
    =========

    The watchdog timer will reset the system after ~0.13 seconds.
    By cgfm's research, exactly 3,244,030 cycles (based on 24MHz clock).

    Newer games force a reset using the following code (this from kof99):
        009CDA  203C 0003 0D40             MOVE.L   #0x30D40,D0
        009CE0  5380                       SUBQ.L   #1,D0
        009CE2  64FC                       BCC.S    *-0x2 [0x9CE0]
    Note however that there is a valid code path after this loop.

    The watchdog is used as a form of protection on a number of games,
    previously this was implemented as a specific hack which locked a single
    address of SRAM.

    What actually happens is if the game doesn't find valid data in the
    backup ram it will initialize it, then sit in a loop.  The watchdog
    should then reset the system while it is in this loop.  If the watchdog
    fails to reset the system the code will continue and set a value in
    backup ram to indiate that the protection check has failed.


    Mahjong Panel notes (2009-03 FP):
    =================================

    * In Service Mode menu with mahjong panel active, controls are as
      follows:

        A = select / up (for options)
        B = down (for options)
        C = go to previous menu
        E = up (for menu entries)
        F = down (for menu entries)
        G = left (for options)
        H = right (for options)

    * These only work with Japanese BIOS, but I think it's not a bug: I
      doubt other BIOS were programmed to be compatible with mahjong panels

****************************************************************************/

#include "includes/neogeo.h"
#include "neogeo.lh"


#define LOG_VIDEO_SYSTEM         (0)
#define LOG_MAIN_CPU_BANKING     (0)
#define LOG_AUDIO_CPU_BANKING    (0)


/*************************************
 *
 *  Main CPU interrupt generation
 *
 *************************************/



// The display counter is automatically reloaded with the load register contents on scanline 224,
// 1146 mclks from the rising edge of /HSYNC.
#define NEOGEO_VBLANK_RELOAD_HTIM (attotime::from_ticks(1146, NEOGEO_MASTER_CLOCK))

#define IRQ2CTRL_ENABLE             (0x10)
#define IRQ2CTRL_LOAD_RELATIVE      (0x20)
#define IRQ2CTRL_AUTOLOAD_VBLANK    (0x40)
#define IRQ2CTRL_AUTOLOAD_REPEAT    (0x80)


void neogeo_state::adjust_display_position_interrupt_timer()
{
	attotime period = attotime::from_ticks((u64)m_display_counter + 1, NEOGEO_PIXEL_CLOCK);
	if (LOG_VIDEO_SYSTEM) logerror("adjust_display_position_interrupt_timer  current y: %02x  current x: %02x   target y: %x  target x: %x\n", m_screen->vpos(), m_screen->hpos(), (m_display_counter + 1) / NEOGEO_HTOTAL, (m_display_counter + 1) % NEOGEO_HTOTAL);

	m_display_position_interrupt_timer->adjust(period);
}


void neogeo_state::neogeo_set_display_position_interrupt_control( u16  data )
{
	m_display_position_interrupt_control = data;
}


void neogeo_state::neogeo_set_display_counter_msb( u16  data )
{
	m_display_counter = (m_display_counter & 0x0000ffff) | ((u32)data << 16);

	if (LOG_VIDEO_SYSTEM) logerror("PC %06x: set_display_counter %08x\n", m_maincpu->pc(), m_display_counter);
}


void neogeo_state::neogeo_set_display_counter_lsb( u16  data )
{
	m_display_counter = (m_display_counter & 0xffff0000) | data;

	if (LOG_VIDEO_SYSTEM) logerror("PC %06x: set_display_counter %08x\n", m_maincpu->pc(), m_display_counter);

	if (m_display_position_interrupt_control & IRQ2CTRL_LOAD_RELATIVE)
	{
		if (LOG_VIDEO_SYSTEM) logerror("AUTOLOAD_RELATIVE ");
		adjust_display_position_interrupt_timer();
	}
}


void neogeo_state::update_interrupts()
{
	m_maincpu->set_input_line(3, m_irq3_pending ? ASSERT_LINE : CLEAR_LINE);
	m_maincpu->set_input_line(m_raster_level, m_display_position_interrupt_pending ? ASSERT_LINE : CLEAR_LINE);
	m_maincpu->set_input_line(m_vblank_level, m_vblank_interrupt_pending ? ASSERT_LINE : CLEAR_LINE);
}


void neogeo_state::neogeo_acknowledge_interrupt( u16  data )
{
	if (data & 0x01)
		m_irq3_pending = 0;
	if (data & 0x02)
		m_display_position_interrupt_pending = 0;
	if (data & 0x04)
		m_vblank_interrupt_pending = 0;

	update_interrupts();
}


TIMER_CALLBACK_MEMBER(neogeo_state::display_position_interrupt_callback)
{
	if (LOG_VIDEO_SYSTEM) logerror("--- Scanline @ %d,%d\n", m_screen->vpos(), m_screen->hpos());

	if (m_display_position_interrupt_control & IRQ2CTRL_ENABLE)
	{
		if (LOG_VIDEO_SYSTEM) logerror("*** Scanline interrupt (IRQ2) ***  y: %02x  x: %02x\n", m_screen->vpos(), m_screen->hpos());
		m_display_position_interrupt_pending = 1;

		update_interrupts();
	}

	if (m_display_position_interrupt_control & IRQ2CTRL_AUTOLOAD_REPEAT)
	{
		if (LOG_VIDEO_SYSTEM) logerror("AUTOLOAD_REPEAT ");
		adjust_display_position_interrupt_timer();
	}
}


TIMER_CALLBACK_MEMBER(neogeo_state::display_position_vblank_callback)
{
	if (m_display_position_interrupt_control & IRQ2CTRL_AUTOLOAD_VBLANK)
	{
		if (LOG_VIDEO_SYSTEM) logerror("AUTOLOAD_VBLANK ");
		adjust_display_position_interrupt_timer();
	}

	/* set timer for next screen */
	m_display_position_vblank_timer->adjust(m_screen->time_until_pos(NEOGEO_VBSTART) + NEOGEO_VBLANK_RELOAD_HTIM);
}


TIMER_CALLBACK_MEMBER(neogeo_state::vblank_interrupt_callback)
{
	if (LOG_VIDEO_SYSTEM) logerror("+++ VBLANK @ %d,%d\n", m_screen->vpos(), m_screen->hpos());

	m_vblank_interrupt_pending = 1;
	update_interrupts();

	/* set timer for next screen */
	m_vblank_interrupt_timer->adjust(m_screen->time_until_pos(NEOGEO_VBSTART) + NEOGEO_VBLANK_IRQ_HTIM);
}


void neogeo_state::create_interrupt_timers()
{
	m_display_position_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(neogeo_state::display_position_interrupt_callback),this));
	m_display_position_vblank_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(neogeo_state::display_position_vblank_callback),this));
	m_vblank_interrupt_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(neogeo_state::vblank_interrupt_callback),this));
}


void neogeo_state::start_interrupt_timers()
{
	m_vblank_interrupt_timer->adjust(m_screen->time_until_pos(NEOGEO_VBSTART) + NEOGEO_VBLANK_IRQ_HTIM);
	m_display_position_vblank_timer->adjust(m_screen->time_until_pos(NEOGEO_VBSTART) + NEOGEO_VBLANK_RELOAD_HTIM);
}



/*************************************
 *
 *  Audio CPU interrupt generation
 *
 *************************************/

void neogeo_state::audio_cpu_check_nmi()
{
	m_audiocpu->set_input_line(INPUT_LINE_NMI, (m_audio_cpu_nmi_enabled && m_audio_cpu_nmi_pending) ? ASSERT_LINE : CLEAR_LINE);
}

void neogeo_state::audio_cpu_enable_nmi_w(offs_t offset, u8 data)
{
	// out ($08) enables the nmi, out ($18) disables it
	m_audio_cpu_nmi_enabled = !(offset & 0x10);
	audio_cpu_check_nmi();
}



/*************************************
 *
 *  Input ports / Controllers
 *
 *************************************/

u16 neogeo_state::in0_r()
{
	return ((m_edge->in0_r() & m_ctrl1->read_ctrl()) << 8) | m_dsw->read();
}

u16 neogeo_state::in1_r()
{
	return ((m_edge->in1_r() & m_ctrl2->read_ctrl()) << 8) | 0xff;
}

CUSTOM_INPUT_MEMBER(neogeo_state::kizuna4p_start_r)
{
	return (m_edge->read_start_sel() & 0x05) | ~0x05;
}

void neogeo_state::io_control_w(offs_t offset, u8 data)
{
	switch (offset)
	{
		case 0x00:
			if (m_ctrl1) m_ctrl1->write_ctrlsel(data);
			if (m_ctrl2) m_ctrl2->write_ctrlsel(data);
			if (m_edge) m_edge->write_ctrlsel(data);
			break;

		case 0x10:
			break;

		case 0x18:
			if (m_type == NEOGEO_MVS)
				set_output_latch(data);
			break;

		case 0x20:
			if (m_type == NEOGEO_MVS)
				set_output_data(data);
			break;

		case 0x28:
			if (m_type == NEOGEO_MVS)
			{
				m_upd4990a->data_in_w(data >> 0 & 1);
				m_upd4990a->clk_w(data >> 1 & 1);
				m_upd4990a->stb_w(data >> 2 & 1);
			}
			break;

//  case 0x30: break; // coin counters
//  case 0x31: break; // coin counters
//  case 0x32: break; // coin lockout
//  case 0x33: break; // coin lockout

		default:
			logerror("PC: %x  Unmapped I/O control write.  Offset: %x  Data: %x\n", machine().describe_context(), offset, data);
			break;
	}
}


/*************************************
 *
 *  Unmapped memory access
 *
 *************************************/

u16 neogeo_state::neogeo_unmapped_r(address_space &space)
{
	u16  ret;

	/* unmapped memory returns the last word on the data bus, which is almost always the opcode
	   of the next instruction due to prefetch */

	/* prevent recursion */
	if (m_recurse)
		ret = 0xffff;
	else
	{
		m_recurse = true;
		ret = space.read_word(m_maincpu->pc());
		m_recurse = false;
	}
	return ret;
}



/*************************************
 *
 *  NVRAM (Save RAM)
 *
 *************************************/

void neogeo_state::set_save_ram_unlock( u8 data )
{
	m_save_ram_unlocked = data;
}


void neogeo_state::save_ram_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (m_save_ram_unlocked)
		COMBINE_DATA(&m_save_ram[offset]);
}



/*************************************
 *
 *  Memory card
 *
 *************************************/

CUSTOM_INPUT_MEMBER(neogeo_state::get_memcard_status)
{
	// D0 and D1 are memcard 1 and 2 presence indicators, D2 indicates memcard
	// write protect status (we are always write enabled)
	return (m_memcard->present() == -1) ? 0x07 : 0x00;
}


u16 neogeo_state::memcard_r(offs_t offset)
{
	m_maincpu->eat_cycles(2); // insert waitstate

	u16  ret;

	if (m_memcard->present() != -1)
		ret = m_memcard->read(offset) | 0xff00;
	else
		ret = 0xffff;

	return ret;
}


void neogeo_state::memcard_w(offs_t offset, u16 data, u16 mem_mask)
{
	m_maincpu->eat_cycles(2); // insert waitstate

	if (ACCESSING_BITS_0_7)
	{
		if (m_memcard->present() != -1)
				m_memcard->write(offset, data);
	}
}

/*************************************
 *
 *  Inter-CPU communications
 *
 *************************************/

void neogeo_state::audio_command_w(u8 data)
{
	m_soundlatch->write(data);

	m_audio_cpu_nmi_pending = true;
	audio_cpu_check_nmi();

	/* boost the interleave to let the audio CPU read the command */
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}


u8 neogeo_state::audio_command_r()
{
	u8 ret = m_soundlatch->read();

	m_audio_cpu_nmi_pending = false;
	audio_cpu_check_nmi();

	return ret;
}


CUSTOM_INPUT_MEMBER(neogeo_state::get_audio_result)
{
	u8 ret = m_soundlatch2->read();

	return ret;
}




void neogeo_state::neogeo_main_cpu_banking_init()
{
	m_use_cart_vectors = 0;

	if (m_type != NEOGEO_CD)
	{
		m_banked_cart->init_banks();
	}
}


/*************************************
 *
 *  Audio CPU banking
 *
 *************************************/

u8 neogeo_state::audio_cpu_bank_select_r(offs_t offset)
{
	m_bank_audio_cart[offset & 3]->set_entry(offset >> 8);

	return 0;
}


void neogeo_state::neogeo_audio_cpu_banking_init(int set_entry)
{
	if (m_type == NEOGEO_CD) return;

	int region;
	int bank;
	u8 *rgn;
	u32 address_mask;

	rgn = memregion("audiocpu")->base();

	/* audio bios/cartridge selection */
	m_bank_audio_main->configure_entry(1, memregion("audiocpu")->base());
	if (memregion("audiobios"))
		m_bank_audio_main->configure_entry(0, memregion("audiobios")->base());
	else /* on hardware with no SM1 ROM, the cart ROM is always enabled */
		m_bank_audio_main->configure_entry(0, memregion("audiocpu")->base());

	m_bank_audio_main->set_entry(m_use_cart_audio);

	/* audio banking */
	m_bank_audio_cart[0] = membank("audio_f000");
	m_bank_audio_cart[1] = membank("audio_e000");
	m_bank_audio_cart[2] = membank("audio_c000");
	m_bank_audio_cart[3] = membank("audio_8000");

	address_mask = (memregion("audiocpu")->bytes() - 0x10000 - 1) & 0x3ffff;


	for (region = 0; region < 4; region++)
	{
		for (bank = 0xff; bank >= 0; bank--)
		{
			u32 bank_address = 0x10000 + ((bank << (11 + region)) & address_mask);
			m_bank_audio_cart[region]->configure_entry(bank, &rgn[bank_address]);
		}
	}

	// set initial audio banks - THIS IS A HACK
	// Z80 banking is handled by the NEO-ZMC chip in the cartridge
	// (in later cartridges, by multifunction banking/protection chips that implement the same bank scheme)
	// On the real chip, initial banks are all 0.
	// However, early cartridges with less than 64KB of Z80 code and data don't have ROM banking at all.
	// These initial bank settings are required so non-banked games will work until we identify them
	// and use a different Z80 address map for them.
	m_bank_audio_cart[0]->set_entry(0x1e);
	m_bank_audio_cart[1]->set_entry(0x0e);
	m_bank_audio_cart[2]->set_entry(0x06);
	m_bank_audio_cart[3]->set_entry(0x02);
}



/*************************************
 *
 *  System control register
 *
 *************************************/

void neogeo_state::system_control_w(offs_t offset, u8 data)
{
	u8 bit = (offset >> 3) & 0x01;

	switch (offset & 0x07)
	{
		default:
		case 0x00:
			neogeo_set_screen_shadow(bit);
			break;

		case 0x01:
			if (m_type == NEOGEO_CD)
				printf("NeoCD: write to regular vector change address? %d\n", bit); // what IS going on with "neocdz doubledr" and why do games write here if it's hooked up to nothing?
			else
				m_use_cart_vectors = bit;
			break;

		case 0x05:
			if (m_type == NEOGEO_MVS)
			{
				m_use_cart_audio = bit;
				m_sprgen->neogeo_set_fixed_layer_source(bit);
				m_bank_audio_main->set_entry(m_use_cart_audio);
			}
			break;

		case 0x06:
			if (m_type == NEOGEO_MVS)
				set_save_ram_unlock(bit);
			break;

		case 0x07:
			neogeo_set_palette_bank(bit);
			break;

		case 0x02: // memory card 1: write enable/disable
		case 0x03: // memory card 2: write disable/enable
		case 0x04: // memory card: register select enable/set to normal (what does it mean?)
			logerror("PC: %x  Unmapped system control write.  Offset: %x  Data: %x\n", machine().describe_context(), offset & 0x07, bit);
			break;
	}

	if (LOG_VIDEO_SYSTEM && ((offset & 0x07) != 0x06)) logerror("PC: %x  System control write.  Offset: %x  Data: %x\n", machine().describe_context(), offset & 0x07, bit);
}



/*************************************
 *
 *  LEDs
 *
 *************************************/

void neogeo_state::set_outputs(  )
{
	static const u8 led_map[0x10] =
		{ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x58,0x4c,0x62,0x69,0x78,0x00 };

	/* EL */
	m_out_digit[0] = led_map[m_el_value];

	/* LED1 */
	m_out_digit[1] = led_map[m_led1_value >> 4];
	m_out_digit[2] = led_map[m_led1_value & 0x0f];

	/* LED2 */
	m_out_digit[3] = led_map[m_led2_value >> 4];
	m_out_digit[4] = led_map[m_led2_value & 0x0f];
}


void neogeo_state::set_output_latch( u8 data )
{
	/* looks like the LEDs are set on the
	   falling edge */
	u8 falling_bits = m_output_latch & ~data;

	if (falling_bits & 0x08)
		m_el_value = 16 - (m_output_data & 0x0f);

	if (falling_bits & 0x10)
		m_led1_value = ~m_output_data;

	if (falling_bits & 0x20)
		m_led2_value = ~m_output_data;

	if (falling_bits & 0xc7)
		logerror("%s  Unmaped LED write.  Data: %x\n", machine().describe_context(), falling_bits);

	m_output_latch = data;

	set_outputs();
}


void neogeo_state::set_output_data( u8 data )
{
	m_output_data = data;
}



/*************************************
 *
 *  Machine initialization
 *
 *************************************/

void neogeo_state::init_neogeo()
{
	m_banked_cart->install_banks(machine(), m_maincpu, m_region_maincpu->base(), m_region_maincpu->bytes());

	m_sprgen->m_fixed_layer_bank_type = 0;

	// install controllers
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x300000, 0x300001, 0, 0x01ff7e, 0, read16smo_delegate(*this, FUNC(neogeo_state::in0_r)));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x340000, 0x340001, 0, 0x01fffe, 0, read16smo_delegate(*this, FUNC(neogeo_state::in1_r)));
}


void neogeo_state::neogeo_postload()
{
	m_bank_audio_main->set_entry(m_use_cart_audio);

	if (m_type == NEOGEO_MVS) set_outputs();
}


void neogeo_state::machine_start()
{
	m_out_digit.resolve();
	m_type = NEOGEO_MVS;

	/* set the initial main CPU bank */
	neogeo_main_cpu_banking_init();

	/* set the initial audio CPU ROM banks */
	neogeo_audio_cpu_banking_init(1);

	create_interrupt_timers();

	/* irq levels for MVS / AES */
	m_vblank_level = 1;
	m_raster_level = 2;

	/* start with an IRQ3 - but NOT on a reset */
	m_irq3_pending = 1;

	// enable rtc and serial mode
	m_upd4990a->cs_w(1);
	m_upd4990a->oe_w(1);
	m_upd4990a->c0_w(1);
	m_upd4990a->c1_w(1);
	m_upd4990a->c2_w(1);

	/* register state save */
	save_item(NAME(m_display_position_interrupt_control));
	save_item(NAME(m_display_counter));
	save_item(NAME(m_vblank_interrupt_pending));
	save_item(NAME(m_display_position_interrupt_pending));
	save_item(NAME(m_irq3_pending));
	save_item(NAME(m_audio_cpu_nmi_enabled));
	save_item(NAME(m_audio_cpu_nmi_pending));
	save_item(NAME(m_save_ram_unlocked));
	save_item(NAME(m_output_data));
	save_item(NAME(m_output_latch));
	save_item(NAME(m_el_value));
	save_item(NAME(m_led1_value));
	save_item(NAME(m_led2_value));

	save_item(NAME(m_use_cart_vectors));
	save_item(NAME(m_use_cart_audio));

	machine().save().register_postload(save_prepost_delegate(FUNC(neogeo_state::neogeo_postload), this));

	m_sprgen->set_screen(m_screen);
	m_sprgen->set_sprite_region(m_region_sprites->base(), m_region_sprites->bytes());
	m_sprgen->set_fixed_regions(m_region_fixed->base(), m_region_fixed->bytes(), m_region_fixedbios);
}



/*************************************
 *
 *  Machine reset
 *
 *************************************/

void neogeo_state::machine_reset()
{
	/* reset system control registers */
	for (offs_t offs = 0; offs < 8; offs++)
		system_control_w(offs, 0);

	// disable audiocpu nmi
	m_audio_cpu_nmi_enabled = false;
	m_audio_cpu_nmi_pending = false;
	audio_cpu_check_nmi();

	m_maincpu->reset();

	start_interrupt_timers();

	/* trigger the IRQ3 that was set by MACHINE_START */
	update_interrupts();

	m_recurse = false;

	u16 game = cpuregion[0x109] * 256 + cpuregion[0x108];
	printf("Game number = %04X\n", game);
}

u16 neogeo_state::banked_vectors_r(offs_t offset)
{
	if (!m_use_cart_vectors)
	{
		u16 * bios = (u16 *)memregion("mainbios")->base();
		return bios[offset];
	}
	else
	{
		u16 * game = (u16 *)m_region_maincpu->base();
		return game[offset];
	}

}

u16 neogeo_state::neogeo_slot_rom_low_r()
{
	return 0;
}

u16 neogeo_state::neogeo_slot_rom_low_vectors_r(offs_t offset)
{
	if (!m_use_cart_vectors)
	{
		u16 * bios = (u16 *)memregion("mainbios")->base();
		return bios[offset];
	}
	else
	{
		return 0;
	}

}


/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

void neogeo_state::neogeo_main_map(address_map &map)
{
	map(0x100000,0x10ffff).mirror(0x0f0000).ram();
	/* some games have protection devices in the 0x200000 region, it appears to map to cart space, not surprising, the ROM is read here too */
	map(0x300080,0x300081).mirror(0x01ff7e).portr("TEST");
	map(0x300000,0x300001).mirror(0x01fffe).w("watchdog",FUNC(watchdog_timer_device::reset_w)).umask16(0x00ff);
	map(0x320000,0x320001).mirror(0x01fffe).portr("AUDIO_COIN");
	map(0x320000,0x320001).mirror(0x01fffe).w(FUNC(neogeo_state::audio_command_w)).umask16(0xff00);
	map(0x360000,0x37ffff).r(FUNC(neogeo_state::neogeo_unmapped_r));
	map(0x380000,0x380001).mirror(0x01fffe).portr("SYSTEM");
	map(0x380000,0x38007f).mirror(0x01ff80).w(FUNC(neogeo_state::io_control_w)).umask16(0x00ff);
	map(0x3a0000,0x3a001f).mirror(0x01ffe0).r(FUNC(neogeo_state::neogeo_unmapped_r));
	map(0x3a0000,0x3a001f).mirror(0x01ffe0).w(FUNC(neogeo_state::system_control_w)).umask16(0x00ff);
	map(0x3c0000,0x3c0007).mirror(0x01fff8).r(FUNC(neogeo_state::neogeo_video_register_r));
	map(0x3c0000,0x3c000f).mirror(0x01fff0).w(FUNC(neogeo_state::neogeo_video_register_w));
	map(0x3e0000,0x3fffff).r(FUNC(neogeo_state::neogeo_unmapped_r));
	map(0x400000,0x401fff).mirror(0x3fe000).rw(FUNC(neogeo_state::neogeo_paletteram_r),FUNC(neogeo_state::neogeo_paletteram_w));
	map(0x800000,0x800fff).rw(FUNC(neogeo_state::memcard_r),FUNC(neogeo_state::memcard_w));
	map(0xc00000,0xc1ffff).mirror(0x0e0000).rom().region("mainbios",0);
	map(0xd00000,0xd0ffff).mirror(0x0f0000).ram().w(FUNC(neogeo_state::save_ram_w)).share("saveram");
	map(0xe00000,0xffffff).r(FUNC(neogeo_state::neogeo_unmapped_r));
}


void neogeo_state::main_map_slot(address_map &map)
{
	neogeo_main_map(map);
	map(0x000000,0x00007f).r(FUNC(neogeo_state::neogeo_slot_rom_low_vectors_r));
	map(0x000080,0x0fffff).r(FUNC(neogeo_state::neogeo_slot_rom_low_r));
	map(0x200000,0x2fffff).bankr("cartridge");
//  AM_RANGE(0x2ffff0, 0x2fffff) AM_WRITE(main_cpu_bank_select_w)
}

/*************************************
 *
 *  Audio CPU memory handlers
 *
 *************************************/

void neogeo_state::audio_map(address_map &map)
{
	map(0x0000,0x7fff).bankr("audio_main");
	map(0x8000,0xbfff).bankr("audio_8000");
	map(0xc000,0xdfff).bankr("audio_c000");
	map(0xe000,0xefff).bankr("audio_e000");
	map(0xf000,0xf7ff).bankr("audio_f000");
	map(0xf800,0xffff).ram();
}



/*************************************
 *
 *  Audio CPU port handlers
 *
 *************************************/

void neogeo_state::audio_io_map(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(neogeo_state::audio_command_r)).w("soundlatch",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(neogeo_state::audio_cpu_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(neogeo_state::audio_cpu_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2",FUNC(generic_latch_8_device::write));
}



/*************************************
 *
 *  Standard Neo-Geo DIPs and
 *  input port definition
 *
 *************************************/

INPUT_PORTS_START( neogeo )
	PORT_START("DSW")
	PORT_DIPNAME( 0x01, 0x01, "Setting Mode" ) PORT_DIPLOCATION("SW:1")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Cabinet ) ) PORT_DIPLOCATION("SW:2")
	PORT_DIPSETTING(    0x02, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x00, "VS Mode" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Controller ) ) PORT_DIPLOCATION("SW:3")
	PORT_DIPSETTING(    0x04, DEF_STR( Joystick ) )
	PORT_DIPSETTING(    0x00, "Mahjong Panel" )
	PORT_DIPNAME( 0x18, 0x18, "COMM Setting (Cabinet No.)" ) PORT_DIPLOCATION("SW:4,5")
	PORT_DIPSETTING(    0x18, "1" )
	PORT_DIPSETTING(    0x10, "2" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPSETTING(    0x00, "4" )
	PORT_DIPNAME( 0x20, 0x20, "COMM Setting (Link Enable)" ) PORT_DIPLOCATION("SW:6")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Free_Play ) ) PORT_DIPLOCATION("SW:7")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "Freeze" ) PORT_DIPLOCATION("SW:8")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("SYSTEM")
	PORT_BIT( 0x00ff, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME("Next Game") PORT_CODE(KEYCODE_3)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_OTHER ) PORT_NAME("Previous Game") PORT_CODE(KEYCODE_4)
	PORT_BIT( 0x7000, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(neogeo_state, get_memcard_status)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_CUSTOM ) /* Hardware type (AES=0, MVS=1). Some games check this and show a piracy warning screen if the hardware and BIOS don't match */

	PORT_START("AUDIO_COIN")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_COIN3 ) /* What is this? "us-e" BIOS uses it as a coin input; Universe BIOS uses it to detect MVS or AES hardware */
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_COIN4 ) /* What is this? "us-e" BIOS uses it as a coin input; Universe BIOS uses it to detect MVS or AES hardware */
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_CUSTOM ) /* what is this? When ACTIVE_HIGH + IN4 bit 6 ACTIVE_LOW MVS-4 slot is detected */
	PORT_BIT( 0x0040, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("upd4990a", upd1990a_device, tp_r)
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("upd4990a", upd1990a_device, data_out_r)
	PORT_BIT( 0xff00, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_CUSTOM_MEMBER(neogeo_state,get_audio_result)

	PORT_START("TEST")
	PORT_BIT( 0x003f, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x0040, IP_ACTIVE_HIGH, IPT_CUSTOM ) /* what is this? If ACTIVE_LOW, MVS-6 slot detected, when ACTIVE_HIGH MVS-1 slot (AES) detected */
	PORT_SERVICE_NO_TOGGLE( 0x0080, IP_ACTIVE_LOW )
	PORT_BIT( 0xff00, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( neogeo_6slot )
	PORT_INCLUDE( neogeo )

	PORT_MODIFY("TEST")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_CUSTOM )
INPUT_PORTS_END




// Fixed
const gfx_layout charlayout =
{
	8,8,            /* 8 x 8 chars */
	RGN_FRAC(1,1),
	4,              /* 4 bits per pixel */
	{ 0, 1, 2, 3 },    /* planes are packed in a nibble */
	{ 33*4, 32*4, 49*4, 48*4, 1*4, 0*4, 17*4, 16*4 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	32*8    /* 32 bytes per char */
};

// Sprites
const gfx_layout tilelayout =
{
	16,16,   /* 16*16 sprites */
	RGN_FRAC(1,1),
	4,
	{ GFX_RAW },
	{ 0 },      /* org displacement */
	{ 8*8 },    /* line modulo */
	128*8       /* char modulo */
};

GFXDECODE_START( gfx_neogeo )
	GFXDECODE_ENTRY( "fixed",     0x0000, charlayout, 0, 0x1fff )
	GFXDECODE_ENTRY( "fixedbios", 0x0000, charlayout, 0, 0x1fff )
	//GFXDECODE_ENTRY( "sprites",   0x0000, tilelayout, 0, 0x1fff )  // not working
GFXDECODE_END


/*************************************
 *
 *  Machine driver
 *
 *************************************/

void neogeo_state::neogeo_base(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, NEOGEO_MAIN_CPU_CLOCK);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::neogeo_main_map);

	Z80(config, m_audiocpu, NEOGEO_AUDIO_CPU_CLOCK);
	m_audiocpu->set_addrmap(AS_PROGRAM, &neogeo_state::audio_map);
	m_audiocpu->set_addrmap(AS_IO, &neogeo_state::audio_io_map);

	/* video hardware */
	config.set_default_layout(layout_neogeo);

	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_raw(NEOGEO_PIXEL_CLOCK, NEOGEO_HTOTAL, NEOGEO_HBEND, NEOGEO_HBSTART, NEOGEO_VTOTAL, NEOGEO_VBEND, NEOGEO_VBSTART);
	m_screen->set_screen_update(FUNC(neogeo_state::screen_update_neogeo));

	GFXDECODE(config, "gfxdecode", m_palette, gfx_neogeo);
	/* 4096 colors * two banks * normal and shadow */
	PALETTE(config, m_palette, palette_device::BLACK, 4096*2*2);

	NEOGEO_SPRITE(config, m_sprgen, 0).set_screen(m_screen);

	/* audio hardware */
	SPEAKER(config, "lspeaker").front_left();
	SPEAKER(config, "rspeaker").front_right();

	GENERIC_LATCH_8(config, m_soundlatch);
	GENERIC_LATCH_8(config, m_soundlatch2);

	YM2610(config, m_ym, NEOGEO_YM2610_CLOCK);
	m_ym->irq_handler().set_inputline(m_audiocpu, 0);
	m_ym->add_route(0, "lspeaker", 0.28);
	m_ym->add_route(0, "rspeaker", 0.28);
	m_ym->add_route(1, "lspeaker", 0.98);
	m_ym->add_route(2, "rspeaker", 0.98);
	NEOGEO_BANKED_CART(config, "banked_cart");
}

void neogeo_state::neogeo_arcade(machine_config &config)
{
	neogeo_base(config);
	WATCHDOG_TIMER(config, "watchdog").set_time(attotime::from_ticks(3244030, NEOGEO_MASTER_CLOCK));
	UPD4990A(config, m_upd4990a);
	NVRAM(config, "saveram", nvram_device::DEFAULT_ALL_0);
	NG_MEMCARD(config, "memcard");
}

void neogeo_state::mvs(machine_config &config)
{
	neogeo_arcade(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::main_map_slot);

	NEOGEO_CTRL_EDGE_CONNECTOR(config, m_edge, neogeo_arc_edge, "joy", false);

	NEOGEO_CONTROL_PORT(config, "ctrl1", neogeo_arc_pin15, nullptr, false);
	NEOGEO_CONTROL_PORT(config, "ctrl2", neogeo_arc_pin15, nullptr, false);
}

void neogeo_state::main_map_noslot(address_map &map)
{
	neogeo_main_map(map);
	map(0x000000,0x00007f).r(FUNC(neogeo_state::banked_vectors_r));
	map(0x000080,0x0fffff).rom();
}

void neogeo_state::neogeo_noslot(machine_config &config)
{
	neogeo_arcade(config); // no slot config (legacy mame)
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::main_map_noslot);

	//joystick controller
	NEOGEO_CTRL_EDGE_CONNECTOR(config, m_edge, neogeo_arc_edge, "joy", true);

	//no mahjong controller
	NEOGEO_CONTROL_PORT(config, "ctrl1", neogeo_arc_pin15, nullptr, true);
	NEOGEO_CONTROL_PORT(config, "ctrl2", neogeo_arc_pin15, nullptr, true);

	MSLUGX_PROT(config, "mslugx_prot");
	SMA_PROT(config, "sma_prot");
	CMC_PROT(config, "cmc_prot");
	PCM2_PROT(config, "pcm2_prot");
	PVC_PROT(config, "pvc_prot");
	NGBOOTLEG_PROT(config, "bootleg_prot");
	KOF2002_PROT(config, "kof2002_prot");
	FATFURY2_PROT(config, "fatfury2_prot");
	KOF98_PROT(config, "kof98_prot");
	SBP_PROT(config, "sbp_prot");
}

void neogeo_state::neogeo_kog(machine_config &config)
{
	neogeo_arcade(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::main_map_noslot);

	//joystick controller
	NEOGEO_CTRL_EDGE_CONNECTOR(config, m_edge, neogeo_arc_edge, "joy", true);

	//no mahjong controller
	NEOGEO_CONTROL_PORT(config, "ctrl1", neogeo_arc_pin15, nullptr, true);
	NEOGEO_CONTROL_PORT(config, "ctrl2", neogeo_arc_pin15, nullptr, true);

	NGBOOTLEG_PROT(config, "bootleg_prot");
	KOG_PROT(config, "kog_prot");
}

// these basically correspond to the cabinets which were available in arcades:
// with mahjong panel, with dial for Pop'n Bounce and with 4 controls for Kizuna...
void neogeo_state::neogeo_mj(machine_config &config)
{
	neogeo_noslot(config);

	//no joystick panel
	NEOGEO_CTRL_EDGE_CONNECTOR(config.replace(), m_edge, neogeo_arc_edge_fixed, nullptr, true);

	//P1 mahjong controller
	config.device_remove("ctrl1");
	config.device_remove("ctrl2");
	NEOGEO_CONTROL_PORT(config, "ctrl1", neogeo_arc_pin15, "mahjong", false);
	NEOGEO_CONTROL_PORT(config, "ctrl2", neogeo_arc_pin15, nullptr, true);
}

void neogeo_state::neogeo_dial(machine_config &config)
{
	neogeo_noslot(config);
	NEOGEO_CTRL_EDGE_CONNECTOR(config.replace(), m_edge, neogeo_arc_edge_fixed, "dial", true);
}

void neogeo_state::neogeo_imaze(machine_config &config)
{
	neogeo_noslot(config);
	NEOGEO_CTRL_EDGE_CONNECTOR(config.replace(), m_edge, neogeo_arc_edge_fixed, "irrmaze", true);
}

void neogeo_state::neogeo_kiz4p(machine_config &config)
{
	neogeo_noslot(config);
	NEOGEO_CTRL_EDGE_CONNECTOR(config.replace(), m_edge, neogeo_arc_edge_fixed, "kiz4p", true);
}

// this is used by V-Liner, which handles differently inputs...
void neogeo_state::neogeo_noctrl(machine_config &config)
{
	neogeo_noslot(config);
	config.device_remove("ctrl1");
	config.device_remove("ctrl2");
}

void neogeo_state::no_watchdog(machine_config &config)
{
	neogeo_noslot(config);
	subdevice<watchdog_timer_device>("watchdog")->set_time(attotime::from_seconds(0.0));
}


void neogeo_state::gsc_map(address_map &map)
{
	main_map_noslot(map);
	map(0x900000,0x91ffff).rom().region("gsc", 0);  // extra rom
}

void neogeo_state::gsc(machine_config &config)
{
	neogeo_noslot(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::gsc_map);
}


/*************************************
 *
 *  Game-specific inits
 *
 *************************************/


/*********************************************** SMA + CMC42 */

void neogeo_state::init_kof99()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1; // only meaningful if s1 > 128k

	// decrypt p roms if needed
	u8 *ram = cpuregion;
	if (ram[0x100] != 0x45)
	{
		//printf("Maincpu=%X\n",ram[0x100]);fflush(stdout);
		m_sma_prot->kof99_decrypt_68k(cpuregion);
		m_sma_prot->kof99_install_protection(m_maincpu, m_banked_cart);
	}

	// decrypt c roms if needed
	ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF99_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = fix_region;
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_garou()
{
	init_neogeo();
	m_sma_prot->garou_decrypt_68k(cpuregion);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, GAROU_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->garou_install_protection(m_maincpu,m_banked_cart);
}

void neogeo_state::init_garouh()
{
	init_neogeo();
	m_sma_prot->garouh_decrypt_68k(cpuregion);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, GAROU_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->garouh_install_protection(m_maincpu,m_banked_cart);
}

void neogeo_state::init_mslug3()
{
	init_neogeo();
	m_sma_prot->mslug3_decrypt_68k(cpuregion);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG3_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->mslug3_install_protection(m_maincpu,m_banked_cart);
}

/*********************************************** SMA + CMC50 */

void neogeo_state::init_kof2000()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2; // only meaningful if s1 > 128k

	// decrypt p roms if needed
	u8 *ram = cpuregion;
	if (ram[0x100] != 0x45)
	{
		//printf("Maincpu=%X\n",ram[0x100]);fflush(stdout);
		m_sma_prot->kof2000_decrypt_68k(cpuregion);
		m_sma_prot->kof2000_install_protection(m_maincpu, m_banked_cart);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt c roms if needed
	ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2000_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = fix_region;
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

/*********************************************** CMC42 */

void neogeo_state::init_mslug3h()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG3_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_ganryu()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, GANRYU_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_s1945p()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, S1945P_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_preisle2()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, PREISLE2_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_bangbead()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, BANGBEAD_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_nitd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, NITD_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_sengoku3()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, SENGOKU3_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_zupapa()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, ZUPAPA_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

/*********************************************** CMC50 */


void neogeo_state::init_kof2001()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1; // only meaningful if s1 > 128k

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt c roms if needed
	u8 *ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2001_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = fix_region;
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

/*********************************************** CMC50 + PCM2 */


void neogeo_state::init_mslug4()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1; /* USA violent content screen is wrong -- not a bug, confirmed on real hardware! */
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG4_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 8);
}


void neogeo_state::init_rotd()
{
	init_neogeo();
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 16);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, ROTD_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_pnyaa()
{
	init_neogeo();
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 4);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, PNYAA_GFX_KEY );
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

/*********************************************** CMC50 + PCM2 + prg scramble */


void neogeo_state::init_kof2002()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1; // only meaningful if s1 > 128k

	// decrypt p roms if needed
	u8 *ram = cpuregion;
	if (ram[0x100002] != 0xFF)
	{
		//printf("Maincpu=%X\n",ram[0x100002]);fflush(stdout);
		m_kof2002_prot->kof2002_decrypt_68k(cpuregion, cpuregion_size);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = ym_region;
	if (ram[1] != 0x99)
	{
		//printf("ym=%X\n",ram[1]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 0);
	}

	// decrypt c roms if needed
	ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2002_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = fix_region;
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_matrim()
{
	init_neogeo();
	m_kof2002_prot->matrim_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 1);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MATRIM_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_samsho5()
{
	init_neogeo();
	m_kof2002_prot->samsho5_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 4);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SAMSHO5_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}


void neogeo_state::init_samsh5sp()
{
	init_neogeo();
	m_kof2002_prot->samsh5sp_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 6);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SAMSHO5SP_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

/*********************************************** CMC50 + PCM2 + PVC */


void neogeo_state::init_mslug5()
{
	init_neogeo();
	m_pvc_prot->mslug5_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 2);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG5_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
}


void neogeo_state::init_svc()
{
	init_neogeo();
	m_pvc_prot->svc_px_decrypt(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 3);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SVC_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
}


void neogeo_state::init_kof2003()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2; // for those sets with 512k of s1

	// decrypt p roms if needed
	u8 *ram = cpuregion;
	if (ram[0x100] != 0x45)
	{
		//printf("Maincpu=%X\n",ram[0x100]);fflush(stdout);
		m_pvc_prot->kof2003_decrypt_68k(cpuregion, cpuregion_size);
		m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = ym_region;
	if (ram[0x91] != 0x33)
	{
		//printf("ym=%X\n",ram[0x91]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	}

	// decrypt c roms if needed
	ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = fix_region;
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_kof2003b() // hacks of kf2k3bl
{
	init_neogeo();
	m_bootleg_prot->kf2k3bl_install_protection(m_maincpu, m_banked_cart, cpuregion, cpuregion_size);

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	u8 *ram = ym_region;
	if (ram[0x91] != 0x33)
	{
		//printf("ym=%X\n",ram[0x91]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	}

	// decrypt c roms if needed
	ram = spr_region;
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	}

	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size, 1);
}

void neogeo_state::init_kof2003h()
{
	init_neogeo();
	m_pvc_prot->kof2003h_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
}

/*********************************************** misc carts */

void neogeo_state::init_mslugx()
{
	init_neogeo();
	m_mslugx_prot->mslugx_install_protection(m_maincpu);
}

void neogeo_state::init_fatfury2()
{
	init_neogeo();
	m_fatfury2_prot->fatfury2_install_protection(m_maincpu,m_banked_cart);
}

void neogeo_state::init_kof98()
{
	init_neogeo();
	m_kof98_prot->kof98_decrypt_68k(cpuregion, cpuregion_size);
	m_kof98_prot->install_kof98_protection(m_maincpu);
}

void neogeo_state::init_sbp()
{
	init_neogeo();
	m_sbp_prot->sbp_install_protection(m_maincpu, cpuregion, cpuregion_size);
}


void neogeo_state::init_jockeygp()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, JOCKEYGP_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);

	/* install some extra RAM */
	m_maincpu->space(AS_PROGRAM).install_ram(0x200000, 0x201fff);

//  m_maincpu->space(AS_PROGRAM).install_read_port(0x280000, 0x280001, "IN5");
//  m_maincpu->space(AS_PROGRAM).install_read_port(0x2c0000, 0x2c0001, "IN6");
}

void neogeo_state::init_vliner()
{
	m_banked_cart->install_banks(machine(), m_maincpu, m_region_maincpu->base(), m_region_maincpu->bytes());

	m_sprgen->m_fixed_layer_bank_type = 0;

	m_maincpu->space(AS_PROGRAM).install_ram(0x200000, 0x201fff);

	m_maincpu->space(AS_PROGRAM).install_read_port(0x300000, 0x300001, 0x01ff7e, "DSW");
	m_maincpu->space(AS_PROGRAM).install_read_port(0x280000, 0x280001, "IN5");
	m_maincpu->space(AS_PROGRAM).install_read_port(0x2c0000, 0x2c0001, "IN6");

}


/*********************************************** bootlegs */

void neogeo_state::init_garoubl()
{
	init_neogeo();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
	m_bootleg_prot->neogeo_bootleg_cx_decrypt(spr_region, spr_region_size);
}

void neogeo_state::init_cthd2003()
{
	init_neogeo();
	m_bootleg_prot->decrypt_cthd2003(spr_region, spr_region_size, audiocpu_region,audio_region_size, fix_region, fix_region_size);
	m_bootleg_prot->patch_cthd2003(m_maincpu,m_banked_cart, cpuregion, cpuregion_size);
}

void neogeo_state::init_ct2k3sp()
{
	init_neogeo();
	m_bootleg_prot->decrypt_ct2k3sp(spr_region, spr_region_size, audiocpu_region,audio_region_size, fix_region, fix_region_size);
	m_bootleg_prot->patch_cthd2003(m_maincpu,m_banked_cart, cpuregion, cpuregion_size);
}

void neogeo_state::init_ct2k3sa()
{
	init_neogeo();
	m_bootleg_prot->decrypt_ct2k3sa(spr_region, spr_region_size, audiocpu_region,audio_region_size);
	m_bootleg_prot->patch_ct2k3sa(cpuregion, cpuregion_size);
}



void neogeo_state::init_kf10thep()
{
	init_neogeo();
	m_bootleg_prot->kf10thep_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
}

void neogeo_state::init_kf2k5uni()
{
	init_neogeo();
	m_bootleg_prot->decrypt_kf2k5uni(cpuregion, cpuregion_size, audiocpu_region,audio_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_kof2k4se()
{
	init_neogeo();
	m_bootleg_prot->decrypt_kof2k4se_68k(cpuregion, cpuregion_size);
}

void neogeo_state::init_svcplus()
{
	init_neogeo();
	m_bootleg_prot->svcplus_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->svcboot_cx_decrypt(spr_region, spr_region_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size, 1);
	m_bootleg_prot->svcplus_px_hack(cpuregion, cpuregion_size);
}

void neogeo_state::init_svcplusa()
{
	init_neogeo();
	m_bootleg_prot->svcplusa_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->svcboot_cx_decrypt(spr_region, spr_region_size);
	m_bootleg_prot->svcplus_px_hack(cpuregion, cpuregion_size);
}

void neogeo_state::init_samsho5b()
{
	init_neogeo();
	m_bootleg_prot->samsho5b_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->samsho5b_vx_decrypt(ym_region, ym_region_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_bootleg_prot->neogeo_bootleg_cx_decrypt(spr_region, spr_region_size);
}

void neogeo_state::init_kof97oro()
{
	init_neogeo();
	m_bootleg_prot->kof97oro_px_decode(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_bootleg_prot->neogeo_bootleg_cx_decrypt(spr_region, spr_region_size);
}

void neogeo_state::init_lans2004()
{
	init_neogeo();
	m_bootleg_prot->lans2004_decrypt_68k(cpuregion, cpuregion_size);
	m_bootleg_prot->lans2004_vx_decrypt(ym_region, ym_region_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_bootleg_prot->neogeo_bootleg_cx_decrypt(spr_region, spr_region_size);
}

void neogeo_state::init_kof10th()
{
	init_neogeo();
	m_bootleg_prot->decrypt_kof10th(cpuregion, cpuregion_size);
	m_bootleg_prot->install_kof10th_protection(m_maincpu,m_banked_cart, cpuregion, cpuregion_size, fix_region, fix_region_size);
}


void neogeo_state::init_kog()
{
	init_neogeo();

	m_kog_prot->kog_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_bootleg_prot->neogeo_bootleg_cx_decrypt(spr_region, spr_region_size);
	m_kog_prot->kog_install_protection(m_maincpu);
}



/*********************************************** bootlegs - can use original prot */


void neogeo_state::init_ms4plus()
{
	init_neogeo();
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG4_GFX_KEY);
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 8);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
}


/*********************************************** bootleg hybrid */


void neogeo_state::init_mslug3b6()
{
	init_neogeo();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG3_GFX_KEY);
}

void neogeo_state::init_kof2002b()
{
	init_neogeo();
	m_kof2002_prot->kof2002_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 0);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_bootleg_prot->kof2002b_gfx_decrypt(spr_region, spr_region_size);
	m_bootleg_prot->kof2002b_gfx_decrypt(fix_region, fix_region_size);
}

void neogeo_state::init_kf2k2mp()
{
	init_neogeo();
	m_bootleg_prot->kf2k2mp_decrypt(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 0);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2002_GFX_KEY);
}

void neogeo_state::init_kf2k2mp2()
{
	init_neogeo();
	m_bootleg_prot->kf2k2mp2_px_decrypt(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 0);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2002_GFX_KEY);
}

void neogeo_state::init_matrimbl()
{
	init_neogeo();
	m_kof2002_prot->matrim_decrypt_68k(cpuregion, cpuregion_size);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_bootleg_prot->matrimbl_decrypt(spr_region, spr_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}


void neogeo_state::init_ms5plus()
{
	init_neogeo();
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG5_GFX_KEY);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 2);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_bootleg_prot->install_ms5plus_protection(m_maincpu,m_banked_cart);
}



void neogeo_state::init_svcboot()
{
	init_neogeo();
	m_bootleg_prot->svcboot_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->svcboot_cx_decrypt(spr_region, spr_region_size);
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
}


void neogeo_state::init_svcsplus()
{
	init_neogeo();
	m_bootleg_prot->svcsplus_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
	m_bootleg_prot->svcboot_cx_decrypt(spr_region, spr_region_size);
	m_bootleg_prot->svcsplus_px_hack(cpuregion, cpuregion_size);
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
}


void neogeo_state::init_kf2k3pl()
{
	init_neogeo();
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_bootleg_prot->kf2k3pl_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	m_bootleg_prot->kf2k3pl_install_protection(m_maincpu, m_banked_cart, cpuregion, cpuregion_size);
}

void neogeo_state::init_kf2k3upl()
{
	init_neogeo();
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_bootleg_prot->kf2k3upl_px_decrypt(cpuregion, cpuregion_size);
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
	m_bootleg_prot->kf2k3bl_install_protection(m_maincpu, m_banked_cart, cpuregion, cpuregion_size);
}






/*********************************************** non-carts */

void neogeo_state::install_banked_bios()
{
	m_maincpu->space(AS_PROGRAM).install_read_bank(0xc00000, 0xc1ffff, 0x0e0000, "bankedbios");
	membank("bankedbios")->configure_entries(0, 2, memregion("mainbios")->base(), 0x20000);
	membank("bankedbios")->set_entry(1);
}

INPUT_CHANGED_MEMBER(neogeo_state::select_bios)
{
	membank("bankedbios")->set_entry(newval ? 0 : 1);
}

void neogeo_state::init_ms5pcb()
{
	init_neogeo();

	m_pvc_prot->mslug5_decrypt_68k(cpuregion, cpuregion_size);
	m_sma_prot->svcpcb_gfx_decrypt(spr_region, spr_region_size);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG5_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_sma_prot->svcpcb_s1data_decrypt(fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 2);
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
	install_banked_bios();
}


void neogeo_state::init_svcpcb()
{
	init_neogeo();

	m_pvc_prot->svc_px_decrypt(cpuregion, cpuregion_size);
	m_sma_prot->svcpcb_gfx_decrypt(spr_region, spr_region_size);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SVC_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->svcpcb_s1data_decrypt(fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 3);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
	install_banked_bios();
}


void neogeo_state::init_kf2k3pcb()
{
	init_neogeo();
	m_pvc_prot->kf2k3pcb_decrypt_68k(cpuregion, cpuregion_size);
	m_sma_prot->kf2k3pcb_gfx_decrypt(spr_region, spr_region_size);
	m_sma_prot->kf2k3pcb_sp1_decrypt((u16 *)memregion("mainbios")->base());
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);

	/* extra little swap on the m1 - this must be performed AFTER the m1 decrypt
	   or the m1 checksum (used to generate the key) for decrypting the m1 is
	   incorrect */
	{
		u8* rom = memregion("audiocpu")->base();
		for (int i = 0; i < 0x90000; i++)
			rom[i] = bitswap<8>(rom[i], 5, 6, 1, 4, 3, 0, 7, 2);
	}

	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, KOF2003_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->kf2k3pcb_decrypt_s1data(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_pvc_prot->install_pvc_protection(m_maincpu,m_banked_cart);
	m_maincpu->space(AS_PROGRAM).install_rom(0xc00000, 0xc7ffff, 0x080000, memregion("mainbios")->base());  // 512k bios
}

void neogeo_state::init_cmc42sfix()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}


/* dummy entry for the dummy bios driver */
ROM_START( neogeo )
	NEOGEO_BIOS

	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )

	ROM_REGION( 0x20000, "audiobios", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_REGION( 0x50000, "audiocpu", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_Y_ZOOM

	ROM_REGION( 0x20000, "fixed", ROMREGION_ERASEFF )

	ROM_REGION( 0x20000, "fixedbios", 0 )
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) )

	ROM_REGION( 0x100000, "sprites", ROMREGION_ERASEFF )
ROM_END


/*    YEAR  NAME        PARENT    MACHINE   INPUT            CLASS         INIT    */
GAME( 1990, neogeo,     0,        mvs,      neogeo_6slot,   neogeo_state, init_neogeo,  ROT0, "SNK", "Neo-Geo", MACHINE_IS_BIOS_ROOT | MACHINE_SUPPORTS_SAVE )
