// license:BSD-3-Clause
// copyright-holders:Paul Leaman, Andreas Naive, Nicola Salmoria (original works), Robbbert (HBMAME additions)
/***************************************************************************

The first 600-odd lines of information have been removed and can be found
in the MAME source.

Local numbering scheme: I made this up as a convenience - it is NOT OFFICIAL


Number    Date         Name
---------------------------------------------------------------------------------------------------------------
C248    1993-09-10     Super Street Fighter II ( New Challengers / Tournament Battle )
C249    1993-12-03     Eco Fighters / Ultimate Ecology
C250    1994-01-13     Dungeons & Dragons: Tower of Doom
C251    1994-02-23     Super Street Fighter II { Turbo / X:Grand Master Challenge }
C252    1994-05-20     Alien vs Predator
C253    1994-07-05     { Darkstalkers / Vampire } : The Night Warriors
C254    1994-08-08     Ring of Destruction: Slammasters II / Super Muscle Bomber: The International Blowout
C255    1994-09-16     Armored Warriors / Powered Gear: Strategic Variant Armor Equipment
C256    1994-12-08     X-Men: Children of the Atom
C257    1995-03-02     { Night Warriors / Vampire Hunter } : Darkstalkers' Revenge
C258    1995-04-20     Cyberbots: Full Metal Madness
C259    1995-06-05     Street Fighter { Alpha / Zero }
C260    1995-09-22     { Megaman / Rockman } : The Power Battle
C261    1995-10-24     Marvel Super Heroes
C262    1995-12-07     19XX: The War Against Destiny
C263    1996-02-06     Dungeons & Dragons: Shadow over Mystara
C264    1996-02-27     Street Fighter { Alpha / Zero } 2
C265    1996-05-29     Super Puzzle Fighter II { Turbo / X }
C266    1996-07-08     { Megaman / Rockman } 2: The Power Fighters
C267    1996-08-05     Street Fighter { Alpha / Zero } 2 Gold (possibly Street Fighter Zero 2 Alpha)
C268    1996-08-26     Quiz Nanairo Dreams: Nijiiocho no Kiseki
C269    1996-09-09     X-Men vs Street Fighter
C270    1997-03-19     Battle Circuit
C271    1997-05-19     Darkstalkers 3 / Vampire Savior: The Lord of Vampire
C272    1997-06-20     Marvel Super Heroes vs Street Fighter
C273    1997-07-22     Capcom Sports Club
C274    1997-09-04     Super Gem Fighter Mini Mix / Pocket Fighter
C275    1997-09-13     Vampire Hunter 2: Darkstalkers' Revenge
C276    1997-09-13     Vampire Savior 2: The Lord of Vampire
C277    1998-01-12     Marvel vs Capcom: Clash of Super Heroes
C278    1998-06-29     Street Fighter { Alpha / Zero } 3
C279    1999-02-22     Giga Wing
C280    1999-05-27     Jyangokushi: Hao no Saihai
C281    2000-01-21     Dimahoo / Great Maho Daisakusen
C282    2000-04-12     Mars Matrix: Hyper Solid Shooting
C283    2000-06-20     1944: The Loop Master
C284    2000-10-10     Mighty! Pang
C285    2001-01-17     Progear no Arashi
C286    2001-02-05     Puzz Loop 2
C287    2001-08-20     Janpai Puzzle Choko
C288    2003-12-22     Hyper Street Fighter II: The Anniversary Edition


Extra CPS-2 info:

CPS-2 Encryption

All credit goes to Andreas Naive for breaking the encryption algorithm.
Code by Nicola Salmoria.
Thanks to Charles MacDonald and Razoola for extracting the data from the hardware.


The encryption only affects opcodes, not data.

It consists of two 4-round Feistel networks (FN) and involves both
the 16-bit opcode and the low 16 bits of the address.

Let be:

E = 16-bit ciphertext
A = 16-bit address
K = 64-bit key
D = 16-bit plaintext
y = FN1(x,k) = function describing the first Feistel network (x,y = 16 bit, k = 64 bit)
y = FN2(x,k) = function describing the second Feistel network (x,y = 16 bit, k = 64 bit)
y = EX(x) = fixed function that expands the 16-bit x to the 64-bit y

Then the cipher can be described as:

D = FN2( E, K XOR EX( FN1(A, K ) ) )


Each round of the Feistel networks consists of four substitution boxes. The boxes
have 6 inputs and 2 outputs. Usually the input is the XOR of a data bit and a key
bit, however in some cases only the key is used.

(TODO-notes about accuracy of s-boxes)

The s-boxes were chosen in order to use an empty key (all FF) for the dead board.


Also, the hardware has different watchdog opcodes and address range (see below)
which are stored in the battery backed RAM. There doesn't appear to be any relation
between those and the 64-bit encryption key, so they probably use an additional
64 bits of battery-backed RAM.



First FN:

 B(0 1 3 5 8 9 11 12)        A(10 4 6 7 2 13 15 14)
         L0                             R0
         |                              |
        XOR<-----------[F1]<------------|
         |                              |
         R1                             L1
         |                              |
         |------------>[F2]----------->XOR
         |                              |
         L2                             R2
         |                              |
        XOR<-----------[F3]<------------|
         |                              |
         R3                             L3
         |                              |
         |------------>[F4]----------->XOR
         |                              |
         L4                             R4
  (10 4 6 7 2 13 15 14)       (0 1 3 5 8 9 11 12)


Second FN:

 B(3 5 9 10 8 15 12 11)      A(6 0 2 13 1 4 14 7)
         L0                             R0
         |                              |
        XOR<-----------[F1]<------------|
         |                              |
         R1                             L1
         |                              |
         |------------>[F2]----------->XOR
         |                              |
         L2                             R2
         |                              |
        XOR<-----------[F3]<------------|
         |                              |
         R3                             L3
         |                              |
         |------------>[F4]----------->XOR
         |                              |
         L4                             R4
  (6 0 2 13 1 4 14 7)         (3 5 9 10 8 15 12 11)


Some Encryption notes.
----------------------

Address range.

The encryption does _not_ cover the entire address space. The range covered
differs per game.


Encryption Watchdog.

The CPS2 system has a watchdog system that will disable the decryption
of data if the watchdog isn't triggered at least once every few seconds.
The trigger varies from game to game (some games do use the same) and is
basically a 68000 opcode/s instruction. The instruction is the same for
all regions of the game. The watchdog instructions are listed alongside
the decryption keys.

*******************************************************************************/

#include "emu.h"
#include "includes/cps2.h"
#include "cpu/m68000/m68000.h"
#include "cpu/z80/z80.h"
#include "machine/eepromser.h"
#include "sound/okim6295.h"
#include "sound/qsound.h"
#include "speaker.h"
#include "ui/uimain.h"

static const int fn1_groupA[8] = { 10, 4, 6, 7, 2, 13, 15, 14 };
static const int fn1_groupB[8] = {  0, 1, 3, 5, 8,  9, 11, 12 };

static const int fn2_groupA[8] = { 6, 0, 2, 13, 1,  4, 14,  7 };
static const int fn2_groupB[8] = { 3, 5, 9, 10, 8, 15, 12, 11 };


// The order of the input and output bits in the s-boxes is arbitrary.
// Each s-box can be XORed with an arbitrary vale in range 0-3 (but the same value
// must be used for the corresponding output bits in f1 and f3 or in f2 and f4)

struct sbox
{
	const u8 table[64];
	const int inputs[6];        // positions of the inputs bits, -1 means no input except from key
	const int outputs[2];       // positions of the output bits
};

// the above struct better defines how the hardware works, however
// to speed up the decryption at run time we convert it to the
// following one
struct optimised_sbox
{
	u8 input_lookup[256];
	u8 output[64];
};


static const struct sbox fn1_r1_boxes[4] =
{
	{   // subkey bits  0- 5
		{
			0,2,2,0,1,0,1,1,3,2,0,3,0,3,1,2,1,1,1,2,1,3,2,2,2,3,3,2,1,1,1,2,
			2,2,0,0,3,1,3,1,1,1,3,0,0,1,0,0,1,2,2,1,2,3,2,2,2,3,1,3,2,0,1,3,
		},
		{ 3, 4, 5, 6, -1, -1 },
		{ 3, 6 }
	},
	{   // subkey bits  6-11
		{
			3,0,2,2,2,1,1,1,1,2,1,0,0,0,2,3,2,3,1,3,0,0,0,2,1,2,2,3,0,3,3,3,
			0,1,3,2,3,3,3,1,1,1,1,2,0,1,2,1,3,2,3,1,1,3,2,2,2,3,1,3,2,3,0,0,
		},
		{ 0, 1, 2, 4, 7, -1 },
		{ 2, 7 }
	},
	{   // subkey bits 12-17
		{
			3,0,3,1,1,0,2,2,3,1,2,0,3,3,2,3,0,1,0,1,2,3,0,2,0,2,0,1,0,0,1,0,
			2,3,1,2,1,0,2,0,2,1,0,1,0,2,1,0,3,1,2,3,1,3,1,1,1,2,0,2,2,0,0,0,
		},
		{ 0, 1, 2, 3, 6, 7 },
		{ 0, 1 }
	},
	{   // subkey bits 18-23
		{
			3,2,0,3,0,2,2,1,1,2,3,2,1,3,2,1,2,2,1,3,3,2,1,0,1,0,1,3,0,0,0,2,
			2,1,0,1,0,1,0,1,3,1,1,2,2,3,2,0,3,3,2,0,2,1,3,3,0,0,3,0,1,1,3,3,
		},
		{ 0, 1, 3, 5, 6, 7 },
		{ 4, 5 }
	},
};

static const struct sbox fn1_r2_boxes[4] =
{
	{   // subkey bits 24-29
		{
			3,3,2,0,3,0,3,1,0,3,0,1,0,2,1,3,1,3,0,3,3,1,3,3,3,2,3,2,2,3,1,2,
			0,2,2,1,0,1,2,0,3,3,0,1,3,2,1,2,3,0,1,3,0,1,2,2,1,2,1,2,0,1,3,0,
		},
		{ 0, 1, 2, 3, 6, -1 },
		{ 1, 6 }
	},
	{   // subkey bits 30-35
		{
			1,2,3,2,1,3,0,1,1,0,2,0,0,2,3,2,3,3,0,1,2,2,1,0,1,0,1,2,3,2,1,3,
			2,2,2,0,1,0,2,3,2,1,2,1,2,1,0,3,0,1,2,3,1,2,1,3,2,0,3,2,3,0,2,0,
		},
		{ 2, 4, 5, 6, 7, -1 },
		{ 5, 7 }
	},
	{   // subkey bits 36-41
		{
			0,1,0,2,1,1,0,1,0,2,2,2,1,3,0,0,1,1,3,1,2,2,2,3,1,0,3,3,3,2,2,2,
			1,1,3,0,3,1,3,0,1,3,3,2,1,1,0,0,1,2,2,2,1,1,1,2,2,0,0,3,2,3,1,3,
		},
		{ 1, 2, 3, 4, 5, 7 },
		{ 0, 3 }
	},
	{   // subkey bits 42-47
		{
			2,1,0,3,3,3,2,0,1,2,1,1,1,0,3,1,1,3,3,0,1,2,1,0,0,0,3,0,3,0,3,0,
			1,3,3,3,0,3,2,0,2,1,2,2,2,1,1,3,0,1,0,1,0,1,1,1,1,3,1,0,1,2,3,3,
		},
		{ 0, 1, 3, 4, 6, 7 },
		{ 2, 4 }
	},
};

static const struct sbox fn1_r3_boxes[4] =
{
	{   // subkey bits 48-53
		{
			0,0,0,3,3,1,1,0,2,0,2,0,0,0,3,2,0,1,2,3,2,2,1,0,3,0,0,0,0,0,2,3,
			3,0,0,1,1,2,3,3,0,1,3,2,0,1,3,3,2,0,0,1,0,2,0,0,0,3,1,3,3,3,3,3,
		},
		{ 0, 1, 5, 6, 7, -1 },
		{ 0, 5 }
	},
	{   // subkey bits 54-59
		{
			2,3,2,3,0,2,3,0,2,2,3,0,3,2,0,2,1,0,2,3,1,1,1,0,0,1,0,2,1,2,2,1,
			3,0,2,1,2,3,3,0,3,2,3,1,0,2,1,0,1,2,2,3,0,2,1,3,1,3,0,2,1,1,1,3,
		},
		{ 2, 3, 4, 6, 7, -1 },
		{ 6, 7 }
	},
	{   // subkey bits 60-65
		{
			3,0,2,1,1,3,1,2,2,1,2,2,2,0,0,1,2,3,1,0,2,0,0,2,3,1,2,0,0,0,3,0,
			2,1,1,2,0,0,1,2,3,1,1,2,0,1,3,0,3,1,1,0,0,2,3,0,0,0,0,3,2,0,0,0,
		},
		{ 0, 2, 3, 4, 5, 6 },
		{ 1, 4 }
	},
	{   // subkey bits 66-71
		{
			0,1,0,0,2,1,3,2,3,3,2,1,0,1,1,1,1,1,0,3,3,1,1,0,0,2,2,1,0,3,3,2,
			1,3,3,0,3,0,2,1,1,2,3,2,2,2,1,0,0,3,3,3,2,2,3,1,0,2,3,0,3,1,1,0,
		},
		{ 0, 1, 2, 3, 5, 7 },
		{ 2, 3 }
	},
};

static const struct sbox fn1_r4_boxes[4] =
{
	{   // subkey bits 72-77
		{
			1,1,1,1,1,0,1,3,3,2,3,0,1,2,0,2,3,3,0,1,2,1,2,3,0,3,2,3,2,0,1,2,
			0,1,0,3,2,1,3,2,3,1,2,3,2,0,1,2,2,0,0,0,2,1,3,0,3,1,3,0,1,3,3,0,
		},
		{ 1, 2, 3, 4, 5, 7 },
		{ 0, 4 }
	},
	{   // subkey bits 78-83
		{
			3,0,0,0,0,1,0,2,3,3,1,3,0,3,1,2,2,2,3,1,0,0,2,0,1,0,2,2,3,3,0,0,
			1,1,3,0,2,3,0,3,0,3,0,2,0,2,0,1,0,3,0,1,3,1,1,0,0,1,3,3,2,2,1,0,
		},
		{ 0, 1, 2, 3, 5, 6 },
		{ 1, 3 }
	},
	{   // subkey bits 84-89
		{
			0,1,1,2,0,1,3,1,2,0,3,2,0,0,3,0,3,0,1,2,2,3,3,2,3,2,0,1,0,0,1,0,
			3,0,2,3,0,2,2,2,1,1,0,2,2,0,0,1,2,1,1,1,2,3,0,3,1,2,3,3,1,1,3,0,
		},
		{ 0, 2, 4, 5, 6, 7 },
		{ 2, 6 }
	},
	{   // subkey bits 90-95
		{
			0,1,2,2,0,1,0,3,2,2,1,1,3,2,0,2,0,1,3,3,0,2,2,3,3,2,0,0,2,1,3,3,
			1,1,1,3,1,2,1,1,0,3,3,2,3,2,3,0,3,1,0,0,3,0,0,0,2,2,2,1,2,3,0,0,
		},
		{ 0, 1, 3, 4, 6, 7 },
		{ 5, 7 }
	},
};

/******************************************************************************/

static const struct sbox fn2_r1_boxes[4] =
{
	{   // subkey bits  0- 5
		{
			2,0,2,0,3,0,0,3,1,1,0,1,3,2,0,1,2,0,1,2,0,2,0,2,2,2,3,0,2,1,3,0,
			0,1,0,1,2,2,3,3,0,3,0,2,3,0,1,2,1,1,0,2,0,3,1,1,2,2,1,3,1,1,3,1,
		},
		{ 0, 3, 4, 5, 7, -1 },
		{ 6, 7 }
	},
	{   // subkey bits  6-11
		{
			1,1,0,3,0,2,0,1,3,0,2,0,1,1,0,0,1,3,2,2,0,2,2,2,2,0,1,3,3,3,1,1,
			1,3,1,3,2,2,2,2,2,2,0,1,0,1,1,2,3,1,1,2,0,3,3,3,2,2,3,1,1,1,3,0,
		},
		{ 1, 2, 3, 4, 6, -1 },
		{ 3, 5 }
	},
	{   // subkey bits 12-17
		{
			1,0,2,2,3,3,3,3,1,2,2,1,0,1,2,1,1,2,3,1,2,0,0,1,2,3,1,2,0,0,0,2,
			2,0,1,1,0,0,2,0,0,0,2,3,2,3,0,1,3,0,0,0,2,3,2,0,1,3,2,1,3,1,1,3,
		},
		{ 1, 2, 4, 5, 6, 7 },
		{ 1, 4 }
	},
	{   // subkey bits 18-23
		{
			1,3,3,0,3,2,3,1,3,2,1,1,3,3,2,1,2,3,0,3,1,0,0,2,3,0,0,0,3,3,0,1,
			2,3,0,0,0,1,2,1,3,0,0,1,0,2,2,2,3,3,1,2,1,3,0,0,0,3,0,1,3,2,2,0,
		},
		{ 0, 2, 3, 5, 6, 7 },
		{ 0, 2 }
	},
};

static const struct sbox fn2_r2_boxes[4] =
{
	{   // subkey bits 24-29
		{
			3,1,3,0,3,0,3,1,3,0,0,1,1,3,0,3,1,1,0,1,2,3,2,3,3,1,2,2,2,0,2,3,
			2,2,2,1,1,3,3,0,3,1,2,1,1,1,0,2,0,3,3,0,0,2,0,0,1,1,2,1,2,1,1,0,
		},
		{ 0, 2, 4, 6, -1, -1 },
		{ 4, 6 }
	},
	{   // subkey bits 30-35
		{
			0,3,0,3,3,2,1,2,3,1,1,1,2,0,2,3,0,3,1,2,2,1,3,3,3,2,1,2,2,0,1,0,
			2,3,0,1,2,0,1,1,2,0,2,1,2,0,2,3,3,1,0,2,3,3,0,3,1,1,3,0,0,1,2,0,
		},
		{ 1, 3, 4, 5, 6, 7 },
		{ 0, 3 }
	},
	{   // subkey bits 36-41
		{
			0,0,2,1,3,2,1,0,1,2,2,2,1,1,0,3,1,2,2,3,2,1,1,0,3,0,0,1,1,2,3,1,
			3,3,2,2,1,0,1,1,1,2,0,1,2,3,0,3,3,0,3,2,2,0,2,2,1,2,3,2,1,0,2,1,
		},
		{ 0, 1, 3, 4, 5, 7 },
		{ 1, 7 }
	},
	{   // subkey bits 42-47
		{
			0,2,1,2,0,2,2,0,1,3,2,0,3,2,3,0,3,3,2,3,1,2,3,1,2,2,0,0,2,2,1,2,
			2,3,3,3,1,1,0,0,0,3,2,0,3,2,3,1,1,1,1,0,1,0,1,3,0,0,1,2,2,3,2,0,
		},
		{ 1, 2, 3, 5, 6, 7 },
		{ 2, 5 }
	},
};

static const struct sbox fn2_r3_boxes[4] =
{
	{   // subkey bits 48-53
		{
			2,1,2,1,2,3,1,3,2,2,1,3,3,0,0,1,0,2,0,3,3,1,0,0,1,1,0,2,3,2,1,2,
			1,1,2,1,1,3,2,2,0,2,2,3,3,3,2,0,0,0,0,0,3,3,3,0,1,2,1,0,2,3,3,1,
		},
		{ 2, 3, 4, 6, -1, -1 },
		{ 3, 5 }
	},
	{   // subkey bits 54-59
		{
			3,2,3,3,1,0,3,0,2,0,1,1,1,0,3,0,3,1,3,1,0,1,2,3,2,2,3,2,0,1,1,2,
			3,0,0,2,1,0,0,2,2,0,1,0,0,2,0,0,1,3,1,3,2,0,3,3,1,0,2,2,2,3,0,0,
		},
		{ 0, 1, 3, 5, 7, -1 },
		{ 0, 2 }
	},
	{   // subkey bits 60-65
		{
			2,2,1,0,2,3,3,0,0,0,1,3,1,2,3,2,2,3,1,3,0,3,0,3,3,2,2,1,0,0,0,2,
			1,2,2,2,0,0,1,2,0,1,3,0,2,3,2,1,3,2,2,2,3,1,3,0,2,0,2,1,0,3,3,1,
		},
		{ 0, 1, 2, 3, 5, 7 },
		{ 1, 6 }
	},
	{   // subkey bits 66-71
		{
			1,2,3,2,0,2,1,3,3,1,0,1,1,2,2,0,0,1,1,1,2,1,1,2,0,1,3,3,1,1,1,2,
			3,3,1,0,2,1,1,1,2,1,0,0,2,2,3,2,3,2,2,0,2,2,3,3,0,2,3,0,2,2,1,1,
		},
		{ 0, 2, 4, 5, 6, 7 },
		{ 4, 7 }
	},
};

static const struct sbox fn2_r4_boxes[4] =
{
	{   // subkey bits 72-77
		{
			2,0,1,1,2,1,3,3,1,1,1,2,0,1,0,2,0,1,2,0,2,3,0,2,3,3,2,2,3,2,0,1,
			3,0,2,0,2,3,1,3,2,0,0,1,1,2,3,1,1,1,0,1,2,0,3,3,1,1,1,3,3,1,1,0,
		},
		{ 0, 1, 3, 6, 7, -1 },
		{ 0, 3 }
	},
	{   // subkey bits 78-83
		{
			1,2,2,1,0,3,3,1,0,2,2,2,1,0,1,0,1,1,0,1,0,2,1,0,2,1,0,2,3,2,3,3,
			2,2,1,2,2,3,1,3,3,3,0,1,0,1,3,0,0,0,1,2,0,3,3,2,3,2,1,3,2,1,0,2,
		},
		{ 0, 1, 2, 4, 5, 6 },
		{ 4, 7 }
	},
	{   // subkey bits 84-89
		{
			2,3,2,1,3,2,3,0,0,2,1,1,0,0,3,2,3,1,0,1,2,2,2,1,3,2,2,1,0,2,1,2,
			0,3,1,0,0,3,1,1,3,3,2,0,1,0,1,3,0,0,1,2,1,2,3,2,1,0,0,3,2,1,1,3,
		},
		{ 0, 2, 3, 4, 5, 7 },
		{ 1, 2 }
	},
	{   // subkey bits 90-95
		{
			2,0,0,3,2,2,2,1,3,3,1,1,2,0,0,3,1,0,3,2,1,0,2,0,3,2,2,3,2,0,3,0,
			1,3,0,2,2,1,3,3,0,1,0,3,1,1,3,2,0,3,0,2,3,2,1,3,2,3,0,0,1,3,2,1,
		},
		{ 2, 3, 4, 5, 6, 7 },
		{ 5, 6 }
	},
};

/******************************************************************************/


static u8 fn(u8 in, const struct optimised_sbox *sboxes, u32 key)
{
	const struct optimised_sbox *sbox1 = &sboxes[0];
	const struct optimised_sbox *sbox2 = &sboxes[1];
	const struct optimised_sbox *sbox3 = &sboxes[2];
	const struct optimised_sbox *sbox4 = &sboxes[3];

	return
		sbox1->output[sbox1->input_lookup[in] ^ ((key >>  0) & 0x3f)] |
		sbox2->output[sbox2->input_lookup[in] ^ ((key >>  6) & 0x3f)] |
		sbox3->output[sbox3->input_lookup[in] ^ ((key >> 12) & 0x3f)] |
		sbox4->output[sbox4->input_lookup[in] ^ ((key >> 18) & 0x3f)];
}



// srckey is the 64-bit master key (2x32 bits)
// dstkey will contain the 96-bit key for the 1st FN (4x24 bits)
static void expand_1st_key(u32 *dstkey, const u32 *srckey)
{
	static const int bits[96] =
	{
		33, 58, 49, 36,  0, 31,
		22, 30,  3, 16,  5, 53,
		10, 41, 23, 19, 27, 39,
		43,  6, 34, 12, 61, 21,
		48, 13, 32, 35,  6, 42,
		43, 14, 21, 41, 52, 25,
		18, 47, 46, 37, 57, 53,
		20,  8, 55, 54, 59, 60,
		27, 33, 35, 18,  8, 15,
		63,  1, 50, 44, 16, 46,
		5,  4, 45, 51, 38, 25,
		13, 11, 62, 29, 48,  2,
		59, 61, 62, 56, 51, 57,
		54,  9, 24, 63, 22,  7,
		26, 42, 45, 40, 23, 14,
		2, 31, 52, 28, 44, 17,
	};
	int i;

	dstkey[0] = 0;
	dstkey[1] = 0;
	dstkey[2] = 0;
	dstkey[3] = 0;

	for (i = 0; i < 96; ++i)
		dstkey[i / 24] |= BIT(srckey[bits[i] / 32], bits[i] % 32) << (i % 24);
}


// srckey is the 64-bit master key (2x32 bits) XORed with the subkey
// dstkey will contain the 96-bit key for the 2nd FN (4x24 bits)
static void expand_2nd_key(u32 *dstkey, const u32 *srckey)
{
	static const int bits[96] =
	{
		34,  9, 32, 24, 44, 54,
		38, 61, 47, 13, 28,  7,
		29, 58, 18,  1, 20, 60,
		15,  6, 11, 43, 39, 19,
		63, 23, 16, 62, 54, 40,
		31,  3, 56, 61, 17, 25,
		47, 38, 55, 57,  5,  4,
		15, 42, 22,  7,  2, 19,
		46, 37, 29, 39, 12, 30,
		49, 57, 31, 41, 26, 27,
		24, 36, 11, 63, 33, 16,
		56, 62, 48, 60, 59, 32,
		12, 30, 53, 48, 10,  0,
		50, 35,  3, 59, 14, 49,
		51, 45, 44,  2, 21, 33,
		55, 52, 23, 28,  8, 26,
	};
	int i;

	dstkey[0] = 0;
	dstkey[1] = 0;
	dstkey[2] = 0;
	dstkey[3] = 0;

	for (i = 0; i < 96; ++i)
		dstkey[i / 24] |= BIT(srckey[bits[i] / 32], bits[i] % 32) << (i % 24);
}



// seed is the 16-bit seed generated by the first FN
// subkey will contain the 64-bit key to be XORed with the master key
// for the 2nd FN (2x32 bits)
static void expand_subkey(u32* subkey, u16 seed)
{
	// Note that each row of the table is a permutation of the seed bits.
	static const int bits[64] =
	{
		5, 10, 14,  9,  4,  0, 15,  6,  1,  8,  3,  2, 12,  7, 13, 11,
		5, 12,  7,  2, 13, 11,  9, 14,  4,  1,  6, 10,  8,  0, 15,  3,
		4, 10,  2,  0,  6,  9, 12,  1, 11,  7, 15,  8, 13,  5, 14,  3,
		14, 11, 12,  7,  4,  5,  2, 10,  1, 15,  0,  9,  8,  6, 13,  3,
	};
	int i;

	subkey[0] = 0;
	subkey[1] = 0;

	for (i = 0; i < 64; ++i)
		subkey[i / 32] |= BIT(seed, bits[i]) << (i % 32);
}



static u16 feistel(u16 val, const int *bitsA, const int *bitsB,
		const struct optimised_sbox* boxes1, const struct optimised_sbox* boxes2,
		const struct optimised_sbox* boxes3, const struct optimised_sbox* boxes4,
		u32 key1, u32 key2, u32 key3, u32 key4)
{
	u8 l = bitswap<8>(val, bitsB[7],bitsB[6],bitsB[5],bitsB[4],bitsB[3],bitsB[2],bitsB[1],bitsB[0]);
	u8 r = bitswap<8>(val, bitsA[7],bitsA[6],bitsA[5],bitsA[4],bitsA[3],bitsA[2],bitsA[1],bitsA[0]);

	l ^= fn(r, boxes1, key1);
	r ^= fn(l, boxes2, key2);
	l ^= fn(r, boxes3, key3);
	r ^= fn(l, boxes4, key4);

	return
		(BIT(l, 0) << bitsA[0]) |
		(BIT(l, 1) << bitsA[1]) |
		(BIT(l, 2) << bitsA[2]) |
		(BIT(l, 3) << bitsA[3]) |
		(BIT(l, 4) << bitsA[4]) |
		(BIT(l, 5) << bitsA[5]) |
		(BIT(l, 6) << bitsA[6]) |
		(BIT(l, 7) << bitsA[7]) |
		(BIT(r, 0) << bitsB[0]) |
		(BIT(r, 1) << bitsB[1]) |
		(BIT(r, 2) << bitsB[2]) |
		(BIT(r, 3) << bitsB[3]) |
		(BIT(r, 4) << bitsB[4]) |
		(BIT(r, 5) << bitsB[5]) |
		(BIT(r, 6) << bitsB[6]) |
		(BIT(r, 7) << bitsB[7]);
}



static int extract_inputs(u32 val, const int *inputs)
{
	int res = 0;

	for (u8 i = 0; i < 6; ++i)
	{
		if (inputs[i] != -1)
			res |= BIT(val, inputs[i]) << i;
	}

	return res;
}



static void optimise_sboxes(struct optimised_sbox* out, const struct sbox* in)
{
	int box, i;

	for (box = 0; box < 4; ++box)
	{
		// precalculate the input lookup
		for (i = 0; i < 256; ++i)
			out[box].input_lookup[i] = extract_inputs(i, in[box].inputs);

		// precalculate the output masks
		for (i = 0; i < 64; ++i)
		{
			int o = in[box].table[i];

			out[box].output[i] = 0;
			if (o & 1)
				out[box].output[i] |= 1 << in[box].outputs[0];
			if (o & 2)
				out[box].output[i] |= 1 << in[box].outputs[1];
		}
	}
}



static void cps2_decrypt(running_machine &machine, u16 *rom, u16 *dec, int length, const u32 *master_key, u32 lower_limit, u32 upper_limit)
{
	int i;
	u32 key1[4] = { };
	struct optimised_sbox sboxes1[4*4];
	struct optimised_sbox sboxes2[4*4];

	optimise_sboxes(&sboxes1[0*4], fn1_r1_boxes);
	optimise_sboxes(&sboxes1[1*4], fn1_r2_boxes);
	optimise_sboxes(&sboxes1[2*4], fn1_r3_boxes);
	optimise_sboxes(&sboxes1[3*4], fn1_r4_boxes);
	optimise_sboxes(&sboxes2[0*4], fn2_r1_boxes);
	optimise_sboxes(&sboxes2[1*4], fn2_r2_boxes);
	optimise_sboxes(&sboxes2[2*4], fn2_r3_boxes);
	optimise_sboxes(&sboxes2[3*4], fn2_r4_boxes);


	// expand master key to 1st FN 96-bit key
	expand_1st_key(key1, master_key);

	// add extra bits for s-boxes with less than 6 inputs
	key1[0] ^= BIT(key1[0], 1) <<  4;
	key1[0] ^= BIT(key1[0], 2) <<  5;
	key1[0] ^= BIT(key1[0], 8) << 11;
	key1[1] ^= BIT(key1[1], 0) <<  5;
	key1[1] ^= BIT(key1[1], 8) << 11;
	key1[2] ^= BIT(key1[2], 1) <<  5;
	key1[2] ^= BIT(key1[2], 8) << 11;

	for (i = 0; i < 0x10000; ++i)
	{
		int a = 0;
		u16 seed = 0;
		u32 subkey[2] = { };
		u32 key2[4] = { };

		if ((i & 0xff) == 0)
		{
			char loadingMessage[256]; // for displaying with UI
			sprintf(loadingMessage, "Decrypting %d%%", i*100/0x10000);
			machine.ui().set_startup_text(loadingMessage,false);
		}

		// pass the address through FN1
		seed = feistel(i, fn1_groupA, fn1_groupB,
				&sboxes1[0*4], &sboxes1[1*4], &sboxes1[2*4], &sboxes1[3*4],
				key1[0], key1[1], key1[2], key1[3]);

		// expand the result to 64-bit
		expand_subkey(subkey, seed);

		// XOR with the master key
		subkey[0] ^= master_key[0];
		subkey[1] ^= master_key[1];

		// expand key to 2nd FN 96-bit key
		expand_2nd_key(key2, subkey);

		// add extra bits for s-boxes with less than 6 inputs
		key2[0] ^= BIT(key2[0], 0) <<  5;
		key2[0] ^= BIT(key2[0], 6) << 11;
		key2[1] ^= BIT(key2[1], 0) <<  5;
		key2[1] ^= BIT(key2[1], 1) <<  4;
		key2[2] ^= BIT(key2[2], 2) <<  5;
		key2[2] ^= BIT(key2[2], 3) <<  4;
		key2[2] ^= BIT(key2[2], 7) << 11;
		key2[3] ^= BIT(key2[3], 1) <<  5;


		// decrypt the opcodes
		for (a = i; a < length/2; a += 0x10000)
		{
			if (a >= lower_limit && a <= upper_limit)
			{
				dec[a] = feistel(rom[a], fn2_groupA, fn2_groupB,
					&sboxes2[0 * 4], &sboxes2[1 * 4], &sboxes2[2 * 4], &sboxes2[3 * 4],
					key2[0], key2[1], key2[2], key2[3]);
			}
			else
				dec[a] = rom[a];
		}
	}
}

struct game_keys
{
	const char *name;             /* game driver name */
	const u32 keys[2];
	u32 upper_limit;
};

void cps2_state::init_cps2crypt()
{
	if (m_region_key)
	{
		u32 key[2] = { };
		u32 upper, lower = 0;

		unsigned short decoded[10] = { };
		//memset(decoded, 0, sizeof(decoded));

		for (int b = 0; b < 10 * 16; b++)
		{
			int bit = (317 - b) % 160;

			if ((m_region_key->base()[bit / 8] >> ((bit ^ 7) % 8)) & 1)
				decoded[b / 16] |= (0x8000 >> (b % 16));
		}

		key[0] = (decoded[0] << 16) | decoded[1];
		key[1] = (decoded[2] << 16) | decoded[3];
		// decoded[4] == watchdog instruction third word
		// decoded[5] == watchdog instruction second word
		// decoded[6] == watchdog instruction first word
		// decoded[7] == 0x4000 (bits 8 to 23 of CPS2 object output address)
		// decoded[8] == 0x0900

		if (decoded[9] == 0xffff)
		{
			// On a dead board, the only encrypted range is actually FF0000-FFFFFF.
			// It doesn't start from 0, and it's the upper half of a 128kB bank.
			upper = 0xffffff;
			lower = 0xff0000;
		}
		else
			upper = (((~decoded[9] & 0x3ff) << 14) | 0x3fff) + 1;

		logerror("cps2 decrypt 0x%08x,0x%08x,0x%08x,0x%08x\n", key[0], key[1], lower, upper);

		// we have a proper key so use it to decrypt
		cps2_decrypt(machine(), (u16 *)memregion("maincpu")->base(), m_decrypted_opcodes, memregion("maincpu")->bytes(), key, lower / 2, upper / 2);
	}
}

/*************************************
 *
 *  Constants
 *
 *************************************/

/* Maximum size of Q Sound Z80 region */
#define QSOUND_SIZE 0x50000

/* Maximum 680000 code size */
#undef  CODE_SIZE
#define CODE_SIZE   0x0400000

u16 cps2_state::qsound_sharedram1_r(offs_t offset)
{
	return m_qsound_sharedram1[offset] | 0xff00;
}

void cps2_state::qsound_sharedram1_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (ACCESSING_BITS_0_7)
		m_qsound_sharedram1[offset] = data;
}

u16 cps2_state::qsound_sharedram2_r(offs_t offset)
{
	return m_qsound_sharedram2[offset] | 0xff00;
}

void cps2_state::qsound_sharedram2_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (ACCESSING_BITS_0_7)
		m_qsound_sharedram2[offset] = data;
}

void cps2_state::qsound_banksw_w(u8 data)
{
	/* Z80 bank register for music note data. */
	int bank = data & 0x0f;
	if ((0x10000 + (bank * 0x4000)) >= memregion("audiocpu")->bytes())
	{
		logerror("WARNING: Q sound bank overflow (%02x)\n", data);
		bank = 0;
	}

	membank("bank1")->set_entry(bank);
}

u16 cps2_state::cps1_in2_r()
{
	int in = ioport("IN2")->read();
	return (in << 8) | in;
}

u16 cps2_state::cps1_in3_r()
{
	int in = ioport("IN3")->read();
	return (in << 8) | in;
}


/*************************************
 *
 *  Interrupt generation
 *
 *************************************/

TIMER_DEVICE_CALLBACK_MEMBER(cps2_state::cps2_interrupt)
{
	/* 2 is vblank, 4 is some sort of scanline interrupt, 6 is both at the same time. */
	if (param == 0)
		m_scancalls = 0;

	if (m_cps_b_regs[0x10 / 2] & 0x8000)
		m_cps_b_regs[0x10 / 2] &= 0x1ff;

	if (m_cps_b_regs[0x12 / 2] & 0x8000)
		m_cps_b_regs[0x12 / 2] &= 0x1ff;

//  popmessage("%04x %04x - %04x %04x",m_scanline1,m_scanline2,m_cps_b_regs[0x10/2],m_cps_b_regs[0x12/2]);

	/* raster effects */
	if (m_scanline1 == param || (m_scanline1 < param && !m_scancalls))
	{
		m_cps_b_regs[0x10/2] = 0;
		m_maincpu->set_input_line(4, HOLD_LINE);
		cps2_set_sprite_priorities();
		m_screen->update_partial(param);
		m_scancalls++;
//      popmessage("IRQ4 scancounter = %04i", param);
	}

	/* raster effects */
	if(m_scanline2 == param || (m_scanline2 < param && !m_scancalls))
	{
		m_cps_b_regs[0x12 / 2] = 0;
		m_maincpu->set_input_line(4, HOLD_LINE);
		cps2_set_sprite_priorities();
		m_screen->update_partial(param);
		m_scancalls++;
//      popmessage("IRQ4 scancounter = %04i", param);
	}

	if (param == 240)  /* VBlank */
	{
		m_cps_b_regs[0x10 / 2] = m_scanline1;
		m_cps_b_regs[0x12 / 2] = m_scanline2;
		m_maincpu->set_input_line(2, HOLD_LINE);
		if(m_scancalls)
		{
			cps2_set_sprite_priorities();
			m_screen->update_partial(256);
		}
		cps2_objram_latch();
	}
//  popmessage("Raster calls = %i", m_scancalls);
}


/*************************************
 *
 *  EEPROM
 *
 *************************************/

void cps2_state::cps2_eeprom_port_w(offs_t offset, u16 data, u16 mem_mask)
{
	if (ACCESSING_BITS_8_15)
	{
		/* bit 0 - Unused */
		/* bit 1 - Unused */
		/* bit 2 - Unused */
		/* bit 3 - Unused? */
		/* bit 4 - Eeprom data  */
		/* bit 5 - Eeprom clock */
		/* bit 6 - */
		/* bit 7 - */

		/* EEPROM */
		ioport("EEPROMOUT")->write(data, 0xffff);
		if (m_cps2_dial_type == 2) // ecofghtr
		{
			m_readpaddle = (data & 0x0100);
		}
	}

	if (ACCESSING_BITS_0_7)
	{
		/* bit 0 - coin counter 1 */
		/* bit 0 - coin counter 2 */
		/* bit 2 - Unused */
		/* bit 3 - Allows access to Z80 address space (Z80 reset) */
		/* bit 4 - lock 1  */
		/* bit 5 - lock 2  */
		/* bit 6 - */
		/* bit 7 - */

			/* Z80 Reset */
		if (m_audiocpu != nullptr)
			m_audiocpu->set_input_line(INPUT_LINE_RESET, (data & 0x0008) ? CLEAR_LINE : ASSERT_LINE);

		machine().bookkeeping().coin_counter_w(0, data & 0x0001);
		if (m_cps2_dial_type == 1) // pzloop2
		{
			// Puzz Loop 2 uses coin counter 2 input to switch between stick and paddle controls
			m_readpaddle = data & 0x0002;
		}
		else
		{
			machine().bookkeeping().coin_counter_w(1, data & 0x0002);
		}

		if (strncmp(machine().system().name, "mmatrix", 7) == 0)      // Mars Matrix seems to require the coin lockout bit to be reversed
		{
			machine().bookkeeping().coin_lockout_w(0, data & 0x0010);
			machine().bookkeeping().coin_lockout_w(1, data & 0x0020);
			machine().bookkeeping().coin_lockout_w(2, data & 0x0040);
			machine().bookkeeping().coin_lockout_w(3, data & 0x0080);
		}
		else
		{
			machine().bookkeeping().coin_lockout_w(0, ~data & 0x0010);
			machine().bookkeeping().coin_lockout_w(1, ~data & 0x0020);
			machine().bookkeeping().coin_lockout_w(2, ~data & 0x0040);
			machine().bookkeeping().coin_lockout_w(3, ~data & 0x0080);
		}

		/*
		output().set_led_value(0, data & 0x01);
		output().set_led_value(1, data & 0x10);
		output().set_led_value(2, data & 0x20);
		*/
	}
}


/*************************************
 *
 *  Sound ?
 *
 *************************************/

TIMER_CALLBACK_MEMBER(cps2_state::cps2_update_digital_volume)
{
	int vol_button_state;

	vol_button_state = ioport("DIGITALVOL")->read();

	if (vol_button_state & 0x01) m_cps2digitalvolumelevel -= 1;
	if (vol_button_state & 0x02) m_cps2digitalvolumelevel += 1;

	if (m_cps2digitalvolumelevel > 39) m_cps2digitalvolumelevel = 39;
	if (m_cps2digitalvolumelevel < 0) m_cps2digitalvolumelevel = 0;

	m_qsound->set_output_gain(0, m_cps2digitalvolumelevel / 39.0);
	m_qsound->set_output_gain(1, m_cps2digitalvolumelevel / 39.0);
}

u16 cps2_state::cps2_qsound_volume_r()
{
	static const u16 cps2_vol_states[40] =
	{
		0xf010, 0xf008, 0xf004, 0xf002, 0xf001, 0xe810, 0xe808, 0xe804, 0xe802, 0xe801,
		0xe410, 0xe408, 0xe404, 0xe402, 0xe401, 0xe210, 0xe208, 0xe204, 0xe202, 0xe201,
		0xe110, 0xe108, 0xe104, 0xe102, 0xe101, 0xe090, 0xe088, 0xe084, 0xe082, 0xe081,
		0xe050, 0xe048, 0xe044, 0xe042, 0xe041, 0xe030, 0xe028, 0xe024, 0xe022, 0xe021
	};

	u16 result;

	result = cps2_vol_states[m_cps2digitalvolumelevel];

	/* Extra adapter memory (0x660000-0x663fff) available when bit 14 = 0 */
	/* Network adapter (ssf2tb) present when bit 15 = 0 */
	/* Only game known to use both these so far is SSF2TB */

	if (m_cps2networkpresent)
		return 0x2021; /* SSF2TB doesn't have a digital slider in the test screen */
	else
		if (m_cps2disabledigitalvolume)
			return 0xd000; /* digital display isn't shown in test mode */
		else
			return result;
}


/*************************************
 *
 *  Read handlers
 *
 *************************************/

u16 cps2_state::kludge_r()
{
	return 0xffff;
}

u16 cps2_state::joy_or_paddle_r()
{
	if (m_readpaddle != 0)
	{
		return (ioport("IN0")->read());
	}
	else
	{
		return (ioport("PADDLE1")->read() & 0xff) | (ioport("PADDLE2")->read() << 8);
	}
}

u16 cps2_state::joy_or_paddle_ecofghtr_r()
{
	if (m_readpaddle == 0 || (m_io_in1->read() & 0x10) == 0x10) // ignore bit if spinner not enabled
	{
		u16 ret = m_io_in0->read();

		if ((m_io_in1->read() & 0x10) == 0x00)
		{
			ret = ret & 0xdfdf;

			ret |= m_ecofghtr_dial_direction1 << 13;
			ret |= m_ecofghtr_dial_direction0 << 5;
		}

		return ret;
	}
	else
	{
		int dial0 = (ioport("DIAL0")->read());
		int dial1 = (ioport("DIAL1")->read());

		u16 ret = (dial0 & 0xff) | ((dial1 & 0xff) << 8);

		// 1st dial
		if ((dial0 & 0x800) == (m_ecofghtr_dial_last0 & 0x800))
		{
			if (dial0 > m_ecofghtr_dial_last0) m_ecofghtr_dial_direction0 = 1;
			else  m_ecofghtr_dial_direction0 = 0;
		}
		// catch wraparound of value
		else if ((dial0 & 0x800) > (m_ecofghtr_dial_last0 & 0x800)) // value gone from 0x000 to 0xfff
		{
			m_ecofghtr_dial_direction0 = 0;
		}
		else if ((dial0 & 0x800) < (m_ecofghtr_dial_last0 & 0x800)) // value gone from 0xfff to 0x000
		{
			m_ecofghtr_dial_direction0 = 1;
		}

		// 2nd dial
		if ((dial1 & 0x800) == (m_ecofghtr_dial_last1 & 0x800))
		{
			if (dial1 > m_ecofghtr_dial_last1) m_ecofghtr_dial_direction1 = 1;
			else  m_ecofghtr_dial_direction1 = 0;
		}
		// catch wraparound of value
		else if ((dial1 & 0x800) > (m_ecofghtr_dial_last1 & 0x800)) // value gone from 0x000 to 0xfff
		{
			m_ecofghtr_dial_direction1 = 0;
		}
		else if ((dial1 & 0x800) < (m_ecofghtr_dial_last1 & 0x800)) // value gone from 0xfff to 0x000
		{
			m_ecofghtr_dial_direction1 = 1;
		}

		m_ecofghtr_dial_last0 = dial0;
		m_ecofghtr_dial_last1 = dial1;

		return ret;
	}
}


/*************************************
 *
 *  Memory map
 *
 *************************************/

void cps2_state::cps2_map(address_map &map)
{
	map(0x000000, 0x3fffff).rom();                                                                         /* 68000 ROM */
	map(0x400000, 0x40000b).ram().share("output");                                                      /* CPS2 object output */
	map(0x618000, 0x619fff).rw( FUNC(cps2_state::qsound_sharedram1_r), FUNC(cps2_state::qsound_sharedram1_w));                         /* Q RAM */
	map(0x660000, 0x663fff).ram();                                                                         /* When bit 14 of 0x804030 equals 0 this space is available. Many games store highscores and other info here if available. */
	map(0x662000, 0x662001).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x662008, 0x662009).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x662020, 0x662021).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x664000, 0x664001).ram();                                                                         /* Unknown - Only used if 0x660000-0x663fff available (could be RAM enable?) */
	map(0x700000, 0x701fff).mirror(0x006000).rw( FUNC(cps2_state::cps2_objram1_r), FUNC(cps2_state::cps2_objram1_w)).share("objram1");    /* Object RAM, no game seems to use it directly */
	map(0x708000, 0x709fff).mirror(0x006000).rw( FUNC(cps2_state::cps2_objram2_r), FUNC(cps2_state::cps2_objram2_w)).share("objram2");           /* Object RAM */
	map(0x800100, 0x80013f).w( FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                              /* mirror (sfa) */
	map(0x800140, 0x80017f).rw( FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w)).share("cps_b_regs");            /* mirror (sfa) */
	map(0x804000, 0x804001).portr("IN0");                                                            /* IN0 */
	map(0x804010, 0x804011).portr("IN1");                                                            /* IN1 */
	map(0x804020, 0x804021).portr("IN2");                                                            /* IN2 + EEPROM */
	map(0x804030, 0x804031).r( FUNC(cps2_state::cps2_qsound_volume_r));                                               /* Master volume. Also when bit 14=0 addon memory is present, when bit 15=0 network adapter present. */
	map(0x804040, 0x804041).w( FUNC(cps2_state::cps2_eeprom_port_w));                                                    /* EEPROM */
	map(0x8040a0, 0x8040a1).nopw();                                                                    /* Unknown (reset once on startup) */
	map(0x8040b0, 0x8040b3).r( FUNC(cps2_state::kludge_r));                                                               /* unknown (xmcotaj hangs if this is 0) */
	map(0x8040e0, 0x8040e1).w( FUNC(cps2_state::cps2_objram_bank_w));                                                   /* bit 0 = Object ram bank swap */
	map(0x804100, 0x80413f).w( FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                          /* CPS-A custom */
	map(0x804140, 0x80417f).rw( FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w));                                       /* CPS-B custom */
	map(0x900000, 0x92ffff).ram().w( FUNC(cps2_state::cps1_gfxram_w)).share("gfxram"); /* Video RAM */
	map(0xff0000, 0xffffff).ram();                                                                         /* RAM */
}

void cps2_state::decrypted_opcodes_map(address_map &map)
{
	map(0x000000, 0x3fffff).rom().share("decrypted_opcodes");                                           /* 68000 ROM */
}

void cps2_state::dead_cps2_map(address_map &map)
{
	map(0x000000, 0x3fffff).rom();                                                                         /* 68000 ROM */
	map(0x400000, 0x40000b).ram().share("output");                                                      /* CPS2 object output */
	map(0x618000, 0x619fff).rw( FUNC(cps2_state::qsound_sharedram1_r), FUNC(cps2_state::qsound_sharedram1_w));                         /* Q RAM */
	map(0x660000, 0x663fff).ram();                                                                         /* When bit 14 of 0x804030 equals 0 this space is available. Many games store highscores and other info here if available. */
	map(0x662000, 0x662001).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x662008, 0x662009).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x662020, 0x662021).ram();                                                                         /* Network adapter related, accessed in SSF2TB */
	map(0x664000, 0x664001).ram();                                                                         /* Unknown - Only used if 0x660000-0x663fff available (could be RAM enable?) */
	map(0x700000, 0x701fff).rw( FUNC(cps2_state::cps2_objram1_r), FUNC(cps2_state::cps2_objram1_w)).share("objram1");    /* Object RAM, no game seems to use it directly */
	map(0x708000, 0x709fff).mirror(0x006000).rw( FUNC(cps2_state::cps2_objram2_r), FUNC(cps2_state::cps2_objram2_w)).share("objram2");           /* Object RAM */
	map(0x800100, 0x80013f).w( FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                              /* mirror (sfa) */
	map(0x800140, 0x80017f).rw( FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w)).share("cps_b_regs");            /* mirror (sfa) */
	map(0x804000, 0x804001).portr("IN0");                                                            /* IN0 */
	map(0x804010, 0x804011).portr("IN1");                                                            /* IN1 */
	map(0x804020, 0x804021).portr("IN2");                                                            /* IN2 + EEPROM */
	map(0x804030, 0x804031).r( FUNC(cps2_state::cps2_qsound_volume_r));                                               /* Master volume. Also when bit 14=0 addon memory is present, when bit 15=0 network adapter present. */
	map(0x804040, 0x804041).w( FUNC(cps2_state::cps2_eeprom_port_w));                                                    /* EEPROM */
	map(0x8040a0, 0x8040a1).nopw();                                                                    /* Unknown (reset once on startup) */
	map(0x8040b0, 0x8040b3).r( FUNC(cps2_state::kludge_r));                                                               /* unknown (xmcotaj hangs if this is 0) */
	map(0x8040e0, 0x8040e1).w( FUNC(cps2_state::cps2_objram_bank_w));                                                   /* bit 0 = Object ram bank swap */
	map(0x804100, 0x80413f).w( FUNC(cps2_state::cps1_cps_a_w)).share("cps_a_regs");                              /* CPS-A custom */
	map(0x804140, 0x80417f).rw( FUNC(cps2_state::cps1_cps_b_r), FUNC(cps2_state::cps1_cps_b_w));                                       /* CPS-B custom */
	map(0x900000, 0x92ffff).ram().w( FUNC(cps2_state::cps1_gfxram_w)).share("gfxram"); /* Video RAM */
	map(0xff0000, 0xffffef).ram();                                                                         /* RAM */
	map(0xfffff0, 0xfffffb).ram().share("output");                                                      /* CPS2 output */
	map(0xfffffc, 0xffffff).ram();
}

void cps2_state::qsound_sub_map(address_map &map)
{
	map(0x0000,0x7fff).rom();
	map(0x8000,0xbfff).bankr("bank1");  /* banked (contains music data) */
	map(0xc000,0xcfff).ram().share("qsound_ram1");
	map(0xd000,0xd002).w("qsound",FUNC(qsound_device::qsound_w));
	map(0xd003,0xd003).w(FUNC(cps2_state::qsound_banksw_w));
	map(0xd007,0xd007).r("qsound",FUNC(qsound_device::qsound_r));
	map(0xf000,0xffff).ram().share("qsound_ram2");
}


/*************************************
 *
 *  Generic port definitions
 *
 *************************************/

/* 4 players and 4 buttons */
static INPUT_PORTS_START( cps2_4p4b )
	PORT_START("IN0")      /* (0x00) */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)

	PORT_START("IN1")      /* (0x10) */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(3)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(3)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(3)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(3)
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(4)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(4)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(4)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(4)

	PORT_START("IN2")      /* (0x20) */
	PORT_BIT( 0x0001, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_DEVICE_MEMBER("eeprom", eeprom_serial_93cxx_device, do_read)
	PORT_SERVICE_NO_TOGGLE( 0x0002, IP_ACTIVE_LOW )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x00f8, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_START3 )
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_START4 )
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_COIN3 )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_COIN4 )

	PORT_START( "EEPROMOUT" )
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("eeprom", eeprom_serial_93cxx_device, di_write)
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("eeprom", eeprom_serial_93cxx_device, clk_write)
	PORT_BIT( 0x4000, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("eeprom", eeprom_serial_93cxx_device, cs_write)

	/* fake inputs for digital volume buttons */
	PORT_START( "DIGITALVOL" )
	PORT_BIT( 0x0001, IP_ACTIVE_HIGH, IPT_VOLUME_DOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_HIGH, IPT_VOLUME_UP )
INPUT_PORTS_END

/* 4 players and 3 buttons */
static INPUT_PORTS_START( cps2_4p3b )
	PORT_INCLUDE(cps2_4p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(1) */
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(2) */

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(3) */
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(4) */
INPUT_PORTS_END

/* 4 players and 2 buttons */
static INPUT_PORTS_START( cps2_4p2b )
	PORT_INCLUDE(cps2_4p3b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(1) */
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(2) */

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(3) */
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(4) */
INPUT_PORTS_END

/* 3 players and 4 buttons */
static INPUT_PORTS_START( cps2_3p4b )
	PORT_INCLUDE(cps2_4p4b)

	PORT_MODIFY("IN1")
	PORT_BIT( 0xff00, IP_ACTIVE_LOW, IPT_UNUSED )           /* PORT_PLAYER(4) inputs */

	PORT_MODIFY("IN2")
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_UNUSED )           /* START4 */
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )           /* COIN4 */
INPUT_PORTS_END

/* 3 players and 3 buttons */
static INPUT_PORTS_START( cps2_3p3b )
	PORT_INCLUDE(cps2_3p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(1) */
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(2) */

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(3) */
INPUT_PORTS_END

/* 3 players and 2 buttons */
#ifdef UNUSED_DEFINITION
static INPUT_PORTS_START( cps2_3p2b )
	PORT_INCLUDE(cps2_3p3b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(1) */
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(2) */

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(3) */
INPUT_PORTS_END
#endif

/* 2 players and 4 buttons */
static INPUT_PORTS_START( cps2_2p4b )
	PORT_INCLUDE(cps2_3p4b)

	PORT_MODIFY("IN1")
	PORT_BIT( 0x00ff, IP_ACTIVE_LOW, IPT_UNUSED )           /* PORT_PLAYER(3) inputs */

	PORT_MODIFY("IN2")
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_UNUSED )           /* START3 */
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )           /* COIN3 */
INPUT_PORTS_END

/* 2 players and 3 buttons */
static INPUT_PORTS_START( cps2_2p3b )
	PORT_INCLUDE(cps2_2p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(1) */
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(2) */
INPUT_PORTS_END

static INPUT_PORTS_START( ecofghtr )
	PORT_INCLUDE(cps2_2p3b)

	PORT_MODIFY("IN1")
	// If this bit is set then the Test Mode shows 'Rolling' instead of Shot2/3 - this is the 'spinner' mode.
	// in this mode the IN0 port is multiplexed with an analog input, what would be Button 2 ends up being 'direction'
	PORT_CONFNAME( 0x10, 0x00, "Use Spinners" )
	PORT_CONFSETTING(    0x00, DEF_STR( Yes ) )
	PORT_CONFSETTING(    0x10, DEF_STR( No ) )

	PORT_START("DIAL0")
	PORT_BIT( 0x0fff, 0x0000, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(20) PORT_CODE_DEC(KEYCODE_Z) PORT_CODE_INC(KEYCODE_X) PORT_PLAYER(1)

	PORT_START("DIAL1")
	PORT_BIT( 0x0fff, 0x0000, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(20) PORT_CODE_DEC(KEYCODE_N) PORT_CODE_INC(KEYCODE_M) PORT_PLAYER(2)
INPUT_PORTS_END



/* 2 players and 2 buttons */
static INPUT_PORTS_START( cps2_2p2b )
	PORT_INCLUDE(cps2_2p3b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(1) */
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(2) */
INPUT_PORTS_END

/* 2 players and 1 button */
static INPUT_PORTS_START( cps2_2p1b )
	PORT_INCLUDE(cps2_2p2b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON2 PORT_PLAYER(1) */
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON2 PORT_PLAYER(2) */
INPUT_PORTS_END

/* 2 players and 6 buttons (2 rows of 3 buttons) */
static INPUT_PORTS_START( cps2_2p6b )
	PORT_INCLUDE(cps2_2p3b)

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(2)

	PORT_MODIFY("IN2")
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(2)
INPUT_PORTS_END

/* 2 players, 6 buttons, and 1 ticket dispenser (2 rows of 3 buttons) */
static INPUT_PORTS_START( cps2_2p6bt )
	PORT_INCLUDE(cps2_2p6b)

	PORT_MODIFY("IN1")
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_UNUSED )           /* Ticket Empty line*/
INPUT_PORTS_END

/* 1 player and 4 buttons */
static INPUT_PORTS_START( cps2_1p4b )
	PORT_INCLUDE(cps2_2p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0xff00, IP_ACTIVE_LOW, IPT_UNUSED )           /* PORT_PLAYER(2) inputs */

	PORT_MODIFY("IN2")
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_UNUSED )           /* START2 */
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_UNUSED )           /* COIN2 */
INPUT_PORTS_END

/* 1 player and 3 buttons */
static INPUT_PORTS_START( cps2_1p3b )
	PORT_INCLUDE(cps2_1p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON4 PORT_PLAYER(1) */
INPUT_PORTS_END

/* 1 player and 2 buttons */
static INPUT_PORTS_START( cps2_1p2b )
	PORT_INCLUDE(cps2_1p3b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )           /* BUTTON3 PORT_PLAYER(1) */
INPUT_PORTS_END


/*************************************
 *
 *  Game-specific port definitions
 *
 *************************************/

/* According to the "test mode", buttons layout look like a 2 players 6 buttons machine where buttons have been removed */
static INPUT_PORTS_START( cybots )
	PORT_INCLUDE(cps2_2p6b)

	PORT_MODIFY("IN1")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNUSED )                     /* BUTTON5 PORT_PLAYER(1) */
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNUSED )                     /* BUTTON6 PORT_PLAYER(1) */
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNUSED )                     /* BUTTON5 PORT_PLAYER(2) */

	PORT_MODIFY("IN2")
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )                     /* BUTTON6 PORT_PLAYER(2) */
INPUT_PORTS_END

/* 2 players, no joysticks which are replaced with 4 buttons, no other buttons */
static INPUT_PORTS_START( qndream )
	PORT_INCLUDE(cps2_2p4b)

	PORT_MODIFY("IN0")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x00f0, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0xf000, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END

/* 2 players and 1 button - either 2 8-way joysticks, 2 2-way joysticks, or 2 paddles */
static INPUT_PORTS_START( pzloop2 )
	PORT_INCLUDE(cps2_2p1b)

	PORT_START("PADDLE1")
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(15) PORT_PLAYER(1)

	PORT_START("PADDLE2")
	PORT_BIT( 0xff, 0x00, IPT_DIAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(15) PORT_PLAYER(2)
INPUT_PORTS_END

/* 1 player and 3 buttons, but 2 coins slots */
static INPUT_PORTS_START( choko )
	PORT_INCLUDE(cps2_1p3b)

	PORT_MODIFY("IN2")
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_COIN2 )
INPUT_PORTS_END


/*************************************
 *
 *  Machine driver
 *
 *************************************/

MACHINE_START_MEMBER(cps2_state,cps2)
{
	if (m_audiocpu) // gigaman2 has an AT89C4051 (8051) MCU as an audio cpu, no qsound.
		membank("bank1")->configure_entries(0, (QSOUND_SIZE - 0x10000) / 0x4000, memregion("audiocpu")->base() + 0x10000, 0x4000);
}


void cps2_state::dead_cps2(machine_config &config)
{
	/* basic machine hardware */
	M68000(config, m_maincpu, XTAL(16'000'000));
	m_maincpu->set_addrmap(AS_PROGRAM, &cps2_state::dead_cps2_map);
	TIMER(config, "scantimer").configure_scanline(FUNC(cps2_state::cps2_interrupt), "screen", 0, 1);

	Z80(config, m_audiocpu, XTAL(8'000'000));
	m_audiocpu->set_addrmap(AS_PROGRAM, &cps2_state::qsound_sub_map);
	m_audiocpu->set_periodic_int(FUNC(cps2_state::irq0_line_hold), attotime::from_hz(250)); // measured

	MCFG_MACHINE_START_OVERRIDE(cps2_state, cps2)

	EEPROM_93C46_16BIT(config, "eeprom");

	/* video hardware */
	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_video_attributes(VIDEO_UPDATE_BEFORE_VBLANK);
	m_screen->set_raw(CPS_PIXEL_CLOCK, CPS_HTOTAL, CPS_HBEND, CPS_HBSTART, CPS_VTOTAL, CPS_VBEND, CPS_VBSTART);
	m_screen->set_screen_update(FUNC(cps2_state::screen_update_cps1));
	m_screen->screen_vblank().set(FUNC(cps2_state::screen_vblank_cps1));
	m_screen->set_palette(m_palette);

	GFXDECODE(config, m_gfxdecode, m_palette, gfx_cps1);
	PALETTE(config, m_palette).set_entries(0xc00);

	MCFG_VIDEO_START_OVERRIDE(cps2_state, cps2)

	/* sound hardware */
	SPEAKER(config, "lspeaker").front_left();
	SPEAKER(config, "rspeaker").front_right();

	QSOUND(config, m_qsound);
	m_qsound->add_route(0, "lspeaker", 1.0);
	m_qsound->add_route(1, "rspeaker", 1.0);
}


void cps2_state::cps2(machine_config &config)
{
	dead_cps2(config);

	m_maincpu->set_addrmap(AS_PROGRAM, &cps2_state::cps2_map);
	m_maincpu->set_addrmap(AS_OPCODES, &cps2_state::decrypted_opcodes_map);
}

void cps2_state::gigaman2(machine_config &config)
{
	cps2(config);

	config.device_remove("audiocpu");
	// gigaman2 has an AT89C4051 (8051) MCU as an audio cpu, no qsound.
	config.device_remove("qsound");

	OKIM6295(config, m_oki, XTAL(32'000'000)/32, okim6295_device::PIN7_HIGH); // clock frequency & pin 7 not verified
	m_oki->add_route(ALL_OUTPUTS, "lspeaker", 0.47);
	m_oki->add_route(ALL_OUTPUTS, "rspeaker", 0.47);
}
/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( 1944 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c83.p1", 0x000000, 0x80000, CRC(7544b926) SHA1(903d9ab45e05a2ab9d7efa074a7ee4ff2006d30a) )
	ROM_LOAD16_WORD_SWAP( "c83.p2", 0x080000, 0x80000, CRC(dba1c66e) SHA1(4764e77d4da5d19d9acded27df1e1bcba06b0fcf) )
	ROM_LOAD16_WORD_SWAP( "c83.p3", 0x100000, 0x80000, CRC(d78d31d3) SHA1(060c3defc87b0cce186df545427bd18687770815) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c83.c1",   0x0000000, 0x400000, CRC(c9fca741) SHA1(1781d4fc18b6d6f79b7b39d9bcace750fb61a5cf) )
	ROM_LOAD64_WORD( "c83.c2",   0x0000002, 0x400000, CRC(f809d898) SHA1(a0b6af49e1780678d808c317b875161cedddb314) )
	ROM_LOAD64_WORD( "c83.c3",   0x0000004, 0x400000, CRC(15ba4507) SHA1(bed6a82bf1dc1aa501d4c2d098115a15e18d446a) )
	ROM_LOAD64_WORD( "c83.c4",   0x0000006, 0x400000, CRC(3dd41b8c) SHA1(676078baad789e25f6e5a79de29672587be7ff00) )
	ROM_LOAD64_WORD( "c83.c5",   0x1000000, 0x100000, CRC(3fe3a54b) SHA1(0a8e5cae141d24fd8b3cb11796c44728b0acd69e) )
	ROM_LOAD64_WORD( "c83.c6",   0x1000002, 0x100000, CRC(565cd231) SHA1(0aecd433fb4ca2de1aca9fbb1e314fb1f6979321) )
	ROM_LOAD64_WORD( "c83.c7",   0x1000004, 0x100000, CRC(63ca5988) SHA1(30137fa77573c84bcc24570bccb7dba61ddb413c) )
	ROM_LOAD64_WORD( "c83.c8",   0x1000006, 0x100000, CRC(21eb8f3b) SHA1(efa69f19a958047dd91a294c88857ed3133fcbef) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c83.m1",   0x00000, 0x08000, CRC(d2e44318) SHA1(33e45f6fe9fed098a4c072b8c39406aef1a949b2) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c83.q1",   0x000000, 0x400000, CRC(243e4e05) SHA1(83281f7290ac105a3f9a7507cbc11317d45ba706) )
	ROM_LOAD16_WORD_SWAP( "c83.q2",   0x400000, 0x400000, CRC(4fcf1600) SHA1(36f18c5d92b79433bdf7088b29a244708929d48e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "1944.key",     0x00, 0x14, CRC(5f22140e) SHA1(e962f50359ea80ba484fe2915f2f642d792764b8) )
ROM_END

ROM_START( 1944u )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c83u.p1", 0x000000, 0x80000, CRC(9693cf8f) SHA1(c296cb008e282f77b44374d1c3638a3f4d5d5d4e) )
	ROM_LOAD16_WORD_SWAP( "c83.p2",  0x080000, 0x80000, CRC(dba1c66e) SHA1(4764e77d4da5d19d9acded27df1e1bcba06b0fcf) )
	ROM_LOAD16_WORD_SWAP( "c83u.p3", 0x100000, 0x80000, CRC(ea813eb7) SHA1(34e0175a5f22d08c3538369b4bfd077a7427a128) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c83.c1",   0x0000000, 0x400000, CRC(c9fca741) SHA1(1781d4fc18b6d6f79b7b39d9bcace750fb61a5cf) )
	ROM_LOAD64_WORD( "c83.c2",   0x0000002, 0x400000, CRC(f809d898) SHA1(a0b6af49e1780678d808c317b875161cedddb314) )
	ROM_LOAD64_WORD( "c83.c3",   0x0000004, 0x400000, CRC(15ba4507) SHA1(bed6a82bf1dc1aa501d4c2d098115a15e18d446a) )
	ROM_LOAD64_WORD( "c83.c4",   0x0000006, 0x400000, CRC(3dd41b8c) SHA1(676078baad789e25f6e5a79de29672587be7ff00) )
	ROM_LOAD64_WORD( "c83.c5",   0x1000000, 0x100000, CRC(3fe3a54b) SHA1(0a8e5cae141d24fd8b3cb11796c44728b0acd69e) )
	ROM_LOAD64_WORD( "c83.c6",   0x1000002, 0x100000, CRC(565cd231) SHA1(0aecd433fb4ca2de1aca9fbb1e314fb1f6979321) )
	ROM_LOAD64_WORD( "c83.c7",   0x1000004, 0x100000, CRC(63ca5988) SHA1(30137fa77573c84bcc24570bccb7dba61ddb413c) )
	ROM_LOAD64_WORD( "c83.c8",   0x1000006, 0x100000, CRC(21eb8f3b) SHA1(efa69f19a958047dd91a294c88857ed3133fcbef) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c83.m1",   0x00000, 0x08000, CRC(d2e44318) SHA1(33e45f6fe9fed098a4c072b8c39406aef1a949b2) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c83.q1",   0x000000, 0x400000, CRC(243e4e05) SHA1(83281f7290ac105a3f9a7507cbc11317d45ba706) )
	ROM_LOAD16_WORD_SWAP( "c83.q2",   0x400000, 0x400000, CRC(4fcf1600) SHA1(36f18c5d92b79433bdf7088b29a244708929d48e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "1944u.key",     0x00, 0x14, CRC(61734f5b) SHA1(f0df9cf7772b48e6422e22bbce9340ee33bc09ba) )
ROM_END

ROM_START( 1944j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c83j.p1", 0x000000, 0x80000, CRC(247521ef) SHA1(c6a04f514dd5ab40d8813dcfb8430bce54e7aa28) )
	ROM_LOAD16_WORD_SWAP( "c83.p2",  0x080000, 0x80000, CRC(dba1c66e) SHA1(4764e77d4da5d19d9acded27df1e1bcba06b0fcf) )
	ROM_LOAD16_WORD_SWAP( "c83j.p3", 0x100000, 0x80000, CRC(7f20c2ef) SHA1(380dc54d94c29c049a4c00ed58013e04eec87086) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c83.c1",   0x0000000, 0x400000, CRC(c9fca741) SHA1(1781d4fc18b6d6f79b7b39d9bcace750fb61a5cf) )
	ROM_LOAD64_WORD( "c83.c2",   0x0000002, 0x400000, CRC(f809d898) SHA1(a0b6af49e1780678d808c317b875161cedddb314) )
	ROM_LOAD64_WORD( "c83.c3",   0x0000004, 0x400000, CRC(15ba4507) SHA1(bed6a82bf1dc1aa501d4c2d098115a15e18d446a) )
	ROM_LOAD64_WORD( "c83.c4",   0x0000006, 0x400000, CRC(3dd41b8c) SHA1(676078baad789e25f6e5a79de29672587be7ff00) )
	ROM_LOAD64_WORD( "c83.c5",   0x1000000, 0x100000, CRC(3fe3a54b) SHA1(0a8e5cae141d24fd8b3cb11796c44728b0acd69e) )
	ROM_LOAD64_WORD( "c83.c6",   0x1000002, 0x100000, CRC(565cd231) SHA1(0aecd433fb4ca2de1aca9fbb1e314fb1f6979321) )
	ROM_LOAD64_WORD( "c83.c7",   0x1000004, 0x100000, CRC(63ca5988) SHA1(30137fa77573c84bcc24570bccb7dba61ddb413c) )
	ROM_LOAD64_WORD( "c83.c8",   0x1000006, 0x100000, CRC(21eb8f3b) SHA1(efa69f19a958047dd91a294c88857ed3133fcbef) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c83.m1",   0x00000, 0x08000, CRC(d2e44318) SHA1(33e45f6fe9fed098a4c072b8c39406aef1a949b2) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c83.q1",   0x000000, 0x400000, CRC(243e4e05) SHA1(83281f7290ac105a3f9a7507cbc11317d45ba706) )
	ROM_LOAD16_WORD_SWAP( "c83.q2",   0x400000, 0x400000, CRC(4fcf1600) SHA1(36f18c5d92b79433bdf7088b29a244708929d48e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "1944j.key",    0x00, 0x14, CRC(210202aa) SHA1(e3ba1d675b3e16e90b709c5aaf10bad7233c9ecb) )
ROM_END

ROM_START( 19xx )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.p1",  0x000000, 0x080000, CRC(9bf9d9b1) SHA1(df32f2e6b3ebafd892368430d786a58e3614ef80) )
	ROM_LOAD16_WORD_SWAP( "c62.p2",  0x080000, 0x080000, CRC(9aa46476) SHA1(dd677b87620198c22738e9f54105998a761298de) )
	ROM_LOAD16_WORD_SWAP( "c62.p3",  0x100000, 0x080000, CRC(a04a2c5e) SHA1(aff5a324daa4641bfdef486824127010e4b803f7) )
	ROM_LOAD16_WORD_SWAP( "c62.p4",  0x180000, 0x080000, CRC(f0a81c33) SHA1(8c44ce3fe5f7bafb3087fd5d1217697fb965886f) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",  0x200000, 0x080000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xx.key",     0x00, 0x14, CRC(6f5d6406) SHA1(f5d9b871de743da952fca33f059b4481e3eb6b89) )
ROM_END

ROM_START( 19xxu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62u.p1", 0x000000, 0x80000, CRC(05955268) SHA1(d3b6b416f1f9eb1c1cbca6647630d1155647082d) )
	ROM_LOAD16_WORD_SWAP( "c62u.p2", 0x080000, 0x80000, CRC(3111ab7f) SHA1(8bbce20ae7ba47949f4939b2f35014fb6decd283) )
	ROM_LOAD16_WORD_SWAP( "c62u.p3", 0x100000, 0x80000, CRC(38df4a63) SHA1(1303f7ab6296f1454907a24d64878bdbd1ef88a7) )
	ROM_LOAD16_WORD_SWAP( "c62u.p4", 0x180000, 0x80000, CRC(5c7e60d3) SHA1(26bf0936962051be871d7a7776cf78abfca5b5ee) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",  0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxu.key",    0x00, 0x14, CRC(77e67ba1) SHA1(8c995bd00941ffe0145cb80e4f105ef8ef0a0001) )
ROM_END

ROM_START( 19xxj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62j.p1", 0x000000, 0x80000, CRC(bcad93dd) SHA1(9b08cfdf278fc0cf60827518c4fecb99f224578b) )
	ROM_LOAD16_WORD_SWAP( "c62j.p2", 0x080000, 0x80000, CRC(931882a1) SHA1(940a4fe52c1eb208071ab9d0a0c256eb39620091) )
	ROM_LOAD16_WORD_SWAP( "c62j.p3", 0x100000, 0x80000, CRC(e7eeddc4) SHA1(40c79bc454d5fb8e5004db3df4fdcb10fae6c1e9) )
	ROM_LOAD16_WORD_SWAP( "c62j.p4", 0x180000, 0x80000, CRC(f27cd6b8) SHA1(7d38660703707382120e6af770543ff50d8190f6) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",  0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30))

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62j.c5",   0x0800000, 0x080000, CRC(8e81f595) SHA1(221016c97300b253301ad4da568ed912e3da6c24) )
	ROM_LOAD64_WORD( "c62j.c6",   0x0800002, 0x080000, CRC(6f8b045e) SHA1(41bc45c89c529011f755b6805ad8bad1a1f5e5e6) )
	ROM_LOAD64_WORD( "c62j.c7",   0x0800004, 0x080000, CRC(c11f88c1) SHA1(3118843063a9caaf9c8627ca2adedcce437cf8d5) )
	ROM_LOAD64_WORD( "c62j.c8",   0x0800006, 0x080000, CRC(9fad3c55) SHA1(9774d015ae417acbcf14c1c84eabd754a299cd50) )
	ROM_LOAD64_WORD( "c62j.c9",   0x0a00000, 0x080000, CRC(6d7ad22e) SHA1(3280f2849361b443c64ca3de4e77390fee4c047a) )
	ROM_LOAD64_WORD( "c62j.c10",  0x0a00002, 0x080000, CRC(ccd5725a) SHA1(20d599ff61632e44a0143957572cb74b934a4aef) )
	ROM_LOAD64_WORD( "c62j.c11",  0x0a00004, 0x080000, CRC(68cc9cd8) SHA1(acb530475e3c66b7e46445332d93fcf6b6058cd2) )
	ROM_LOAD64_WORD( "c62j.c12",  0x0a00006, 0x080000, CRC(e10e252c) SHA1(6c4acc7f8b3f8f5df3768b2fabaf160502c17573) )
	ROM_LOAD64_WORD( "c62j.c13",  0x0c00000, 0x080000, CRC(cb1a1b6a) SHA1(e2b30956684c89525bbc3fed841ba839ca732e21) )
	ROM_LOAD64_WORD( "c62j.c14",  0x0c00002, 0x080000, CRC(6cf6db35) SHA1(4b28b61cb01c81a24b0aea3bafac049b50338515) )
	ROM_LOAD64_WORD( "c62j.c15",  0x0c00004, 0x080000, CRC(f213666b) SHA1(33ede06b46022fc348b9b81530892d132071da0d) )
	ROM_LOAD64_WORD( "c62j.c16",  0x0c00006, 0x080000, CRC(2b86fa67) SHA1(96a3cb8b203738fff33e2ca6340fdef928a816b9) )
	ROM_LOAD64_WORD( "c62j.c17",  0x0e00000, 0x080000, CRC(26fc2b08) SHA1(4d962e1172044c1996139cfd56cc6c00ee2137d5) )
	ROM_LOAD64_WORD( "c62j.c18",  0x0e00002, 0x080000, CRC(16115dd3) SHA1(e83886dba35cf31c956cd084141c3ae4078b3b72) )
	ROM_LOAD64_WORD( "c62j.c19",  0x0e00004, 0x080000, CRC(574e0473) SHA1(6ac52750c1e4ddc0ed8fbe5ace2e827944aa9275) )
	ROM_LOAD64_WORD( "c62j.c20",  0x0e00006, 0x080000, CRC(ae6eb692) SHA1(4b564a375a08872aea534635ec526cce62dcdadd) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62j.q1",   0x000000, 0x080000, CRC(e9cd7780) SHA1(23c3d3968c2aec01ca25338b687be14407580b32) )
	ROM_LOAD16_WORD_SWAP( "c62j.q2",   0x080000, 0x080000, CRC(b27b91a8) SHA1(1fd424c118fabcdb70ea025804552e99b8f8348a) )
	ROM_LOAD16_WORD_SWAP( "c62j.q3",   0x100000, 0x080000, CRC(2e563ee2) SHA1(ed91404f27e36ff7c4bf4c297e81c4ebeb125db7) )
	ROM_LOAD16_WORD_SWAP( "c62j.q4",   0x180000, 0x080000, CRC(f47c1f24) SHA1(3c2ae88946f86288886a901310165f5885b2d878) )
	ROM_LOAD16_WORD_SWAP( "c62j.q5",   0x200000, 0x080000, CRC(0b1af6e0) SHA1(b889e58c8b223c38858680c5e2d0bab5cfa323da) )
	ROM_LOAD16_WORD_SWAP( "c62j.q6",   0x280000, 0x080000, CRC(dfa8819f) SHA1(b5112ced4632c888aee0ca6c1bc4b5097bbdabda) )
	ROM_LOAD16_WORD_SWAP( "c62j.q7",   0x300000, 0x080000, CRC(229ba777) SHA1(2506b967689697fdb4e43d8b01fc9b564641a70d) )
	ROM_LOAD16_WORD_SWAP( "c62j.q8",   0x380000, 0x080000, CRC(c7dceba4) SHA1(6a2684bc9738a3f6d071d72ffa678316a029f1c5) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxj.key",    0x00, 0x14, CRC(9aafa71a) SHA1(82188cc69e59d5ce86d0e178cf6d9f8f04da0633) )
ROM_END

ROM_START( 19xxjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62jr1.p1", 0x000000, 0x80000, CRC(ed08bdd1) SHA1(4b49f988faf4a6a99d3596bb12e4685862a20a3e) )
	ROM_LOAD16_WORD_SWAP( "c62jr1.p2", 0x080000, 0x80000, CRC(fb8e3f29) SHA1(513b85bfe5b86692faa4d20b755fe261a7f95bfc) )
	ROM_LOAD16_WORD_SWAP( "c62jr1.p3", 0x100000, 0x80000, CRC(aa508ac4) SHA1(c906f4a92872f4ecda662146690acbe5165ae79e) )
	ROM_LOAD16_WORD_SWAP( "c62jr1.p4", 0x180000, 0x80000, CRC(ff2d785b) SHA1(9294fb3ed378ecc9dccdeff05df09cbb4eeaa9b3) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",    0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxj.key",    0x00, 0x14, CRC(9aafa71a) SHA1(82188cc69e59d5ce86d0e178cf6d9f8f04da0633) )
ROM_END

ROM_START( 19xxjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62jr2.p1", 0x000000, 0x80000, CRC(26a381ed) SHA1(9a6bd9a8c152096f653c0b5a161dd08314fdb5e7) )
	ROM_LOAD16_WORD_SWAP( "c62jr2.p2", 0x080000, 0x80000, CRC(30100cca) SHA1(3fc964e6daffa5dd7b9f72c8ace3a4b9d515e9ce) )
	ROM_LOAD16_WORD_SWAP( "c62jr2.p3", 0x100000, 0x80000, CRC(de67e938) SHA1(5f977c07c6ffa816ccfa2c7bab8a77b64c232610) )
	ROM_LOAD16_WORD_SWAP( "c62jr2.p4", 0x180000, 0x80000, CRC(39f9a409) SHA1(45799204d2400a591c526f8c750e4728701372bf) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",  0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxj.key",    0x00, 0x14, CRC(9aafa71a) SHA1(82188cc69e59d5ce86d0e178cf6d9f8f04da0633) )
ROM_END

ROM_START( 19xxa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62a.p1", 0x000000, 0x80000, CRC(2e994897) SHA1(8dee5d84fd01f416fa6dfef76be0dfff90f45595) )
	ROM_LOAD16_WORD_SWAP( "c62a.p2", 0x080000, 0x80000, CRC(6364d001) SHA1(a6f3f0015c9a3c27bc2109d557b88d0773a83465) )
	ROM_LOAD16_WORD_SWAP( "c62a.p3", 0x100000, 0x80000, CRC(00c1949b) SHA1(4120a57d06638c637b74442b200e46d9b13fa431) )
	ROM_LOAD16_WORD_SWAP( "c62a.p4", 0x180000, 0x80000, CRC(363c1f6e) SHA1(2e358e345f1d5963836b0ac8c6a1b455638bd19d) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",  0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxa.key",    0x00, 0x14, CRC(2cd32eb9) SHA1(3c2df0e93175c2e25faae2ab3e062ac5d79b35a2) )
ROM_END

ROM_START( 19xxar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62ar1.p1", 0x000000, 0x80000, CRC(0c20fd50) SHA1(3aeb698ac67e6c8d0224e68d9258ef45f735432a) )
	ROM_LOAD16_WORD_SWAP( "c62ar1.p2", 0x080000, 0x80000, CRC(1fc37508) SHA1(f4b858b5dc6243c5cd432d1a72d828831c8eca6f) )
	ROM_LOAD16_WORD_SWAP( "c62ar1.p3", 0x100000, 0x80000, CRC(6c9cc4ed) SHA1(2b01ffe0bba41640ffc0c13dfdacf3cf0e3e131d) )
	ROM_LOAD16_WORD_SWAP( "c62ar1.p4", 0x180000, 0x80000, CRC(ca5b9f76) SHA1(961aed25cb445722de5001ba687dbe85b80cba29) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",    0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxa.key",    0x00, 0x14, CRC(2cd32eb9) SHA1(3c2df0e93175c2e25faae2ab3e062ac5d79b35a2) )
ROM_END

ROM_START( 19xxh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62h.p1", 0x000000, 0x80000, CRC(357be2ac) SHA1(660641d8cd2f7b574809badf99924f0a31a0cccd) )
	ROM_LOAD16_WORD_SWAP( "c62h.p2", 0x080000, 0x80000, CRC(bb13ea3b) SHA1(3ae0fa09ae031e2a0f1ea8645a9baced44289383) )
	ROM_LOAD16_WORD_SWAP( "c62h.p3", 0x100000, 0x80000, CRC(cbd76601) SHA1(a6b64e5f4b35a120dc463a6c9e98e2ec8e739e59) )
	ROM_LOAD16_WORD_SWAP( "c62h.p4", 0x180000, 0x80000, CRC(b362de8b) SHA1(0383a44efbfccdc78637995ed4f99740ef96cbad) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",   0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxh.key",    0x00, 0x14, CRC(215cf208) SHA1(86b83775d0bb76a21d12be81d4dcdeb17d7787ef) )
ROM_END

ROM_START( 19xxb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62b.p1",  0x000000, 0x80000, CRC(341bdf4a) SHA1(e7deccd034f2a2664507dbb7fed9757c2424dbf7) )
	ROM_LOAD16_WORD_SWAP( "c62b.p2",  0x080000, 0x80000, CRC(dff8069e) SHA1(3b31d1f66680cee1da3f3d3fd822739e99f48ccd) )
	ROM_LOAD16_WORD_SWAP( "c62b.p3",  0x100000, 0x80000, CRC(a47a92a8) SHA1(20254ec70029ec027793d4fbd9a7067c2a756315) )
	ROM_LOAD16_WORD_SWAP( "c62b.p4",  0x180000, 0x80000, CRC(c52df10d) SHA1(88707c4d1ec8649a7e10ec60e5bbc6f5ffb26f73) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",   0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "19xxb.key",    0x00, 0x14, CRC(4200e334) SHA1(8adae391e2e62ca1cf3caece813bf7f7e9a1f164) )
ROM_END

ROM_START( armwar )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.p1",  0x000000, 0x80000, CRC(31f74931) SHA1(66150fc9896acca1691c9d586abeb2c7299bb9ad) )
	ROM_LOAD16_WORD_SWAP( "c55.p2",  0x080000, 0x80000, CRC(16f34f5f) SHA1(b831e3915d8cbffdfe4720d356e5196cdebdb6e7) )
	ROM_LOAD16_WORD_SWAP( "c55.p3",  0x100000, 0x80000, CRC(4403ed08) SHA1(cc78c84105a58e43e7a6429281397d68b91e86e5) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwar.key",   0x00, 0x14, CRC(fe979382) SHA1(ec4e0b24d6de9660221e63063fba04df48682e1a) )
ROM_END

ROM_START( armwarb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55b.p1", 0x000000, 0x80000, CRC(d0161e14) SHA1(605a3ce5016a5fa2c35a4c172e4a2fba28fc7be9) )
	ROM_LOAD16_WORD_SWAP( "c55b.p2", 0x080000, 0x80000, CRC(7fb39864) SHA1(471b80196eceebad84659a1480cb94ec83ff662c) )
	ROM_LOAD16_WORD_SWAP( "c55b.p3", 0x100000, 0x80000, CRC(52a1c12c) SHA1(52fe325b29319402f35ae5dfd69ef942cfe6ef4f) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwarb.key",  0x00, 0x14, CRC(eeb97bdb) SHA1(1c4e45240a5a24537b2a512c033d2e9048ea3e24) )
ROM_END

ROM_START( armwarr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55r1.p1", 0x000000, 0x80000, CRC(e822e3e9) SHA1(dcd153bb70f6c2baffa2f3687def30d85fca44ba) )
	ROM_LOAD16_WORD_SWAP( "c55r1.p2", 0x080000, 0x80000, CRC(4f89de39) SHA1(1e54ed70a6ed9330ec83fb189f76e9417c6dfc13) )
	ROM_LOAD16_WORD_SWAP( "c55r1.p3", 0x100000, 0x80000, CRC(83df24e5) SHA1(39801452769569c3271b26c6be8d3ce5e72b0629) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwar.key",   0x00, 0x14, CRC(fe979382) SHA1(ec4e0b24d6de9660221e63063fba04df48682e1a) )
ROM_END

ROM_START( armwaru )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55u.p1", 0x000000, 0x80000, CRC(8b95497a) SHA1(0c037b8a484d69f5e8c9600de71177fb78e9ede0) )
	ROM_LOAD16_WORD_SWAP( "c55u.p2", 0x080000, 0x80000, CRC(29eb5661) SHA1(7ee9150072882c9e158ca8231f26a9f62c8fa50e) )
	ROM_LOAD16_WORD_SWAP( "c55u.p3", 0x100000, 0x80000, CRC(a54e9e44) SHA1(e235dcdbd0111f018519d9c8eef130121ea20a20) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwaru.key",  0x00, 0x14, CRC(fb9aada5) SHA1(3a9b133930be67fad4d2de6e0ca51377e73edd25) )
ROM_END

ROM_START( armwaru1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55u1.p1", 0x000000, 0x80000, CRC(73d397b1) SHA1(43c33f5268e98411fbbb27f8b59c2ff5dcaf3c34) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p2", 0x080000, 0x80000, CRC(1f1de215) SHA1(b0a74a4effddd30fbc972d94e4bf3848c4893363) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p3", 0x100000, 0x80000, CRC(835fbe73) SHA1(6218aa1b480105ffabded980c92679fafb19b824) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p7",   0x300000, 0x80000, CRC(ddc85ca6) SHA1(e794c679531632e2142c6a5e3b858494389ce65e) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",   0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwaru.key",  0x00, 0x14, CRC(fb9aada5) SHA1(3a9b133930be67fad4d2de6e0ca51377e73edd25) )
ROM_END

ROM_START( pgear )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55j.p1", 0x000000, 0x80000, CRC(c79c0c02) SHA1(4e24d34be255bb3886bd6b767779ee5fc81dca6e) )
	ROM_LOAD16_WORD_SWAP( "c55j.p2", 0x080000, 0x80000, CRC(167c6ed8) SHA1(23a4a7faae817ffc6c5faa4db5b96b8c8c0dfe86) )
	ROM_LOAD16_WORD_SWAP( "c55j.p3", 0x100000, 0x80000, CRC(a63fb400) SHA1(b27464b000cd12d9247254f843be27639fbf3a48) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pgear.key",    0x00, 0x14, CRC(c576d6fd) SHA1(e480d16bcfb58f9a8402bbb81aedd686a0f2f754) )
ROM_END

ROM_START( pgearr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55jr1.p1", 0x000000, 0x80000, CRC(f264e74b) SHA1(db0a675a9d46df9227334259db633e27f7dc79ab) )
	ROM_LOAD16_WORD_SWAP( "c55jr1.p2", 0x080000, 0x80000, CRC(23a84983) SHA1(a3ed606f6213bb6e447c4ff84d6d3435a0170762) )
	ROM_LOAD16_WORD_SWAP( "c55jr1.p3", 0x100000, 0x80000, CRC(bef58c62) SHA1(178c255171c4010cec758ee11d96bdcee85abee0) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",    0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",    0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",    0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p7",  0x300000, 0x80000, CRC(ddc85ca6) SHA1(e794c679531632e2142c6a5e3b858494389ce65e) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",    0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pgear.key",    0x00, 0x14, CRC(c576d6fd) SHA1(e480d16bcfb58f9a8402bbb81aedd686a0f2f754) )
ROM_END

ROM_START( armwara )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55a.p1", 0x000000, 0x80000, CRC(347743e1) SHA1(0cf27209792e70c361e2981b82a4d37643d1b298) )
	ROM_LOAD16_WORD_SWAP( "c55a.p2", 0x080000, 0x80000, CRC(42dbfb2e) SHA1(cbb7ac2a97de977394eb015f3582ea72432a4529) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p3", 0x100000, 0x80000, CRC(835fbe73) SHA1(6218aa1b480105ffabded980c92679fafb19b824) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55.p7",  0x300000, 0x80000, CRC(4c26baee) SHA1(685f050206b9b904ce6a1ae9a8e8f019012cea43) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",  0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwara.key",  0x00, 0x14, CRC(525439c0) SHA1(51cca388e36712cbc8312774d07fa5fc6d06618e) )
ROM_END

ROM_START( armwarar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55ar1.p1", 0x000000, 0x80000, CRC(8d474ab1) SHA1(46baa3a263189001cfc6003fcb346a1996be8b24) )
	ROM_LOAD16_WORD_SWAP( "c55ar1.p2", 0x080000, 0x80000, CRC(81b5aec7) SHA1(f1371149a00e7c52d022d5c0cb6f8821c6474d35) )
	ROM_LOAD16_WORD_SWAP( "c55ar1.p3", 0x100000, 0x80000, CRC(2618e819) SHA1(58c857988e0ad2839d936d3e405637d8e2a45fe9) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",  0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",  0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",  0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p7",   0x300000, 0x80000, CRC(ddc85ca6) SHA1(e794c679531632e2142c6a5e3b858494389ce65e) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",   0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "armwara.key",  0x00, 0x14, CRC(525439c0) SHA1(51cca388e36712cbc8312774d07fa5fc6d06618e) )
ROM_END

ROM_START( avsp )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.p1", 0x000000, 0x80000, CRC(774334a9) SHA1(f60b0e39139ea40e0b0ba97ed01d4a757ed65e1a) )
	ROM_LOAD16_WORD_SWAP( "c52.p2", 0x080000, 0x80000, CRC(7fa83769) SHA1(930f02e4d35686e80fbdd673380c4b2bd784a9e5) )
	ROM_LOAD16_WORD_SWAP( "c52.p3", 0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52.p4", 0x180000, 0x80000, CRC(190b817f) SHA1(9bcfc0a015ffba9cdac25b6270939a9690de5da7) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "avsp.key",     0x00, 0x14, CRC(e69fa35b) SHA1(ed0ca61eefe9ba136539300b5bbbc85676abd490) )
ROM_END

ROM_START( avspu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52u.p1", 0x000000, 0x80000, CRC(42757950) SHA1(e6acae73a300c0e07c21f776e6aa87628184b152) )
	ROM_LOAD16_WORD_SWAP( "c52u.p2", 0x080000, 0x80000, CRC(5abcdee6) SHA1(205e1ac8f4e359fd04e3a1e12425ba0b8330b1c1) )
	ROM_LOAD16_WORD_SWAP( "c52.p3",  0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52.p4",  0x180000, 0x80000, CRC(190b817f) SHA1(9bcfc0a015ffba9cdac25b6270939a9690de5da7) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "avspu.key",    0x00, 0x14, CRC(4e68e346) SHA1(60d5a12c77d07efc01f48968f4a12c59bf6b629e) )
ROM_END

ROM_START( avspj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52j.p1", 0x000000, 0x80000, CRC(49799119) SHA1(71a938b779291c3092ef6ef22935d89fd9c1186c) )
	ROM_LOAD16_WORD_SWAP( "c52j.p2", 0x080000, 0x80000, CRC(8cd2bba8) SHA1(1ea493d0d4b6e202ad38843b93035fa3f7e1b8c7) )
	ROM_LOAD16_WORD_SWAP( "c52.p3",  0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52.p4",  0x180000, 0x80000, CRC(190b817f) SHA1(9bcfc0a015ffba9cdac25b6270939a9690de5da7) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "avspj.key",    0x00, 0x14, CRC(3d5ccc08) SHA1(85b30ccde86e2e1f50e662598dc81d47219519a1) )
ROM_END

ROM_START( avspa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52a.p1", 0x000000, 0x80000, CRC(6c1c1858) SHA1(29af268cf070ea2adc0aac0c5187debdd9706037) )
	ROM_LOAD16_WORD_SWAP( "c52a.p2", 0x080000, 0x80000, CRC(94f50b0c) SHA1(607b13e4cb4968c47a598f7dfec965c6d6ba68f0) )
	ROM_LOAD16_WORD_SWAP( "c52.p3",  0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52.p4",  0x180000, 0x80000, CRC(190b817f) SHA1(9bcfc0a015ffba9cdac25b6270939a9690de5da7) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "avspa.key",    0x00, 0x14, CRC(728efc00) SHA1(3f74bdab733af431f83fc99b112c6085d8973753) )
ROM_END

ROM_START( avsph )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52h.p1", 0x000000, 0x80000, CRC(3e440447) SHA1(5b7431de6b9e243f041d0e76b3a69002662c321a) )
	ROM_LOAD16_WORD_SWAP( "c52h.p2", 0x080000, 0x80000, CRC(af6fc82f) SHA1(c0293d71a657dbbe14ce15121d0970ccc7e584cf) )
	ROM_LOAD16_WORD_SWAP( "c52.p3",  0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52.p4",  0x180000, 0x80000, CRC(190b817f) SHA1(9bcfc0a015ffba9cdac25b6270939a9690de5da7) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "avsph.key",    0x00, 0x14, CRC(cae7b680) SHA1(b5f7c90c6c258ee9d0943028298bc8a0fcba63fb) )
ROM_END

ROM_START( batcir )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c70.p1", 0x000000, 0x80000, CRC(bc60484b) SHA1(9b4e46d0f3d96edcd1c3707409507a5027c69039) )
	ROM_LOAD16_WORD_SWAP( "c70.p2", 0x080000, 0x80000, CRC(457d55f6) SHA1(19a39ec30166d4b797babe9d70328ac572d1f916) )
	ROM_LOAD16_WORD_SWAP( "c70.p3", 0x100000, 0x80000, CRC(e86560d7) SHA1(a978a7f5e0069cd78c8588c2d91b825796c723a5) )
	ROM_LOAD16_WORD_SWAP( "c70.p4", 0x180000, 0x80000, CRC(f778e61b) SHA1(e8321dece8977131e41c9207946b627074c13ee7) )
	ROM_LOAD16_WORD_SWAP( "c70.p5",  0x200000, 0x80000, CRC(7322d5db) SHA1(473be1f1bf603bdd82451661a6206507f50ed2b6) )
	ROM_LOAD16_WORD_SWAP( "c70.p6",  0x280000, 0x80000, CRC(6aac85ab) SHA1(ad02d4185c2b3664fb96350d8ad317d3939a7554) )
	ROM_LOAD16_WORD_SWAP( "c70.p7",  0x300000, 0x80000, CRC(1203db08) SHA1(fdbea14618b277132f9e010ef36c134a8ea42162) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c70.c1",   0x000000, 0x400000, CRC(dc705bad) SHA1(96e37147674bf9cd21c770897da59daac25d921a) )
	ROM_LOAD64_WORD( "c70.c2",   0x000002, 0x400000, CRC(e5779a3c) SHA1(bbd7fbe061e751388d2f02434144daf9b1e36640) )
	ROM_LOAD64_WORD( "c70.c3",   0x000004, 0x400000, CRC(b33f4112) SHA1(e501fd921c8bcede69946b029e05d422714c1040) )
	ROM_LOAD64_WORD( "c70.c4",   0x000006, 0x400000, CRC(a6fcdb7e) SHA1(7a28d5d7aa036d23d97fad17d0cdb8210dc8153a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c70.m1",   0x00000, 0x08000, CRC(1e194310) SHA1(3b29de0aca9dbca59d6b50fb2509e2a913c6b0af) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c70.m2",   0x28000, 0x20000, CRC(01aeb8e6) SHA1(50a5d1cce0caf7c5143d4904431e8f41e2a57464) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c70.q1",   0x000000, 0x200000, CRC(c27f2229) SHA1(df2459493af40937b6656a16fad43ff51bed2204) )
	ROM_LOAD16_WORD_SWAP( "c70.q2",   0x200000, 0x200000, CRC(418a2e33) SHA1(0642ddff2ab9255f154419da24ba644ed63f34ab) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "batcir.key",   0x00, 0x14, CRC(e316ae67) SHA1(d390c5ce88ccc6c40ec39fdd93475a7deb5be2a5) )
ROM_END

ROM_START( batcirj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c70j.p1", 0x000000, 0x80000, CRC(6b7e168d) SHA1(7e95cc436d53d1ce34b575bc7e2b6e2a7ae06cfb) )
	ROM_LOAD16_WORD_SWAP( "c70j.p2", 0x080000, 0x80000, CRC(46ba3467) SHA1(0cc4a6c82f110d2334fd81f2d3abe5de882768bd) )
	ROM_LOAD16_WORD_SWAP( "c70j.p3", 0x100000, 0x80000, CRC(0e23a859) SHA1(6c7eec9bf823c66fddbc6b297ea6aa883d03bff5) )
	ROM_LOAD16_WORD_SWAP( "c70j.p4", 0x180000, 0x80000, CRC(a853b59c) SHA1(841c178722d4850994afee90ef3079607d8847ed) )
	ROM_LOAD16_WORD_SWAP( "c70.p5",  0x200000, 0x80000, CRC(7322d5db) SHA1(473be1f1bf603bdd82451661a6206507f50ed2b6) )
	ROM_LOAD16_WORD_SWAP( "c70.p6",  0x280000, 0x80000, CRC(6aac85ab) SHA1(ad02d4185c2b3664fb96350d8ad317d3939a7554) )
	ROM_LOAD16_WORD_SWAP( "c70.p7",  0x300000, 0x80000, CRC(1203db08) SHA1(fdbea14618b277132f9e010ef36c134a8ea42162) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c70.c1",   0x000000, 0x400000, CRC(dc705bad) SHA1(96e37147674bf9cd21c770897da59daac25d921a) )
	ROM_LOAD64_WORD( "c70.c2",   0x000002, 0x400000, CRC(e5779a3c) SHA1(bbd7fbe061e751388d2f02434144daf9b1e36640) )
	ROM_LOAD64_WORD( "c70.c3",   0x000004, 0x400000, CRC(b33f4112) SHA1(e501fd921c8bcede69946b029e05d422714c1040) )
	ROM_LOAD64_WORD( "c70.c4",   0x000006, 0x400000, CRC(a6fcdb7e) SHA1(7a28d5d7aa036d23d97fad17d0cdb8210dc8153a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c70.m1",   0x00000, 0x08000, CRC(1e194310) SHA1(3b29de0aca9dbca59d6b50fb2509e2a913c6b0af) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c70.m2",   0x28000, 0x20000, CRC(01aeb8e6) SHA1(50a5d1cce0caf7c5143d4904431e8f41e2a57464) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c70.q1",   0x000000, 0x200000, CRC(c27f2229) SHA1(df2459493af40937b6656a16fad43ff51bed2204) )
	ROM_LOAD16_WORD_SWAP( "c70.q2",   0x200000, 0x200000, CRC(418a2e33) SHA1(0642ddff2ab9255f154419da24ba644ed63f34ab) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "batcirj.key",  0x00, 0x14, CRC(9f9fb965) SHA1(9ea7e255f48ef5eaa89598355292b858f37f342f) )
ROM_END

ROM_START( batcira )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c70a.p1", 0x000000, 0x80000, CRC(1ad20d87) SHA1(0ad8b7725604a61314883cd4ed8599445fe1cbf8) )
	ROM_LOAD16_WORD_SWAP( "c70a.p2", 0x080000, 0x80000, CRC(2b3f4dbe) SHA1(be4ab2ac411523def5e05081a754b651ead52e1f) )
	ROM_LOAD16_WORD_SWAP( "c70a.p3", 0x100000, 0x80000, CRC(8238a3d9) SHA1(4b0fe0e6c6a8a6572fc3554f2ee77dc01c2f75c3) )
	ROM_LOAD16_WORD_SWAP( "c70a.p4", 0x180000, 0x80000, CRC(446c7c02) SHA1(2fda5d0fef3ca556976ec9126cb04af4fa883a38) )
	ROM_LOAD16_WORD_SWAP( "c70.p5",  0x200000, 0x80000, CRC(7322d5db) SHA1(473be1f1bf603bdd82451661a6206507f50ed2b6) )
	ROM_LOAD16_WORD_SWAP( "c70.p6",  0x280000, 0x80000, CRC(6aac85ab) SHA1(ad02d4185c2b3664fb96350d8ad317d3939a7554) )
	ROM_LOAD16_WORD_SWAP( "c70.p7",  0x300000, 0x80000, CRC(1203db08) SHA1(fdbea14618b277132f9e010ef36c134a8ea42162) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c70.c1",   0x000000, 0x400000, CRC(dc705bad) SHA1(96e37147674bf9cd21c770897da59daac25d921a) )
	ROM_LOAD64_WORD( "c70.c2",   0x000002, 0x400000, CRC(e5779a3c) SHA1(bbd7fbe061e751388d2f02434144daf9b1e36640) )
	ROM_LOAD64_WORD( "c70.c3",   0x000004, 0x400000, CRC(b33f4112) SHA1(e501fd921c8bcede69946b029e05d422714c1040) )
	ROM_LOAD64_WORD( "c70.c4",   0x000006, 0x400000, CRC(a6fcdb7e) SHA1(7a28d5d7aa036d23d97fad17d0cdb8210dc8153a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c70.m1",   0x00000, 0x08000, CRC(1e194310) SHA1(3b29de0aca9dbca59d6b50fb2509e2a913c6b0af) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c70.m2",   0x28000, 0x20000, CRC(01aeb8e6) SHA1(50a5d1cce0caf7c5143d4904431e8f41e2a57464) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c70.q1",   0x000000, 0x200000, CRC(c27f2229) SHA1(df2459493af40937b6656a16fad43ff51bed2204) )
	ROM_LOAD16_WORD_SWAP( "c70.q2",   0x200000, 0x200000, CRC(418a2e33) SHA1(0642ddff2ab9255f154419da24ba644ed63f34ab) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "batcira.key",  0x00, 0x14, CRC(384500f3) SHA1(9f2252205d73c771389d03d087f2a8e5492d309d) )
ROM_END

ROM_START( choko )
	ROM_REGION( CODE_SIZE, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "c87.p1", 0x000000, 0x80000, CRC(11f5452f) SHA1(1575729cdbb857a3a780df6e3e0efd6968926fb8) )
	ROM_LOAD16_WORD_SWAP( "c87.p2", 0x080000, 0x80000, CRC(68655378) SHA1(a2d82996394cc28622e93f6c338f9b78aa798775) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c87.c1",   0x0000000, 0x200000, CRC(6933377d) SHA1(a79e129e5faaadd401379905ac76a24fa616d736) )
	ROM_LOAD64_BYTE( "c87.c2",   0x0000001, 0x200000, CRC(7f668950) SHA1(247b2b3fa24afd43b0fe6cfb3df987a38c7385cf) )
	ROM_LOAD64_BYTE( "c87.c3",   0x0000002, 0x200000, CRC(cfb68ca9) SHA1(36460724b8df36a4ccf88228d9d5f027714c6628) )
	ROM_LOAD64_BYTE( "c87.c4",   0x0000003, 0x200000, CRC(437e21c5) SHA1(2c4ace6fa421c91effab8ab8db931b8451b8e6b1) )
	ROM_LOAD64_BYTE( "c87.c5",   0x0000004, 0x200000, CRC(a9e32b57) SHA1(0ddbfefa0cc110e46297ecbfbf4b8bc87ce43c95) )
	ROM_LOAD64_BYTE( "c87.c6",   0x0000005, 0x200000, CRC(b7ab9338) SHA1(0bb57640eed167e672b5f40cdff0a7b177ff2507) )
	ROM_LOAD64_BYTE( "c87.c7",   0x0000006, 0x200000, CRC(4d3f919a) SHA1(eb7f90415a263f0a2b94d9699d72c1d14b4fdaad) )
	ROM_LOAD64_BYTE( "c87.c8",   0x0000007, 0x200000, CRC(cfef17ab) SHA1(1de738c1e537c3df80171e82bc1cc05a25d9cc13) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c87.m1",   0x00000, 0x08000, CRC(6eda50c2) SHA1(7e67c104094a3ced8b3fdd81f52ee42483b30fc5) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_BYTE( "c87.q1",   0x000000, 0x200000, CRC(ab45d509) SHA1(c58cf87d3828dfe0643cf4c58615f3352bd45508) )
	ROM_LOAD16_BYTE( "c87.q2",   0x000001, 0x200000, CRC(fa905c3d) SHA1(3eae65b01d50ec4ec4aeff49f434b9b88a50463c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "choko.key",    0x00, 0x14, CRC(08505e8b) SHA1(5c481ffaa93faec57d0b80b678c8c0cca1a699c0) )
ROM_END

ROM_START( csclub )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73e.p1", 0x000000, 0x80000, CRC(824082be) SHA1(a44e7e17d73e089b4e7784317f2387d135d01482) )
	ROM_LOAD16_WORD_SWAP( "c73e.p2", 0x080000, 0x80000, CRC(74e6a4fe) SHA1(2a732a6e57088885e77256eb17ddd3ae523db09f) )
	ROM_LOAD16_WORD_SWAP( "c73e.p3", 0x100000, 0x80000, CRC(8ae0df19) SHA1(88e7bf1ee0e18d74748cecd875cb96524dfa01cf) )
	ROM_LOAD16_WORD_SWAP( "c73e.p4", 0x180000, 0x80000, CRC(51f2f0d3) SHA1(067a0bded69767b9f30073012ad62f4608b7610a) )
	ROM_LOAD16_WORD_SWAP( "c73e.p5", 0x200000, 0x80000, CRC(003968fd) SHA1(95f59a29a404a1c1e86d5f43526a80aa97f25621) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c01",  0x800000, 0x080000, CRC(335f07c3) SHA1(44e0385120e2c81fd1072e19b7e3ff05d42be226) )
	ROM_LOAD64_WORD( "c73.c02",  0x800002, 0x080000, CRC(3711b8ca) SHA1(ff7ba4f73d227212377327f4fcbe8f555ac4b9c9) )
	ROM_LOAD64_WORD( "c73.c03",  0x800004, 0x080000, CRC(0750d12a) SHA1(4c36cba88c58bdbfed923f56a6d489f42a4d0f6e) )
	ROM_LOAD64_WORD( "c73.c04",  0x800006, 0x080000, CRC(a756c7f7) SHA1(d0f44f8ded12291d5c79282eac45d088cb365b09) )
	ROM_LOAD64_WORD( "c73.c05",  0xa00000, 0x080000, CRC(ab215357) SHA1(c2600b5ba62a570f8a32de9c06f93ea8ae6cd854) )
	ROM_LOAD64_WORD( "c73.c06",  0xa00002, 0x080000, CRC(828a06d8) SHA1(ce1d147f1d747fcd90d93f7dcca0cdd24c85a971) )
	ROM_LOAD64_WORD( "c73.c07",  0xa00004, 0x080000, CRC(90a92f39) SHA1(75bef2440147bac417b48d61ff64b71ed5b9eb67) )
	ROM_LOAD64_WORD( "c73.c08",  0xa00006, 0x080000, CRC(fb7ccc73) SHA1(763e6fe1baf73341747c74d89df9443561fca4fa) )
	ROM_LOAD64_WORD( "c73.c09",  0xc00000, 0x080000, CRC(a2367381) SHA1(49d562be42c10cdf4e55e3b76c388f0b0121a967) )
	ROM_LOAD64_WORD( "c73.c10",  0xc00002, 0x080000, CRC(86ee4569) SHA1(725d82a1669f6f78fb4ef8756ceac8b444dcbd43) )
	ROM_LOAD64_WORD( "c73.c11",  0xc00004, 0x080000, CRC(d08ab012) SHA1(7f2e71b6bd85c6a3efbd417977261c21f3d6cb7c) )
	ROM_LOAD64_WORD( "c73.c12",  0xc00006, 0x080000, CRC(4d014297) SHA1(df172a3723793b9955ff1f65e76fe5c20fc37b89) )
	ROM_LOAD64_WORD( "c73.c13",  0xe00000, 0x080000, CRC(728aac1f) SHA1(2d04e8803f41af2372fc342fcd8a076c14338198) )
	ROM_LOAD64_WORD( "c73.c14",  0xe00002, 0x080000, CRC(c24f577f) SHA1(d3b97091e1f0171e087feb9d497c94a48872cd21) )
	ROM_LOAD64_WORD( "c73.c15",  0xe00004, 0x080000, CRC(41652583) SHA1(7132647dc9a29ce98866f489140703c5ed3d6051) )
	ROM_LOAD64_WORD( "c73.c16",  0xe00006, 0x080000, CRC(6754b1ef) SHA1(ab5d62056b19999ccecd0eeec7b7c5869ca8fea8) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73e.q1",   0x000000, 0x080000, CRC(5a52afd5) SHA1(ab873ec556933b75be82ba249357cceb01a7a0bd) )
	ROM_LOAD16_WORD_SWAP( "c73e.q2",   0x080000, 0x080000, CRC(1408a811) SHA1(1d31e3cd505245b5ff4d3c55d5fa7017c0f1e168) )
	ROM_LOAD16_WORD_SWAP( "c73e.q3",   0x100000, 0x080000, CRC(4fb9f57c) SHA1(093e8e3a03b62783a84fe4ae239e9eb46cbfd71e) )
	ROM_LOAD16_WORD_SWAP( "c73e.q4",   0x180000, 0x080000, CRC(9a8f40ec) SHA1(c8db1ecfd6b08e9c83ae53a1d25c1387ab95535c) )
	ROM_LOAD16_WORD_SWAP( "c73e.q5",   0x200000, 0x080000, CRC(91529a91) SHA1(183569100ae98e17688e0e25932850e73a41eb88) )
	ROM_LOAD16_WORD_SWAP( "c73e.q6",   0x280000, 0x080000, CRC(9a345334) SHA1(330291400a73215c9797457507a86c90ba415247) )
	ROM_LOAD16_WORD_SWAP( "c73e.q7",   0x300000, 0x080000, CRC(aedc27f2) SHA1(55137f0f22c4823558e6a8ba76011695579a4f1f) )
	ROM_LOAD16_WORD_SWAP( "c73e.q8",   0x380000, 0x080000, CRC(2300b7b3) SHA1(f5ecbb45c24f7de1c1aa435870695551d4e343ca) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "csclub.key",   0x00, 0x14, CRC(903907d7) SHA1(d70384b64637dbbcc064d39f984f73fc48bbac44) )
ROM_END

ROM_START( csclub1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73.p1", 0x000000, 0x80000, CRC(f2c852ef) SHA1(bc2d403958640d7ab0785d01a3df79ec31d0c239) )
	ROM_LOAD16_WORD_SWAP( "c73.p2", 0x080000, 0x80000, CRC(1184530f) SHA1(18565f6a06e6078fc20dd9cf70802ac1da60c67a) )
	ROM_LOAD16_WORD_SWAP( "c73.p3", 0x100000, 0x80000, CRC(804e2b6b) SHA1(e638f73442e3165ace84cdb1bd2a9d419e2d8c41) )
	ROM_LOAD16_WORD_SWAP( "c73.p4", 0x180000, 0x80000, CRC(09277cb9) SHA1(51a0d335b5d6cde61c32f4e7ea49403f400db7fb) )
	ROM_LOAD16_WORD_SWAP( "c73.p5",  0x200000, 0x80000, CRC(01b05caa) SHA1(5b84487da68e6b6f2889c76bf9e070e25941988c) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c1",  0x800000, 0x200000, CRC(e8904afa) SHA1(39713ffca4e3a754c7c44c0ef4d99fb5a77d8da7) )
	ROM_LOAD64_WORD( "c73.c2",  0x800002, 0x200000, CRC(c98c8079) SHA1(22d68ba2ef62b51981bb3e99ec2cde8d1b36514b) )
	ROM_LOAD64_WORD( "c73.c3",  0x800004, 0x200000, CRC(c030df5a) SHA1(6d5e5a05531e168d0d44c591f9185ae300908fc2) )
	ROM_LOAD64_WORD( "c73.c4",  0x800006, 0x200000, CRC(b4e55863) SHA1(da66f0a36266b906e4c149aec152c323bb184c57) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73.q1",   0x000000, 0x200000, CRC(a027b827) SHA1(6d58a63efc7bd5d07353d9b55826c01a3c416c33) )
	ROM_LOAD16_WORD_SWAP( "c73.q2",   0x200000, 0x200000, CRC(cb7f6e55) SHA1(b64e6b663fd09e887d2dc0f4b545e88688c0af55) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "csclub.key",   0x00, 0x14, CRC(903907d7) SHA1(d70384b64637dbbcc064d39f984f73fc48bbac44) )
ROM_END

ROM_START( csclubj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73j.p1", 0x000000, 0x80000, CRC(ec4ddaa2) SHA1(f84af8bd01cc994ecd6ac24e829e2bd33817d862) )
	ROM_LOAD16_WORD_SWAP( "c73j.p2", 0x080000, 0x80000, CRC(60c632bb) SHA1(0d42c33aa476d2cc4efcdad78667353b88225966) )
	ROM_LOAD16_WORD_SWAP( "c73j.p3", 0x100000, 0x80000, CRC(ad042003) SHA1(1e167c88f3b0617c38c9f43bdc816045ac0296e0) )
	ROM_LOAD16_WORD_SWAP( "c73j.p4", 0x180000, 0x80000, CRC(169e4d40) SHA1(6540d89df5e76189d32b696be7626087fe26e33b) )
	ROM_LOAD16_WORD_SWAP( "c73.p5",  0x200000, 0x80000, CRC(01b05caa) SHA1(5b84487da68e6b6f2889c76bf9e070e25941988c) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c1",  0x800000, 0x200000, CRC(e8904afa) SHA1(39713ffca4e3a754c7c44c0ef4d99fb5a77d8da7) )
	ROM_LOAD64_WORD( "c73.c2",  0x800002, 0x200000, CRC(c98c8079) SHA1(22d68ba2ef62b51981bb3e99ec2cde8d1b36514b) )
	ROM_LOAD64_WORD( "c73.c3",  0x800004, 0x200000, CRC(c030df5a) SHA1(6d5e5a05531e168d0d44c591f9185ae300908fc2) )
	ROM_LOAD64_WORD( "c73.c4",  0x800006, 0x200000, CRC(b4e55863) SHA1(da66f0a36266b906e4c149aec152c323bb184c57) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73.q1",   0x000000, 0x200000, CRC(a027b827) SHA1(6d58a63efc7bd5d07353d9b55826c01a3c416c33) )
	ROM_LOAD16_WORD_SWAP( "c73.q2",   0x200000, 0x200000, CRC(cb7f6e55) SHA1(b64e6b663fd09e887d2dc0f4b545e88688c0af55) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "csclubj.key",  0x00, 0x14, CRC(519a04db) SHA1(8f792831dc7e07fb621806c4e320a047f801e9b8) )
ROM_END

ROM_START( csclubjy ) // this is fairly redundant, same code as csclubj, same gfx as csclub (yellow case - all eprom), but it's a valid shipped combination
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73j.p1", 0x000000, 0x80000, CRC(ec4ddaa2) SHA1(f84af8bd01cc994ecd6ac24e829e2bd33817d862) )
	ROM_LOAD16_WORD_SWAP( "c73j.p2", 0x080000, 0x80000, CRC(60c632bb) SHA1(0d42c33aa476d2cc4efcdad78667353b88225966) )
	ROM_LOAD16_WORD_SWAP( "c73j.p3", 0x100000, 0x80000, CRC(ad042003) SHA1(1e167c88f3b0617c38c9f43bdc816045ac0296e0) )
	ROM_LOAD16_WORD_SWAP( "c73j.p4", 0x180000, 0x80000, CRC(169e4d40) SHA1(6540d89df5e76189d32b696be7626087fe26e33b) )
	ROM_LOAD16_WORD_SWAP( "c73.p5",  0x200000, 0x80000, CRC(01b05caa) SHA1(5b84487da68e6b6f2889c76bf9e070e25941988c) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c01",  0x800000, 0x080000, CRC(335f07c3) SHA1(44e0385120e2c81fd1072e19b7e3ff05d42be226) )
	ROM_LOAD64_WORD( "c73.c02",  0x800002, 0x080000, CRC(3711b8ca) SHA1(ff7ba4f73d227212377327f4fcbe8f555ac4b9c9) )
	ROM_LOAD64_WORD( "c73.c03",  0x800004, 0x080000, CRC(0750d12a) SHA1(4c36cba88c58bdbfed923f56a6d489f42a4d0f6e) )
	ROM_LOAD64_WORD( "c73.c04",  0x800006, 0x080000, CRC(a756c7f7) SHA1(d0f44f8ded12291d5c79282eac45d088cb365b09) )
	ROM_LOAD64_WORD( "c73.c05",  0xa00000, 0x080000, CRC(ab215357) SHA1(c2600b5ba62a570f8a32de9c06f93ea8ae6cd854) )
	ROM_LOAD64_WORD( "c73.c06",  0xa00002, 0x080000, CRC(828a06d8) SHA1(ce1d147f1d747fcd90d93f7dcca0cdd24c85a971) )
	ROM_LOAD64_WORD( "c73.c07",  0xa00004, 0x080000, CRC(90a92f39) SHA1(75bef2440147bac417b48d61ff64b71ed5b9eb67) )
	ROM_LOAD64_WORD( "c73.c08",  0xa00006, 0x080000, CRC(fb7ccc73) SHA1(763e6fe1baf73341747c74d89df9443561fca4fa) )
	ROM_LOAD64_WORD( "c73.c09",  0xc00000, 0x080000, CRC(a2367381) SHA1(49d562be42c10cdf4e55e3b76c388f0b0121a967) )
	ROM_LOAD64_WORD( "c73.c10",  0xc00002, 0x080000, CRC(86ee4569) SHA1(725d82a1669f6f78fb4ef8756ceac8b444dcbd43) )
	ROM_LOAD64_WORD( "c73.c11",  0xc00004, 0x080000, CRC(d08ab012) SHA1(7f2e71b6bd85c6a3efbd417977261c21f3d6cb7c) )
	ROM_LOAD64_WORD( "c73.c12",  0xc00006, 0x080000, CRC(4d014297) SHA1(df172a3723793b9955ff1f65e76fe5c20fc37b89) )
	ROM_LOAD64_WORD( "c73.c13",  0xe00000, 0x080000, CRC(728aac1f) SHA1(2d04e8803f41af2372fc342fcd8a076c14338198) )
	ROM_LOAD64_WORD( "c73.c14",  0xe00002, 0x080000, CRC(c24f577f) SHA1(d3b97091e1f0171e087feb9d497c94a48872cd21) )
	ROM_LOAD64_WORD( "c73.c15",  0xe00004, 0x080000, CRC(41652583) SHA1(7132647dc9a29ce98866f489140703c5ed3d6051) )
	ROM_LOAD64_WORD( "c73.c16",  0xe00006, 0x080000, CRC(6754b1ef) SHA1(ab5d62056b19999ccecd0eeec7b7c5869ca8fea8) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73e.q1",   0x000000, 0x080000, CRC(5a52afd5) SHA1(ab873ec556933b75be82ba249357cceb01a7a0bd) )
	ROM_LOAD16_WORD_SWAP( "c73e.q2",   0x080000, 0x080000, CRC(1408a811) SHA1(1d31e3cd505245b5ff4d3c55d5fa7017c0f1e168) )
	ROM_LOAD16_WORD_SWAP( "c73e.q3",   0x100000, 0x080000, CRC(4fb9f57c) SHA1(093e8e3a03b62783a84fe4ae239e9eb46cbfd71e) )
	ROM_LOAD16_WORD_SWAP( "c73e.q4",   0x180000, 0x080000, CRC(9a8f40ec) SHA1(c8db1ecfd6b08e9c83ae53a1d25c1387ab95535c) )
	ROM_LOAD16_WORD_SWAP( "c73e.q5",   0x200000, 0x080000, CRC(91529a91) SHA1(183569100ae98e17688e0e25932850e73a41eb88) )
	ROM_LOAD16_WORD_SWAP( "c73e.q6",   0x280000, 0x080000, CRC(9a345334) SHA1(330291400a73215c9797457507a86c90ba415247) )
	ROM_LOAD16_WORD_SWAP( "c73e.q7",   0x300000, 0x080000, CRC(aedc27f2) SHA1(55137f0f22c4823558e6a8ba76011695579a4f1f) )
	ROM_LOAD16_WORD_SWAP( "c73e.q8",   0x380000, 0x080000, CRC(2300b7b3) SHA1(f5ecbb45c24f7de1c1aa435870695551d4e343ca) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "csclubj.key",  0x00, 0x14, CRC(519a04db) SHA1(8f792831dc7e07fb621806c4e320a047f801e9b8) )
ROM_END


ROM_START( cscluba )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73a.p1", 0x000000, 0x80000, CRC(b6acd708) SHA1(27d316053b0e74b1e9db979d500767cfa49fbce3) )
	ROM_LOAD16_WORD_SWAP( "c73a.p2", 0x080000, 0x80000, CRC(d44ae35f) SHA1(cd464792fe777183b0b0587239fb1b52bd7f9ec7) )
	ROM_LOAD16_WORD_SWAP( "c73a.p3", 0x100000, 0x80000, CRC(8da76aec) SHA1(04552f2c9c424d808703136a7909df903aec290a) )
	ROM_LOAD16_WORD_SWAP( "c73a.p4", 0x180000, 0x80000, CRC(a1b7b1ee) SHA1(77ba745f094a29521bb686982399b8b9babd7cc6) )
	ROM_LOAD16_WORD_SWAP( "c73.p5",  0x200000, 0x80000, CRC(01b05caa) SHA1(5b84487da68e6b6f2889c76bf9e070e25941988c) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c1",  0x800000, 0x200000, CRC(e8904afa) SHA1(39713ffca4e3a754c7c44c0ef4d99fb5a77d8da7) )
	ROM_LOAD64_WORD( "c73.c2",  0x800002, 0x200000, CRC(c98c8079) SHA1(22d68ba2ef62b51981bb3e99ec2cde8d1b36514b) )
	ROM_LOAD64_WORD( "c73.c3",  0x800004, 0x200000, CRC(c030df5a) SHA1(6d5e5a05531e168d0d44c591f9185ae300908fc2) )
	ROM_LOAD64_WORD( "c73.c4",  0x800006, 0x200000, CRC(b4e55863) SHA1(da66f0a36266b906e4c149aec152c323bb184c57) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73.q1",   0x000000, 0x200000, CRC(a027b827) SHA1(6d58a63efc7bd5d07353d9b55826c01a3c416c33) )
	ROM_LOAD16_WORD_SWAP( "c73.q2",   0x200000, 0x200000, CRC(cb7f6e55) SHA1(b64e6b663fd09e887d2dc0f4b545e88688c0af55) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "cscluba.key",  0x00, 0x14, CRC(591908dc) SHA1(aacc663cd0a34505fc6aa94fdeaddfa3409a9ded) )
ROM_END

ROM_START( csclubh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73h.p1", 0x000000, 0x80000, CRC(0dd7e46d) SHA1(deacd350b8954998636065cf070c9955d08402b8) )
	ROM_LOAD16_WORD_SWAP( "c73h.p2", 0x080000, 0x80000, CRC(486e8143) SHA1(d50ab8a5fdc194a9cded74cff94e5b3b69069826) )
	ROM_LOAD16_WORD_SWAP( "c73h.p3", 0x100000, 0x80000, CRC(9e509dfb) SHA1(4a6cd8488a63ad3f7d5a08f2a6af4728dc147790) )
	ROM_LOAD16_WORD_SWAP( "c73h.p4", 0x180000, 0x80000, CRC(817ba313) SHA1(674e10e642c09d26886f3deb829dee330ff472be) )
	ROM_LOAD16_WORD_SWAP( "c73.p5",  0x200000, 0x80000, CRC(01b05caa) SHA1(5b84487da68e6b6f2889c76bf9e070e25941988c) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c1",  0x800000, 0x200000, CRC(e8904afa) SHA1(39713ffca4e3a754c7c44c0ef4d99fb5a77d8da7) )
	ROM_LOAD64_WORD( "c73.c2",  0x800002, 0x200000, CRC(c98c8079) SHA1(22d68ba2ef62b51981bb3e99ec2cde8d1b36514b) )
	ROM_LOAD64_WORD( "c73.c3",  0x800004, 0x200000, CRC(c030df5a) SHA1(6d5e5a05531e168d0d44c591f9185ae300908fc2) )
	ROM_LOAD64_WORD( "c73.c4",  0x800006, 0x200000, CRC(b4e55863) SHA1(da66f0a36266b906e4c149aec152c323bb184c57) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73.q1",   0x000000, 0x200000, CRC(a027b827) SHA1(6d58a63efc7bd5d07353d9b55826c01a3c416c33) )
	ROM_LOAD16_WORD_SWAP( "c73.q2",   0x200000, 0x200000, CRC(cb7f6e55) SHA1(b64e6b663fd09e887d2dc0f4b545e88688c0af55) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "csclubh.key",  0x00, 0x14, CRC(b0adc39e) SHA1(37f29580497d24c8f248031acdfab06374ed77fd) )
ROM_END

ROM_START( cybots )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.p1",  0x000000, 0x80000, CRC(234381cd) SHA1(6202a2a318feda525e16fd7b31c03af9ccc5d964) )
	ROM_LOAD16_WORD_SWAP( "c58.p2",  0x080000, 0x80000, CRC(80691061) SHA1(4f3ef24fc76d3a5b369aa6192ad390d9c3c9b0e8) )
	ROM_LOAD16_WORD_SWAP( "c58.p3",  0x100000, 0x80000, CRC(ec40408e) SHA1(dd611c1708e7ef86e4f7cac4b7b0dff7baaee5ed) )
	ROM_LOAD16_WORD_SWAP( "c58.p4",  0x180000, 0x80000, CRC(1ad0bed2) SHA1(2ea005f3e73b05f8f0ec006cd9e95f7731a73897) )
	ROM_LOAD16_WORD_SWAP( "c58.p5",  0x200000, 0x80000, CRC(6245a39a) SHA1(4f607e733e2dea80211497522be6d0f09571928d) )
	ROM_LOAD16_WORD_SWAP( "c58.p6",  0x280000, 0x80000, CRC(4b48e223) SHA1(9714579a7a78b9716e44bca6c18bf1a93aa4e482) )
	ROM_LOAD16_WORD_SWAP( "c58.p7",  0x300000, 0x80000, CRC(e15238f6) SHA1(16abd92ebed921a6a7e8eac4b098dc61f7e5485c) )
	ROM_LOAD16_WORD_SWAP( "c58.p8",  0x380000, 0x80000, CRC(75f4003b) SHA1(8a65026ae35247cda016ce85a34034c62b3aa1a6) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c58.c1",   0x0000000, 0x400000, CRC(f0dce192) SHA1(b743938dc8e772dc3f63ed88a4a54c34fffdba21) )
	ROM_LOAD64_WORD( "c58.c2",   0x0000002, 0x400000, CRC(187aa39c) SHA1(80e3cf5c69f13343de667e1476bb716d45d3ff63) )
	ROM_LOAD64_WORD( "c58.c3",   0x0000004, 0x400000, CRC(8a0e4b12) SHA1(40132f3cc79b0a74460ebd4e0d4ddbe240efc06f) )
	ROM_LOAD64_WORD( "c58.c4",   0x0000006, 0x400000, CRC(34b62612) SHA1(154bbceb7d303a208abb1b2f3d507d5afacc71ed) )
	ROM_LOAD64_WORD( "c58.c5",   0x1000000, 0x400000, CRC(c1537957) SHA1(bfb1cc6786277b94ce28bfd464e2bbb6f6d3486e) )
	ROM_LOAD64_WORD( "c58.c6",   0x1000002, 0x400000, CRC(15349e86) SHA1(b0cde577d29a9f4e718b673c8645529ef0ababc9) )
	ROM_LOAD64_WORD( "c58.c7",   0x1000004, 0x400000, CRC(d83e977d) SHA1(e03f4a120c95a2f476ffc8492bca85e0c5cea068) )
	ROM_LOAD64_WORD( "c58.c8",   0x1000006, 0x400000, CRC(77cdad5c) SHA1(94d0cc5f05de4bc2d43977d91f887005dc10310c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c58.m1",   0x00000, 0x08000, CRC(9c0fb079) SHA1(06d260875a76da08d56ea2b2ae277e8c2dbae6e3) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c58.m2",   0x28000, 0x20000, CRC(51cb0c4e) SHA1(c322957558d8d3e9dad090aebbe485978cbce8f5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.q1",   0x000000, 0x200000, CRC(362ccab2) SHA1(28e537067d4846f22657ee37166d18b8f05f4da1) )
	ROM_LOAD16_WORD_SWAP( "c58.q2",   0x200000, 0x200000, CRC(7066e9cc) SHA1(eb6a9d4998b3311344d73bae88d661d81609c492) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "cybots.key",   0x00, 0x14, CRC(9bbcbef3) SHA1(ec7a322c8dceb16c98f7c2837679e91b738fdd0a) )
ROM_END

ROM_START( cybotsu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c58u.p1", 0x000000, 0x80000, CRC(db4da8f4) SHA1(de9f3f261003f4f70ae32114a15e498387c23f6d) )
	ROM_LOAD16_WORD_SWAP( "c58u.p2", 0x080000, 0x80000, CRC(1eec68ac) SHA1(b2b9379c84b121048cb83a8c48756b48cdbc3ea1) )
	ROM_LOAD16_WORD_SWAP( "c58.p3",  0x100000, 0x80000, CRC(ec40408e) SHA1(dd611c1708e7ef86e4f7cac4b7b0dff7baaee5ed) )
	ROM_LOAD16_WORD_SWAP( "c58.p4",  0x180000, 0x80000, CRC(1ad0bed2) SHA1(2ea005f3e73b05f8f0ec006cd9e95f7731a73897) )
	ROM_LOAD16_WORD_SWAP( "c58.p5",  0x200000, 0x80000, CRC(6245a39a) SHA1(4f607e733e2dea80211497522be6d0f09571928d) )
	ROM_LOAD16_WORD_SWAP( "c58.p6",  0x280000, 0x80000, CRC(4b48e223) SHA1(9714579a7a78b9716e44bca6c18bf1a93aa4e482) )
	ROM_LOAD16_WORD_SWAP( "c58.p7",  0x300000, 0x80000, CRC(e15238f6) SHA1(16abd92ebed921a6a7e8eac4b098dc61f7e5485c) )
	ROM_LOAD16_WORD_SWAP( "c58.p8",  0x380000, 0x80000, CRC(75f4003b) SHA1(8a65026ae35247cda016ce85a34034c62b3aa1a6) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c58.c1",   0x0000000, 0x400000, CRC(f0dce192) SHA1(b743938dc8e772dc3f63ed88a4a54c34fffdba21) )
	ROM_LOAD64_WORD( "c58.c2",   0x0000002, 0x400000, CRC(187aa39c) SHA1(80e3cf5c69f13343de667e1476bb716d45d3ff63) )
	ROM_LOAD64_WORD( "c58.c3",   0x0000004, 0x400000, CRC(8a0e4b12) SHA1(40132f3cc79b0a74460ebd4e0d4ddbe240efc06f) )
	ROM_LOAD64_WORD( "c58.c4",   0x0000006, 0x400000, CRC(34b62612) SHA1(154bbceb7d303a208abb1b2f3d507d5afacc71ed) )
	ROM_LOAD64_WORD( "c58.c5",   0x1000000, 0x400000, CRC(c1537957) SHA1(bfb1cc6786277b94ce28bfd464e2bbb6f6d3486e) )
	ROM_LOAD64_WORD( "c58.c6",   0x1000002, 0x400000, CRC(15349e86) SHA1(b0cde577d29a9f4e718b673c8645529ef0ababc9) )
	ROM_LOAD64_WORD( "c58.c7",   0x1000004, 0x400000, CRC(d83e977d) SHA1(e03f4a120c95a2f476ffc8492bca85e0c5cea068) )
	ROM_LOAD64_WORD( "c58.c8",   0x1000006, 0x400000, CRC(77cdad5c) SHA1(94d0cc5f05de4bc2d43977d91f887005dc10310c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c58.m1",   0x00000, 0x08000, CRC(9c0fb079) SHA1(06d260875a76da08d56ea2b2ae277e8c2dbae6e3) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c58.m2",   0x28000, 0x20000, CRC(51cb0c4e) SHA1(c322957558d8d3e9dad090aebbe485978cbce8f5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.q1",   0x000000, 0x200000, CRC(362ccab2) SHA1(28e537067d4846f22657ee37166d18b8f05f4da1) )
	ROM_LOAD16_WORD_SWAP( "c58.q2",   0x200000, 0x200000, CRC(7066e9cc) SHA1(eb6a9d4998b3311344d73bae88d661d81609c492) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "cybotsu.key",  0x00, 0x14, CRC(7a09403c) SHA1(1a37f09028714d2b0f4543fab3cbfe5ba4b571ff) )
ROM_END

ROM_START( cybotsj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c58j.p1", 0x000000, 0x80000, CRC(6096eada) SHA1(ea3fa2e6229d90cc3f69c59f447b6b373d64d2aa) )
	ROM_LOAD16_WORD_SWAP( "c58j.p2", 0x080000, 0x80000, CRC(7b0ffaa9) SHA1(595c3e679ea02282bf8a5aa6c7c09e5c30e839c7) )
	ROM_LOAD16_WORD_SWAP( "c58.p3",  0x100000, 0x80000, CRC(ec40408e) SHA1(dd611c1708e7ef86e4f7cac4b7b0dff7baaee5ed) )
	ROM_LOAD16_WORD_SWAP( "c58.p4",  0x180000, 0x80000, CRC(1ad0bed2) SHA1(2ea005f3e73b05f8f0ec006cd9e95f7731a73897) )
	ROM_LOAD16_WORD_SWAP( "c58.p5",  0x200000, 0x80000, CRC(6245a39a) SHA1(4f607e733e2dea80211497522be6d0f09571928d) )
	ROM_LOAD16_WORD_SWAP( "c58.p6",  0x280000, 0x80000, CRC(4b48e223) SHA1(9714579a7a78b9716e44bca6c18bf1a93aa4e482) )
	ROM_LOAD16_WORD_SWAP( "c58.p7",  0x300000, 0x80000, CRC(e15238f6) SHA1(16abd92ebed921a6a7e8eac4b098dc61f7e5485c) )
	ROM_LOAD16_WORD_SWAP( "c58.p8",  0x380000, 0x80000, CRC(75f4003b) SHA1(8a65026ae35247cda016ce85a34034c62b3aa1a6) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c58.c1",   0x0000000, 0x400000, CRC(f0dce192) SHA1(b743938dc8e772dc3f63ed88a4a54c34fffdba21) )
	ROM_LOAD64_WORD( "c58.c2",   0x0000002, 0x400000, CRC(187aa39c) SHA1(80e3cf5c69f13343de667e1476bb716d45d3ff63) )
	ROM_LOAD64_WORD( "c58.c3",   0x0000004, 0x400000, CRC(8a0e4b12) SHA1(40132f3cc79b0a74460ebd4e0d4ddbe240efc06f) )
	ROM_LOAD64_WORD( "c58.c4",   0x0000006, 0x400000, CRC(34b62612) SHA1(154bbceb7d303a208abb1b2f3d507d5afacc71ed) )
	ROM_LOAD64_WORD( "c58.c5",   0x1000000, 0x400000, CRC(c1537957) SHA1(bfb1cc6786277b94ce28bfd464e2bbb6f6d3486e) )
	ROM_LOAD64_WORD( "c58.c6",   0x1000002, 0x400000, CRC(15349e86) SHA1(b0cde577d29a9f4e718b673c8645529ef0ababc9) )
	ROM_LOAD64_WORD( "c58.c7",   0x1000004, 0x400000, CRC(d83e977d) SHA1(e03f4a120c95a2f476ffc8492bca85e0c5cea068) )
	ROM_LOAD64_WORD( "c58.c8",   0x1000006, 0x400000, CRC(77cdad5c) SHA1(94d0cc5f05de4bc2d43977d91f887005dc10310c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c58.m1",   0x00000, 0x08000, CRC(9c0fb079) SHA1(06d260875a76da08d56ea2b2ae277e8c2dbae6e3) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c58.m2",   0x28000, 0x20000, CRC(51cb0c4e) SHA1(c322957558d8d3e9dad090aebbe485978cbce8f5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.q1",   0x000000, 0x200000, CRC(362ccab2) SHA1(28e537067d4846f22657ee37166d18b8f05f4da1) )
	ROM_LOAD16_WORD_SWAP( "c58.q2",   0x200000, 0x200000, CRC(7066e9cc) SHA1(eb6a9d4998b3311344d73bae88d661d81609c492) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "cybotsj.key",  0x00, 0x14, CRC(d4d560b7) SHA1(66f2d85a25140ab9071a0e192ee40e750381bffc) )
ROM_END

ROM_START( ddtod )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.p1", 0x000000, 0x80000, CRC(8e73533d) SHA1(6eece222e562dd0c453d8dec188c9553c46dfe3c) )
	ROM_LOAD16_WORD_SWAP( "c50.p2", 0x080000, 0x80000, CRC(00c2e82e) SHA1(fad4dcdac8d6ef04b71e987936bf27e3d93809fc) )
	ROM_LOAD16_WORD_SWAP( "c50.p3", 0x100000, 0x80000, CRC(ea996008) SHA1(9f41679531e971e62483415c07ef4ee7489ff779) )
	ROM_LOAD16_WORD_SWAP( "c50.p4",  0x180000, 0x80000, CRC(6225495a) SHA1(a9a02abb072e3482ac92d7aed8ce9a5bcf636bc0) )
	ROM_LOAD16_WORD_SWAP( "c50.p5",  0x200000, 0x80000, CRC(b3480ec3) SHA1(a66f8dba67101fd71c2af4f3c3d71e55778a9f2c) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtod.key",    0x00, 0x14, CRC(41dfca41) SHA1(d5fb970d54dff0ab085778530bae32791cb0f38e) )
ROM_END

ROM_START( ddtodr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50r1.p1", 0x000000, 0x80000, CRC(665a035e) SHA1(4aa81f7055bc288be1282dcbf9a33c77d3c963f5) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p2", 0x080000, 0x80000, CRC(02613207) SHA1(a29258848e8f6ac7469c88668a83e07bf325f96a) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p3", 0x100000, 0x80000, CRC(36845996) SHA1(a767564b62c1e25c62e4d31201f10d8c4a368197) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4", 0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5", 0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtod.key",    0x00, 0x14, CRC(41dfca41) SHA1(d5fb970d54dff0ab085778530bae32791cb0f38e) )
ROM_END

ROM_START( ddtodu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50u.p1", 0x000000, 0x80000, CRC(a519905f) SHA1(7f846d7ac5d5e0d06657f712a7a09bee984a4f4b) )
	ROM_LOAD16_WORD_SWAP( "c50u.p2", 0x080000, 0x80000, CRC(52562d38) SHA1(3ee21399a19ee5e2db2a8c2a893d8a31a3419399) )
	ROM_LOAD16_WORD_SWAP( "c50u.p3", 0x100000, 0x80000, CRC(ee1cfbfe) SHA1(4107e495827ada1712a2393dffcdf52d98aca2e0) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodu.key",   0x00, 0x14, CRC(7c03ec9e) SHA1(4fbcb1484dd5b8d569e54a96c81b3aa0faf37789) )
ROM_END

ROM_START( ddtodur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50ur1.p1", 0x000000, 0x80000, CRC(4413f177) SHA1(26c8d06adc83ffc5bec4abf05aa64e874e85d539) )
	ROM_LOAD16_WORD_SWAP( "c50ur1.p2", 0x080000, 0x80000, CRC(168de230) SHA1(3f8af1625bb0d9097e538f8ba7cd23d95b0233aa) )
	ROM_LOAD16_WORD_SWAP( "c50ur1.p3", 0x100000, 0x80000, CRC(03d39e91) SHA1(92461b87c55cb41bbe89bcb3e3f2e9b1ed521067) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",  0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",  0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodu.key",   0x00, 0x14, CRC(7c03ec9e) SHA1(4fbcb1484dd5b8d569e54a96c81b3aa0faf37789) )
ROM_END

ROM_START( ddtodj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50j.p1", 0x000000, 0x80000, CRC(0b1b5798) SHA1(ce2749164a6cf4c99db4bfa7e515a9022006cf92) )
	ROM_LOAD16_WORD_SWAP( "c50j.p2", 0x080000, 0x80000, CRC(c6a2fbc8) SHA1(cb7105e9d35c9e64f5535eb5f491e3f2cf6de64e) )
	ROM_LOAD16_WORD_SWAP( "c50j.p3", 0x100000, 0x80000, CRC(189b15fe) SHA1(91baef189549a25122fd5ab238a849bad2766862) )
	ROM_LOAD16_WORD_SWAP( "c50.p4",  0x180000, 0x80000, CRC(6225495a) SHA1(a9a02abb072e3482ac92d7aed8ce9a5bcf636bc0) )
	ROM_LOAD16_WORD_SWAP( "c50.p5",  0x200000, 0x80000, CRC(b3480ec3) SHA1(a66f8dba67101fd71c2af4f3c3d71e55778a9f2c) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodj.key",   0x00, 0x14, CRC(5414dfca) SHA1(f8cc1e2b9d9b897899e55cc3679b836b27d22453) )
ROM_END

ROM_START( ddtodjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50jr1.p1", 0x000000, 0x80000, CRC(87606b85) SHA1(1311c9ae321207db4632572f6c838b732974b087) )
	ROM_LOAD16_WORD_SWAP( "c50jr1.p2", 0x080000, 0x80000, CRC(24d49575) SHA1(419d7d2f970c23c39334a7f2e8c5caa237769c5d) )
	ROM_LOAD16_WORD_SWAP( "c50jr1.p3", 0x100000, 0x80000, CRC(56ce51f7) SHA1(3a15537c479e3798ec30d5e313a727d7f91955c0) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodj.key",   0x00, 0x14, CRC(5414dfca) SHA1(f8cc1e2b9d9b897899e55cc3679b836b27d22453) )
ROM_END

ROM_START( ddtodjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50jr2.p1", 0x000000, 0x80000, CRC(711638dc) SHA1(30c1d1a694aa8e51d072b26b47ba55aed6d77b7b) )
	ROM_LOAD16_WORD_SWAP( "c50jr2.p2", 0x080000, 0x80000, CRC(4869639c) SHA1(1544813e6712a78267c1d27b6b49148d42c11127) )
	ROM_LOAD16_WORD_SWAP( "c50jr2.p3", 0x100000, 0x80000, CRC(484c0efa) SHA1(d4ddef54149ef0141dcbe05df5f669fccf462559) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodj.key",   0x00, 0x14, CRC(5414dfca) SHA1(f8cc1e2b9d9b897899e55cc3679b836b27d22453) )
ROM_END

ROM_START( ddtoda )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50a.p1", 0x000000, 0x80000, CRC(bf243e15) SHA1(6ee212defb69064a9caddf2e3aab71c24cb67a82) )
	ROM_LOAD16_WORD_SWAP( "c50a.p2", 0x080000, 0x80000, CRC(76551eec) SHA1(71f4fd59ccbca6b212ec21729ac024933abafb74) )
	ROM_LOAD16_WORD_SWAP( "c50a.p3", 0x100000, 0x80000, CRC(0a0ad827) SHA1(f17a3100f40c24862d717edc8e2a75b9cb29f4fe) )
	ROM_LOAD16_WORD_SWAP( "c50.p4",  0x180000, 0x80000, CRC(6225495a) SHA1(a9a02abb072e3482ac92d7aed8ce9a5bcf636bc0) )
	ROM_LOAD16_WORD_SWAP( "c50.p5",  0x200000, 0x80000, CRC(b3480ec3) SHA1(a66f8dba67101fd71c2af4f3c3d71e55778a9f2c) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtoda.key",   0x00, 0x14, CRC(e5e8d1b8) SHA1(04667003776e2dbd207933b5c707bed635871cac) )
ROM_END

ROM_START( ddtodar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50ar1.p1", 0x000000, 0x80000, CRC(fc6f2dd7) SHA1(82f59670ec77a11e9765e2acd0e846d1c768b542) )
	ROM_LOAD16_WORD_SWAP( "c50ar1.p2", 0x080000, 0x80000, CRC(d4be4009) SHA1(c914ddc8f0c237efb52dd1a8f56395b17a6583be) )
	ROM_LOAD16_WORD_SWAP( "c50ar1.p3", 0x100000, 0x80000, CRC(6712d1cf) SHA1(a716ee5ca434badc57f67e0802c6b184bf243dbb) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtoda.key",   0x00, 0x14, CRC(e5e8d1b8) SHA1(04667003776e2dbd207933b5c707bed635871cac) )
ROM_END

ROM_START( ddtodh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50h.p1", 0x000000, 0x80000, CRC(5750a861) SHA1(3b94999779f1b5f7cdaf78468256e2b60ba8c076) )
	ROM_LOAD16_WORD_SWAP( "c50h.p2", 0x080000, 0x80000, CRC(cfbf1b56) SHA1(d8919397f1d6dacddc6811889b69b65cd91be683) )
	ROM_LOAD16_WORD_SWAP( "c50h.p3", 0x100000, 0x80000, CRC(a6e562ba) SHA1(42998024a3bb4464843411ebe7283c6f5369694d) )
	ROM_LOAD16_WORD_SWAP( "c50.p4",  0x180000, 0x80000, CRC(6225495a) SHA1(a9a02abb072e3482ac92d7aed8ce9a5bcf636bc0) )
	ROM_LOAD16_WORD_SWAP( "c50.p5",  0x200000, 0x80000, CRC(b3480ec3) SHA1(a66f8dba67101fd71c2af4f3c3d71e55778a9f2c) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodh.key",   0x00, 0x14, CRC(65f33a1c) SHA1(4bc2c0968115365fd8cd721b769f62ed32b02cf5) )
ROM_END

ROM_START( ddtodhr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50hr1.p1", 0x000000, 0x80000, CRC(ae0cb98e) SHA1(e85fb56d8f55fd1626a47301953b66597814e516) )
	ROM_LOAD16_WORD_SWAP( "c50hr1.p2", 0x080000, 0x80000, CRC(b5774363) SHA1(c91a6b257de4355a29d0a9742909592e69d287fb) )
	ROM_LOAD16_WORD_SWAP( "c50hr1.p3", 0x100000, 0x80000, CRC(6ce2a485) SHA1(7397105bbf88f6f2aa46614395df38b205e6461c) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodh.key",   0x00, 0x14, CRC(65f33a1c) SHA1(4bc2c0968115365fd8cd721b769f62ed32b02cf5) )
ROM_END

ROM_START( ddtodhr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50hr2.p1", 0x000000, 0x80000, CRC(43d04aa3) SHA1(550fcc8ebf48d704223347abee759d0ed903432b) )
	ROM_LOAD16_WORD_SWAP( "c50hr2.p2", 0x080000, 0x80000, CRC(8b8d296c) SHA1(4cd6612855317ba13dd4c6b0dd024243677b5fbe) )
	ROM_LOAD16_WORD_SWAP( "c50hr2.p3", 0x100000, 0x80000, CRC(daae6b14) SHA1(ee132b19f8d8c17da6fd2d2da24205f2404a62d1) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p4",   0x180000, 0x80000, CRC(13aa3e56) SHA1(ccd3cda528d625bbf4dc0e8c5ad629af6080d705) )
	ROM_LOAD16_WORD_SWAP( "c50r1.p5",   0x200000, 0x80000, CRC(431cb6dd) SHA1(ad3342e2fb8f0b3d7f57e845d5b80a871923324d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddtodh.key",   0x00, 0x14, CRC(65f33a1c) SHA1(4bc2c0968115365fd8cd721b769f62ed32b02cf5) )
ROM_END

ROM_START( ddsom )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.p1",  0x000000, 0x80000, CRC(449361af) SHA1(14af2b35e6f43f92c9e071f1dc85b18cf73ecb35) )
	ROM_LOAD16_WORD_SWAP( "c63.p2",  0x080000, 0x80000, CRC(5b7052b6) SHA1(8a5f069f450da939d0f02518751cd9815d621d81) )
	ROM_LOAD16_WORD_SWAP( "c63.p3",  0x100000, 0x80000, CRC(788d5f60) SHA1(b8b42c11530a34c2878fb119c0a388e33067b66d) )
	ROM_LOAD16_WORD_SWAP( "c63.p4",  0x180000, 0x80000, CRC(e0807e1e) SHA1(4b978f5f647fff84d456eb14c9fd202d9a276997) )
	ROM_LOAD16_WORD_SWAP( "c63.p5",  0x200000, 0x80000, CRC(bb777a02) SHA1(4b2c65a9129fc2262b35be1c10d06f60f5108981) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsom.key",    0x00, 0x14, CRC(541e425d) SHA1(9239f9ad7c2ed0d7d062da2db92b07104b5bb9a7) )
ROM_END

ROM_START( ddsomr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63r1.p1", 0x000000, 0x80000, CRC(6c084ab5) SHA1(edfb4094086836c9ba47fed149b5756cf40d8bc1) )
	ROM_LOAD16_WORD_SWAP( "c63r1.p2", 0x080000, 0x80000, CRC(9b94a947) SHA1(6b170d9d07c5fe98a186c2eb7d65639cbbe828fc) )
	ROM_LOAD16_WORD_SWAP( "c63r1.p3", 0x100000, 0x80000, CRC(5d6a63c6) SHA1(2f1cefe2531688c305b3ddcc4c8adae39dcadb33) )
	ROM_LOAD16_WORD_SWAP( "c63r1.p4", 0x180000, 0x80000, CRC(31bde8ee) SHA1(ec409a38ce9b5d464e19a1c93caab2f2de191fa1) )
	ROM_LOAD16_WORD_SWAP( "c63.p5",  0x200000, 0x80000, CRC(bb777a02) SHA1(4b2c65a9129fc2262b35be1c10d06f60f5108981) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsom.key",    0x00, 0x14, CRC(541e425d) SHA1(9239f9ad7c2ed0d7d062da2db92b07104b5bb9a7) )
ROM_END

ROM_START( ddsomr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63r2.p1", 0x000000, 0x80000, CRC(cd2deb66) SHA1(8a3fa5aca364f11bea76f69504e82416efc0ec11) )
	ROM_LOAD16_WORD_SWAP( "c63r2.p2", 0x080000, 0x80000, CRC(bfee43cc) SHA1(16cb34103bede42599ff3083a70ff918fdce9929) )
	ROM_LOAD16_WORD_SWAP( "c63r2.p3", 0x100000, 0x80000, CRC(049ab19d) SHA1(dfd2ed64c409389fed9b1d96955cbe0cf2abd2b7) )
	ROM_LOAD16_WORD_SWAP( "c63r2.p4", 0x180000, 0x80000, CRC(3994fb8b) SHA1(9b864f6cbd9b12d9409fcc2739e12f9a0775f205) )
	ROM_LOAD16_WORD_SWAP( "c63.p5",  0x200000, 0x80000, CRC(bb777a02) SHA1(4b2c65a9129fc2262b35be1c10d06f60f5108981) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsom.key",    0x00, 0x14, CRC(541e425d) SHA1(9239f9ad7c2ed0d7d062da2db92b07104b5bb9a7) )
ROM_END

ROM_START( ddsomr3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63r3.p1", 0x000000, 0x80000, CRC(6de67678) SHA1(df4846b963ea0449fbd16152600a9628d20b278a) )
	ROM_LOAD16_WORD_SWAP( "c63r3.p2", 0x080000, 0x80000, CRC(0e45739a) SHA1(4fa9dc8109fad30e4037047ddec1b367b5b7600f) )
	ROM_LOAD16_WORD_SWAP( "c63r3.p3", 0x100000, 0x80000, CRC(3dce8025) SHA1(5fb0a58b7cccc889507b0085b5e74d7aef507f08) )
	ROM_LOAD16_WORD_SWAP( "c63r3.p4", 0x180000, 0x80000, CRC(51bafbef) SHA1(08edeca2339e4b48e78ff9b71f576379ae03f5f4) )
	ROM_LOAD16_WORD_SWAP( "c63.p5",  0x200000, 0x80000, CRC(bb777a02) SHA1(4b2c65a9129fc2262b35be1c10d06f60f5108981) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsom.key",    0x00, 0x14, CRC(541e425d) SHA1(9239f9ad7c2ed0d7d062da2db92b07104b5bb9a7) )
ROM_END

ROM_START( ddsomu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63u.p1", 0x000000, 0x80000, CRC(fb089b39) SHA1(2d00ad87d5e862745d730a84a8b9b9a8c9423282) )
	ROM_LOAD16_WORD_SWAP( "c63u.p2", 0x080000, 0x80000, CRC(cd432b73) SHA1(7c5ddad66f9f08fef79efb01ccf230a9eae366c6) )
	ROM_LOAD16_WORD_SWAP( "c63u.p3", 0x100000, 0x80000, CRC(5eb1991c) SHA1(429a60b5396ff4192904867fbe0524268f0edbcb) )
	ROM_LOAD16_WORD_SWAP( "c63u.p4", 0x180000, 0x80000, CRC(c26b5e55) SHA1(9590206f30459941880ff4b56c7f276cc78e3a22) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomu.key",   0x00, 0x14, CRC(09ae0f7c) SHA1(c8c4e8298c8a6c37b91f9ae95474a15dfad3e986) )
ROM_END

ROM_START( ddsomur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63ur1.p1", 0x000000, 0x80000, CRC(0f700d84) SHA1(f4788d4046e0f6aba146c18a930196f5f9f8f14a) )
	ROM_LOAD16_WORD_SWAP( "c63ur1.p2", 0x080000, 0x80000, CRC(b99eb254) SHA1(507ad31b0d77dfbaaaf0fa5830c4ef14845a80de) )
	ROM_LOAD16_WORD_SWAP( "c63ur1.p3", 0x100000, 0x80000, CRC(b23061f3) SHA1(471a1238770a5109f34a0b450b214a5490cc6ecb) )
	ROM_LOAD16_WORD_SWAP( "c63ur1.p4", 0x180000, 0x80000, CRC(8bf1d8ce) SHA1(384dda9dfa2a851d30432f29bba456e138a5ca28) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomu.key",   0x00, 0x14, CRC(09ae0f7c) SHA1(c8c4e8298c8a6c37b91f9ae95474a15dfad3e986) )
ROM_END

ROM_START( ddsomj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63j.p1", 0x000000, 0x80000, CRC(e6c8c985) SHA1(0736a84d7d9d37d51826eac6826a7728260bc625) )
	ROM_LOAD16_WORD_SWAP( "c63j.p2", 0x080000, 0x80000, CRC(8386c0bd) SHA1(59bfc71914ec2bb7d1b9f327b25d2399181d4bb2) )
	ROM_LOAD16_WORD_SWAP( "c63u.p3", 0x100000, 0x80000, CRC(5eb1991c) SHA1(429a60b5396ff4192904867fbe0524268f0edbcb) )
	ROM_LOAD16_WORD_SWAP( "c63u.p4", 0x180000, 0x80000, CRC(c26b5e55) SHA1(9590206f30459941880ff4b56c7f276cc78e3a22) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomj.key",   0x00, 0x14, CRC(d8dadb22) SHA1(e07b50ae464c35439333dd983e2acac48a1ce529) )
ROM_END

ROM_START( ddsomjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63jr1.p1", 0x000000, 0x80000, CRC(965d74e5) SHA1(d7aa1b78043cdf09ee71a6dd5fe78e0588ca7875) )
	ROM_LOAD16_WORD_SWAP( "c63jr1.p2", 0x080000, 0x80000, CRC(958eb8f3) SHA1(3d9747bc9091b0b42c953a19992b94cb2bf69159) )
	ROM_LOAD16_WORD_SWAP( "c63jr1.p3",  0x100000, 0x80000, CRC(d38571ca) SHA1(f0105a4f201e11f489e44c8061b0025de2e32f93) )
	ROM_LOAD16_WORD_SWAP( "c63jr1.p4",  0x180000, 0x80000, CRC(6d5a3bbb) SHA1(549e31398e706a80d41db6600555e27e902c335c) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomj.key",   0x00, 0x14, CRC(d8dadb22) SHA1(e07b50ae464c35439333dd983e2acac48a1ce529) )
ROM_END

ROM_START( ddsomjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63jr2.p1", 0x000000, 0x80000, CRC(b2fd4a24) SHA1(6b5bd11e665e84a9de56d167a5d8957a3a967f8c) )
	ROM_LOAD16_WORD_SWAP( "c63jr2.p2", 0x080000, 0x80000, CRC(3a68c310) SHA1(8806ea2765e82f396158f0ae8a906cda90bd3472) )
	ROM_LOAD16_WORD_SWAP( "c63jr2.p3",  0x100000, 0x80000, CRC(aa56f42f) SHA1(2795a1c8978c5f459daaca57985c24027139d7e1) )
	ROM_LOAD16_WORD_SWAP( "c63jr2.p4",  0x180000, 0x80000, CRC(2f8cd040) SHA1(f2ec09d06432a5f44dd85557d94b35d01b0ece6a) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomj.key",   0x00, 0x14, CRC(d8dadb22) SHA1(e07b50ae464c35439333dd983e2acac48a1ce529) )
ROM_END

ROM_START( ddsoma )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63a.p1", 0x000000, 0x80000, CRC(0b4fec22) SHA1(4dd69637898e0bc64d1b1dc34561ce1807da314b) )
	ROM_LOAD16_WORD_SWAP( "c63a.p2", 0x080000, 0x80000, CRC(055b7019) SHA1(5dab39552fee20bd6f94c992c1c3a995595fdf94) )
	ROM_LOAD16_WORD_SWAP( "c63u.p3",  0x100000, 0x80000, CRC(5eb1991c) SHA1(429a60b5396ff4192904867fbe0524268f0edbcb) )
	ROM_LOAD16_WORD_SWAP( "c63u.p4",  0x180000, 0x80000, CRC(c26b5e55) SHA1(9590206f30459941880ff4b56c7f276cc78e3a22) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsoma.key",   0x00, 0x14, CRC(8c3cc560) SHA1(ad19e78b938a67c9f5ed0dcd5c68b383e6ce0575) )
ROM_END


ROM_START( ddsomar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63ar1.p1", 0x000000, 0x80000, CRC(17162039) SHA1(d6e161cc888c2c7cd7b955ef9f1fc47a5302e29d) )
	ROM_LOAD16_WORD_SWAP( "c63ar1.p2", 0x080000, 0x80000, CRC(950bec38) SHA1(99b50388015730142d28263de1b4f918f7a353fa) )
	ROM_LOAD16_WORD_SWAP( "c63ar1.p3", 0x100000, 0x80000, CRC(fa298eba) SHA1(64d248de4e5655129bb3a420020ca041f5e7b740) )
	ROM_LOAD16_WORD_SWAP( "c63ar1.p4", 0x180000, 0x80000, CRC(28f75b35) SHA1(01f8eb18918a674e5076ef2c09508b90d05d2e7d) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63u.p8", 0x380000, 0x80000, CRC(ad954c26) SHA1(468c01735dbdb1114b37060546a660678290a97f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsoma.key",   0x00, 0x14, CRC(8c3cc560) SHA1(ad19e78b938a67c9f5ed0dcd5c68b383e6ce0575) )
ROM_END

ROM_START( ddsomb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63b.p1", 0x000000, 0x80000, CRC(e8ce7fbb) SHA1(645133fb07b34f663709896a0f55a9a51de4ee9b) )
	ROM_LOAD16_WORD_SWAP( "c63b.p2", 0x080000, 0x80000, CRC(6b679664) SHA1(480d8b225c69d528528b6a4db86797a9d9e6ac80) )
	ROM_LOAD16_WORD_SWAP( "c63b.p3", 0x100000, 0x80000, CRC(9b2534eb) SHA1(04a9f9b75f817dff1b94641aba399d487b57a9f7) )
	ROM_LOAD16_WORD_SWAP( "c63b.p4", 0x180000, 0x80000, CRC(3b21ba59) SHA1(0b9be23253c42047ebfe3e656670ebf5e792766f) )
	ROM_LOAD16_WORD_SWAP( "c63b.p5", 0x200000, 0x80000, CRC(fce2558d) SHA1(67041b550bcb357f1c76e3ab703c7db3cc071515) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomb.key",   0x00, 0x14, CRC(00b4cc49) SHA1(f905cfa6d97741f77994c8c9d0a1164f064f9343) )
ROM_END

ROM_START( ddsomh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63h.p1", 0x000000, 0x80000, CRC(e472c9f3) SHA1(43e0767cca3ce0c151f1bd12d476cc2f0013b5e5) )
	ROM_LOAD16_WORD_SWAP( "c63h.p2", 0x080000, 0x80000, CRC(315a7706) SHA1(de6ced251a483455b0d0371c60bce0c209879b43) )
	ROM_LOAD16_WORD_SWAP( "c63b.p3", 0x100000, 0x80000, CRC(9b2534eb) SHA1(04a9f9b75f817dff1b94641aba399d487b57a9f7) )
	ROM_LOAD16_WORD_SWAP( "c63b.p4", 0x180000, 0x80000, CRC(3b21ba59) SHA1(0b9be23253c42047ebfe3e656670ebf5e792766f) )
	ROM_LOAD16_WORD_SWAP( "c63b.p5", 0x200000, 0x80000, CRC(fce2558d) SHA1(67041b550bcb357f1c76e3ab703c7db3cc071515) )
	ROM_LOAD16_WORD_SWAP( "c63.p6",  0x280000, 0x80000, CRC(30970890) SHA1(fd366a9323230f6997006ab4cc216f9a97865ebe) )
	ROM_LOAD16_WORD_SWAP( "c63.p7",  0x300000, 0x80000, CRC(99e2194d) SHA1(cbcecdf5beeac3eac6c2c3fa395710e1b8347531) )
	ROM_LOAD16_WORD_SWAP( "c63.p8",  0x380000, 0x80000, CRC(e198805e) SHA1(37ae9d88d98c59337b657cfa6feb56e4f9cae95f) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ddsomh.key",   0x00, 0x14, CRC(caf6b540) SHA1(c17ea2dab973f54ce2eee457f83c519b86087117) )
ROM_END

ROM_START( dimahoo )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c81.p1",  0x000000, 0x80000, CRC(968fcecd) SHA1(82d6eb6488df48bc7b977fc900c24b29bd6573a9) )
	ROM_LOAD16_WORD_SWAP( "c81.p2",  0x080000, 0x80000, CRC(37485567) SHA1(643c41fce6057bcaef0e0bedc62914c33d97eeaf) )
	ROM_LOAD16_WORD_SWAP( "c81.p3",  0x100000, 0x80000, CRC(da269ffb) SHA1(e99b04192030b6006cf67b563f40cea29c1b2e78) )
	ROM_LOAD16_WORD_SWAP( "c81.p4",  0x180000, 0x80000, CRC(55b483c9) SHA1(d47e077312f3c044d3647b79fa9e0581ccff5992) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c81.c1",   0x000000, 0x400000, CRC(80dd19f0) SHA1(0fd8b1e8d73cc83e6c34f0d94487938da2344f76) )
	ROM_LOAD64_WORD( "c81.c2",   0x000002, 0x400000, CRC(dfd93a78) SHA1(c343d5ddcc25bd0739491e7439d7c0d0a8881a04) )
	ROM_LOAD64_WORD( "c81.c3",   0x000004, 0x400000, CRC(16356520) SHA1(058713bef30c1b1d8b7dd0ceaaa57a3ab9751a70) )
	ROM_LOAD64_WORD( "c81.c4",   0x000006, 0x400000, CRC(dfc33031) SHA1(a1ceaeddc2a79d5b79f1b107cac2ef6a5e621e77) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c81.m1",   0x00000, 0x08000, CRC(3f9bc985) SHA1(1616bbee82877b1052a07531066f5009a80706be) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c81.m2",   0x28000, 0x20000, CRC(3fd39dde) SHA1(6a6e3ef9baa430ee83ab2312aa0221bae4d73dbd) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c81.q1",   0x000000, 0x400000, CRC(06a65542) SHA1(a1b3df70c90055a3cd59d0149fd18a74eff5bcc9) )
	ROM_LOAD16_WORD_SWAP( "c81.q2",   0x400000, 0x400000, CRC(50bc7a31) SHA1(7283569fc646c39f4c693f14e0ce7ff2ee49111a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dimahoo.key",  0x00, 0x14, CRC(7d6d2db9) SHA1(b21f0e7b6c8952c53be43cd33044feeb7736cae6) )
ROM_END

ROM_START( dimahoou )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c81u.p1", 0x000000, 0x80000, CRC(43bcb15f) SHA1(8cf758f9b3b416273e5b20e5d1c09c0a67029a01) )
	ROM_LOAD16_WORD_SWAP( "c81.p2",  0x080000, 0x80000, CRC(37485567) SHA1(643c41fce6057bcaef0e0bedc62914c33d97eeaf) )
	ROM_LOAD16_WORD_SWAP( "c81.p3",  0x100000, 0x80000, CRC(da269ffb) SHA1(e99b04192030b6006cf67b563f40cea29c1b2e78) )
	ROM_LOAD16_WORD_SWAP( "c81.p4",  0x180000, 0x80000, CRC(55b483c9) SHA1(d47e077312f3c044d3647b79fa9e0581ccff5992) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c81.c1",   0x000000, 0x400000, CRC(80dd19f0) SHA1(0fd8b1e8d73cc83e6c34f0d94487938da2344f76) )
	ROM_LOAD64_WORD( "c81.c2",   0x000002, 0x400000, CRC(dfd93a78) SHA1(c343d5ddcc25bd0739491e7439d7c0d0a8881a04) )
	ROM_LOAD64_WORD( "c81.c3",   0x000004, 0x400000, CRC(16356520) SHA1(058713bef30c1b1d8b7dd0ceaaa57a3ab9751a70) )
	ROM_LOAD64_WORD( "c81.c4",   0x000006, 0x400000, CRC(dfc33031) SHA1(a1ceaeddc2a79d5b79f1b107cac2ef6a5e621e77) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c81.m1",   0x00000, 0x08000, CRC(3f9bc985) SHA1(1616bbee82877b1052a07531066f5009a80706be) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c81.m2",   0x28000, 0x20000, CRC(3fd39dde) SHA1(6a6e3ef9baa430ee83ab2312aa0221bae4d73dbd) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c81.q1",   0x000000, 0x400000, CRC(06a65542) SHA1(a1b3df70c90055a3cd59d0149fd18a74eff5bcc9) )
	ROM_LOAD16_WORD_SWAP( "c81.q2",   0x400000, 0x400000, CRC(50bc7a31) SHA1(7283569fc646c39f4c693f14e0ce7ff2ee49111a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dimahoou.key", 0x00, 0x14, CRC(8254d7ab) SHA1(023ffb14dd03548a23ad4ab07d16041a31c75691) )
ROM_END

ROM_START( gmahou )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c81j.p1", 0x000000, 0x80000, CRC(cd6979e3) SHA1(b033408f49299eac376fc798c3429e5db97dd4fe) )
	ROM_LOAD16_WORD_SWAP( "c81.p2",  0x080000, 0x80000, CRC(37485567) SHA1(643c41fce6057bcaef0e0bedc62914c33d97eeaf) )
	ROM_LOAD16_WORD_SWAP( "c81.p3",  0x100000, 0x80000, CRC(da269ffb) SHA1(e99b04192030b6006cf67b563f40cea29c1b2e78) )
	ROM_LOAD16_WORD_SWAP( "c81.p4",  0x180000, 0x80000, CRC(55b483c9) SHA1(d47e077312f3c044d3647b79fa9e0581ccff5992) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c81.c1",   0x000000, 0x400000, CRC(80dd19f0) SHA1(0fd8b1e8d73cc83e6c34f0d94487938da2344f76) )
	ROM_LOAD64_WORD( "c81.c2",   0x000002, 0x400000, CRC(dfd93a78) SHA1(c343d5ddcc25bd0739491e7439d7c0d0a8881a04) )
	ROM_LOAD64_WORD( "c81.c3",   0x000004, 0x400000, CRC(16356520) SHA1(058713bef30c1b1d8b7dd0ceaaa57a3ab9751a70) )
	ROM_LOAD64_WORD( "c81.c4",   0x000006, 0x400000, CRC(dfc33031) SHA1(a1ceaeddc2a79d5b79f1b107cac2ef6a5e621e77) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c81.m1",   0x00000, 0x08000, CRC(3f9bc985) SHA1(1616bbee82877b1052a07531066f5009a80706be) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c81.m2",   0x28000, 0x20000, CRC(3fd39dde) SHA1(6a6e3ef9baa430ee83ab2312aa0221bae4d73dbd) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c81.q1",   0x000000, 0x400000, CRC(06a65542) SHA1(a1b3df70c90055a3cd59d0149fd18a74eff5bcc9) )
	ROM_LOAD16_WORD_SWAP( "c81.q2",   0x400000, 0x400000, CRC(50bc7a31) SHA1(7283569fc646c39f4c693f14e0ce7ff2ee49111a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gmahou.key",   0x00, 0x14, CRC(76a5e659) SHA1(7babf076dee618ec9f88d8ca0eaef6af29c43a3b) )
ROM_END

ROM_START( dstlk )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.p1", 0x000000, 0x80000, CRC(004c9cff) SHA1(9cab8ab734b29abf321b47e46271dab549bf46df) )
	ROM_LOAD16_WORD_SWAP( "c53.p2", 0x080000, 0x80000, CRC(ae413ff2) SHA1(e9b85ac04d6d1a57368c70aa24e3ab8a8d67409f) )
	ROM_LOAD16_WORD_SWAP( "c53.p3", 0x100000, 0x80000, CRC(60678756) SHA1(5d10829ad7522b5de3b318dd8cbf1b506ba4c2d4) )
	ROM_LOAD16_WORD_SWAP( "c53.p4", 0x180000, 0x80000, CRC(912870b3) SHA1(9c7620c7e25d236050411ba94fbc5b3b501970a3) )
	ROM_LOAD16_WORD_SWAP( "c53.p5", 0x200000, 0x80000, CRC(dabae3e8) SHA1(126f8433491db36649f5e1908bbe45eb123048e4) )
	ROM_LOAD16_WORD_SWAP( "c53.p6", 0x280000, 0x80000, CRC(2c6e3077) SHA1(d8042312ec546e3e807e3ef0a14af9b4f716e415) )
	ROM_LOAD16_WORD_SWAP( "c53.p7", 0x300000, 0x80000, CRC(f16db74b) SHA1(7b7e31916a61e7fb35ec20849c6d22d74e169ec0) )
	ROM_LOAD16_WORD_SWAP( "c53.p8", 0x380000, 0x80000, CRC(701e2147) SHA1(c0a0603e01fbed67a600b83902091c1073e2ed27) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dstlk.key",    0x00, 0x14, CRC(cfa46dec) SHA1(ce57b3d53557f04ac53dac97d67374c288d85866) )
ROM_END

ROM_START( dstlku )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53u.p1", 0x000000, 0x80000, CRC(68a6343f) SHA1(9e1b13e3419470b3c14065c85342b2dcf42eb4cd) )
	ROM_LOAD16_WORD_SWAP( "c53u.p2", 0x080000, 0x80000, CRC(58161453) SHA1(7b5674b8bdb7e3165e090105f6716073696d4bd0) )
	ROM_LOAD16_WORD_SWAP( "c53u.p3", 0x100000, 0x80000, CRC(dfc038b8) SHA1(1b8911033a458f2d20f740c1bd1b3a2157d24b8a) )
	ROM_LOAD16_WORD_SWAP( "c53u.p4", 0x180000, 0x80000, CRC(c3842c89) SHA1(38137ae2c4ec2a6523413c0891287ad7ae70f005) )
	ROM_LOAD16_WORD_SWAP( "c53u.p5", 0x200000, 0x80000, CRC(25b60b6e) SHA1(8b7dc014d1953a6f4c003811ef8813d46136959d) )
	ROM_LOAD16_WORD_SWAP( "c53u.p6", 0x280000, 0x80000, CRC(2113c596) SHA1(6c0e5c406c08af922920500679eaa89e0b83f029) )
	ROM_LOAD16_WORD_SWAP( "c53u.p7", 0x300000, 0x80000, CRC(2d1e9ae5) SHA1(1c4aced7dd0356ee445ca1e5db2c3a2ad4ee56c6) )
	ROM_LOAD16_WORD_SWAP( "c53u.p8", 0x380000, 0x80000, CRC(81145622) SHA1(66c5439b564cea4b49c47db7e095283481d962c7) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dstlku.key",   0x00, 0x14, CRC(c76091ba) SHA1(bfd2f74d0b4a4df128f2874f09b0e8b957d1f47b) )
ROM_END

ROM_START( dstlkur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p1", 0x000000, 0x80000, CRC(628899f9) SHA1(989414a62aed67504f15a542a148e32a4b349949) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p2", 0x080000, 0x80000, CRC(696d9b25) SHA1(743c53ac7fc27960ecc80fed3f2a3c506ee655a1) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p3", 0x100000, 0x80000, CRC(673ed50a) SHA1(7dff27dba1da55a18eb459e4a2d679cf699f2804) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p4", 0x180000, 0x80000, CRC(f2377be7) SHA1(4520d44f94a03bd40c27062344e56ba8718c2fb8) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p5", 0x200000, 0x80000, CRC(d8f498c4) SHA1(569d9c309e9d95d2501a7c0a2c1291b49320d767) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p6", 0x280000, 0x80000, CRC(e6a8a1a0) SHA1(adf621e12623a2af4dbf0858a8fa3816e7c7073b) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p7", 0x300000, 0x80000, CRC(8dd55b24) SHA1(d99c2cbc4a9899a3d187201e6e730b7b8fb13d1d) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p8", 0x380000, 0x80000, CRC(c1a3d9be) SHA1(82b4ce3325a7ecf3a60dd781f9b224fdde8daa65) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dstlku.key",   0x00, 0x14, CRC(c76091ba) SHA1(bfd2f74d0b4a4df128f2874f09b0e8b957d1f47b) )
ROM_END

ROM_START( vampj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53j.p1", 0x000000, 0x80000, CRC(f55d3722) SHA1(e26bbcc47a2485914d567a6cf1cddd0f668689a1) )
	ROM_LOAD16_WORD_SWAP( "c53j.p2", 0x080000, 0x80000, CRC(4d9c43c4) SHA1(2087090306646fed959d503ee75e24996ad95b88) )
	ROM_LOAD16_WORD_SWAP( "c53j.p3", 0x100000, 0x80000, CRC(6c497e92) SHA1(7c1ccdfd77fb50afe024c8402376daaeab641a24) )
	ROM_LOAD16_WORD_SWAP( "c53j.p4", 0x180000, 0x80000, CRC(f1bbecb6) SHA1(6adba89393e05f16f70b57085cabd6b4c20f53e8) )
	ROM_LOAD16_WORD_SWAP( "c53j.p5", 0x200000, 0x80000, CRC(1067ad84) SHA1(5e4cc75cfdfd512b6230c656e7304262b5143aee) )
	ROM_LOAD16_WORD_SWAP( "c53j.p6", 0x280000, 0x80000, CRC(4b89f41f) SHA1(bd78f33a6d448655eecf7448921d282b302fa4cb) )
	ROM_LOAD16_WORD_SWAP( "c53j.p7", 0x300000, 0x80000, CRC(fc0a4aac) SHA1(a2c79eb4dc838c238e182a4da3567ac8db3488d8) )
	ROM_LOAD16_WORD_SWAP( "c53j.p8", 0x380000, 0x80000, CRC(9270c26b) SHA1(c2a7e199a74c9f27704cf935483ebddc6da256a1) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vampj.key",    0x00, 0x14, CRC(8418cc6f) SHA1(e7128ac94fc7c37a64e4640db08f1769215769e5) )
ROM_END

ROM_START( vampja )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53j.p1", 0x000000, 0x80000, CRC(f55d3722) SHA1(e26bbcc47a2485914d567a6cf1cddd0f668689a1) )
	ROM_LOAD16_WORD_SWAP( "c53ja.p2", 0x080000, 0x80000, CRC(fdcbdae3) SHA1(46a1251a2affbe13b200448cf77a455d840f3c9f) )
	ROM_LOAD16_WORD_SWAP( "c53j.p3", 0x100000, 0x80000, CRC(6c497e92) SHA1(7c1ccdfd77fb50afe024c8402376daaeab641a24) )
	ROM_LOAD16_WORD_SWAP( "c53j.p4", 0x180000, 0x80000, CRC(f1bbecb6) SHA1(6adba89393e05f16f70b57085cabd6b4c20f53e8) )
	ROM_LOAD16_WORD_SWAP( "c53j.p5", 0x200000, 0x80000, CRC(1067ad84) SHA1(5e4cc75cfdfd512b6230c656e7304262b5143aee) )
	ROM_LOAD16_WORD_SWAP( "c53j.p6", 0x280000, 0x80000, CRC(4b89f41f) SHA1(bd78f33a6d448655eecf7448921d282b302fa4cb) )
	ROM_LOAD16_WORD_SWAP( "c53j.p7", 0x300000, 0x80000, CRC(fc0a4aac) SHA1(a2c79eb4dc838c238e182a4da3567ac8db3488d8) )
	ROM_LOAD16_WORD_SWAP( "c53j.p8", 0x380000, 0x80000, CRC(9270c26b) SHA1(c2a7e199a74c9f27704cf935483ebddc6da256a1) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vampj.key",    0x00, 0x14, CRC(8418cc6f) SHA1(e7128ac94fc7c37a64e4640db08f1769215769e5) )
ROM_END

ROM_START( vampjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p1", 0x000000, 0x80000, CRC(8895bf77) SHA1(7977dad8c4baf89f28668f54225233a8e759aa3e) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p2", 0x080000, 0x80000, CRC(5027db3d) SHA1(64bd09f2b5fd2435d8ec86f64543b640ab08f82f) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p3", 0x100000, 0x80000, CRC(97c66fdb) SHA1(fe5c099dd29797aef28a247913f8931aa8ce6160) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p4", 0x180000, 0x80000, CRC(9b4c3426) SHA1(a527535e5d23c3d12bac7617fd5d8e15c2522bbd) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p5", 0x200000, 0x80000, CRC(303bc4fd) SHA1(2e3b687c725d389afa7c3e1fe8720a53d0f40269) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p6", 0x280000, 0x80000, CRC(3dea3646) SHA1(3b3f7105284a04b12b3de40633bc8f21a8d73f58) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p7", 0x300000, 0x80000, CRC(c119a827) SHA1(422864dda2a12621175350b8a130f970ed690719) )
	ROM_LOAD16_WORD_SWAP( "c53jr1.p8", 0x380000, 0x80000, CRC(46593b79) SHA1(ff003cc80ed4f3cfaff722b43a09076828c9a9d7) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vampj.key",    0x00, 0x14, CRC(8418cc6f) SHA1(e7128ac94fc7c37a64e4640db08f1769215769e5) )
ROM_END

ROM_START( dstlka )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53a.p1", 0x000000, 0x80000, CRC(294e0bec) SHA1(e90844cd18ad431e999e606d076738384b346b9d) )
	ROM_LOAD16_WORD_SWAP( "c53a.p2", 0x080000, 0x80000, CRC(bc18e128) SHA1(53116cddb7123b573d76064640c3829fd978c67a) )
	ROM_LOAD16_WORD_SWAP( "c53a.p3", 0x100000, 0x80000, CRC(e709fa59) SHA1(824d2b22c5627b9dba046b76c1ff5a46f577eddd) )
	ROM_LOAD16_WORD_SWAP( "c53a.p4", 0x180000, 0x80000, CRC(55e4d387) SHA1(c8b9be072e5de44e6d50f7a80d4c79ae1449588e) )
	ROM_LOAD16_WORD_SWAP( "c53a.p5", 0x200000, 0x80000, CRC(24e8f981) SHA1(5dd28efa325fded290d9eb1643be83ab84a2ac8e) )
	ROM_LOAD16_WORD_SWAP( "c53a.p6", 0x280000, 0x80000, CRC(743f3a8e) SHA1(f7bde0f989582ba2cf93c9397cc38d3eec9ad92d) )
	ROM_LOAD16_WORD_SWAP( "c53a.p7", 0x300000, 0x80000, CRC(67fa5573) SHA1(2dab32cf0d361d2c52cce5eb41b389a0e32dd192) )
	ROM_LOAD16_WORD_SWAP( "c53a.p8", 0x380000, 0x80000, CRC(5e03d747) SHA1(044ef85ca927108f5e66967819dbf7c25bb34f77) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dstlka.key",   0x00, 0x14, CRC(d31d61bc) SHA1(d008f22de82d6df1430ceb5c121962bd0e322057) )
ROM_END

ROM_START( dstlkh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53h.p1", 0x000000, 0x80000, CRC(4d7b9e8f) SHA1(08555da1b024e6ab4dfe802352065c132ddc2abb) )
	ROM_LOAD16_WORD_SWAP( "c53h.p2", 0x080000, 0x80000, CRC(2217e9a0) SHA1(b86ee89457d8a0cf828f1bed247f3b2c0c91b170) )
	ROM_LOAD16_WORD_SWAP( "c53h.p3", 0x100000, 0x80000, CRC(3a05b13c) SHA1(14b58954bdff8dd699f867037a86f0bae8095e9d) )
	ROM_LOAD16_WORD_SWAP( "c53h.p4", 0x180000, 0x80000, CRC(11d70a1c) SHA1(e13c5afeb9cb64ec60d570b81d7fac4869c76d1d) )
	ROM_LOAD16_WORD_SWAP( "c53h.p5", 0x200000, 0x80000, CRC(db5a8767) SHA1(86274080e4423d09e10f2db56a4e685b32acfa18) )
	ROM_LOAD16_WORD_SWAP( "c53h.p6", 0x280000, 0x80000, CRC(2a4fd79b) SHA1(ff0398db43ef849365ad88b9b57661db3a3b65c6) )
	ROM_LOAD16_WORD_SWAP( "c53h.p7", 0x300000, 0x80000, CRC(15187632) SHA1(81b7166334dc3c331673822c31581e0e7809b698) )
	ROM_LOAD16_WORD_SWAP( "c53h.p8", 0x380000, 0x80000, CRC(192d2d81) SHA1(ea99f2ea3e28edfc203e967924500dad10abb43f) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "dstlkh.key",   0x00, 0x14, CRC(d748cb77) SHA1(748be38bbc766be8eebf6a60770801942ad502f2) )
ROM_END

ROM_START( ecofghtr )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.p1", 0x000000, 0x80000, CRC(ec2c1137) SHA1(19c5b68cccd682d3996faf8c5f07a644b2384b1c) )
	ROM_LOAD16_WORD_SWAP( "c49.p2", 0x080000, 0x80000, CRC(b35f99db) SHA1(4dd5c4840406a9323431f5bda7224cadacf8b419) )
	ROM_LOAD16_WORD_SWAP( "c49.p3", 0x100000, 0x80000, CRC(d9d42d31) SHA1(58e7438fa212655ca56cbb477ea353e1083e0933) )
	ROM_LOAD16_WORD_SWAP( "c49.p4", 0x180000, 0x80000, CRC(9d9771cf) SHA1(d1c76672f2e0437cd1204d5552d32ed3377c1356) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ecofghtr.key", 0x00, 0x14, CRC(2250fd9e) SHA1(167a189a4bd4a50c5df4ef9c19cb3d1ff11848f5) )
ROM_END

ROM_START( ecofghtru )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49u.p1", 0x000000, 0x80000, CRC(22d88a4d) SHA1(0aa5a4b51ae98b8b3bfc65aef9449796ffad7f10) )
	ROM_LOAD16_WORD_SWAP( "c49u.p2", 0x080000, 0x80000, CRC(6436cfcd) SHA1(adb4e1ab2a01a1ea1b08a76ecf58654450e13cf9) )
	ROM_LOAD16_WORD_SWAP( "c49u.p3", 0x100000, 0x80000, CRC(336f121b) SHA1(93800c459b516382cc62cebeb456274f48322fab) )
	ROM_LOAD16_WORD_SWAP( "c49u.p4", 0x180000, 0x80000, CRC(6f99d984) SHA1(3f962197edab648bf87c1f2976956ea21e7ac3c4) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ecofghtru.key", 0x00, 0x14, CRC(611aa137) SHA1(933e651779ea0b26b657de5dc62703e37821b07f) )
ROM_END

ROM_START( ecofghtru1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49ur1.p1", 0x000000, 0x80000, CRC(6792480c) SHA1(89d7095a36a1094237f9e1d82a0dc482409999ca) )
	ROM_LOAD16_WORD_SWAP( "c49ur1.p2", 0x080000, 0x80000, CRC(95ce69d5) SHA1(d32c7e2a99ae29cbf9fee1e092a418f300a218ca) )
	ROM_LOAD16_WORD_SWAP( "c49ur1.p3", 0x100000, 0x80000, CRC(3a1e78ad) SHA1(133b65cd4bfd45a9d9d3feec9ccb1e6fc2891818) )
	ROM_LOAD16_WORD_SWAP( "c49ur1.p4", 0x180000, 0x80000, CRC(a3e2f3cc) SHA1(45272d49cf6927eb33af275c003ce6eb1766f045) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ecofghtru.key", 0x00, 0x14, CRC(611aa137) SHA1(933e651779ea0b26b657de5dc62703e37821b07f) )
ROM_END

ROM_START( uecology )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49j.p1", 0x000000, 0x80000, CRC(94c40a4c) SHA1(6446b22a30a9a3c87b7a9fc2f15fbceccfbfb942) )
	ROM_LOAD16_WORD_SWAP( "c49j.p2", 0x080000, 0x80000, CRC(8d6e3a09) SHA1(80167275f288a4c4b2bb61bdde956015f4206b78) )
	ROM_LOAD16_WORD_SWAP( "c49j.p3", 0x100000, 0x80000, CRC(8604ecd7) SHA1(e1690565b40db84f4ce30e6eb2d7940b82989678) )
	ROM_LOAD16_WORD_SWAP( "c49j.p4", 0x180000, 0x80000, CRC(b7e1d31f) SHA1(6567f14af9fd567dea963fda5cd37c55cab30704) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "uecology.key", 0x00, 0x14, CRC(0bab792d) SHA1(e71ca1abf4101a8d580f9a81ac71b5fd33c0fbfb) )
ROM_END

ROM_START( ecofghtra )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49a.p1", 0x000000, 0x80000, CRC(bd4589b1) SHA1(8ec03a750de155c6ce0a2c3a6b57e6a6dcaf9ebc) )
	ROM_LOAD16_WORD_SWAP( "c49a.p2", 0x080000, 0x80000, CRC(1d134b7d) SHA1(c9dd725ff45f29a3fa68bfe6d5aea2e8c3c64bd8) )
	ROM_LOAD16_WORD_SWAP( "c49a.p3", 0x100000, 0x80000, CRC(9c581fc7) SHA1(300983148da59da7d2fcbc5bc45b068fdfbcb512) )
	ROM_LOAD16_WORD_SWAP( "c49a.p4", 0x180000, 0x80000, CRC(c92a7c50) SHA1(820dfa8fff32404caee65a7a5bcf7cafa9939f74) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ecofghtra.key", 0x00, 0x14, CRC(4f99a9f5) SHA1(6af89f5c8cec038b9066eb4ea0a5d719cfa58ed0) )
ROM_END

ROM_START( ecofghtrh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49h.p1", 0x000000, 0x80000, CRC(14c9365e) SHA1(caba5e617a2c9516c6f24a327451edcff39b1bfd) )
	ROM_LOAD16_WORD_SWAP( "c49h.p2", 0x080000, 0x80000, CRC(579495dc) SHA1(4b0f59da30c86d6c433429c46b4f24c7c55d6731) )
	ROM_LOAD16_WORD_SWAP( "c49h.p3", 0x100000, 0x80000, CRC(96807a8e) SHA1(6f12d5fbff17797338f26e2b32e9f27d8288262e) )
	ROM_LOAD16_WORD_SWAP( "c49h.p4", 0x180000, 0x80000, CRC(682b9dbc) SHA1(80d86283ce850f1e9ac868a7e77819d2876df982) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ecofghtrh.key", 0x00, 0x14, CRC(9a9027c8) SHA1(d337dc0508eb896fcf6494dfc00e2eafebcfedfe) )
ROM_END

ROM_START( gigawing )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79u.p1", 0x000000, 0x80000, CRC(ac725eb2) SHA1(a4be9fe537cdb47b37478c8397f6effe8a536233) )
	ROM_LOAD16_WORD_SWAP( "c79u.p2", 0x080000, 0x80000, CRC(392f4118) SHA1(3bb0bd9503ef60892d5abd8640af524cf71da848) )
	ROM_LOAD16_WORD_SWAP( "c79.p3",  0x100000, 0x80000, CRC(3239d642) SHA1(2fe3984c46a72aedb30a28e3db5af2612bdf0045) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gigawing.key", 0x00, 0x14, CRC(5076c26b) SHA1(81a0aa6768a1e94aed25503ad8b3814bb23c41b5) )
ROM_END

ROM_START( gigawingj )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79j.p1", 0x000000, 0x80000, CRC(fdd23b91) SHA1(c805473d3dc6bdb7ce232a9d7181d213544b2e7b) )
	ROM_LOAD16_WORD_SWAP( "c79j.p2", 0x080000, 0x80000, CRC(8c6e093c) SHA1(a4864b3b54cf648af81f74e2936d2bb8b99d68a9) )
	ROM_LOAD16_WORD_SWAP( "c79j.p3", 0x100000, 0x80000, CRC(43811454) SHA1(2a9563c840bd934c7e94f434a01686b7ff92e6d2) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gigawingj.key", 0x00, 0x14, CRC(8121a25e) SHA1(6e21e1f639394e0e442f6250a60f9d78e160ec09) )
ROM_END

ROM_START( gigawinga )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79a.p1", 0x000000, 0x80000, CRC(116f8837) SHA1(9ade6c2fae319e0111e7afd3af10096d8d88f0c5) )
	ROM_LOAD16_WORD_SWAP( "c79a.p2", 0x080000, 0x80000, CRC(e6e3f0c4) SHA1(3c28cc050f36fb070a8abf057f0972dc16bc5629) )
	ROM_LOAD16_WORD_SWAP( "c79a.p3", 0x100000, 0x80000, CRC(465e8ac9) SHA1(da94fb64c7ea64ac9e4e847b69e2b870f716dd34) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gigawinga.key", 0x00, 0x14, CRC(7401627e) SHA1(efc33bf70953eeabf314ba0bf3d65d65113b4432) )
ROM_END

ROM_START( gigawingh )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79h.p1", 0x000000, 0x80000, CRC(b9ee36eb) SHA1(7b977dcd27c1ed8f9d2ebaccf2497abeafcc6081) )
	ROM_LOAD16_WORD_SWAP( "c79h.p2", 0x080000, 0x80000, CRC(72e548fe) SHA1(67d4870d6f7df4b947f1ac6f6605a2344bfc04c1) )
	ROM_LOAD16_WORD_SWAP( "c79.p3",   0x100000, 0x80000, CRC(3239d642) SHA1(2fe3984c46a72aedb30a28e3db5af2612bdf0045) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gigawingh.key", 0x00, 0x14, CRC(43198223) SHA1(3285db4c4faffa57dcd506e08fe36739a5fa545d) )
ROM_END

ROM_START( gigawingb )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79b.p1", 0x000000, 0x80000, CRC(a1f8a448) SHA1(a88ed9fea9ec08fda59a1ca9622dfb9f8afdda78) )
	ROM_LOAD16_WORD_SWAP( "c79b.p2", 0x080000, 0x80000, CRC(6a423e76) SHA1(f6ed20f09e852b4fad78ee32617122d5deb98789) )
	ROM_LOAD16_WORD_SWAP( "c79.p3",  0x100000, 0x80000, CRC(3239d642) SHA1(2fe3984c46a72aedb30a28e3db5af2612bdf0045) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "gigawingb.key", 0x00, 0x14, CRC(5e7805fa) SHA1(034ba943a9c190a99641301cc24422d4d299869f) )
ROM_END

ROM_START( hsf2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c88u.p1",  0x000000, 0x80000, CRC(b308151e) SHA1(afdfd3b049c6435e2291bc35d8c26ff5bff223d8) )
	ROM_LOAD16_WORD_SWAP( "c88u.p2",  0x080000, 0x80000, CRC(327aa49c) SHA1(6719cd6ecc2a4487fdbf5cbcd47e35fc43000607) )
	ROM_LOAD16_WORD_SWAP( "c88.p3",   0x100000, 0x80000, CRC(dde34a35) SHA1(f5be2d2916db6e86e0886d61d55bddf138273ebc) )
	ROM_LOAD16_WORD_SWAP( "c88.p4",   0x180000, 0x80000, CRC(f4e56dda) SHA1(c6490707c2a416ab88612c2d73abbe5853d8cb92) )
	ROM_LOAD16_WORD_SWAP( "c88.p5",   0x200000, 0x80000, CRC(ee4420fc) SHA1(06cf76660b0c794d2460c52d9fe8334fff51e9de) )
	ROM_LOAD16_WORD_SWAP( "c88.p6",   0x280000, 0x80000, CRC(c9441533) SHA1(bf178fac1f060fcce3ff9118333c8517dadc9429) )
	ROM_LOAD16_WORD_SWAP( "c88.p7",   0x300000, 0x80000, CRC(3fc638a8) SHA1(2a42877b26c8abc437da46225701f0bba6e40058) )
	ROM_LOAD16_WORD_SWAP( "c88.p8",   0x380000, 0x80000, CRC(20d0f9e4) SHA1(80a5eeef9472e327b0d4ee26434bad109a9434ea) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c88.c1",   0x0000000, 0x800000, CRC(a6ecab17) SHA1(6749a4c8dc81f4b10f910c31c82cf6674e2a44eb) )
	ROM_LOAD64_WORD( "c88.c2",   0x0000002, 0x800000, CRC(10a0ae4d) SHA1(701b4900fbc8bef20efa1a706891c8df4bf14641) )
	ROM_LOAD64_WORD( "c88.c3",   0x0000004, 0x800000, CRC(adfa7726) SHA1(8d36ec125a8c91abfe5213893d794f8bc11c8acd) )
	ROM_LOAD64_WORD( "c88.c4",   0x0000006, 0x800000, CRC(bb3ae322) SHA1(ecd289d7a0fe365fdd7c5527cb17796002beb553) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c88.m1",   0x00000, 0x08000, CRC(c1a13786) SHA1(c7392c7efb15ea4042e75bd9007e974293d8935d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c88.m2",   0x28000, 0x20000, CRC(2d8794aa) SHA1(c634affdc2568020cce6af97b4fa79925d9943f3) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c88.q1",   0x000000, 0x800000, CRC(0e15c359) SHA1(176108b0d76d821a849324680aba0cd04b5016c1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "hsf2.key",     0x00, 0x14, CRC(fc9b18c9) SHA1(457b6ba05458f1be9ebb8aa9cbab0a8808cb6857) )
ROM_END

ROM_START( hsf2a )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c88a.p1",  0x000000, 0x80000, CRC(d50a17e0) SHA1(5d8d6d309260cc2d862aa080d44a72886ee08c77) )
	ROM_LOAD16_WORD_SWAP( "c88a.p2",  0x080000, 0x80000, CRC(a27f42de) SHA1(7a355831b57a35e327b2618fd5dca11afed2a233) )
	ROM_LOAD16_WORD_SWAP( "c88.p3",   0x100000, 0x80000, CRC(dde34a35) SHA1(f5be2d2916db6e86e0886d61d55bddf138273ebc) )
	ROM_LOAD16_WORD_SWAP( "c88.p4",   0x180000, 0x80000, CRC(f4e56dda) SHA1(c6490707c2a416ab88612c2d73abbe5853d8cb92) )
	ROM_LOAD16_WORD_SWAP( "c88.p5",   0x200000, 0x80000, CRC(ee4420fc) SHA1(06cf76660b0c794d2460c52d9fe8334fff51e9de) )
	ROM_LOAD16_WORD_SWAP( "c88.p6",   0x280000, 0x80000, CRC(c9441533) SHA1(bf178fac1f060fcce3ff9118333c8517dadc9429) )
	ROM_LOAD16_WORD_SWAP( "c88.p7",   0x300000, 0x80000, CRC(3fc638a8) SHA1(2a42877b26c8abc437da46225701f0bba6e40058) )
	ROM_LOAD16_WORD_SWAP( "c88.p8",   0x380000, 0x80000, CRC(20d0f9e4) SHA1(80a5eeef9472e327b0d4ee26434bad109a9434ea) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c88.c1",   0x0000000, 0x800000, CRC(a6ecab17) SHA1(6749a4c8dc81f4b10f910c31c82cf6674e2a44eb) )
	ROM_LOAD64_WORD( "c88.c2",   0x0000002, 0x800000, CRC(10a0ae4d) SHA1(701b4900fbc8bef20efa1a706891c8df4bf14641) )
	ROM_LOAD64_WORD( "c88.c3",   0x0000004, 0x800000, CRC(adfa7726) SHA1(8d36ec125a8c91abfe5213893d794f8bc11c8acd) )
	ROM_LOAD64_WORD( "c88.c4",   0x0000006, 0x800000, CRC(bb3ae322) SHA1(ecd289d7a0fe365fdd7c5527cb17796002beb553) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c88.m1",   0x00000, 0x08000, CRC(c1a13786) SHA1(c7392c7efb15ea4042e75bd9007e974293d8935d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c88.m2",   0x28000, 0x20000, CRC(2d8794aa) SHA1(c634affdc2568020cce6af97b4fa79925d9943f3) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c88.q1",   0x000000, 0x800000, CRC(0e15c359) SHA1(176108b0d76d821a849324680aba0cd04b5016c1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "hsf2a.key",    0x00, 0x14, CRC(2cd9eb99) SHA1(65f999569d8479bb8d27ba8c3e3c92f880b69195) )
ROM_END

ROM_START( hsf2j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c88j.p1", 0x000000, 0x80000, CRC(6efe661f) SHA1(5e8df6183cc04ae0f3002f85ffedaa09b4397cd9) )
	ROM_LOAD16_WORD_SWAP( "c88j.p2", 0x080000, 0x80000, CRC(93f2500a) SHA1(d340e8afd0a05066e19d795fdea31b4d1e00ad97) )
	ROM_LOAD16_WORD_SWAP( "c88.p3",  0x100000, 0x80000, CRC(dde34a35) SHA1(f5be2d2916db6e86e0886d61d55bddf138273ebc) )
	ROM_LOAD16_WORD_SWAP( "c88.p4",  0x180000, 0x80000, CRC(f4e56dda) SHA1(c6490707c2a416ab88612c2d73abbe5853d8cb92) )
	ROM_LOAD16_WORD_SWAP( "c88.p5",  0x200000, 0x80000, CRC(ee4420fc) SHA1(06cf76660b0c794d2460c52d9fe8334fff51e9de) )
	ROM_LOAD16_WORD_SWAP( "c88.p6",  0x280000, 0x80000, CRC(c9441533) SHA1(bf178fac1f060fcce3ff9118333c8517dadc9429) )
	ROM_LOAD16_WORD_SWAP( "c88.p7",  0x300000, 0x80000, CRC(3fc638a8) SHA1(2a42877b26c8abc437da46225701f0bba6e40058) )
	ROM_LOAD16_WORD_SWAP( "c88.p8",  0x380000, 0x80000, CRC(20d0f9e4) SHA1(80a5eeef9472e327b0d4ee26434bad109a9434ea) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c88.c1",   0x0000000, 0x800000, CRC(a6ecab17) SHA1(6749a4c8dc81f4b10f910c31c82cf6674e2a44eb) )
	ROM_LOAD64_WORD( "c88.c2",   0x0000002, 0x800000, CRC(10a0ae4d) SHA1(701b4900fbc8bef20efa1a706891c8df4bf14641) )
	ROM_LOAD64_WORD( "c88.c3",   0x0000004, 0x800000, CRC(adfa7726) SHA1(8d36ec125a8c91abfe5213893d794f8bc11c8acd) )
	ROM_LOAD64_WORD( "c88.c4",   0x0000006, 0x800000, CRC(bb3ae322) SHA1(ecd289d7a0fe365fdd7c5527cb17796002beb553) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c88.m1",   0x00000, 0x08000, CRC(c1a13786) SHA1(c7392c7efb15ea4042e75bd9007e974293d8935d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c88.m2",   0x28000, 0x20000, CRC(2d8794aa) SHA1(c634affdc2568020cce6af97b4fa79925d9943f3) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c88.q1",   0x000000, 0x800000, CRC(0e15c359) SHA1(176108b0d76d821a849324680aba0cd04b5016c1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "hsf2j.key",    0x00, 0x14, CRC(19455a93) SHA1(925cee29b37cb7a4048383ac715ad96f83280d37) )
ROM_END

ROM_START( hsf2j1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c88j1.p1",  0x000000, 0x80000, CRC(00738f73) SHA1(99947a621f21a88dc4c425d9bfbc30b3c5f76ecd) )
	ROM_LOAD16_WORD_SWAP( "c88j1.p2",  0x080000, 0x80000, CRC(40072c4a) SHA1(85b95bd3c907b4276a31777e092b8c40d3763257) )
	ROM_LOAD16_WORD_SWAP( "c88.p3",   0x100000, 0x80000, CRC(dde34a35) SHA1(f5be2d2916db6e86e0886d61d55bddf138273ebc) )
	ROM_LOAD16_WORD_SWAP( "c88.p4",   0x180000, 0x80000, CRC(f4e56dda) SHA1(c6490707c2a416ab88612c2d73abbe5853d8cb92) )
	ROM_LOAD16_WORD_SWAP( "c88.p5",   0x200000, 0x80000, CRC(ee4420fc) SHA1(06cf76660b0c794d2460c52d9fe8334fff51e9de) )
	ROM_LOAD16_WORD_SWAP( "c88.p6",   0x280000, 0x80000, CRC(c9441533) SHA1(bf178fac1f060fcce3ff9118333c8517dadc9429) )
	ROM_LOAD16_WORD_SWAP( "c88.p7",   0x300000, 0x80000, CRC(3fc638a8) SHA1(2a42877b26c8abc437da46225701f0bba6e40058) )
	ROM_LOAD16_WORD_SWAP( "c88.p8",   0x380000, 0x80000, CRC(20d0f9e4) SHA1(80a5eeef9472e327b0d4ee26434bad109a9434ea) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c88.c1",   0x0000000, 0x800000, CRC(a6ecab17) SHA1(6749a4c8dc81f4b10f910c31c82cf6674e2a44eb) )
	ROM_LOAD64_WORD( "c88.c2",   0x0000002, 0x800000, CRC(10a0ae4d) SHA1(701b4900fbc8bef20efa1a706891c8df4bf14641) )
	ROM_LOAD64_WORD( "c88.c3",   0x0000004, 0x800000, CRC(adfa7726) SHA1(8d36ec125a8c91abfe5213893d794f8bc11c8acd) )
	ROM_LOAD64_WORD( "c88.c4",   0x0000006, 0x800000, CRC(bb3ae322) SHA1(ecd289d7a0fe365fdd7c5527cb17796002beb553) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c88.m1",   0x00000, 0x08000, CRC(c1a13786) SHA1(c7392c7efb15ea4042e75bd9007e974293d8935d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c88.m2",   0x28000, 0x20000, CRC(2d8794aa) SHA1(c634affdc2568020cce6af97b4fa79925d9943f3) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c88.q1",   0x000000, 0x800000, CRC(0e15c359) SHA1(176108b0d76d821a849324680aba0cd04b5016c1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "hsf2j.key",    0x00, 0x14, CRC(19455a93) SHA1(925cee29b37cb7a4048383ac715ad96f83280d37) )
ROM_END

ROM_START( jyangoku )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c80.p1", 0x000000, 0x80000, CRC(4614a3b2) SHA1(f7226006feafaf561046ae7fce18bf62289d41df) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c80.c1",   0x0000000, 0x200000, CRC(ba0fe27b) SHA1(60a4fdee8da663777af1e126a1aa6308c9d9a5a9) )
	ROM_LOAD64_BYTE( "c80.c2",   0x0000001, 0x200000, CRC(2cd141bf) SHA1(57ec73ea24d594fc1e4d2d194a3c548a7043666e) )
	ROM_LOAD64_BYTE( "c80.c3",   0x0000002, 0x200000, CRC(e29e4c26) SHA1(51e99536f40481c4c208695354e90fb3fe9416d5) )
	ROM_LOAD64_BYTE( "c80.c4",   0x0000003, 0x200000, CRC(7f68b88a) SHA1(944bf34dc998dffe39b25c3e9fcec17ad421ce81) )
	ROM_LOAD64_BYTE( "c80.c5",   0x0000004, 0x200000, CRC(3aaeb90b) SHA1(d426d5c7ae5ca99321ec1280abdd1fdfe4882829) )
	ROM_LOAD64_BYTE( "c80.c6",   0x0000005, 0x200000, CRC(97894cea) SHA1(a501cd80e6da75409e3381d66bd0a13e021e89f3) )
	ROM_LOAD64_BYTE( "c80.c7",   0x0000006, 0x200000, CRC(ec737d9d) SHA1(cfff42cc24ac011fab2670dec42cab16f4e0d84d) )
	ROM_LOAD64_BYTE( "c80.c8",   0x0000007, 0x200000, CRC(c23b6f22) SHA1(fb3120ea28c67ecb7c4a2b61a64feb62c033ef68) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c80.m1",  0x00000, 0x08000, CRC(1fe8c213) SHA1(e0045566337851d8261ed65d5bea483f09ae96b4) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_BYTE( "c80.q1",   0x000000, 0x200000, CRC(5ad9ee53) SHA1(acfb7ec137209409c1a439ebc62d8fa3f87e7012) )
	ROM_LOAD16_BYTE( "c80.q2",   0x000001, 0x200000, CRC(efb3dbfb) SHA1(3b32b9890f79805b6a2e1ec63f2cadaca14cf11a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "jyangoku.key", 0x00, 0x14, CRC(95b0a560) SHA1(6c0684ab321bd555ec9f8f16e63f6c358318ad7e) )
ROM_END

ROM_START( megaman2 )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.p1", 0x000000, 0x80000, CRC(8ffc2cd1) SHA1(919ef08311008288b31ed42fb13172580d50433a) )
	ROM_LOAD16_WORD_SWAP( "c66.p2", 0x080000, 0x80000, CRC(bb30083a) SHA1(466b818a01ad367a8df6df8661f616f5a0236714) )
	ROM_LOAD16_WORD_SWAP( "c66.p3",  0x100000, 0x80000, CRC(02ee9efc) SHA1(1b80c40389b51a03b930051f232630616c12e6c5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c66.c1",  0x800000, 0x200000, CRC(9b1f00b4) SHA1(c1c5c2d9d00121425ae6598444d704f420ef4eef) )
	ROM_LOAD64_WORD( "c66.c2",  0x800002, 0x200000, CRC(c2bb0c24) SHA1(38724c49d9db49765a4ed9bc2dc8f57cec45ec7c) )
	ROM_LOAD64_WORD( "c66.c3",  0x800004, 0x200000, CRC(12257251) SHA1(20cb58afda0e6200991277817485340a6a41ae2b) )
	ROM_LOAD64_WORD( "c66.c4",  0x800006, 0x200000, CRC(f9b6e786) SHA1(aeb4acff7208e66a35198143fd2478039fdaa3a6) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c66.m1",  0x00000, 0x08000, CRC(d18e7859) SHA1(0939fac70042d0b4db5c2fdcef1f79b95febd45e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c66.m2",   0x28000, 0x20000, CRC(c463ece0) SHA1(5c3e41eb61610b3f8c431206f6672907e3a0bdb0) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.q1",   0x000000, 0x200000, CRC(2106174d) SHA1(0a35d9ca8ebcad74904b20648d5320f839d6377e) )
	ROM_LOAD16_WORD_SWAP( "c66.q2",   0x200000, 0x200000, CRC(546c1636) SHA1(f96b172ab899f2c6ee17a5dd1fb61af9432e3cd2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "megaman2.key", 0x00, 0x14, CRC(6828ed6d) SHA1(1039ed396ee580dc46d104fa4cd97b6e67930ede) )
ROM_END

ROM_START( megaman2a )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c66a.p1", 0x000000, 0x80000, CRC(2b330ca7) SHA1(afa86ef73f5660600d18ff221ed135c026042e05) )
	ROM_LOAD16_WORD_SWAP( "c66a.p2", 0x080000, 0x80000, CRC(8b47942b) SHA1(160574a38e89d31b975c56264f3f5a7a68ce760c) )
	ROM_LOAD16_WORD_SWAP( "c66.p3",  0x100000, 0x80000, CRC(02ee9efc) SHA1(1b80c40389b51a03b930051f232630616c12e6c5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c66.c1",  0x800000, 0x200000, CRC(9b1f00b4) SHA1(c1c5c2d9d00121425ae6598444d704f420ef4eef) )
	ROM_LOAD64_WORD( "c66.c2",  0x800002, 0x200000, CRC(c2bb0c24) SHA1(38724c49d9db49765a4ed9bc2dc8f57cec45ec7c) )
	ROM_LOAD64_WORD( "c66.c3",  0x800004, 0x200000, CRC(12257251) SHA1(20cb58afda0e6200991277817485340a6a41ae2b) )
	ROM_LOAD64_WORD( "c66.c4",  0x800006, 0x200000, CRC(f9b6e786) SHA1(aeb4acff7208e66a35198143fd2478039fdaa3a6) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c66.m1",  0x00000, 0x08000, CRC(d18e7859) SHA1(0939fac70042d0b4db5c2fdcef1f79b95febd45e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c66.m2",   0x28000, 0x20000, CRC(c463ece0) SHA1(5c3e41eb61610b3f8c431206f6672907e3a0bdb0) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.q1",   0x000000, 0x200000, CRC(2106174d) SHA1(0a35d9ca8ebcad74904b20648d5320f839d6377e) )
	ROM_LOAD16_WORD_SWAP( "c66.q2",   0x200000, 0x200000, CRC(546c1636) SHA1(f96b172ab899f2c6ee17a5dd1fb61af9432e3cd2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "megaman2a.key", 0x00, 0x14, CRC(d6e8dcd7) SHA1(8aef786fc619ecc7572f4dc166bae789bf436c6b) )
ROM_END

ROM_START( rockman2j )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c66j.p1", 0x000000, 0x80000, CRC(dbaa1437) SHA1(849572090bdbde7d9f191959f4b6ad26f46811f4) )
	ROM_LOAD16_WORD_SWAP( "c66j.p2", 0x080000, 0x80000, CRC(cf5ba612) SHA1(f0b56db8df7ad676e00325c97cf16791f409e35a) )
	ROM_LOAD16_WORD_SWAP( "c66.p3",  0x100000, 0x80000, CRC(02ee9efc) SHA1(1b80c40389b51a03b930051f232630616c12e6c5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c66.c1",  0x800000, 0x200000, CRC(9b1f00b4) SHA1(c1c5c2d9d00121425ae6598444d704f420ef4eef) )
	ROM_LOAD64_WORD( "c66.c2",  0x800002, 0x200000, CRC(c2bb0c24) SHA1(38724c49d9db49765a4ed9bc2dc8f57cec45ec7c) )
	ROM_LOAD64_WORD( "c66.c3",  0x800004, 0x200000, CRC(12257251) SHA1(20cb58afda0e6200991277817485340a6a41ae2b) )
	ROM_LOAD64_WORD( "c66.c4",  0x800006, 0x200000, CRC(f9b6e786) SHA1(aeb4acff7208e66a35198143fd2478039fdaa3a6) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c66.m1",  0x00000, 0x08000, CRC(d18e7859) SHA1(0939fac70042d0b4db5c2fdcef1f79b95febd45e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c66.m2",   0x28000, 0x20000, CRC(c463ece0) SHA1(5c3e41eb61610b3f8c431206f6672907e3a0bdb0) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.q1",   0x000000, 0x200000, CRC(2106174d) SHA1(0a35d9ca8ebcad74904b20648d5320f839d6377e) )
	ROM_LOAD16_WORD_SWAP( "c66.q2",   0x200000, 0x200000, CRC(546c1636) SHA1(f96b172ab899f2c6ee17a5dd1fb61af9432e3cd2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "rockman2j.key", 0x00, 0x14, CRC(c590187a) SHA1(f9140da0849711938c4d8a98fb38bc1504582a43) )
ROM_END

ROM_START( megaman2h )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c66h.p1", 0x000000, 0x80000, CRC(bb180378) SHA1(ecf5e9907500139073e3b1b4e384039957dca354) )
	ROM_LOAD16_WORD_SWAP( "c66h.p2", 0x080000, 0x80000, CRC(205ffcd6) SHA1(d1e3a164f4fd4f372ad2f3b4e2026d6487395503) )
	ROM_LOAD16_WORD_SWAP( "c66.p3",  0x100000, 0x80000, CRC(02ee9efc) SHA1(1b80c40389b51a03b930051f232630616c12e6c5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c66.c1",  0x800000, 0x200000, CRC(9b1f00b4) SHA1(c1c5c2d9d00121425ae6598444d704f420ef4eef) )
	ROM_LOAD64_WORD( "c66.c2",  0x800002, 0x200000, CRC(c2bb0c24) SHA1(38724c49d9db49765a4ed9bc2dc8f57cec45ec7c) )
	ROM_LOAD64_WORD( "c66.c3",  0x800004, 0x200000, CRC(12257251) SHA1(20cb58afda0e6200991277817485340a6a41ae2b) )
	ROM_LOAD64_WORD( "c66.c4",  0x800006, 0x200000, CRC(f9b6e786) SHA1(aeb4acff7208e66a35198143fd2478039fdaa3a6) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c66.m1",  0x00000, 0x08000, CRC(d18e7859) SHA1(0939fac70042d0b4db5c2fdcef1f79b95febd45e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c66.m2",   0x28000, 0x20000, CRC(c463ece0) SHA1(5c3e41eb61610b3f8c431206f6672907e3a0bdb0) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.q1",   0x000000, 0x200000, CRC(2106174d) SHA1(0a35d9ca8ebcad74904b20648d5320f839d6377e) )
	ROM_LOAD16_WORD_SWAP( "c66.q2",   0x200000, 0x200000, CRC(546c1636) SHA1(f96b172ab899f2c6ee17a5dd1fb61af9432e3cd2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "megaman2h.key", 0x00, 0x14, CRC(99cb8d19) SHA1(24b059ec7ef2188cce0636eacfbc9e676e8d558b) )
ROM_END

/*

Gigaman 2 - 2004 Chinese rebuild Bootleg

Just dumped the program roms. Other 3 are soldered and are MX26L6420MC-90
Probably a rebuild for chinese market
Copyrighted J-TECH 2004 on game :)


CPU : Motorola 68000 16 mhz
video : Actel A54SX16A-F
Sound : Atmel AT89C4051-24PI + M6295 (noted AD-65)

*/

ROM_START( gigaman2 )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "sys_rom1.bin", 0x000000, 0x400000, CRC(2eaa5e10) SHA1(79f9a137bf5b3317579c548f346c1dc1cccdb771) )

	ROM_REGION(0x10000, "mcu", 0 )      /* sound MCU code */
	ROM_LOAD( "89c4051.bin", 0x000000, 0x10000, NO_DUMP )

	ROM_REGION( 0x800000, "gfx", 0 )
	ROM_LOAD( "cg_rom1.bin",  0x0000000, 0x400000, CRC(ed55a641) SHA1(fa798779a3787317937a646047620b1c0dbe102a) )
	ROM_IGNORE( 0x400000 )
	ROM_LOAD( "cg_rom2.bin",  0x0400000, 0x400000, CRC(63918c05) SHA1(3a7fdf88e87bdbc622504276287740c08df38f6f) )
	ROM_IGNORE( 0x400000 )

	ROM_REGION( 0x800000, "oki", 0 )
	/* No Qsound, OKI instead.. */
	ROM_LOAD( "pcm_rom1.bin", 0x000000, 0x800000, CRC(41a854ab) SHA1(206448c3e27d260d0650c28a9cd524854ce0b64d) )
ROM_END

ROM_START( mmatrix )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c82u.p1", 0x000000, 0x80000, CRC(ab65b599) SHA1(d4c35f5a6cf0b37a35c466f0e347a660b2e0b21b) )
	ROM_LOAD16_WORD_SWAP( "c82u.p2", 0x080000, 0x80000, CRC(0135fc6c) SHA1(e40c8fa51dcb300b3ee72dc7de137e0b39dea490) )
	ROM_LOAD16_WORD_SWAP( "c82u.p3", 0x100000, 0x80000, CRC(f1fd2b84) SHA1(d34816eff4af98009f94f5dd14097b39426e0468) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c82.c1",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "c82.c2",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "c82.c3",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "c82.c4",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "c82.c5",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "c82.c6",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "c82.c7",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "c82.c8",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c82.m1",   0x00000, 0x08000, CRC(c57e8171) SHA1(dedb92af1910d38727f816e6f507d25148f31b74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c82.q1",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "c82.q2",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmatrix.key",  0x00, 0x14, CRC(8ed66bc4) SHA1(324d7bf9d95220ecab140b1385eba7087f1a9669) )
ROM_END

ROM_START( mmatrixa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c82a.p1", 0x000000, 0x80000, CRC(1d670ed7) SHA1(10d1ae8f9586c658f73b2b27344c3c868fa2cb89) )
	ROM_LOAD16_WORD_SWAP( "c82a.p2", 0x080000, 0x80000, CRC(43be66d2) SHA1(e5b772c15320c7f8804d6fc6f8f2edc53012fce2) )
	ROM_LOAD16_WORD_SWAP( "c82a.p3", 0x100000, 0x80000, CRC(9bfe3082) SHA1(6119dee8e84e80243659ce43e90675a7a4409615) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c82.c1",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "c82.c2",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "c82.c3",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "c82.c4",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "c82.c5",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "c82.c6",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "c82.c7",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "c82.c8",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c82.m1",   0x00000, 0x08000, CRC(c57e8171) SHA1(dedb92af1910d38727f816e6f507d25148f31b74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c82.q1",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "c82.q2",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmatrixa.key",  0x00, 0x14, CRC(f6a944b4) SHA1(f76aa23c0c19a1a220be9cf3c55a8617b3c3dce5) )
ROM_END

ROM_START( mmatrixj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c82j.p1", 0x000000, 0x80000, CRC(1d5de213) SHA1(2d7ad9cb50540a14aa0ac564d4ab84a3779d595c) )
	ROM_LOAD16_WORD_SWAP( "c82j.p2", 0x080000, 0x80000, CRC(d943a339) SHA1(ae3d217b35f92fc727bda3b14f13f3658dab3dd8) )
	ROM_LOAD16_WORD_SWAP( "c82j.p3", 0x100000, 0x80000, CRC(0c8b4abb) SHA1(c136186b9f57d68c0b36f5a4273347f696a227c0) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c82.c1",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "c82.c2",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "c82.c3",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "c82.c4",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "c82.c5",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "c82.c6",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "c82.c7",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "c82.c8",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c82.m1",   0x00000, 0x08000, CRC(c57e8171) SHA1(dedb92af1910d38727f816e6f507d25148f31b74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c82.q1",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "c82.q2",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmatrixj.key", 0x00, 0x14, CRC(3b50d889) SHA1(fb749a5da0a799322ec131a15a017971e586c5fc) )
ROM_END

ROM_START( msh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.p1",  0x000000, 0x80000, CRC(bd951414) SHA1(5585bdd1484dc18c7630d689f60d91c068aafc97) )
	ROM_LOAD16_WORD_SWAP( "c61.p2",  0x080000, 0x80000, CRC(19dd42f2) SHA1(48bd3e4d2d7e9e07275bd9c00530719deb100090) )
	ROM_LOAD16_WORD_SWAP( "c61.p3",  0x100000, 0x80000, CRC(6a091b9e) SHA1(7fa54e69e1a1ca348cb08d892d55023e9a3ff4cb) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "msh.key",      0x00, 0x14, CRC(b494368e) SHA1(8efc561cec2bcc17902d27ed4e27b7d2b9eccdbc) )
ROM_END

ROM_START( mshu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61u.p1", 0x000000, 0x80000, CRC(d2805bdd) SHA1(a6f78c31a82168bb5f7d614dcebbeab8231e2d75) )
	ROM_LOAD16_WORD_SWAP( "c61u.p2", 0x080000, 0x80000, CRC(743f96ff) SHA1(abb82359bb68966028ea33e94996803599f3e273) )
	ROM_LOAD16_WORD_SWAP( "c61.p3",  0x100000, 0x80000, CRC(6a091b9e) SHA1(7fa54e69e1a1ca348cb08d892d55023e9a3ff4cb) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshu.key",     0x00, 0x14, CRC(745c1bee) SHA1(86d31f266f0fc20ca5f1607eebf4db688323147f) )
ROM_END

ROM_START( mshj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61j.p1", 0x000000, 0x80000, CRC(261f4091) SHA1(f4509780768e3601720d0d78c8a9824d410d59da) )
	ROM_LOAD16_WORD_SWAP( "c61j.p2", 0x080000, 0x80000, CRC(61d791c6) SHA1(9f883bcc48058a99c4ba653d0855c58c5d081243) )
	ROM_LOAD16_WORD_SWAP( "c61j.p3", 0x100000, 0x80000, CRC(f37539e6) SHA1(770febc25ca5615b6c2023727edab3c68b15b2c4) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshj.key",     0x00, 0x14, CRC(888761ac) SHA1(a1c72deedab2bafe5d594bba905a6274575b6e56) )
ROM_END

ROM_START( mshjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61jr1.p1", 0x000000, 0x80000, CRC(ff172fd2) SHA1(2dd507e3fcf1a30fde1e6ce63d4233a67e7bfc9e) )
	ROM_LOAD16_WORD_SWAP( "c61jr1.p2", 0x080000, 0x80000, CRC(ebbb205a) SHA1(0b110ea4c71bdab819b72e6f9736368575e4cccf) )
	ROM_LOAD16_WORD_SWAP( "c61.p3",  0x100000, 0x80000, CRC(6a091b9e) SHA1(7fa54e69e1a1ca348cb08d892d55023e9a3ff4cb) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshj.key",     0x00, 0x14, CRC(888761ac) SHA1(a1c72deedab2bafe5d594bba905a6274575b6e56) )
ROM_END

ROM_START( msha )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61a.p1", 0x000000, 0x80000, CRC(ec84ec44) SHA1(4d434df6cf5c961f0dbba352d1353db0a8f353dc) )
	ROM_LOAD16_WORD_SWAP( "c61a.p2", 0x080000, 0x80000, CRC(098b8503) SHA1(4cc74754796d5e41f13bf5cd4e8868b0d0c7852c) )
	ROM_LOAD16_WORD_SWAP( "c61.p3",  0x100000, 0x80000, CRC(6a091b9e) SHA1(7fa54e69e1a1ca348cb08d892d55023e9a3ff4cb) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "msha.key",     0x00, 0x14, CRC(00f3f2ca) SHA1(cf51a47b9a03c845fa815183dc43dcd6bdee369b) )
ROM_END

ROM_START( mshh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61h.p1", 0x000000, 0x80000, CRC(8d84b0fa) SHA1(e1fd2869abbe4f8736e496f194e23a1ab0526811) )
	ROM_LOAD16_WORD_SWAP( "c61h.p2", 0x080000, 0x80000, CRC(d638f601) SHA1(cbdd9776f71c6ef8d80be23a57cba3529d53a070) )
	ROM_LOAD16_WORD_SWAP( "c61j.p3", 0x100000, 0x80000, CRC(f37539e6) SHA1(770febc25ca5615b6c2023727edab3c68b15b2c4) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshh.key",     0x00, 0x14, CRC(5dddf5e7) SHA1(318b450aa9501f506511a57e8c991659453c9a31) )
ROM_END

ROM_START( mshb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61b.p1", 0x000000, 0x80000, CRC(19697f74) SHA1(c3809ecbdb242bdbb57f8d9b029264e9c0ed8a13) )
	ROM_LOAD16_WORD_SWAP( "c61b.p2", 0x080000, 0x80000, CRC(95317a6f) SHA1(143a26e349f21d3a720320bb7010a26f767e5e73) )
	ROM_LOAD16_WORD_SWAP( "c61j.p3", 0x100000, 0x80000, CRC(f37539e6) SHA1(770febc25ca5615b6c2023727edab3c68b15b2c4) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshb.key",     0x00, 0x14, CRC(92196837) SHA1(46042a8b846272102cc8f967a6b81e48013d19f4) )
ROM_END

ROM_START( mshbr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61br1.p1", 0x000000, 0x80000, CRC(eb16165b) SHA1(37ca9d7f2ee5320aff1d9de56f766e2dcad5935c) )
	ROM_LOAD16_WORD_SWAP( "c61br1.p2", 0x080000, 0x80000, CRC(94fb3c97) SHA1(fd5fe7ed39bb1bae436ccd510f88979a16c1e4ab) )
	ROM_LOAD16_WORD_SWAP( "c61.p3",  0x100000, 0x80000, CRC(6a091b9e) SHA1(7fa54e69e1a1ca348cb08d892d55023e9a3ff4cb) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshb.key",     0x00, 0x14, CRC(92196837) SHA1(46042a8b846272102cc8f967a6b81e48013d19f4) )
ROM_END

ROM_START( mshvsf )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.p1", 0x000000, 0x80000, CRC(b72dc199) SHA1(61bd581ea4b969298f8a39fe03023b5456cac750) )
	ROM_LOAD16_WORD_SWAP( "c72.p2", 0x080000, 0x80000, CRC(6ef799f9) SHA1(2d45dbf7bc277b84c6bcd9615ab3b80c42af7781) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsf.key",   0x00, 0x14, CRC(64660867) SHA1(448eb440494c971800370f103b4d1f21a7266c6c) )
ROM_END

ROM_START( mshvsfu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72u.p1", 0x000000, 0x80000, CRC(0664ab15) SHA1(939fb1e3c06c33fc212b26ecfceac3180e108e9d) )
	ROM_LOAD16_WORD_SWAP( "c72u.p2", 0x080000, 0x80000, CRC(97e060ee) SHA1(787924e04508c83ecd4c3a872882d2be9e57eb50) )
	ROM_LOAD16_WORD_SWAP( "c72u.p3",  0x100000, 0x80000, CRC(921fc542) SHA1(b813082a480d42d663c713062892245faabe9101) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfu.key",  0x00, 0x14, CRC(4c04797b) SHA1(dc1d59862d07112b75348ae5ad925d3716099e82) )
ROM_END

ROM_START( mshvsfu1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72u1.p1", 0x000000, 0x80000, CRC(ae60a66a) SHA1(1fa7e6534d02ec8059153705b1161a55b9cfe803) )
	ROM_LOAD16_WORD_SWAP( "c72u1.p2", 0x080000, 0x80000, CRC(91f67d8a) SHA1(e95f7a3fb281e1bafdbe7a1b22532c4fab5ec89d) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfu.key",  0x00, 0x14, CRC(4c04797b) SHA1(dc1d59862d07112b75348ae5ad925d3716099e82) )
ROM_END

ROM_START( mshvsfj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72j.p1", 0x000000, 0x80000, CRC(d8cbb691) SHA1(16820cf3bc7285477e61bd598a3ed4ea5e0e770d) )
	ROM_LOAD16_WORD_SWAP( "c72j.p2", 0x080000, 0x80000, CRC(32741ace) SHA1(36db3a3aeaf29369977593c051bf5665cffefb2d) )
	ROM_LOAD16_WORD_SWAP( "c72j.p3",  0x100000, 0x80000, CRC(77870dc3) SHA1(924a7c82456bb44d7b0be65af11dbe1a2420a3f0) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfj.key",  0x00, 0x14, CRC(565eeebb) SHA1(762844b59b2fcf529a26ad3dde8282415db926b3) )
ROM_END

ROM_START( mshvsfj1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72j1.p1", 0x000000, 0x80000, CRC(fbe2115f) SHA1(b2d8a62e394c2eb4070cac742b0f403252e46a25) )
	ROM_LOAD16_WORD_SWAP( "c72j1.p2", 0x080000, 0x80000, CRC(b528a367) SHA1(ecac71b032b431c63a4cf73a1d1d1be1faebc12b) )
	ROM_LOAD16_WORD_SWAP( "c72j1.p3",  0x100000, 0x80000, CRC(9515a245) SHA1(eafa877fd4a4e58e7c98336658e986a4a27d6b91) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfj.key",  0x00, 0x14, CRC(565eeebb) SHA1(762844b59b2fcf529a26ad3dde8282415db926b3) )
ROM_END

ROM_START( mshvsfj2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72j2.p1", 0x000000, 0x80000, CRC(fdfa7e26) SHA1(e9fb93249e48e1bb7c769c3ce674dd4be404574f) )
	ROM_LOAD16_WORD_SWAP( "c72j2.p2", 0x080000, 0x80000, CRC(c921825f) SHA1(471e44268cebba631b81f131bf31e27b8a28c548) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfj.key",  0x00, 0x14, CRC(565eeebb) SHA1(762844b59b2fcf529a26ad3dde8282415db926b3) )
ROM_END

ROM_START( mshvsfh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72h.p1", 0x000000, 0x80000, CRC(4f60f41e) SHA1(dd926a9cac4bff05845615d0b61948e2dc4b1ed8) )
	ROM_LOAD16_WORD_SWAP( "c72h.p2", 0x080000, 0x80000, CRC(dc08ec12) SHA1(594e4383eb776c09075577cd1f4e42ef11748f0f) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfh.key",  0x00, 0x14, CRC(b93d576f) SHA1(d7b83c72fe67394a8ee46fbb3168c0e5f4cab1b3) )
ROM_END

ROM_START( mshvsfa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72a.p1", 0x000000, 0x80000, CRC(5b863716) SHA1(6a129274711765bbf5acbb225e3fce6d93d7f421) )
	ROM_LOAD16_WORD_SWAP( "c72a.p2", 0x080000, 0x80000, CRC(4886e65f) SHA1(758fc9c453a864e32588c7fb33166c93e798a39c) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfa.key",  0x00, 0x14, CRC(6810a3af) SHA1(6d5fff45e7cc4050b528debaec68a8fa60d40f34) )
ROM_END

ROM_START( mshvsfa1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72a1.p1", 0x000000, 0x80000, CRC(92ef1933) SHA1(34e6a074734032af74afa52bfebbc213a9c886d7) )
	ROM_LOAD16_WORD_SWAP( "c72a1.p2", 0x080000, 0x80000, CRC(4b24373c) SHA1(f340dda7d5339645fd1ea523e72783fb7bb7aba1) )
	ROM_LOAD16_WORD_SWAP( "c72a1.p3",  0x100000, 0x80000, CRC(ac180c1c) SHA1(1b368ebe7680796dc068b511b72359eec546cd9f) )
	ROM_LOAD16_WORD_SWAP( "c72.p4", 0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5", 0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6", 0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7", 0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8", 0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfa.key",  0x00, 0x14, CRC(6810a3af) SHA1(6d5fff45e7cc4050b528debaec68a8fa60d40f34) )
ROM_END

ROM_START( mshvsfb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72b.p1", 0x000000, 0x80000, CRC(143895ef) SHA1(0664fad64996118df86e9887bd6e301d04d84978) )
	ROM_LOAD16_WORD_SWAP( "c72b.p2", 0x080000, 0x80000, CRC(dd8a886c) SHA1(a16f262fd14e726c7837980d0556a9c3bdc7fb11) )
	ROM_LOAD16_WORD_SWAP( "c72u.p3",  0x100000, 0x80000, CRC(921fc542) SHA1(b813082a480d42d663c713062892245faabe9101) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfb.key",  0x00, 0x14, CRC(3f5bb6e4) SHA1(dbebcebf24997de48e5174d82af5cd53dbe0dada) )
ROM_END

ROM_START( mshvsfb1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72b1.p1", 0x000000, 0x80000, CRC(9c4bb950) SHA1(85d0026a691682c195d6e617bf28def50029cb03) )
	ROM_LOAD16_WORD_SWAP( "c72b1.p2", 0x080000, 0x80000, CRC(d3320d13) SHA1(c6fa2b8b727a1192fd21131496067447053b5547) )
	ROM_LOAD16_WORD_SWAP( "c72.p3",  0x100000, 0x80000, CRC(1a5de0cb) SHA1(738a27f83704c208d36d73bf766d861ef2d51a89) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",  0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",  0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",  0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",  0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",  0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mshvsfb.key",  0x00, 0x14, CRC(3f5bb6e4) SHA1(dbebcebf24997de48e5174d82af5cd53dbe0dada) )
ROM_END

/*
Marvel Vs. Capcom: Clash of Super Heroes (Euro 980123):
The originally dumped ROM4 contains garbage instructions that cause the game to crash during the ending staff roll.
The ROM has been repaired so that the code matches the other sets after decryption:

    offset      dumped  repaired    decrypted
    0x40/2      001a    189a        EB11 -> 4EF9
    0x60/2      800b    c00b        0843 -> 227C
*/

ROM_START( mvsc )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.p1", 0x000000, 0x80000, CRC(824e4a90) SHA1(5c79c166d988d8a75d9941f4ee6fa4d6476e55e1) )
//  ROM_LOAD16_WORD_SWAP( "c77.p2", 0x080000, 0x80000, BAD_DUMP CRC(cac02153) SHA1(ee9f9da6fda53f21ba7b74367612c90281269690) )
	ROM_LOAD16_WORD_SWAP( "c77.p2", 0x080000, 0x80000, CRC(436c5a4e) SHA1(82f4586e888f2550c53bfdc93a53791a595e05bd) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",  0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",  0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",   0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",   0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvsc.key",     0x00, 0x14, CRC(7e101e09) SHA1(9d725a7c6bbc20e46f749eaec4bab15b0195077a) )
ROM_END

ROM_START( mvscr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77e.p1", 0x000000, 0x80000, CRC(e0633fc0) SHA1(d2feffb5505a2f8940192ae267f49561fc580b87) )
	ROM_LOAD16_WORD_SWAP( "c77e.p2", 0x080000, 0x80000, CRC(a450a251) SHA1(1e34fa55bb93c7573ab0205f8c5620a51765d3eb) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p3",  0x100000, 0x80000, CRC(7db71ce9) SHA1(a0097109e9f4aba40791932269d600c0ffa099a7) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p4",  0x180000, 0x80000, CRC(4b0b6d3e) SHA1(375372adf0a508bb6fc6a79326b2d4171db9ca0f) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",  0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",  0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",  0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",  0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvsc.key",     0x00, 0x14, CRC(7e101e09) SHA1(9d725a7c6bbc20e46f749eaec4bab15b0195077a) )
ROM_END

ROM_START( mvscu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77u.p1", 0x000000, 0x80000, CRC(c6007557) SHA1(c027c1a204345ce611cb042d60939e4de156763f) )
	ROM_LOAD16_WORD_SWAP( "c77u.p2", 0x080000, 0x80000, CRC(724b2b20) SHA1(872bbcf5d344d634f3523318fa4763e6d6302bb5) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",  0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",  0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",   0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",   0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscu.key",    0x00, 0x14, CRC(a83db333) SHA1(7f7288ceadf233d913728f7c4a8841adcb5994e8) )
ROM_END

ROM_START( mvscur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77ur1.p1",  0x000000, 0x80000, CRC(13f2be57) SHA1(229520c5f4171abae1acdfb9544b356b568b5f2d) )
	ROM_LOAD16_WORD_SWAP( "c77ur1.p2",  0x080000, 0x80000, CRC(5e9b380d) SHA1(8989192b575fb717c13bfb8175e83d7f1a285310) )
	ROM_LOAD16_WORD_SWAP( "c77u.p3",  0x100000, 0x80000, CRC(12f321be) SHA1(32942c4690253c1545ce05dc084de227e46d942a) )
	ROM_LOAD16_WORD_SWAP( "c77u.p4",  0x180000, 0x80000, CRC(2f1524bc) SHA1(b6543d40fb98eabec82787e0fd60fbc59069e72e) )
	ROM_LOAD16_WORD_SWAP( "c77u.p5",  0x200000, 0x80000, CRC(5fdecadb) SHA1(59726e535d1fd9cc4ddfdd02c936121d70723ced) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77u.p8",  0x380000, 0x80000, CRC(4f36cd63) SHA1(ee6bdbf14bd524f76f4f756f332315331d5e1e4b) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscu.key",    0x00, 0x14, CRC(a83db333) SHA1(7f7288ceadf233d913728f7c4a8841adcb5994e8) )
ROM_END

ROM_START( mvscj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77j.p1", 0x000000, 0x80000, CRC(3df18879) SHA1(2b91da6e5dd792967337e873ebb08ecf5194a97b) )
	ROM_LOAD16_WORD_SWAP( "c77j.p2", 0x080000, 0x80000, CRC(07d212e8) SHA1(c5420e9bd580910c1f1d0264240aeef20aac30a7) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",  0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",  0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",   0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",   0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscj.key",    0x00, 0x14, CRC(9dedbcaf) SHA1(6468dd20ba89e4f6dc03340d218694690151ebe0) )
ROM_END

ROM_START( mvscjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77jr1.p1", 0x000000, 0x80000, CRC(2164213f) SHA1(00e3500ed334bb80d4d159eacf439860a2bfc3b7) )
	ROM_LOAD16_WORD_SWAP( "c77jr1.p2", 0x080000, 0x80000, CRC(c905c86f) SHA1(965fa3bdc29bd901e9efcc53b195c6be3a74c9f9) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p3",  0x100000, 0x80000, CRC(7db71ce9) SHA1(a0097109e9f4aba40791932269d600c0ffa099a7) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p4",  0x180000, 0x80000, CRC(4b0b6d3e) SHA1(375372adf0a508bb6fc6a79326b2d4171db9ca0f) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",  0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",  0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",  0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",  0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscj.key",    0x00, 0x14, CRC(9dedbcaf) SHA1(6468dd20ba89e4f6dc03340d218694690151ebe0) )
ROM_END

/*

   This is the Single PCB release of Marvel Vs. Capcom, it comes in a black case and has the Program ROM stored on
   SIMMs similar to CPS3.  All ROM data (including the code) is *identical* to the regular Japan set (mvscj) just
   in different sized ROMs.

   The original PCB is said to show the volume / sound test in a slightly different way, but this must be down to
   some software configuration or an unemulated hardware register, not the code.

   Another board with an alternative SIMM card configuration has been dumped. The data stored on SIMMs is the same
   but it uses a different card with a different ROM type. Here are the differences:

   Ver. 1

   SIMMs: mvc_ja.simm1, mvc_ja.simm3
   Card Type: HB29A216SB10CP 9649 3 (front side), M72A-0 (back side)
   SIMM Type: HN98F1600T10

   Ver. 2

   SIMMs: mvc_ja.simm4, mvc_ja.simm6
   Card Type: FF03216 MB85713-90 9619 M26 (front side), MC72-89 (B) IBI,M4V0 (back side)
   SIMM Type: 29F016 (reverse pin type)

*/

ROM_START( mvscjsing )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_BYTE( "c77jsing.p1", 0x000000, 0x200000, CRC(6a2ef7c2) SHA1(625530b92217375014db4694196e6ab2a4684db6) )
	ROM_LOAD16_BYTE( "c77jsing.p2", 0x000001, 0x200000, CRC(699d09ad) SHA1(67f6587808f55f10f58e067512f8db3f67dda770) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77jsing.c1",   0x0000000, 0x800000, CRC(8428ce69) SHA1(65b1cdb40e5bd0c9afc21d267d02d118f8c9a44a) )
	ROM_LOAD64_WORD( "c77jsing.c2",   0x0000002, 0x800000, CRC(2e0028f4) SHA1(be21622c5e3ba9a0a799d943fc6cc2bf7ec9582f) )
	ROM_LOAD64_WORD( "c77jsing.c3",   0x0000004, 0x800000, CRC(308ca826) SHA1(2ef1fb4999e7e25e7f605c788f61a85da6715475) )
	ROM_LOAD64_WORD( "c77jsing.c4",   0x0000006, 0x800000, CRC(10699fe1) SHA1(4bb65999c2a73c46cd0c7b6ea26ffb0d8ab24602) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77jsing.q1",   0x000000, 0x800000, CRC(5d8819e0) SHA1(afe2ec7fa4786e6d9a9a0ffa5787862ad69b0010) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscj.key",    0x00, 0x14, CRC(9dedbcaf) SHA1(6468dd20ba89e4f6dc03340d218694690151ebe0) )
ROM_END

ROM_START( mvsca )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77a.p1", 0x000000, 0x80000, CRC(2ff4ae25) SHA1(74cc3656a265f912c72cc6e591de14791fa9a50d) )
	ROM_LOAD16_WORD_SWAP( "c77a.p2", 0x080000, 0x80000, CRC(f28427ef) SHA1(3e4c91753b19c6307abd0ad87a0184730a418efb) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",  0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",  0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",   0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",   0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvsca.key",    0x00, 0x14, CRC(31edaee8) SHA1(17267ef1939348ad1f840df4913ff04e43a3aac8) )
ROM_END

ROM_START( mvscar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77ar1.p1", 0x000000, 0x80000, CRC(fe5fa7b9) SHA1(c27b987ffb631c3433aa32a29989dbf2b3e53f1e) )
	ROM_LOAD16_WORD_SWAP( "c77ar1.p2", 0x080000, 0x80000, CRC(082b701c) SHA1(363770ecd5f4e160db6448845ba6d7fd0beea291) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p3",  0x100000, 0x80000, CRC(7db71ce9) SHA1(a0097109e9f4aba40791932269d600c0ffa099a7) )
	ROM_LOAD16_WORD_SWAP( "c77r1.p4",  0x180000, 0x80000, CRC(4b0b6d3e) SHA1(375372adf0a508bb6fc6a79326b2d4171db9ca0f) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",  0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",  0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",  0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",  0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvsca.key",    0x00, 0x14, CRC(31edaee8) SHA1(17267ef1939348ad1f840df4913ff04e43a3aac8) )
ROM_END

ROM_START( mvsch )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77h.p1", 0x000000, 0x80000, CRC(6a0ec9f7) SHA1(62d7b28cc9ddf975ccdc8992d51bd3d085e3e136) )
	ROM_LOAD16_WORD_SWAP( "c77h.p2", 0x080000, 0x80000, CRC(00f03fa4) SHA1(3a79400a7ac6e7594ca7e0fbb2486ddf6c759d18) )
	ROM_LOAD16_WORD_SWAP( "c77.p3", 0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4", 0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",  0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",  0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",  0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",  0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvsch.key",    0x00, 0x14, CRC(dd647c0d) SHA1(942a2d8a294e826de725aed25cb918fe815be33a) )
ROM_END

ROM_START( mvscb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77b.p1", 0x000000, 0x80000, CRC(7155953b) SHA1(acd1f86bc24636a02aeef566994d38ffa6dbf72f) )
	ROM_LOAD16_WORD_SWAP( "c77b.p2", 0x080000, 0x80000, CRC(fb117d0e) SHA1(c513f430e7c4f32e220a800c53ffdcea68c6cfcf) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",  0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",  0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77.p5",   0x200000, 0x80000, CRC(c3baa32b) SHA1(d35589847e0753e869ffcd7c3abed925bfdb0fa2) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",   0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",   0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",   0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mvscb.key",    0x00, 0x14, CRC(d74a7a3d) SHA1(31ff54f75b73661fbfa8ebbb2395924fea3cbae3) )
ROM_END

ROM_START( mpang )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c84.p1", 0x000000, 0x80000, CRC(fe16fc9f) SHA1(be22bf8a0abd53d2e7ebc12d3d0020bf799a25e9) )
	ROM_LOAD16_WORD_SWAP( "c84.p2", 0x080000, 0x80000, CRC(2cc5ec22) SHA1(c188349c26a64bad325cfa218849ed1e94303087) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c84.c1",   0x0000000, 0x200000, CRC(388db66b) SHA1(7416cce3d0dbea71c92ea9f72f5536146f757b45) )
	ROM_LOAD64_BYTE( "c84.c2",   0x0000001, 0x200000, CRC(aff1b494) SHA1(d376c02ce01e71a7707d3d3fe5b0ae59ce781686) )
	ROM_LOAD64_BYTE( "c84.c3",   0x0000002, 0x200000, CRC(a9c4857b) SHA1(66f538105c710d1480141e48a15b1a760f5ce985) )
	ROM_LOAD64_BYTE( "c84.c4",   0x0000003, 0x200000, CRC(f759df22) SHA1(1678e3e819dd808f3a6fdd52b7c933eac4777b5b) )
	ROM_LOAD64_BYTE( "c84.c5",   0x0000004, 0x200000, CRC(dec6b720) SHA1(331776e1cba3fb82071e7c2195dc4ae27b3613a2) )
	ROM_LOAD64_BYTE( "c84.c6",   0x0000005, 0x200000, CRC(f8774c18) SHA1(58e0ea4dd62e39bcfaa3a2be4ef08eb2f0bd3c00) )
	ROM_LOAD64_BYTE( "c84.c7",   0x0000006, 0x200000, CRC(c2aea4ec) SHA1(f5e2a815fa802598611efa48e5de97e929155e77) )
	ROM_LOAD64_BYTE( "c84.c8",   0x0000007, 0x200000, CRC(84d6dc33) SHA1(f5ababb479facc08c425381570644230c09334e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c84.m1",   0x00000, 0x08000, CRC(90c7adb6) SHA1(a2653e977e5e0457b249e098e5ca0abc93dac336) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c84.q1",   0x000000, 0x200000, CRC(318a2e21) SHA1(c573cd88f8279a062c73ef1d79cd8421dbdcd93e) )
	ROM_LOAD16_WORD_SWAP( "c84.q2",   0x200000, 0x200000, CRC(5462f4e8) SHA1(299fbdab700e735e6395c5d9e3f079bb2e3dbd73) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mpang.key",    0x00, 0x14, CRC(95354b0f) SHA1(6e015eda29ac94aa420d7c70cdebffee52fde032) )
ROM_END

ROM_START( mpangr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c84r1.p1", 0x000000, 0x80000, CRC(6ef0f9b2) SHA1(fd1c76e151466fe27b02a6d07683fdd9c6a4816d) )
	ROM_LOAD16_WORD_SWAP( "c84r1.p2", 0x080000, 0x80000, CRC(30a468bb) SHA1(f2f8f600a079ff050c9a0c12e1a6368943bdc536) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c84.c1",   0x0000000, 0x200000, CRC(388db66b) SHA1(7416cce3d0dbea71c92ea9f72f5536146f757b45) )
	ROM_LOAD64_BYTE( "c84.c2",   0x0000001, 0x200000, CRC(aff1b494) SHA1(d376c02ce01e71a7707d3d3fe5b0ae59ce781686) )
	ROM_LOAD64_BYTE( "c84.c3",   0x0000002, 0x200000, CRC(a9c4857b) SHA1(66f538105c710d1480141e48a15b1a760f5ce985) )
	ROM_LOAD64_BYTE( "c84.c4",   0x0000003, 0x200000, CRC(f759df22) SHA1(1678e3e819dd808f3a6fdd52b7c933eac4777b5b) )
	ROM_LOAD64_BYTE( "c84.c5",   0x0000004, 0x200000, CRC(dec6b720) SHA1(331776e1cba3fb82071e7c2195dc4ae27b3613a2) )
	ROM_LOAD64_BYTE( "c84.c6",   0x0000005, 0x200000, CRC(f8774c18) SHA1(58e0ea4dd62e39bcfaa3a2be4ef08eb2f0bd3c00) )
	ROM_LOAD64_BYTE( "c84.c7",   0x0000006, 0x200000, CRC(c2aea4ec) SHA1(f5e2a815fa802598611efa48e5de97e929155e77) )
	ROM_LOAD64_BYTE( "c84.c8",   0x0000007, 0x200000, CRC(84d6dc33) SHA1(f5ababb479facc08c425381570644230c09334e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c84.m1",   0x00000, 0x08000, CRC(90c7adb6) SHA1(a2653e977e5e0457b249e098e5ca0abc93dac336) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c84.q1",   0x000000, 0x200000, CRC(318a2e21) SHA1(c573cd88f8279a062c73ef1d79cd8421dbdcd93e) )
	ROM_LOAD16_WORD_SWAP( "c84.q2",   0x200000, 0x200000, CRC(5462f4e8) SHA1(299fbdab700e735e6395c5d9e3f079bb2e3dbd73) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mpang.key",    0x00, 0x14, CRC(95354b0f) SHA1(6e015eda29ac94aa420d7c70cdebffee52fde032) )
ROM_END

ROM_START( mpangu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c84u.p1", 0x000000, 0x80000, CRC(6e7ed03c) SHA1(3562362d9573252d4d19dbfd0ec7e47e9eaa5f46) )
	ROM_LOAD16_WORD_SWAP( "c84u.p2", 0x080000, 0x80000, CRC(de079131) SHA1(95da2a03cb642963aabfebd8337003961ca4db71) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c84u.c1",  0x800000, 0x200000, CRC(c5f123dc) SHA1(e459571416ab64f0280cda7dfa0d8836faa745c9) )
	ROM_LOAD64_WORD( "c84u.c2",  0x800002, 0x200000, CRC(8e033265) SHA1(d05ca8fb825423ff6b099c06aaa500c0a947454a) )
	ROM_LOAD64_WORD( "c84u.c3",  0x800004, 0x200000, CRC(cfcd73d2) SHA1(3415c063ac3632159df0fa74899bde8c90f4a2a1) )
	ROM_LOAD64_WORD( "c84u.c4",  0x800006, 0x200000, CRC(2db1ffbc) SHA1(ca58c9359c2c629896c40e78c14907f562e2cdad) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c84.m1",   0x00000, 0x08000, CRC(90c7adb6) SHA1(a2653e977e5e0457b249e098e5ca0abc93dac336) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c84u.q1",   0x000000, 0x100000, CRC(d21c1f5a) SHA1(94cfcf01e656c0fb690e6204964ac70fbc89064d) ) /* This version uses roms chips */
	ROM_LOAD16_WORD_SWAP( "c84u.q2",   0x100000, 0x100000, CRC(d22090b1) SHA1(0ba65c0efb46af3cfb8ea3fe5087186248c57420) )
	ROM_LOAD16_WORD_SWAP( "c84u.q3",   0x200000, 0x100000, CRC(60aa5ef2) SHA1(03fa994d7f5b43e05e8417a8769e07c22548a27a) )
	ROM_LOAD16_WORD_SWAP( "c84u.q4",   0x300000, 0x100000, CRC(3a67d203) SHA1(7213364745d0c4c7fe11573afb9678483e24acb4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mpang.key",    0x00, 0x14, CRC(95354b0f) SHA1(6e015eda29ac94aa420d7c70cdebffee52fde032) )
ROM_END

ROM_START( mpangj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c84j.p1", 0x000000, 0x80000, CRC(bf597b1c) SHA1(0412e826eec7a9f3e70c84b64c9fbcecf7e0c56a) )
	ROM_LOAD16_WORD_SWAP( "c84j.p2", 0x080000, 0x80000, CRC(f4a3ab0f) SHA1(2e54bbc95304827fcd24dab35e4895f4e6566be0) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c84.c1",   0x0000000, 0x200000, CRC(388db66b) SHA1(7416cce3d0dbea71c92ea9f72f5536146f757b45) )
	ROM_LOAD64_BYTE( "c84.c2",   0x0000001, 0x200000, CRC(aff1b494) SHA1(d376c02ce01e71a7707d3d3fe5b0ae59ce781686) )
	ROM_LOAD64_BYTE( "c84.c3",   0x0000002, 0x200000, CRC(a9c4857b) SHA1(66f538105c710d1480141e48a15b1a760f5ce985) )
	ROM_LOAD64_BYTE( "c84.c4",   0x0000003, 0x200000, CRC(f759df22) SHA1(1678e3e819dd808f3a6fdd52b7c933eac4777b5b) )
	ROM_LOAD64_BYTE( "c84.c5",   0x0000004, 0x200000, CRC(dec6b720) SHA1(331776e1cba3fb82071e7c2195dc4ae27b3613a2) )
	ROM_LOAD64_BYTE( "c84.c6",   0x0000005, 0x200000, CRC(f8774c18) SHA1(58e0ea4dd62e39bcfaa3a2be4ef08eb2f0bd3c00) )
	ROM_LOAD64_BYTE( "c84.c7",   0x0000006, 0x200000, CRC(c2aea4ec) SHA1(f5e2a815fa802598611efa48e5de97e929155e77) )
	ROM_LOAD64_BYTE( "c84.c8",   0x0000007, 0x200000, CRC(84d6dc33) SHA1(f5ababb479facc08c425381570644230c09334e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c84.m1",   0x00000, 0x08000, CRC(90c7adb6) SHA1(a2653e977e5e0457b249e098e5ca0abc93dac336) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c84.q1",   0x000000, 0x200000, CRC(318a2e21) SHA1(c573cd88f8279a062c73ef1d79cd8421dbdcd93e) )
	ROM_LOAD16_WORD_SWAP( "c84.q2",   0x200000, 0x200000, CRC(5462f4e8) SHA1(299fbdab700e735e6395c5d9e3f079bb2e3dbd73) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mpang.key",    0x00, 0x14, CRC(95354b0f) SHA1(6e015eda29ac94aa420d7c70cdebffee52fde032) )
ROM_END

ROM_START( mpanga )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c84a.p1", 0x000000, 0x80000, CRC(b05ebc34) SHA1(b30784dcc1aaf0f37930c125331dfa41240c5859) )
	ROM_LOAD16_WORD_SWAP( "c84a.p2", 0x080000, 0x80000, CRC(8e4814b4) SHA1(f86e0b7986de115b76c579e6ffd4e77da2360a01) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c84.c1",   0x0000000, 0x200000, CRC(388db66b) SHA1(7416cce3d0dbea71c92ea9f72f5536146f757b45) )
	ROM_LOAD64_BYTE( "c84.c2",   0x0000001, 0x200000, CRC(aff1b494) SHA1(d376c02ce01e71a7707d3d3fe5b0ae59ce781686) )
	ROM_LOAD64_BYTE( "c84.c3",   0x0000002, 0x200000, CRC(a9c4857b) SHA1(66f538105c710d1480141e48a15b1a760f5ce985) )
	ROM_LOAD64_BYTE( "c84.c4",   0x0000003, 0x200000, CRC(f759df22) SHA1(1678e3e819dd808f3a6fdd52b7c933eac4777b5b) )
	ROM_LOAD64_BYTE( "c84.c5",   0x0000004, 0x200000, CRC(dec6b720) SHA1(331776e1cba3fb82071e7c2195dc4ae27b3613a2) )
	ROM_LOAD64_BYTE( "c84.c6",   0x0000005, 0x200000, CRC(f8774c18) SHA1(58e0ea4dd62e39bcfaa3a2be4ef08eb2f0bd3c00) )
	ROM_LOAD64_BYTE( "c84.c7",   0x0000006, 0x200000, CRC(c2aea4ec) SHA1(f5e2a815fa802598611efa48e5de97e929155e77) )
	ROM_LOAD64_BYTE( "c84.c8",   0x0000007, 0x200000, CRC(84d6dc33) SHA1(f5ababb479facc08c425381570644230c09334e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c84.m1",   0x00000, 0x08000, CRC(90c7adb6) SHA1(a2653e977e5e0457b249e098e5ca0abc93dac336) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c84.q1",   0x000000, 0x200000, CRC(318a2e21) SHA1(c573cd88f8279a062c73ef1d79cd8421dbdcd93e) )
	ROM_LOAD16_WORD_SWAP( "c84.q2",   0x200000, 0x200000, CRC(5462f4e8) SHA1(299fbdab700e735e6395c5d9e3f079bb2e3dbd73) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mpang.key",    0x00, 0x14, CRC(95354b0f) SHA1(6e015eda29ac94aa420d7c70cdebffee52fde032) )
ROM_END

ROM_START( nwarr )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.p1", 0x000000, 0x80000, CRC(a922c44f) SHA1(aa06c7ee9b9878d999a21201f4ff3134cef1fff0) )
	ROM_LOAD16_WORD_SWAP( "c57.p2", 0x080000, 0x80000, CRC(7312d890) SHA1(8cf2ff6eb2c13e308e65d7ac7a5f9664c44170df) )
	ROM_LOAD16_WORD_SWAP( "c57.p3", 0x100000, 0x80000, CRC(cde8b506) SHA1(1c4f7fa6799be1aa444ed31464cddb79fee22928) )
	ROM_LOAD16_WORD_SWAP( "c57.p4", 0x180000, 0x80000, CRC(be99e7d0) SHA1(863c713ba7b6120a50835ee7a0153021fbbcef81) )
	ROM_LOAD16_WORD_SWAP( "c57.p5", 0x200000, 0x80000, CRC(69e0e60c) SHA1(8b9b7280dfbbb45a875d78d40703503ffce663bb) )
	ROM_LOAD16_WORD_SWAP( "c57.p6", 0x280000, 0x80000, CRC(d95a3849) SHA1(9ec77670fb83cf9beba95439b709c138e800bcc8) )
	ROM_LOAD16_WORD_SWAP( "c57.p7", 0x300000, 0x80000, CRC(9882561c) SHA1(cc6d4a50819cd4e6b8c39c60e7c8ce46ba0f05d8) )
	ROM_LOAD16_WORD_SWAP( "c57.p8", 0x380000, 0x80000, CRC(976fa62f) SHA1(ede8cb58365795f7068f3be52639383e226cf751) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "nwarr.key",    0x00, 0x14, CRC(618a13ca) SHA1(b22762b9e85f0e70e0ec4833bcca7c5b6b902c4c) )
ROM_END

ROM_START( nwarru )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57u.p1", 0x000000, 0x80000, CRC(85d6a359) SHA1(38f4bb5cf2e1e82ce9673b911329fdc8220ce0dc) )
	ROM_LOAD16_WORD_SWAP( "c57u.p2", 0x080000, 0x80000, CRC(cb7fce77) SHA1(85a8c8b1c71df0eee5f23e0bf28b2d95af2ce830) )
	ROM_LOAD16_WORD_SWAP( "c57u.p3", 0x100000, 0x80000, CRC(e08f2bba) SHA1(0f6d01a3b05085df23ead4c09c5363a1587b527e) )
	ROM_LOAD16_WORD_SWAP( "c57u.p4", 0x180000, 0x80000, CRC(08c04cdb) SHA1(b78d87631a13c26cc1580d2ecc0d137105c23f0a) )
	ROM_LOAD16_WORD_SWAP( "c57u.p5", 0x200000, 0x80000, CRC(b5a5ab19) SHA1(f7b35b8cba81f88a6bdfea7e2dc12eca480c276c) )
	ROM_LOAD16_WORD_SWAP( "c57u.p6", 0x280000, 0x80000, CRC(51bb20fb) SHA1(a98c569dd45b4bd2275f9bd1df060d6eaead53df) )
	ROM_LOAD16_WORD_SWAP( "c57u.p7", 0x300000, 0x80000, CRC(41a64205) SHA1(1f5af658b7c3fb09cab3dd10d6dc433a0605f81a) )
	ROM_LOAD16_WORD_SWAP( "c57u.p8", 0x380000, 0x80000, CRC(2b1d43ae) SHA1(fa9a456fe92783c7cb93ca231b24387cf56644d7) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "nwarru.key",   0x00, 0x14, CRC(1c593f9b) SHA1(77a91d9a223238ce80e20eb7b385994fc414eee9) )
ROM_END

ROM_START( nwarrh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57h.p1", 0x000000, 0x80000, CRC(6029c7be) SHA1(687c382d02d18ef5781c9d928e74f161461c2641) )
	ROM_LOAD16_WORD_SWAP( "c57h.p2", 0x080000, 0x80000, CRC(d26625ee) SHA1(2b415a28ee39949a1e80e7e65b89f3d707bdfae7) )
	ROM_LOAD16_WORD_SWAP( "c57h.p3", 0x100000, 0x80000, CRC(73ee0b8a) SHA1(d31ccb2cbc6133972f6dc76f0a40154368953625) )
	ROM_LOAD16_WORD_SWAP( "c57h.p4", 0x180000, 0x80000, CRC(a5b3a50a) SHA1(de382ab707eeb4ec7ffbc637611296ee35acdce1) )
	ROM_LOAD16_WORD_SWAP( "c57h.p5",  0x200000, 0x80000, CRC(5fc2bdc1) SHA1(5936f2d3eb6becefa3ede98107eb03723555cc22) )
	ROM_LOAD16_WORD_SWAP( "c57h.p6",  0x280000, 0x80000, CRC(e65588d9) SHA1(4b15009d5aa2d91736af1ee7c52d6b49cc696724) )
	ROM_LOAD16_WORD_SWAP( "c57h.p7",  0x300000, 0x80000, CRC(a2ce6d63) SHA1(52aed61d0c7a6191016f1ec4b0a4372fbf55bf49) )
	ROM_LOAD16_WORD_SWAP( "c57h.p8",  0x380000, 0x80000, CRC(e2f4f4b9) SHA1(8d3e857ccd4654d2801ce6830c0d556a81c4d433) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "nwarrh.key",   0x00, 0x14, CRC(5fb16b23) SHA1(c383c1df3deb8ca9d3b7677fbaa85424a18c8115) )
ROM_END

ROM_START( nwarrb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57b.p1", 0x000000, 0x80000, CRC(3a426d3f) SHA1(76d7c39c901aa768bb1600179509752d1fc0d558) )
	ROM_LOAD16_WORD_SWAP( "c57b.p2", 0x080000, 0x80000, CRC(51c4bb2f) SHA1(c885813ff13bfd251accf38da1bc0bd9c526e4c5) )
	ROM_LOAD16_WORD_SWAP( "c57b.p3", 0x100000, 0x80000, CRC(ac44d997) SHA1(b28e55d83a33e885125f2def76259d0ab21b0f9b) )
	ROM_LOAD16_WORD_SWAP( "c57b.p4", 0x180000, 0x80000, CRC(5072a5fe) SHA1(78b3f2ef8bc16441d0d977dbec2246c9f9b28dbc) )
	ROM_LOAD16_WORD_SWAP( "c57b.p5",  0x200000, 0x80000, CRC(9b355192) SHA1(10b5542fcc0af936868af9abf70d3303be543f21) )
	ROM_LOAD16_WORD_SWAP( "c57b.p6",  0x280000, 0x80000, CRC(42220f84) SHA1(f6ef52b1dff86c25852aa05be4a5b39995c26dd7) )
	ROM_LOAD16_WORD_SWAP( "c57b.p7",  0x300000, 0x80000, CRC(029e015d) SHA1(441d0ea36484cbffe783cb0a1133537c09783022) )
	ROM_LOAD16_WORD_SWAP( "c57b.p8",  0x380000, 0x80000, CRC(37b3ce37) SHA1(5919b44415e4d5b242fcdd69efd0ab1722e4da8c) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "nwarrb.key",   0x00, 0x14, CRC(4ffc0a54) SHA1(632c0e2ac666a97bb5d8ac596cde8dd0bbfe303b) )
ROM_END

ROM_START( nwarra )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57a.p1", 0x000000, 0x80000, CRC(0a70cdd6) SHA1(08d3f3892bfc8e741c59071150e6c3a8bf5ffbef) )
	ROM_LOAD16_WORD_SWAP( "c57a.p2", 0x080000, 0x80000, CRC(70ce62e4) SHA1(8bc9066071585805b6420bcc40eb03e641e3f027) )
	ROM_LOAD16_WORD_SWAP( "c57a.p3", 0x100000, 0x80000, CRC(5692a03f) SHA1(7c2ba1022ee4045d985477a6c9026140b7809234) )
	ROM_LOAD16_WORD_SWAP( "c57a.p4", 0x180000, 0x80000, CRC(b810fe66) SHA1(787198f546a0d3dbf0f2d54a4328aaaf4e3cdcae) )
	ROM_LOAD16_WORD_SWAP( "c57a.p5", 0x200000, 0x80000, CRC(1be264f3) SHA1(b08d0771963e6b8c1a2d32e6db3edd9bffea39e1) )
	ROM_LOAD16_WORD_SWAP( "c57a.p6", 0x280000, 0x80000, CRC(86f1ed52) SHA1(95095ef7037cf5f90f00b15f224e4b35e3ea675e) )
	ROM_LOAD16_WORD_SWAP( "c57a.p7", 0x300000, 0x80000, CRC(7e96bd0a) SHA1(f7750209b157a405710c0797e9eb30f980db582d) )
	ROM_LOAD16_WORD_SWAP( "c57a.p8", 0x380000, 0x80000, CRC(58bce2fd) SHA1(2df1460cb349d64ca84fd3372a9a64b6cdbe078f) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "nwarra.key",   0x00, 0x14, CRC(9bafff67) SHA1(e8856b4e60a9e42bd5126323ad4f70f6a88b6f4c) )
ROM_END

ROM_START( vhuntj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57j.p1", 0x000000, 0x80000, CRC(3de2e333) SHA1(2f9f756c5c91646625d70debd5b19b8dbd13a62f) )
	ROM_LOAD16_WORD_SWAP( "c57j.p2", 0x080000, 0x80000, CRC(c95cf304) SHA1(0544ab9d0f398b558e1119d94885058ad4a7d929) )
	ROM_LOAD16_WORD_SWAP( "c57j.p3", 0x100000, 0x80000, CRC(50de5ddd) SHA1(2bcc6c254ead06e9ea0a9ae4348195d3d55de277) )
	ROM_LOAD16_WORD_SWAP( "c57j.p4", 0x180000, 0x80000, CRC(ac3bd3d5) SHA1(c0aa04c43dba2876d97d95fffd4766a28193b300) )
	ROM_LOAD16_WORD_SWAP( "c57j.p5", 0x200000, 0x80000, CRC(0761309f) SHA1(7c6f9ec4d93ea9dbd634142558baaaf170cd4c76) )
	ROM_LOAD16_WORD_SWAP( "c57j.p6", 0x280000, 0x80000, CRC(5a5c2bf5) SHA1(296c6a5a0062b58bc71a297bc8b27eea099c8518) )
	ROM_LOAD16_WORD_SWAP( "c57j.p7", 0x300000, 0x80000, CRC(823d6d99) SHA1(17be75b2ebfbf60a2141aef67c386454d23565f2) )
	ROM_LOAD16_WORD_SWAP( "c57j.p8", 0x380000, 0x80000, CRC(32c7d8f0) SHA1(47075fa80ceff6adfa6cc58dbe32ed4ee01ba4fc) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhuntj.key",   0x00, 0x14, CRC(72854f68) SHA1(657a4484c40a8b583abd74a8c7f0524ec6199dd2) )
ROM_END

ROM_START( vhuntjr1s )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57jr1s.p1", 0x000000, 0x80000,CRC(9c4e6191) SHA1(a247beae3400583d5bf950b7c856befef45d3ad6) )
	ROM_LOAD16_WORD_SWAP( "c57j.p2", 0x080000, 0x80000, CRC(c95cf304) SHA1(0544ab9d0f398b558e1119d94885058ad4a7d929) )
	ROM_LOAD16_WORD_SWAP( "c57j.p3", 0x100000, 0x80000, CRC(50de5ddd) SHA1(2bcc6c254ead06e9ea0a9ae4348195d3d55de277) )
	ROM_LOAD16_WORD_SWAP( "c57j.p4", 0x180000, 0x80000, CRC(ac3bd3d5) SHA1(c0aa04c43dba2876d97d95fffd4766a28193b300) )
	ROM_LOAD16_WORD_SWAP( "c57j.p5", 0x200000, 0x80000, CRC(0761309f) SHA1(7c6f9ec4d93ea9dbd634142558baaaf170cd4c76) )
	ROM_LOAD16_WORD_SWAP( "c57j.p6", 0x280000, 0x80000, CRC(5a5c2bf5) SHA1(296c6a5a0062b58bc71a297bc8b27eea099c8518) )
	ROM_LOAD16_WORD_SWAP( "c57j.p7", 0x300000, 0x80000, CRC(823d6d99) SHA1(17be75b2ebfbf60a2141aef67c386454d23565f2) )
	ROM_LOAD16_WORD_SWAP( "c57j.p8", 0x380000, 0x80000, CRC(32c7d8f0) SHA1(47075fa80ceff6adfa6cc58dbe32ed4ee01ba4fc) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhuntj.key",   0x00, 0x14, CRC(72854f68) SHA1(657a4484c40a8b583abd74a8c7f0524ec6199dd2) )
ROM_END

ROM_START( vhuntjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57jr1.p1", 0x000000, 0x80000, CRC(606b682a) SHA1(dd5c1b90a050e344b4f29fc468cfbd92aa392edf) )
	ROM_LOAD16_WORD_SWAP( "c57jr1.p2", 0x080000, 0x80000, CRC(a3b40393) SHA1(04929e24c14b40f55745e02f07ff6af61739a8b4) )
	ROM_LOAD16_WORD_SWAP( "c57jr1.p3", 0x100000, 0x80000, CRC(fccd5558) SHA1(a4d0f4cad7666e61c11e2571850df79b48332f28) )
	ROM_LOAD16_WORD_SWAP( "c57jr1.p4", 0x180000, 0x80000, CRC(07e10a73) SHA1(d3f4f8d52dc3230fb3e8986adb2f00909782160a) )
	ROM_LOAD16_WORD_SWAP( "c57j.p5", 0x200000, 0x80000, CRC(0761309f) SHA1(7c6f9ec4d93ea9dbd634142558baaaf170cd4c76) )
	ROM_LOAD16_WORD_SWAP( "c57j.p6", 0x280000, 0x80000, CRC(5a5c2bf5) SHA1(296c6a5a0062b58bc71a297bc8b27eea099c8518) )
	ROM_LOAD16_WORD_SWAP( "c57j.p7", 0x300000, 0x80000, CRC(823d6d99) SHA1(17be75b2ebfbf60a2141aef67c386454d23565f2) )
	ROM_LOAD16_WORD_SWAP( "c57j.p8", 0x380000, 0x80000, CRC(32c7d8f0) SHA1(47075fa80ceff6adfa6cc58dbe32ed4ee01ba4fc) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhuntj.key",   0x00, 0x14, CRC(72854f68) SHA1(657a4484c40a8b583abd74a8c7f0524ec6199dd2) )
ROM_END

ROM_START( vhuntjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p1", 0x000000, 0x80000, CRC(679c3fa9) SHA1(25c3f595e4d93c16ac483e4f9ba20ad714ecf4ef) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p2", 0x080000, 0x80000, CRC(eb6e71e4) SHA1(7a7cd34f7a70d87b817c0a4242844103db3e9f66) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p3", 0x100000, 0x80000, CRC(eaf634ea) SHA1(d46cb9d5172bb626396354ff2742d4394f0816f1) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p4", 0x180000, 0x80000, CRC(b70cc6be) SHA1(02fc8070bb75a2075de01b891249e6891687440a) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p5", 0x200000, 0x80000, CRC(46ab907d) SHA1(18215ff19e2b0c6505b5b5dfe24ef09fc8539ae5) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p6", 0x280000, 0x80000, CRC(1c00355e) SHA1(72b94b6c5a10ecd11169048d991bcb7550968cc9) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p7", 0x300000, 0x80000, CRC(026e6f82) SHA1(4dffda5e2bcd2fbc9084782e9a79ebd2be1338e7) )
	ROM_LOAD16_WORD_SWAP( "c57jr2.p8", 0x380000, 0x80000, CRC(aadfb3ea) SHA1(f42b76a98f657ba67aee69025476e8114acce4c5) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhuntj.key",   0x00, 0x14, CRC(72854f68) SHA1(657a4484c40a8b583abd74a8c7f0524ec6199dd2) )
ROM_END

ROM_START( progear )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.p1", 0x000000, 0x80000, CRC(343a783e) SHA1(7ba8ae041b062767bf64328adf22ef100c38cdfd) )
	ROM_LOAD16_WORD_SWAP( "c85.p2", 0x080000, 0x80000, CRC(16208d79) SHA1(c477de7f31df44144a60d10dc4d933f3a7c20722) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c85.c1",   0x0000000, 0x200000,  CRC(452f98b0) SHA1(a10e615c32098f6d25becd466da8faa967523a7b) )
	ROM_LOAD64_BYTE( "c85.c2",   0x0000001, 0x200000,  CRC(9e672092) SHA1(fce0b8b43a1c069262f4e3e81c1a04621e232c88) )
	ROM_LOAD64_BYTE( "c85.c3",   0x0000002, 0x200000,  CRC(ae9ddafe) SHA1(afbb26fed6cd0cb5c0099a10d35aeb453318c14d) )
	ROM_LOAD64_BYTE( "c85.c4",   0x0000003, 0x200000,  CRC(94d72d94) SHA1(df6a3fe49c008f73b160eb6f2a44dc371ff73cba) )
	ROM_LOAD64_BYTE( "c85.c5",   0x0000004, 0x200000,  CRC(48a1886d) SHA1(ebf44b42d784924e08a832a7e5f66a887bab244b) )
	ROM_LOAD64_BYTE( "c85.c6",   0x0000005, 0x200000,  CRC(172d7e37) SHA1(0eaedd24cd3fa87b6f35fbd63078d40c493c92d0) )
	ROM_LOAD64_BYTE( "c85.c7",   0x0000006, 0x200000,  CRC(9ee33d98) SHA1(85d1bd31940e35ac8c732165020881a2d65cd6b1) )
	ROM_LOAD64_BYTE( "c85.c8",   0x0000007, 0x200000,  CRC(848dee32) SHA1(c591288e86ad1624d0fe66563808af9fac786e64) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.q1",   0x000000, 0x200000, CRC(c0aac80c) SHA1(91784d35d4f7e113529bb5be6081b67094b150ea) )
	ROM_LOAD16_WORD_SWAP( "c85.q2",   0x200000, 0x200000, CRC(37a65d86) SHA1(374d562a4648734f82aa2ddb6d258e870896dd45) )
	ROM_LOAD16_WORD_SWAP( "c85.q3",   0x400000, 0x200000, CRC(d3f1e934) SHA1(5dcea28c873d0d472f5b94e07d97cd77ace2b252) )
	ROM_LOAD16_WORD_SWAP( "c85.q4",   0x600000, 0x200000, CRC(8b39489a) SHA1(fd790efaf37dc2c4c16f657941044e3e2d3c2711) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "progear.key",  0x00, 0x14, CRC(46736b17) SHA1(3fd0cc78fad80210a7cf8b1150cba1e6121998dd) )
ROM_END

ROM_START( progearj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85j.p1", 0x000000, 0x80000, CRC(06dbba54) SHA1(b0b808e9974c727bd187f2cdcba71a301b78c759) )
	ROM_LOAD16_WORD_SWAP( "c85j.p2", 0x080000, 0x80000, CRC(a1f1f1bc) SHA1(839cdc89d9483632883c185951c76deb4ff7657e) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c85.c1",   0x0000000, 0x200000,  CRC(452f98b0) SHA1(a10e615c32098f6d25becd466da8faa967523a7b) )
	ROM_LOAD64_BYTE( "c85.c2",   0x0000001, 0x200000,  CRC(9e672092) SHA1(fce0b8b43a1c069262f4e3e81c1a04621e232c88) )
	ROM_LOAD64_BYTE( "c85.c3",   0x0000002, 0x200000,  CRC(ae9ddafe) SHA1(afbb26fed6cd0cb5c0099a10d35aeb453318c14d) )
	ROM_LOAD64_BYTE( "c85.c4",   0x0000003, 0x200000,  CRC(94d72d94) SHA1(df6a3fe49c008f73b160eb6f2a44dc371ff73cba) )
	ROM_LOAD64_BYTE( "c85.c5",   0x0000004, 0x200000,  CRC(48a1886d) SHA1(ebf44b42d784924e08a832a7e5f66a887bab244b) )
	ROM_LOAD64_BYTE( "c85.c6",   0x0000005, 0x200000,  CRC(172d7e37) SHA1(0eaedd24cd3fa87b6f35fbd63078d40c493c92d0) )
	ROM_LOAD64_BYTE( "c85.c7",   0x0000006, 0x200000,  CRC(9ee33d98) SHA1(85d1bd31940e35ac8c732165020881a2d65cd6b1) )
	ROM_LOAD64_BYTE( "c85.c8",   0x0000007, 0x200000,  CRC(848dee32) SHA1(c591288e86ad1624d0fe66563808af9fac786e64) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.q1",   0x000000, 0x200000, CRC(c0aac80c) SHA1(91784d35d4f7e113529bb5be6081b67094b150ea) )
	ROM_LOAD16_WORD_SWAP( "c85.q2",   0x200000, 0x200000, CRC(37a65d86) SHA1(374d562a4648734f82aa2ddb6d258e870896dd45) )
	ROM_LOAD16_WORD_SWAP( "c85.q3",   0x400000, 0x200000, CRC(d3f1e934) SHA1(5dcea28c873d0d472f5b94e07d97cd77ace2b252) )
	ROM_LOAD16_WORD_SWAP( "c85.q4",   0x600000, 0x200000, CRC(8b39489a) SHA1(fd790efaf37dc2c4c16f657941044e3e2d3c2711) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "progearj.key", 0x00, 0x14, CRC(d8d515e5) SHA1(d0cfe4effac0e9ede8ad31db8972436638ff8d6f) )
ROM_END

ROM_START( progeara )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85a.p1", 0x000000, 0x80000, CRC(25e6e2ce) SHA1(15b702ccc2bab7f2f9e7724ed83931c686763ffe) )
	ROM_LOAD16_WORD_SWAP( "c85a.p2", 0x080000, 0x80000, CRC(8104307e) SHA1(3ed134fcbf5c1c9f068f59b49b96d01a1ea33eeb) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c85.c1",   0x0000000, 0x200000,  CRC(452f98b0) SHA1(a10e615c32098f6d25becd466da8faa967523a7b) )
	ROM_LOAD64_BYTE( "c85.c2",   0x0000001, 0x200000,  CRC(9e672092) SHA1(fce0b8b43a1c069262f4e3e81c1a04621e232c88) )
	ROM_LOAD64_BYTE( "c85.c3",   0x0000002, 0x200000,  CRC(ae9ddafe) SHA1(afbb26fed6cd0cb5c0099a10d35aeb453318c14d) )
	ROM_LOAD64_BYTE( "c85.c4",   0x0000003, 0x200000,  CRC(94d72d94) SHA1(df6a3fe49c008f73b160eb6f2a44dc371ff73cba) )
	ROM_LOAD64_BYTE( "c85.c5",   0x0000004, 0x200000,  CRC(48a1886d) SHA1(ebf44b42d784924e08a832a7e5f66a887bab244b) )
	ROM_LOAD64_BYTE( "c85.c6",   0x0000005, 0x200000,  CRC(172d7e37) SHA1(0eaedd24cd3fa87b6f35fbd63078d40c493c92d0) )
	ROM_LOAD64_BYTE( "c85.c7",   0x0000006, 0x200000,  CRC(9ee33d98) SHA1(85d1bd31940e35ac8c732165020881a2d65cd6b1) )
	ROM_LOAD64_BYTE( "c85.c8",   0x0000007, 0x200000,  CRC(848dee32) SHA1(c591288e86ad1624d0fe66563808af9fac786e64) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.q1",   0x000000, 0x200000, CRC(c0aac80c) SHA1(91784d35d4f7e113529bb5be6081b67094b150ea) )
	ROM_LOAD16_WORD_SWAP( "c85.q2",   0x200000, 0x200000, CRC(37a65d86) SHA1(374d562a4648734f82aa2ddb6d258e870896dd45) )
	ROM_LOAD16_WORD_SWAP( "c85.q3",   0x400000, 0x200000, CRC(d3f1e934) SHA1(5dcea28c873d0d472f5b94e07d97cd77ace2b252) )
	ROM_LOAD16_WORD_SWAP( "c85.q4",   0x600000, 0x200000, CRC(8b39489a) SHA1(fd790efaf37dc2c4c16f657941044e3e2d3c2711) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "progeara.key", 0x00, 0x14, CRC(30a0fab6) SHA1(be3884a223e76750abe2556b000a821a262423f0) )
ROM_END

ROM_START( pzloop2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c86.p1", 0x000000, 0x80000, CRC(3b1285b2) SHA1(f90f98fb15068306a57109ad954845be0a99e8ab) )
	ROM_LOAD16_WORD_SWAP( "c86.p2", 0x080000, 0x80000, CRC(40a2d647) SHA1(d1c5fa87b368efe0d2cc3f614d0165bd95748b81) )
	ROM_LOAD16_WORD_SWAP( "c86.p3", 0x100000, 0x80000, CRC(0f11d818) SHA1(ca2d5ea892aebfa1a2a825fb45c57b7923936917) )
	ROM_LOAD16_WORD_SWAP( "c86.p4", 0x180000, 0x80000, CRC(86fbbdf4) SHA1(325ba5dc54f3d82f39e1299d5f27de7227a458b2) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c86.c1",   0x0000000, 0x200000, CRC(137b13a7) SHA1(a1ca1bc8699ddfc54d5de1b39a9db9a5ac8b12e5) )
	ROM_LOAD64_BYTE( "c86.c2",   0x0000001, 0x200000, CRC(a2db1507) SHA1(61c84c8d698a846d54a571b5f7b4824e22136db7) )
	ROM_LOAD64_BYTE( "c86.c3",   0x0000002, 0x200000, CRC(7e80ff8e) SHA1(afcebfa995ace8b8973e75f1589980c5c4535bca) )
	ROM_LOAD64_BYTE( "c86.c4",   0x0000003, 0x200000, CRC(cd93e6ed) SHA1(e4afce48fe481d8291ed2475d5de446afad65351) )
	ROM_LOAD64_BYTE( "c86.c5",   0x0000004, 0x200000, CRC(0f52bbca) SHA1(e76c29d445062f5e16d06bdc4ab44640ba35aaac) )
	ROM_LOAD64_BYTE( "c86.c6",   0x0000005, 0x200000, CRC(a62712c3) SHA1(2abfe0209e188010a0ae969f0d9eb7d28820b3f2) )
	ROM_LOAD64_BYTE( "c86.c7",   0x0000006, 0x200000, CRC(b60c9f8e) SHA1(40c7985e04463fb2bd59b3bb6aa5897328d37ff3) )
	ROM_LOAD64_BYTE( "c86.c8",   0x0000007, 0x200000, CRC(83fef284) SHA1(ef4429f54c456d6485a7d642d49dffafef4435fe) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c86.m1",   0x00000, 0x08000, CRC(35697569) SHA1(13718923cffb9ec53cef9e22d8875370b5f3dd74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c86.q1",   0x000000, 0x200000, CRC(85d8fbe8) SHA1(c19d5e9084d07e610379b6e1b6be7bdf0b9b7f7f) )
	ROM_LOAD16_WORD_SWAP( "c86.q2",   0x200000, 0x200000, CRC(1ed62584) SHA1(28411f610f48cca6424a2d53e2a4ac691e826317) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pzloop2.key",  0x00, 0x14, CRC(ae13be78) SHA1(5c715f0ef1e0664027faa6c2a7f0f878462cb7ae) )
ROM_END

ROM_START( pzloop2j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c86j.p1", 0x000000, 0x80000, CRC(3b76b806) SHA1(743808ccd470da075ca2f4a8872b22feb7e993ef) )
	ROM_LOAD16_WORD_SWAP( "c86j.p2", 0x080000, 0x80000, CRC(8878a42a) SHA1(178efe9be9ab7f209489f35d653cb5fef4e9565b) )
	ROM_LOAD16_WORD_SWAP( "c86j.p3", 0x100000, 0x80000, CRC(51081ea4) SHA1(175dc70ba0b6a158d23e7271dd65f28e7beea746) )
	ROM_LOAD16_WORD_SWAP( "c86j.p4", 0x180000, 0x80000, CRC(51c68494) SHA1(c3b2055af11138f5280fe89e248536069a22ddca) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c86.c1",   0x0000000, 0x200000, CRC(137b13a7) SHA1(a1ca1bc8699ddfc54d5de1b39a9db9a5ac8b12e5) )
	ROM_LOAD64_BYTE( "c86.c2",   0x0000001, 0x200000, CRC(a2db1507) SHA1(61c84c8d698a846d54a571b5f7b4824e22136db7) )
	ROM_LOAD64_BYTE( "c86.c3",   0x0000002, 0x200000, CRC(7e80ff8e) SHA1(afcebfa995ace8b8973e75f1589980c5c4535bca) )
	ROM_LOAD64_BYTE( "c86.c4",   0x0000003, 0x200000, CRC(cd93e6ed) SHA1(e4afce48fe481d8291ed2475d5de446afad65351) )
	ROM_LOAD64_BYTE( "c86.c5",   0x0000004, 0x200000, CRC(0f52bbca) SHA1(e76c29d445062f5e16d06bdc4ab44640ba35aaac) )
	ROM_LOAD64_BYTE( "c86.c6",   0x0000005, 0x200000, CRC(a62712c3) SHA1(2abfe0209e188010a0ae969f0d9eb7d28820b3f2) )
	ROM_LOAD64_BYTE( "c86.c7",   0x0000006, 0x200000, CRC(b60c9f8e) SHA1(40c7985e04463fb2bd59b3bb6aa5897328d37ff3) )
	ROM_LOAD64_BYTE( "c86.c8",   0x0000007, 0x200000, CRC(83fef284) SHA1(ef4429f54c456d6485a7d642d49dffafef4435fe) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c86.m1",   0x00000, 0x08000, CRC(35697569) SHA1(13718923cffb9ec53cef9e22d8875370b5f3dd74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c86.q1",   0x000000, 0x200000, CRC(85d8fbe8) SHA1(c19d5e9084d07e610379b6e1b6be7bdf0b9b7f7f) )
	ROM_LOAD16_WORD_SWAP( "c86.q2",   0x200000, 0x200000, CRC(1ed62584) SHA1(28411f610f48cca6424a2d53e2a4ac691e826317) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pzloop2.key",  0x00, 0x14, CRC(ae13be78) SHA1(5c715f0ef1e0664027faa6c2a7f0f878462cb7ae) )
ROM_END

ROM_START( pzloop2jr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c86jr1.p1", 0x000000, 0x80000, CRC(0a751bd0) SHA1(a5a0b60387aacdafdf46ecd1acd764c9cb086b90) )
	ROM_LOAD16_WORD_SWAP( "c86jr1.p2", 0x080000, 0x80000, CRC(c3f72afe) SHA1(597a302e4bba50193c53f239e715962fcc4e3e5e) )
	ROM_LOAD16_WORD_SWAP( "c86jr1.p3", 0x100000, 0x80000, CRC(6ea9dbfc) SHA1(c3065e02516755e8b94a741dd2ab960c96d0ff8c) )
	ROM_LOAD16_WORD_SWAP( "c86jr1.p4", 0x180000, 0x80000, CRC(0f14848d) SHA1(94a3ee00d65cd9a310b3a330e2c37467b5863c64) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c86.c1",   0x0000000, 0x200000, CRC(137b13a7) SHA1(a1ca1bc8699ddfc54d5de1b39a9db9a5ac8b12e5) )
	ROM_LOAD64_BYTE( "c86.c2",   0x0000001, 0x200000, CRC(a2db1507) SHA1(61c84c8d698a846d54a571b5f7b4824e22136db7) )
	ROM_LOAD64_BYTE( "c86.c3",   0x0000002, 0x200000, CRC(7e80ff8e) SHA1(afcebfa995ace8b8973e75f1589980c5c4535bca) )
	ROM_LOAD64_BYTE( "c86.c4",   0x0000003, 0x200000, CRC(cd93e6ed) SHA1(e4afce48fe481d8291ed2475d5de446afad65351) )
	ROM_LOAD64_BYTE( "c86.c5",   0x0000004, 0x200000, CRC(0f52bbca) SHA1(e76c29d445062f5e16d06bdc4ab44640ba35aaac) )
	ROM_LOAD64_BYTE( "c86.c6",   0x0000005, 0x200000, CRC(a62712c3) SHA1(2abfe0209e188010a0ae969f0d9eb7d28820b3f2) )
	ROM_LOAD64_BYTE( "c86.c7",   0x0000006, 0x200000, CRC(b60c9f8e) SHA1(40c7985e04463fb2bd59b3bb6aa5897328d37ff3) )
	ROM_LOAD64_BYTE( "c86.c8",   0x0000007, 0x200000, CRC(83fef284) SHA1(ef4429f54c456d6485a7d642d49dffafef4435fe) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c86.m1",   0x00000, 0x08000, CRC(35697569) SHA1(13718923cffb9ec53cef9e22d8875370b5f3dd74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c86.q1",   0x000000, 0x200000, CRC(85d8fbe8) SHA1(c19d5e9084d07e610379b6e1b6be7bdf0b9b7f7f) )
	ROM_LOAD16_WORD_SWAP( "c86.q2",   0x200000, 0x200000, CRC(1ed62584) SHA1(28411f610f48cca6424a2d53e2a4ac691e826317) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pzloop2.key",  0x00, 0x14, CRC(ae13be78) SHA1(5c715f0ef1e0664027faa6c2a7f0f878462cb7ae) )
ROM_END

ROM_START( qndream )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c68.p1",  0x000000, 0x80000, CRC(7acf3e30) SHA1(5e2a697f98185731afc4130286a2699033dd02af) )
	ROM_LOAD16_WORD_SWAP( "c68.p2",  0x080000, 0x80000, CRC(f1044a87) SHA1(3fd6e5dd6be8c037c8a77cb840bf7d387497a98b) )
	ROM_LOAD16_WORD_SWAP( "c68.p3",  0x100000, 0x80000, CRC(4105ba0e) SHA1(73aacdf4176029f8e21506319e41ce03ed480122) )
	ROM_LOAD16_WORD_SWAP( "c68.p4",  0x180000, 0x80000, CRC(c371e8a5) SHA1(5a93e46e46acfdc93fcb069e2426627e948655bf) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c68.c1",  0x800000, 0x200000, CRC(98af88a2) SHA1(d3620faf2162a1f3a62a238715da4da429376d3c) )
	ROM_LOAD64_WORD( "c68.c2",  0x800002, 0x200000, CRC(df82d491) SHA1(fd3c8303cbcacb132a90398ff61f47d2d68157ae) )
	ROM_LOAD64_WORD( "c68.c3",  0x800004, 0x200000, CRC(42f132ff) SHA1(0e0a128524010dba033a9b9ab2c56fe92a1767da) )
	ROM_LOAD64_WORD( "c68.c4",  0x800006, 0x200000, CRC(b2e128a3) SHA1(8ae3161749d5206f16b755c29466cd5ca249b665) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c68.m1",   0x00000, 0x08000, CRC(e9ce9d0a) SHA1(29f2987788e914e0a55f9130a99e411d15a7cc9b) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c68.q1",   0x000000, 0x200000, CRC(78e7884f) SHA1(82fbbf704ac4bc0e0e7a6f407686861aa3693c23) )
	ROM_LOAD16_WORD_SWAP( "c68.q2",   0x200000, 0x200000, CRC(2e049b13) SHA1(e026f444b905e679e8240c7dd371658c4a3fd713) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "qndream.key",  0x00, 0x14, CRC(97eee4ff) SHA1(541da3a62311e35e563cbf720389988464b6ed7d) )
ROM_END

ROM_START( ringdest )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.p1", 0x000000, 0x80000, CRC(b8016278) SHA1(f744b08b27c11b8567ca7a94fbd75e398563c008) )
	ROM_LOAD16_WORD_SWAP( "c54.p2", 0x080000, 0x80000, CRC(18c4c447) SHA1(3723ad6d6939fa1ac7dd016254b32017b5e7b24e) )
	ROM_LOAD16_WORD_SWAP( "c54.p3", 0x100000, 0x80000, CRC(18ebda7f) SHA1(eacc3e76af5c47abe0a778be7a7beacf0924884e) )
	ROM_LOAD16_WORD_SWAP( "c54.p4", 0x180000, 0x80000, CRC(89c80007) SHA1(4c85aa5b224fdbb64f719a7b8b5b2e7413107c70) )
	ROM_LOAD16_WORD_SWAP( "c54.p5",   0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6",   0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ringdest.key", 0x00, 0x14, CRC(17f9269c) SHA1(596d94ba937a407da94e85d9c2026a7c7d592173) )
ROM_END

ROM_START( smbomb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54j.p1", 0x000000, 0x80000, CRC(1c5613de) SHA1(e6257078ad2e18537aa606b0d0c5e04806244386) )
	ROM_LOAD16_WORD_SWAP( "c54j.p2", 0x080000, 0x80000, CRC(29071ed7) SHA1(eb438fcb42e3fbe38e20bc021be079a3dd7a89fa) )
	ROM_LOAD16_WORD_SWAP( "c54j.p3", 0x100000, 0x80000, CRC(eb20bce4) SHA1(b78a447d3d1d3f9a62a6b5abcd893f5e091f1bbc) )
	ROM_LOAD16_WORD_SWAP( "c54j.p4", 0x180000, 0x80000, CRC(94b420cd) SHA1(4cc43d3f7fed224443e26df5b0076bd24e6cd04b) )
	ROM_LOAD16_WORD_SWAP( "c54.p5",   0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6",   0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "smbomb.key",   0x00, 0x14, CRC(f690069b) SHA1(efd40f5ff900b2ba1e1da5e31d4a4717adacfe79) )
ROM_END

ROM_START( smbombr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54jr1.p1", 0x000000, 0x80000, CRC(52eafb10) SHA1(5abfe07e948748eba982dc8f2e21462aec187590) )
	ROM_LOAD16_WORD_SWAP( "c54jr1.p2", 0x080000, 0x80000, CRC(aa6e8078) SHA1(58b4e15e7e3209e59a37ce48d8b9f0dc8b933cdc) )
	ROM_LOAD16_WORD_SWAP( "c54jr1.p3", 0x100000, 0x80000, CRC(b69e7d5f) SHA1(e66430ef05ed0d1c848d24c7436ee5f1b511dcea) )
	ROM_LOAD16_WORD_SWAP( "c54jr1.p4", 0x180000, 0x80000, CRC(8d857b56) SHA1(48c4e5f195e4343a8b7b9ec496fa1a77d659f72e) )
	ROM_LOAD16_WORD_SWAP( "c54.p5",  0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6",  0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "smbomb.key",   0x00, 0x14, CRC(f690069b) SHA1(efd40f5ff900b2ba1e1da5e31d4a4717adacfe79) )
ROM_END

ROM_START( ringdesta )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54a.p1", 0x000000, 0x80000, CRC(d3744dfd) SHA1(462dbdbdda75c498680889355cc4897d67ceddec) )
	ROM_LOAD16_WORD_SWAP( "c54a.p2", 0x080000, 0x80000, CRC(f32d5b4f) SHA1(de001ff00c4bc7797f4dd4c2d0b59b9b0cbb6d1f) )
	ROM_LOAD16_WORD_SWAP( "c54a.p3", 0x100000, 0x80000, CRC(1016454f) SHA1(3b176afdf550d565a75a0f3e8238c79d4e6863ac) )
	ROM_LOAD16_WORD_SWAP( "c54j.p4", 0x180000, 0x80000, CRC(94b420cd) SHA1(4cc43d3f7fed224443e26df5b0076bd24e6cd04b) )
	ROM_LOAD16_WORD_SWAP( "c54.p5",   0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6",   0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ringdesta.key", 0x00, 0x14, CRC(905c9065) SHA1(88ccc95cc2184b7e37e09b19ff88ef056a1561af) )
ROM_END

ROM_START( ringdesth )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54h.p1", 0x000000, 0x80000, CRC(2e316584) SHA1(7a7c2ec21bd46d784c2971e1dbb8d8312362371a) )
	ROM_LOAD16_WORD_SWAP( "c54h.p2", 0x080000, 0x80000, CRC(9950a23a) SHA1(5afd12ae7f3e7283ba42f844f2ee5a96875dc74e) )
	ROM_LOAD16_WORD_SWAP( "c54h.p3", 0x100000, 0x80000, CRC(41e0b3fc) SHA1(67fea6e194650c2a45cc0408ab1b0c2743ec6efa) )
	ROM_LOAD16_WORD_SWAP( "c54.p4", 0x180000, 0x80000, CRC(89c80007) SHA1(4c85aa5b224fdbb64f719a7b8b5b2e7413107c70) )
	ROM_LOAD16_WORD_SWAP( "c54.p5", 0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6", 0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ringdesth.key", 0x00, 0x14, CRC(ffb8d049) SHA1(c6d111412c3960b24a1be5c49fe4ec4d17324e06) )
ROM_END

ROM_START( ringdestb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54b.p1", 0x000000, 0x80000, CRC(234eebcc) SHA1(61ae407d0b2fb6e685231849f4db7ddf249466d1) )
	ROM_LOAD16_WORD_SWAP( "c54h.p2", 0x080000, 0x80000, CRC(9950a23a) SHA1(5afd12ae7f3e7283ba42f844f2ee5a96875dc74e) )
	ROM_LOAD16_WORD_SWAP( "c54h.p3", 0x100000, 0x80000, CRC(41e0b3fc) SHA1(67fea6e194650c2a45cc0408ab1b0c2743ec6efa) )
	ROM_LOAD16_WORD_SWAP( "c54.p4", 0x180000, 0x80000, CRC(89c80007) SHA1(4c85aa5b224fdbb64f719a7b8b5b2e7413107c70) )
	ROM_LOAD16_WORD_SWAP( "c54.p5", 0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6", 0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ringdesth.key", 0x00, 0x14, CRC(ffb8d049) SHA1(c6d111412c3960b24a1be5c49fe4ec4d17324e06) ) /* Brazilian set, but still uses the Hispanic key */
ROM_END

ROM_START( mmancp2u )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c60u.p1", 0x000000, 0x80000, CRC(c39f037f) SHA1(eefc5aa0fde322c6f895a37399424060c702a459) )
	ROM_LOAD16_WORD_SWAP( "c60u.p2", 0x080000, 0x80000, CRC(cd6f5e99) SHA1(46d5298bdf7dd3ccfe5d491c61f3c2e2da011e3b) )
	ROM_LOAD16_WORD_SWAP( "c35.p3",  0x100000, 0x80000, CRC(4376ea95) SHA1(7370ceffca513aa9f68a74f6869d561476589200) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c60.c01",   0x800000, 0x080000, CRC(774c6e04) SHA1(6bd14960218e31d5a043b20a1b9d2d69cace761e) )
	ROM_LOAD64_WORD( "c60.c02",   0x800002, 0x080000, CRC(acad7c62) SHA1(eec10990339c9fd8fdae896a5fd98d5bf0220ed1) )
	ROM_LOAD64_WORD( "c60.c03",   0x800004, 0x080000, CRC(6af30499) SHA1(a97bf2f382b6edc1e920e18d8ad5ca18131a2f21) )
	ROM_LOAD64_WORD( "c60.c04",   0x800006, 0x080000, CRC(7a5a5166) SHA1(2160015e2e43e2c024a3af56da961ac02ffc74e9) )
	ROM_LOAD64_WORD( "c60.c05",   0xa00000, 0x080000, CRC(004ec725) SHA1(b1d3bcf920b0a1d0f4b59c77f5962e8162bbef65) )
	ROM_LOAD64_WORD( "c60.c06",   0xa00002, 0x080000, CRC(65c0464e) SHA1(4a035f4d28fc8aa72bfe0a06392b93b0abfba458) )
	ROM_LOAD64_WORD( "c60.c07",   0xa00004, 0x080000, CRC(fb3097cc) SHA1(7bff2372809cdfa4dcd8537a448b177c0e86a94e) )
	ROM_LOAD64_WORD( "c60.c08",   0xa00006, 0x080000, CRC(2e16557a) SHA1(f4a916c1524a8de23fc6afabc8c724a89530c631) )
	ROM_LOAD64_WORD( "c60.c09",   0xc00000, 0x080000, CRC(70a73f99) SHA1(6dd126b4e64e34d5911ed5877b1b94b503404249) )
	ROM_LOAD64_WORD( "c60.c10",   0xc00002, 0x080000, CRC(ecedad3d) SHA1(dea1377f086ea3a45ced983e258beb6607b295c9) )
	ROM_LOAD64_WORD( "c60.c11",   0xc00004, 0x080000, CRC(3d6186d8) SHA1(70f1ff678dd1ec533360458a564953fc5634cbc5) )
	ROM_LOAD64_WORD( "c60.c12",   0xc00006, 0x080000, CRC(8c7700f1) SHA1(84846af535e9e333e911486d545988568554b67b) )
	ROM_LOAD64_WORD( "c60.c13",   0xe00000, 0x080000, CRC(89a889ad) SHA1(1ffe112051a3afc94df1326f17ef58dc2fc531aa) )
	ROM_LOAD64_WORD( "c60.c14",   0xe00002, 0x080000, CRC(1300eb7b) SHA1(db4f53b1cf521df99d073dcf0bfafe7b113e95d5) )
	ROM_LOAD64_WORD( "c60.c15",   0xe00004, 0x080000, CRC(6d974ebd) SHA1(41de66481a64fa8a6471c512fc5dcaf96d95ee69) )
	ROM_LOAD64_WORD( "c60.c16",   0xe00006, 0x080000, CRC(7da4cd24) SHA1(dd44377ff9f83f15cac032c4e7ef2071adcfa196) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c60.m1",   0x00000, 0x08000, CRC(d60cf8a3) SHA1(dccd84b93e62489c703011422d0fe84444c7f7db) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c60.q1",   0x000000, 0x80000, CRC(b6d07080) SHA1(b8f07553c01b1f67b0696110cd4e35e4cf4fa158) )
	ROM_LOAD16_WORD_SWAP( "c60.q2",   0x080000, 0x80000, CRC(dfddc493) SHA1(56b5129f24d05d2c85a767b0a632bf260f5425b2) )
	ROM_LOAD16_WORD_SWAP( "c60.q3",   0x100000, 0x80000, CRC(6062ae3a) SHA1(28a4d59bce0c341c240ee8cc92f85850ea8ffb10) )
	ROM_LOAD16_WORD_SWAP( "c60.q4",   0x180000, 0x80000, CRC(08c6f3bf) SHA1(6b8175748ff25b90572f914e4565935e27aa09a5) )
	ROM_LOAD16_WORD_SWAP( "c60.q5",   0x200000, 0x80000, CRC(f97dfccc) SHA1(752a3855d78e55dc31291e14d0223104691784f4) )
	ROM_LOAD16_WORD_SWAP( "c60.q6",   0x280000, 0x80000, CRC(ade475bc) SHA1(02ae6bc21d5e41e05595845f264c9ad040d70b37) )
	ROM_LOAD16_WORD_SWAP( "c60.q7",   0x300000, 0x80000, CRC(075effb3) SHA1(4be02d966d933c0d92908c5e05842c6b33c9703b) )
	ROM_LOAD16_WORD_SWAP( "c60.q8",   0x380000, 0x80000, CRC(f6c1f87b) SHA1(d6cd8d2bac96404b6983d738093b7177d478f12e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmancp2u.key", 0x00, 0x14, CRC(17ca6659) SHA1(f1f01efafd16019a150dce2b80bd0349b4015d76) )
ROM_END

ROM_START( mmancp2ur1 )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c60ur1.p1", 0x000000, 0x80000, CRC(c6b75320) SHA1(652aa5cf916fa50ce5e4023cebc234fbf404cd01) )
	ROM_LOAD16_WORD_SWAP( "c60ur1.p2", 0x080000, 0x80000, CRC(47880111) SHA1(893da977b3b1153646a524b106407c43ef8209c8) )
	ROM_LOAD16_WORD_SWAP( "c35.p3", 0x100000, 0x80000, CRC(4376ea95) SHA1(7370ceffca513aa9f68a74f6869d561476589200) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c60.c01",   0x800000, 0x080000, CRC(774c6e04) SHA1(6bd14960218e31d5a043b20a1b9d2d69cace761e) )
	ROM_LOAD64_WORD( "c60.c02",   0x800002, 0x080000, CRC(acad7c62) SHA1(eec10990339c9fd8fdae896a5fd98d5bf0220ed1) )
	ROM_LOAD64_WORD( "c60.c03",   0x800004, 0x080000, CRC(6af30499) SHA1(a97bf2f382b6edc1e920e18d8ad5ca18131a2f21) )
	ROM_LOAD64_WORD( "c60.c04",   0x800006, 0x080000, CRC(7a5a5166) SHA1(2160015e2e43e2c024a3af56da961ac02ffc74e9) )
	ROM_LOAD64_WORD( "c60.c05",   0xa00000, 0x080000, CRC(004ec725) SHA1(b1d3bcf920b0a1d0f4b59c77f5962e8162bbef65) )
	ROM_LOAD64_WORD( "c60.c06",   0xa00002, 0x080000, CRC(65c0464e) SHA1(4a035f4d28fc8aa72bfe0a06392b93b0abfba458) )
	ROM_LOAD64_WORD( "c60.c07",   0xa00004, 0x080000, CRC(fb3097cc) SHA1(7bff2372809cdfa4dcd8537a448b177c0e86a94e) )
	ROM_LOAD64_WORD( "c60.c08",   0xa00006, 0x080000, CRC(2e16557a) SHA1(f4a916c1524a8de23fc6afabc8c724a89530c631) )
	ROM_LOAD64_WORD( "c60.c09",   0xc00000, 0x080000, CRC(70a73f99) SHA1(6dd126b4e64e34d5911ed5877b1b94b503404249) )
	ROM_LOAD64_WORD( "c60.c10",   0xc00002, 0x080000, CRC(ecedad3d) SHA1(dea1377f086ea3a45ced983e258beb6607b295c9) )
	ROM_LOAD64_WORD( "c60.c11",   0xc00004, 0x080000, CRC(3d6186d8) SHA1(70f1ff678dd1ec533360458a564953fc5634cbc5) )
	ROM_LOAD64_WORD( "c60.c12",   0xc00006, 0x080000, CRC(8c7700f1) SHA1(84846af535e9e333e911486d545988568554b67b) )
	ROM_LOAD64_WORD( "c60.c13",   0xe00000, 0x080000, CRC(89a889ad) SHA1(1ffe112051a3afc94df1326f17ef58dc2fc531aa) )
	ROM_LOAD64_WORD( "c60.c14",   0xe00002, 0x080000, CRC(1300eb7b) SHA1(db4f53b1cf521df99d073dcf0bfafe7b113e95d5) )
	ROM_LOAD64_WORD( "c60.c15",   0xe00004, 0x080000, CRC(6d974ebd) SHA1(41de66481a64fa8a6471c512fc5dcaf96d95ee69) )
	ROM_LOAD64_WORD( "c60.c16",   0xe00006, 0x080000, CRC(7da4cd24) SHA1(dd44377ff9f83f15cac032c4e7ef2071adcfa196) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c60.m1",   0x00000, 0x08000, CRC(d60cf8a3) SHA1(dccd84b93e62489c703011422d0fe84444c7f7db) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c60.q1",   0x000000, 0x80000, CRC(b6d07080) SHA1(b8f07553c01b1f67b0696110cd4e35e4cf4fa158) )
	ROM_LOAD16_WORD_SWAP( "c60.q2",   0x080000, 0x80000, CRC(dfddc493) SHA1(56b5129f24d05d2c85a767b0a632bf260f5425b2) )
	ROM_LOAD16_WORD_SWAP( "c60.q3",   0x100000, 0x80000, CRC(6062ae3a) SHA1(28a4d59bce0c341c240ee8cc92f85850ea8ffb10) )
	ROM_LOAD16_WORD_SWAP( "c60.q4",   0x180000, 0x80000, CRC(08c6f3bf) SHA1(6b8175748ff25b90572f914e4565935e27aa09a5) )
	ROM_LOAD16_WORD_SWAP( "c60.q5",   0x200000, 0x80000, CRC(f97dfccc) SHA1(752a3855d78e55dc31291e14d0223104691784f4) )
	ROM_LOAD16_WORD_SWAP( "c60.q6",   0x280000, 0x80000, CRC(ade475bc) SHA1(02ae6bc21d5e41e05595845f264c9ad040d70b37) )
	ROM_LOAD16_WORD_SWAP( "c60.q7",   0x300000, 0x80000, CRC(075effb3) SHA1(4be02d966d933c0d92908c5e05842c6b33c9703b) )
	ROM_LOAD16_WORD_SWAP( "c60.q8",   0x380000, 0x80000, CRC(f6c1f87b) SHA1(d6cd8d2bac96404b6983d738093b7177d478f12e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmancp2u.key", 0x00, 0x14, CRC(17ca6659) SHA1(f1f01efafd16019a150dce2b80bd0349b4015d76) )
ROM_END

ROM_START( mmancp2ur2 )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c60ur2.p1", 0x000000, 0x80000, CRC(3cfdb1bb) SHA1(4dfef21dffc01b5f5bd7d8cc1eb1ba009fa846e8) )
	ROM_LOAD16_WORD_SWAP( "c60ur2.p2", 0x080000, 0x80000, CRC(169bf5e8) SHA1(8a967fa36474aea229a247b70e03d229b7bc2c17) )
	ROM_LOAD16_WORD_SWAP( "c35.p3", 0x100000, 0x80000, CRC(4376ea95) SHA1(7370ceffca513aa9f68a74f6869d561476589200) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c60.c01",   0x800000, 0x080000, CRC(774c6e04) SHA1(6bd14960218e31d5a043b20a1b9d2d69cace761e) )
	ROM_LOAD64_WORD( "c60.c02",   0x800002, 0x080000, CRC(acad7c62) SHA1(eec10990339c9fd8fdae896a5fd98d5bf0220ed1) )
	ROM_LOAD64_WORD( "c60.c03",   0x800004, 0x080000, CRC(6af30499) SHA1(a97bf2f382b6edc1e920e18d8ad5ca18131a2f21) )
	ROM_LOAD64_WORD( "c60.c04",   0x800006, 0x080000, CRC(7a5a5166) SHA1(2160015e2e43e2c024a3af56da961ac02ffc74e9) )
	ROM_LOAD64_WORD( "c60.c05",   0xa00000, 0x080000, CRC(004ec725) SHA1(b1d3bcf920b0a1d0f4b59c77f5962e8162bbef65) )
	ROM_LOAD64_WORD( "c60.c06",   0xa00002, 0x080000, CRC(65c0464e) SHA1(4a035f4d28fc8aa72bfe0a06392b93b0abfba458) )
	ROM_LOAD64_WORD( "c60.c07",   0xa00004, 0x080000, CRC(fb3097cc) SHA1(7bff2372809cdfa4dcd8537a448b177c0e86a94e) )
	ROM_LOAD64_WORD( "c60.c08",   0xa00006, 0x080000, CRC(2e16557a) SHA1(f4a916c1524a8de23fc6afabc8c724a89530c631) )
	ROM_LOAD64_WORD( "c60.c09",   0xc00000, 0x080000, CRC(70a73f99) SHA1(6dd126b4e64e34d5911ed5877b1b94b503404249) )
	ROM_LOAD64_WORD( "c60.c10",   0xc00002, 0x080000, CRC(ecedad3d) SHA1(dea1377f086ea3a45ced983e258beb6607b295c9) )
	ROM_LOAD64_WORD( "c60.c11",   0xc00004, 0x080000, CRC(3d6186d8) SHA1(70f1ff678dd1ec533360458a564953fc5634cbc5) )
	ROM_LOAD64_WORD( "c60.c12",   0xc00006, 0x080000, CRC(8c7700f1) SHA1(84846af535e9e333e911486d545988568554b67b) )
	ROM_LOAD64_WORD( "c60.c13",   0xe00000, 0x080000, CRC(89a889ad) SHA1(1ffe112051a3afc94df1326f17ef58dc2fc531aa) )
	ROM_LOAD64_WORD( "c60.c14",   0xe00002, 0x080000, CRC(1300eb7b) SHA1(db4f53b1cf521df99d073dcf0bfafe7b113e95d5) )
	ROM_LOAD64_WORD( "c60.c15",   0xe00004, 0x080000, CRC(6d974ebd) SHA1(41de66481a64fa8a6471c512fc5dcaf96d95ee69) )
	ROM_LOAD64_WORD( "c60.c16",   0xe00006, 0x080000, CRC(7da4cd24) SHA1(dd44377ff9f83f15cac032c4e7ef2071adcfa196) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c60.m1",   0x00000, 0x08000, CRC(d60cf8a3) SHA1(dccd84b93e62489c703011422d0fe84444c7f7db) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c60.q1",   0x000000, 0x80000, CRC(b6d07080) SHA1(b8f07553c01b1f67b0696110cd4e35e4cf4fa158) )
	ROM_LOAD16_WORD_SWAP( "c60.q2",   0x080000, 0x80000, CRC(dfddc493) SHA1(56b5129f24d05d2c85a767b0a632bf260f5425b2) )
	ROM_LOAD16_WORD_SWAP( "c60.q3",   0x100000, 0x80000, CRC(6062ae3a) SHA1(28a4d59bce0c341c240ee8cc92f85850ea8ffb10) )
	ROM_LOAD16_WORD_SWAP( "c60.q4",   0x180000, 0x80000, CRC(08c6f3bf) SHA1(6b8175748ff25b90572f914e4565935e27aa09a5) )
	ROM_LOAD16_WORD_SWAP( "c60.q5",   0x200000, 0x80000, CRC(f97dfccc) SHA1(752a3855d78e55dc31291e14d0223104691784f4) )
	ROM_LOAD16_WORD_SWAP( "c60.q6",   0x280000, 0x80000, CRC(ade475bc) SHA1(02ae6bc21d5e41e05595845f264c9ad040d70b37) )
	ROM_LOAD16_WORD_SWAP( "c60.q7",   0x300000, 0x80000, CRC(075effb3) SHA1(4be02d966d933c0d92908c5e05842c6b33c9703b) )
	ROM_LOAD16_WORD_SWAP( "c60.q8",   0x380000, 0x80000, CRC(f6c1f87b) SHA1(d6cd8d2bac96404b6983d738093b7177d478f12e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmancp2u.key", 0x00, 0x14, CRC(17ca6659) SHA1(f1f01efafd16019a150dce2b80bd0349b4015d76) )
ROM_END

ROM_START( rmancp2j )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c60j.p1", 0x000000, 0x80000, CRC(30559f60) SHA1(7a7c7c00613e379317383d68cac47dfbbb2200c9) )
	ROM_LOAD16_WORD_SWAP( "c60j.p2", 0x080000, 0x80000, CRC(5efc9366) SHA1(33420ac6ccf3c4982ce7644c574414574f706bd3) )
	ROM_LOAD16_WORD_SWAP( "c60j.p3", 0x100000, 0x80000, CRC(517ccde2) SHA1(492256c192f0c4814efa1ee1dd390453dd2e5865) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c60.c01",   0x800000, 0x080000, CRC(774c6e04) SHA1(6bd14960218e31d5a043b20a1b9d2d69cace761e) )
	ROM_LOAD64_WORD( "c60.c02",   0x800002, 0x080000, CRC(acad7c62) SHA1(eec10990339c9fd8fdae896a5fd98d5bf0220ed1) )
	ROM_LOAD64_WORD( "c60.c03",   0x800004, 0x080000, CRC(6af30499) SHA1(a97bf2f382b6edc1e920e18d8ad5ca18131a2f21) )
	ROM_LOAD64_WORD( "c60.c04",   0x800006, 0x080000, CRC(7a5a5166) SHA1(2160015e2e43e2c024a3af56da961ac02ffc74e9) )
	ROM_LOAD64_WORD( "c60.c05",   0xa00000, 0x080000, CRC(004ec725) SHA1(b1d3bcf920b0a1d0f4b59c77f5962e8162bbef65) )
	ROM_LOAD64_WORD( "c60.c06",   0xa00002, 0x080000, CRC(65c0464e) SHA1(4a035f4d28fc8aa72bfe0a06392b93b0abfba458) )
	ROM_LOAD64_WORD( "c60.c07",   0xa00004, 0x080000, CRC(fb3097cc) SHA1(7bff2372809cdfa4dcd8537a448b177c0e86a94e) )
	ROM_LOAD64_WORD( "c60.c08",   0xa00006, 0x080000, CRC(2e16557a) SHA1(f4a916c1524a8de23fc6afabc8c724a89530c631) )
	ROM_LOAD64_WORD( "c60.c09",   0xc00000, 0x080000, CRC(70a73f99) SHA1(6dd126b4e64e34d5911ed5877b1b94b503404249) )
	ROM_LOAD64_WORD( "c60.c10",   0xc00002, 0x080000, CRC(ecedad3d) SHA1(dea1377f086ea3a45ced983e258beb6607b295c9) )
	ROM_LOAD64_WORD( "c60.c11",   0xc00004, 0x080000, CRC(3d6186d8) SHA1(70f1ff678dd1ec533360458a564953fc5634cbc5) )
	ROM_LOAD64_WORD( "c60.c12",   0xc00006, 0x080000, CRC(8c7700f1) SHA1(84846af535e9e333e911486d545988568554b67b) )
	ROM_LOAD64_WORD( "c60.c13",   0xe00000, 0x080000, CRC(89a889ad) SHA1(1ffe112051a3afc94df1326f17ef58dc2fc531aa) )
	ROM_LOAD64_WORD( "c60.c14",   0xe00002, 0x080000, CRC(1300eb7b) SHA1(db4f53b1cf521df99d073dcf0bfafe7b113e95d5) )
	ROM_LOAD64_WORD( "c60.c15",   0xe00004, 0x080000, CRC(6d974ebd) SHA1(41de66481a64fa8a6471c512fc5dcaf96d95ee69) )
	ROM_LOAD64_WORD( "c60.c16",   0xe00006, 0x080000, CRC(7da4cd24) SHA1(dd44377ff9f83f15cac032c4e7ef2071adcfa196) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c60.m1",   0x00000, 0x08000, CRC(d60cf8a3) SHA1(dccd84b93e62489c703011422d0fe84444c7f7db) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c60.q1",   0x000000, 0x80000, CRC(b6d07080) SHA1(b8f07553c01b1f67b0696110cd4e35e4cf4fa158) )
	ROM_LOAD16_WORD_SWAP( "c60.q2",   0x080000, 0x80000, CRC(dfddc493) SHA1(56b5129f24d05d2c85a767b0a632bf260f5425b2) )
	ROM_LOAD16_WORD_SWAP( "c60.q3",   0x100000, 0x80000, CRC(6062ae3a) SHA1(28a4d59bce0c341c240ee8cc92f85850ea8ffb10) )
	ROM_LOAD16_WORD_SWAP( "c60.q4",   0x180000, 0x80000, CRC(08c6f3bf) SHA1(6b8175748ff25b90572f914e4565935e27aa09a5) )
	ROM_LOAD16_WORD_SWAP( "c60.q5",   0x200000, 0x80000, CRC(f97dfccc) SHA1(752a3855d78e55dc31291e14d0223104691784f4) )
	ROM_LOAD16_WORD_SWAP( "c60.q6",   0x280000, 0x80000, CRC(ade475bc) SHA1(02ae6bc21d5e41e05595845f264c9ad040d70b37) )
	ROM_LOAD16_WORD_SWAP( "c60.q7",   0x300000, 0x80000, CRC(075effb3) SHA1(4be02d966d933c0d92908c5e05842c6b33c9703b) )
	ROM_LOAD16_WORD_SWAP( "c60.q8",   0x380000, 0x80000, CRC(f6c1f87b) SHA1(d6cd8d2bac96404b6983d738093b7177d478f12e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "rmancp2j.key", 0x00, 0x14, CRC(17309a70) SHA1(7d72a07bd556874a4b9b5cf3727d3c547051bb7a) )
ROM_END

ROM_START( sfa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.p1", 0x000000, 0x80000, CRC(ebf2054d) SHA1(6e7b9e4202b86ab237ea5634c98b71b82d812ef2) )
	ROM_LOAD16_WORD_SWAP( "c59.p2", 0x080000, 0x80000, CRC(8b73b0e5) SHA1(5318761f615c21395366b5333e75eaaa73ef2073) )
	ROM_LOAD16_WORD_SWAP( "c59.p3", 0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4", 0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa.key",      0x00, 0x14, CRC(7c095631) SHA1(aa54369b385435991ae884801228427a0d07cfc7) )
ROM_END

ROM_START( sfar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59r1.p1", 0x000000, 0x80000, CRC(a1b69dd7) SHA1(b41440eba8f33eed955c987a04d99fca6c5c90e5) )
	ROM_LOAD16_WORD_SWAP( "c59r1.p2", 0x080000, 0x80000, CRC(bb90acd5) SHA1(a19795963b90f1152f44cae29e78dd2ce67a41d6) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa.key",      0x00, 0x14, CRC(7c095631) SHA1(aa54369b385435991ae884801228427a0d07cfc7) )
ROM_END

ROM_START( sfar2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59r2.p1", 0x000000, 0x80000, CRC(2bf5708e) SHA1(6ce55082e0befef47f25b4be76c607d79ec0828c) )
	ROM_LOAD16_WORD_SWAP( "c59r2.p2",  0x080000, 0x80000, CRC(5f99e9a5) SHA1(e9f286315d17096adc08e6b4e6ff7c5351f5bef3) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa.key",      0x00, 0x14, CRC(7c095631) SHA1(aa54369b385435991ae884801228427a0d07cfc7) )
ROM_END

ROM_START( sfar3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59r3.p1", 0x000000, 0x80000, CRC(fdbcd434) SHA1(1d5f9b821d9e0d45be61896969500b877a112fad) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p2",   0x080000, 0x80000, CRC(0c436d30) SHA1(84229896c99bb2a4fbbab33644f779c9f86704fb) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p3",   0x100000, 0x80000, CRC(1f363612) SHA1(87203b5db2d3887762da431d6fc2f2b76d4feedb) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa.key",      0x00, 0x14, CRC(7c095631) SHA1(aa54369b385435991ae884801228427a0d07cfc7) )
ROM_END

ROM_START( sfau )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59u.p1", 0x000000, 0x80000, CRC(49fc7db9) SHA1(2a13d987fade88e0372f418cf451f34de67372d5) )
	ROM_LOAD16_WORD_SWAP( "c59r2.p2",  0x080000, 0x80000, CRC(5f99e9a5) SHA1(e9f286315d17096adc08e6b4e6ff7c5351f5bef3) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfau.key",     0x00, 0x14, CRC(1dd0998d) SHA1(6fe6fc5a7984de068b4a31ae64cd06b57aac682e) )
ROM_END

ROM_START( sfza )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59ja.p1", 0x000000, 0x80000, CRC(ca91bed9) SHA1(af238a4a1b87fa09ae7da7e0c41964c95dae6513) )
	ROM_LOAD16_WORD_SWAP( "c59r2.p2",  0x080000, 0x80000, CRC(5f99e9a5) SHA1(e9f286315d17096adc08e6b4e6ff7c5351f5bef3) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfza.key",     0x00, 0x14, CRC(2aa6ac63) SHA1(0dc4732d7f1dc5742db646d5dc22fab2dc1620dc) )
ROM_END

ROM_START( sfzar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jar1.p1", 0x000000, 0x80000, CRC(f38d8c8d) SHA1(06770ea1944da5867483e647a306591be454d25e) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p2",   0x080000, 0x80000, CRC(0c436d30) SHA1(84229896c99bb2a4fbbab33644f779c9f86704fb) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p3",   0x100000, 0x80000, CRC(1f363612) SHA1(87203b5db2d3887762da431d6fc2f2b76d4feedb) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfza.key",     0x00, 0x14, CRC(2aa6ac63) SHA1(0dc4732d7f1dc5742db646d5dc22fab2dc1620dc) )
ROM_END

ROM_START( sfzj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59j.p1", 0x000000, 0x80000, CRC(f5444120) SHA1(22158894971754ad83b8eeb8bdfb9874794b98c0) )
	ROM_LOAD16_WORD_SWAP( "c59.p2",  0x080000, 0x80000, CRC(8b73b0e5) SHA1(5318761f615c21395366b5333e75eaaa73ef2073) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzj.key",     0x00, 0x14, CRC(355d85b8) SHA1(5da2c407852ce7b28153f3f94de8328802ff2f2d) )
ROM_END

ROM_START( sfzjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jr1.p1", 0x000000, 0x80000, CRC(844220c2) SHA1(ff295207e0f9679285d805aa494537ae7daf1634) )
	ROM_LOAD16_WORD_SWAP( "c59r2.p2",  0x080000, 0x80000, CRC(5f99e9a5) SHA1(e9f286315d17096adc08e6b4e6ff7c5351f5bef3) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzj.key",     0x00, 0x14, CRC(355d85b8) SHA1(5da2c407852ce7b28153f3f94de8328802ff2f2d) )
ROM_END

ROM_START( sfzjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jr2.p1", 0x000000, 0x80000, CRC(3cfce93c) SHA1(5f64e9707cb3d911f44e041d980e4b2250f49d75) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p2",   0x080000, 0x80000, CRC(0c436d30) SHA1(84229896c99bb2a4fbbab33644f779c9f86704fb) )
	ROM_LOAD16_WORD_SWAP( "c59r3.p3",   0x100000, 0x80000, CRC(1f363612) SHA1(87203b5db2d3887762da431d6fc2f2b76d4feedb) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzj.key",     0x00, 0x14, CRC(355d85b8) SHA1(5da2c407852ce7b28153f3f94de8328802ff2f2d) )
ROM_END

ROM_START( sfzh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jh.p1", 0x000000, 0x80000, CRC(6e08cbe0) SHA1(c9d0e709004677b51efe0b62e0ae9b681dde1744) )
	ROM_LOAD16_WORD_SWAP( "c59r1.p2",  0x080000, 0x80000, CRC(bb90acd5) SHA1(a19795963b90f1152f44cae29e78dd2ce67a41d6) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzh.key",     0x00, 0x14, CRC(4763446f) SHA1(f1397e31f85fc35f8d4aeee638c414595ca09ce1) )
ROM_END

ROM_START( sfzhr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jhr1.p1", 0x000000, 0x80000, CRC(bce635aa) SHA1(323da2de6c3ff6fd8c2c66ce6bd1d287873db9b1) )
	ROM_LOAD16_WORD_SWAP( "c59r2.p2",  0x080000, 0x80000, CRC(5f99e9a5) SHA1(e9f286315d17096adc08e6b4e6ff7c5351f5bef3) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",  0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",   0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzh.key",     0x00, 0x14, CRC(4763446f) SHA1(f1397e31f85fc35f8d4aeee638c414595ca09ce1) )
ROM_END

ROM_START( sfzb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jb.p1", 0x000000, 0x80000, CRC(348862d4) SHA1(b48c7df17f8b681fc726931dbf81f5aeb762a5b3) )
	ROM_LOAD16_WORD_SWAP( "c59jb.p2", 0x080000, 0x80000, CRC(8d9b2480) SHA1(405305c1572908d00eab735f28676fbbadb4fac6) )
	ROM_LOAD16_WORD_SWAP( "c59.p3", 0x100000,  0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",  0x180000,  0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzb.key",     0x00, 0x14, CRC(b0570359) SHA1(1d35b79cc1c48c866d381f9a0a4b40626ec617c5) )
ROM_END

ROM_START( sfzbr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59jbr1.p1", 0x000000, 0x80000, CRC(ecba89a3) SHA1(5a3d7a978b6dc1f334eddf8e065318d60501f223) )
	ROM_LOAD16_WORD_SWAP( "c59.p2",  0x080000, 0x80000, CRC(8b73b0e5) SHA1(5318761f615c21395366b5333e75eaaa73ef2073) )
	ROM_LOAD16_WORD_SWAP( "c59.p3", 0x100000,  0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",  0x180000,  0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfzb.key",     0x00, 0x14, CRC(b0570359) SHA1(1d35b79cc1c48c866d381f9a0a4b40626ec617c5) )
ROM_END

ROM_START( sfa2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.p1",  0x000000, 0x80000, CRC(1061e6bb) SHA1(f9b05e5cbcb1dc874de6874b01058defd6e4c407) )
	ROM_LOAD16_WORD_SWAP( "c64.p2",  0x080000, 0x80000, CRC(22d17b26) SHA1(2d7a9cedae1bb2b7cfb80d2bfcf24cb7738df0bf) )
	ROM_LOAD16_WORD_SWAP( "c64.p3",  0x100000, 0x80000, CRC(4b442a7c) SHA1(a0d7d229cff8efb2a253ff06270258b0b4d2761e) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",  0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64.p5",  0x200000, 0x80000, CRC(8e184246) SHA1(c51f6480cfa1dcec6c4713fd38c7a27338c3fa65) )
	ROM_LOAD16_WORD_SWAP( "c64.p6",  0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa2.key",     0x00, 0x14, CRC(1578dcb0) SHA1(a8bc83b341f9ea2bcc18861419bd574d159c6fcc) )
ROM_END

ROM_START( sfa2u )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64u.p1",  0x000000, 0x80000, CRC(d03e504f) SHA1(14173d650ca4ca7f53e6dab76bac6e2724ff3d9d) )
	ROM_LOAD16_WORD_SWAP( "c64u.p2",  0x080000, 0x80000, CRC(fae0e9c3) SHA1(d84cfbf16079f9987df683911b83896b80b52b1a) )
	ROM_LOAD16_WORD_SWAP( "c64u.p3",  0x100000, 0x80000, CRC(d02dd758) SHA1(581bcd451b1d081b9b73d4a1b485da9f29756613) )
	ROM_LOAD16_WORD_SWAP( "c64u.p4",  0x180000, 0x80000, CRC(c5c8eb63) SHA1(4ea033834c7b260877335296f88c0db484dea289) )
	ROM_LOAD16_WORD_SWAP( "c64u.p5",  0x200000, 0x80000, CRC(5de01cc5) SHA1(b19bfe970b217c96e782860fc3ae3fcb976ed30d) )
	ROM_LOAD16_WORD_SWAP( "c64u.p6",  0x280000, 0x80000, CRC(bea11d56) SHA1(a1d475066d36de7cc5d931671ccdcd89737bc7ee) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa2u.key",    0x00, 0x14, CRC(4a8d91ef) SHA1(e22567ab83b35c39c6545742bc85ca98ae97f288) )
ROM_END

ROM_START( sfa2ur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64ur1.p1", 0x000000, 0x80000, CRC(84a09006) SHA1(334c33f9eb324d71443cc9c44e94f5a72451fa3f) )
	ROM_LOAD16_WORD_SWAP( "c64ur1.p2", 0x080000, 0x80000, CRC(ac46e5ed) SHA1(a01b57daba4c255d5f07465c553bcbfe51d9ab0d) )
	ROM_LOAD16_WORD_SWAP( "c64ur1.p3", 0x100000, 0x80000, CRC(6c0c79d3) SHA1(ae2a4e2903beec1f10fff6edac1a2385d6ac1c38) )
	ROM_LOAD16_WORD_SWAP( "c64u.p4", 0x180000, 0x80000, CRC(c5c8eb63) SHA1(4ea033834c7b260877335296f88c0db484dea289) )
	ROM_LOAD16_WORD_SWAP( "c64u.p5", 0x200000, 0x80000, CRC(5de01cc5) SHA1(b19bfe970b217c96e782860fc3ae3fcb976ed30d) )
	ROM_LOAD16_WORD_SWAP( "c64u.p6", 0x280000, 0x80000, CRC(bea11d56) SHA1(a1d475066d36de7cc5d931671ccdcd89737bc7ee) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa2u.key",    0x00, 0x14, CRC(4a8d91ef) SHA1(e22567ab83b35c39c6545742bc85ca98ae97f288) )
ROM_END

ROM_START( sfz2j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64j.p1", 0x000000, 0x80000, CRC(3e1e2e85) SHA1(11447b08803d6bf7aeca7aa348b817bd6d448fe8) )
	ROM_LOAD16_WORD_SWAP( "c64j.p2", 0x080000, 0x80000, CRC(f53d6c45) SHA1(674c06b2775834887db3b7705941feea7367f500) )
	ROM_LOAD16_WORD_SWAP( "c64j.p3", 0x100000, 0x80000, CRC(dd224156) SHA1(85d29f2a288430d51c53b88130f255131e5dc601) )
	ROM_LOAD16_WORD_SWAP( "c64j.p4", 0x180000, 0x80000, CRC(a45a75a6) SHA1(e9cd4ad08ac0d058e9e1660acb07eb350a141fd6) )
	ROM_LOAD16_WORD_SWAP( "c64j.p5", 0x200000, 0x80000, CRC(6352f038) SHA1(720a9865ecd0b34315c59ee88d137b4afcdd91cb) )
	ROM_LOAD16_WORD_SWAP( "c64j.p6", 0x280000, 0x80000, CRC(92b66e01) SHA1(f09cb38aa49b22a9c98219fb2ad8a66b11fa5872) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2j.key",    0x00, 0x14, CRC(455bd098) SHA1(6a928a2d44f416ccdb0c09a8dd7466ac12f38110) )
ROM_END

ROM_START( sfz2jr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p1", 0x000000, 0x80000, CRC(97461e28) SHA1(8fbe4c9a59f51612f86adb8ef5057e43be0348bf) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p2", 0x080000, 0x80000, CRC(ae4851a9) SHA1(4771bc22fe1b376b753a68506c012c52bd4b886d) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p3", 0x100000, 0x80000, CRC(98e8e992) SHA1(41745b63e6b3888081d189b8315ed3b7526b3d20) )
	ROM_LOAD16_WORD_SWAP( "c64.p4", 0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p5", 0x200000, 0x80000, CRC(d910b2a2) SHA1(aa201660caa9cef993c147a1077c9e7767b34a78) )
	ROM_LOAD16_WORD_SWAP( "c64.p6", 0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2j.key",    0x00, 0x14, CRC(455bd098) SHA1(6a928a2d44f416ccdb0c09a8dd7466ac12f38110) )
ROM_END

ROM_START( sfz2a )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64ja.p1", 0x000000, 0x80000, CRC(30d2099f) SHA1(d4c7d8c2ad08cae228544bd692aedecd4fab829c) )
	ROM_LOAD16_WORD_SWAP( "c64ja.p2", 0x080000, 0x80000, CRC(1cc94db1) SHA1(518151f443ff5219b20c9fd59b7614920302aecd) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p3",  0x100000, 0x80000, CRC(98e8e992) SHA1(41745b63e6b3888081d189b8315ed3b7526b3d20) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",  0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64ja.p5", 0x200000, 0x80000, CRC(0aed2494) SHA1(7beb1a394f17cd78a27128292b626aae28754ca2) )
	ROM_LOAD16_WORD_SWAP( "c64.p6",  0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2a.key",    0x00, 0x14, CRC(777b7358) SHA1(afae8510347495835936fa04acab72afbff7a3dd) )
ROM_END

ROM_START( sfz2b )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jb.p1", 0x000000, 0x80000, CRC(1ac12812) SHA1(b948b939021ffe20437b19325fe94daa072c1271) )
	ROM_LOAD16_WORD_SWAP( "c64jb.p2", 0x080000, 0x80000, CRC(e4ffaf68) SHA1(e22bb4f92a965108570c2beee1fd533380838d90) )
	ROM_LOAD16_WORD_SWAP( "c64j.p3", 0x100000, 0x80000, CRC(dd224156) SHA1(85d29f2a288430d51c53b88130f255131e5dc601) )
	ROM_LOAD16_WORD_SWAP( "c64j.p4", 0x180000, 0x80000, CRC(a45a75a6) SHA1(e9cd4ad08ac0d058e9e1660acb07eb350a141fd6) )
	ROM_LOAD16_WORD_SWAP( "c64jb.p5", 0x200000, 0x80000, CRC(7d19d5ec) SHA1(ab88dfcb2029499578837b8f97fbf55412c8f756) )
	ROM_LOAD16_WORD_SWAP( "c64j.p6",  0x280000, 0x80000, CRC(92b66e01) SHA1(f09cb38aa49b22a9c98219fb2ad8a66b11fa5872) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2b.key",    0x00, 0x14, CRC(35b1df07) SHA1(acdac3c4d422a34529f3c7a151d1d3ab9404f999) )
ROM_END

ROM_START( sfz2br1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jbr1.p1", 0x000000, 0x80000, CRC(e6ce530b) SHA1(044c3f6f6c64d18f4f9ce96b67ff86b3c8bcd065) )
	ROM_LOAD16_WORD_SWAP( "c64jbr1.p2", 0x080000, 0x80000, CRC(1605a0cb) SHA1(5041c87dbb3ed41fe1cb3e9eade195bc2f7cba2a) )
	ROM_LOAD16_WORD_SWAP( "c64.p3",  0x100000, 0x80000, CRC(4b442a7c) SHA1(a0d7d229cff8efb2a253ff06270258b0b4d2761e) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",  0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64jbr1.p5", 0x200000, 0x80000, CRC(947e8ac6) SHA1(da82be7cba9cd557da3ee35be9194130a959d5cb) )
	ROM_LOAD16_WORD_SWAP( "c64j.p6", 0x280000, 0x80000, CRC(92b66e01) SHA1(f09cb38aa49b22a9c98219fb2ad8a66b11fa5872) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2b.key",    0x00, 0x14, CRC(35b1df07) SHA1(acdac3c4d422a34529f3c7a151d1d3ab9404f999) )
ROM_END

ROM_START( sfz2h )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jh.p1", 0x000000, 0x80000, CRC(bfeddf5b) SHA1(dd4a748ba8674725a399b78e721ff9c7adaaf890) )
	ROM_LOAD16_WORD_SWAP( "c64jh.p2", 0x080000, 0x80000, CRC(ea5009fb) SHA1(9186c702994f99488d52d4dbccb3823d2b9a6dd9) )
	ROM_LOAD16_WORD_SWAP( "c64.p3",  0x100000, 0x80000, CRC(4b442a7c) SHA1(a0d7d229cff8efb2a253ff06270258b0b4d2761e) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",  0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64jbr1.p5", 0x200000, 0x80000, CRC(947e8ac6) SHA1(da82be7cba9cd557da3ee35be9194130a959d5cb) )
	ROM_LOAD16_WORD_SWAP( "c64j.p6", 0x280000, 0x80000, CRC(92b66e01) SHA1(f09cb38aa49b22a9c98219fb2ad8a66b11fa5872) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2h.key",    0x00, 0x14, CRC(2719ea16) SHA1(f50ea318185133ca4aef296d70b471364372e060) )
ROM_END

ROM_START( sfz2n )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jn.p1", 0x000000, 0x80000, CRC(58924741) SHA1(b077867a6a601b5f87a644ebfe9b8d0c87d0471f) )
	ROM_LOAD16_WORD_SWAP( "c64jn.p2", 0x080000, 0x80000, CRC(592a17c5) SHA1(7262f9017834e932858e81dadfba9d9feb946530) )
	ROM_LOAD16_WORD_SWAP( "c64.p3",  0x100000, 0x80000, CRC(4b442a7c) SHA1(a0d7d229cff8efb2a253ff06270258b0b4d2761e) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",  0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64.p5",  0x200000, 0x80000, CRC(8e184246) SHA1(c51f6480cfa1dcec6c4713fd38c7a27338c3fa65) )
	ROM_LOAD16_WORD_SWAP( "c64.p6",  0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2n.key",    0x00, 0x14, CRC(d1cc49d5) SHA1(bcf9f8d1fb5fe74dd2568a4a8d8a00d5384ea0db) )
ROM_END

ROM_START( sfz2al )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67.p1", 0x000000, 0x80000, CRC(88e7023e) SHA1(34e74ec54c05d75e5cf207abb6e536fcca233b8b) )
	ROM_LOAD16_WORD_SWAP( "c67.p2", 0x080000, 0x80000, CRC(ae8ec36e) SHA1(b2f3de9e33169f6266aaabd5eae6c057ea10dcab) )
	ROM_LOAD16_WORD_SWAP( "c67.p3", 0x100000, 0x80000, CRC(f053a55e) SHA1(f98a8af5cd33a543a5596d59381f9adafed38854) )
	ROM_LOAD16_WORD_SWAP( "c67.p4", 0x180000, 0x80000, CRC(cfc0e7a8) SHA1(31ed58451c7a6ac88a8fccab369167694698f044) )
	ROM_LOAD16_WORD_SWAP( "c67.p5", 0x200000, 0x80000, CRC(5feb8b20) SHA1(13c79c9b72c3abf0a0b75d507d91ece71e460c06) )
	ROM_LOAD16_WORD_SWAP( "c67.p6", 0x280000, 0x80000, CRC(6eb6d412) SHA1(c858fec9c1dfea70dfcca629c1c24306f8ae6d81) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2al.key",   0x00, 0x14, CRC(2904963e) SHA1(f4fa44646746ab4c6f2e76eaba57a7aee32e2933) )
ROM_END

ROM_START( sfz2alr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67r1.p1", 0x000000, 0x80000, CRC(9f67c585) SHA1(ff2196e1bf142230972dec7b70ffd2b35c4600b0) )
	ROM_LOAD16_WORD_SWAP( "c67r1.p2", 0x080000, 0x80000, CRC(5a447a54) SHA1(4f5463209e753af2ab8b66ac34903ef409020b21) )
	ROM_LOAD16_WORD_SWAP( "c67.p3", 0x100000, 0x80000, CRC(f053a55e) SHA1(f98a8af5cd33a543a5596d59381f9adafed38854) )
	ROM_LOAD16_WORD_SWAP( "c67.p4", 0x180000, 0x80000, CRC(cfc0e7a8) SHA1(31ed58451c7a6ac88a8fccab369167694698f044) )
	ROM_LOAD16_WORD_SWAP( "c67.p5", 0x200000, 0x80000, CRC(5feb8b20) SHA1(13c79c9b72c3abf0a0b75d507d91ece71e460c06) )
	ROM_LOAD16_WORD_SWAP( "c67.p6", 0x280000, 0x80000, CRC(6eb6d412) SHA1(c858fec9c1dfea70dfcca629c1c24306f8ae6d81) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2al.key",   0x00, 0x14, CRC(2904963e) SHA1(f4fa44646746ab4c6f2e76eaba57a7aee32e2933) )
ROM_END

ROM_START( sfz2alj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67j.p1", 0x000000, 0x80000, CRC(a3802fe3) SHA1(c983a15ed675b22aebfe6ac55890b4e0b5eb8d48) )
	ROM_LOAD16_WORD_SWAP( "c67j.p2", 0x080000, 0x80000, CRC(e7ca87c7) SHA1(e44c930b27431dd2b983d93471a440d292e7a8bb) )
	ROM_LOAD16_WORD_SWAP( "c67j.p3", 0x100000, 0x80000, CRC(c88ebf88) SHA1(e37cf232fc70b9a3254dea99754e288232f04e25) )
	ROM_LOAD16_WORD_SWAP( "c67j.p4", 0x180000, 0x80000, CRC(35ed5b7a) SHA1(b03cb92f594eb35fa374445f74930e9040a2baff) )
	ROM_LOAD16_WORD_SWAP( "c67j.p5", 0x200000, 0x80000, CRC(975dcb3e) SHA1(a2ca8e5a768e49cce9e2137ec0dcba9337ed2ad5) )
	ROM_LOAD16_WORD_SWAP( "c67j.p6", 0x280000, 0x80000, CRC(dc73f2d7) SHA1(09fa10e7d1ff5f0dac87a6cf3d66730e3ab9ad25) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2alj.key",  0x00, 0x14, CRC(4c42320f) SHA1(0fabdab677416c5a8060526d997e6f80b26f9bb3) )
ROM_END

ROM_START( sfz2alh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67h.p1", 0x000000, 0x80000, CRC(06f93d1d) SHA1(495de176ba55b35270fc05f19edf17a0f249ff0e) )
	ROM_LOAD16_WORD_SWAP( "c67h.p2", 0x080000, 0x80000, CRC(e62ee914) SHA1(def4f27c1b64be5143234f1f402260adae66cdde) )
	ROM_LOAD16_WORD_SWAP( "c67h.p3", 0x100000, 0x80000, CRC(2b7f4b20) SHA1(5511263f5f6e532ee7fe1995f08f16651a1d45a1) )
	ROM_LOAD16_WORD_SWAP( "c67h.p4",  0x180000, 0x80000, CRC(0abda2fc) SHA1(830da40f6a9bb3bc866ee9c5cab1b0eb3c4dcb71) )
	ROM_LOAD16_WORD_SWAP( "c67h.p5",  0x200000, 0x80000, CRC(e9430762) SHA1(923aea8db5f9b59212ec6dbc35be0808ea015140) )
	ROM_LOAD16_WORD_SWAP( "c67h.p6",  0x280000, 0x80000, CRC(b65711a9) SHA1(3918f44e1bb189e2a115625b35f477eb91a65f04) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2alh.key",  0x00, 0x14, CRC(f320f655) SHA1(2bedd94afd600da2f2a5c74bc0b0c2d33a4c13cf) )
ROM_END

ROM_START( sfz2alb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67b.p1", 0x000000, 0x80000, CRC(cb436eca) SHA1(406bea614429f78c0150c2f5042abc8673a6722e) )
	ROM_LOAD16_WORD_SWAP( "c67b.p2", 0x080000, 0x80000, CRC(14534bea) SHA1(8fff2cd9221ef12de9364cc15429b6df6bacc48e) )
	ROM_LOAD16_WORD_SWAP( "c67b.p3", 0x100000, 0x80000, CRC(7fb10658) SHA1(f9eba0271d92d6d29156a7b4dd8b1cdb3dd8aa48) )
	ROM_LOAD16_WORD_SWAP( "c67h.p4",  0x180000, 0x80000, CRC(0abda2fc) SHA1(830da40f6a9bb3bc866ee9c5cab1b0eb3c4dcb71) )
	ROM_LOAD16_WORD_SWAP( "c67h.p5",  0x200000, 0x80000, CRC(e9430762) SHA1(923aea8db5f9b59212ec6dbc35be0808ea015140) )
	ROM_LOAD16_WORD_SWAP( "c67h.p6",  0x280000, 0x80000, CRC(b65711a9) SHA1(3918f44e1bb189e2a115625b35f477eb91a65f04) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz2alb.key",  0x00, 0x14, CRC(c8b3ac73) SHA1(42feb1c5c3fc67574deef5cd0d783a29d108be08) )
ROM_END

ROM_START( sfa3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78e.p1", 0x000000, 0x80000, CRC(9762b206) SHA1(fc4561ca990dd11ed2c5203540102078b721db2f) )
	ROM_LOAD16_WORD_SWAP( "c78e.p2", 0x080000, 0x80000, CRC(5ad3f721) SHA1(fec11cc5f63593f0181eb28ae85ad916686793eb) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",  0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",  0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",  0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",  0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",  0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",  0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3.key",     0x00, 0x14, CRC(54fa39c6) SHA1(a0d2f8e697a7a4201762cb02842581840b2a7f78) )
ROM_END

ROM_START( sfa3u )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78u.p1", 0x000000, 0x80000, CRC(e007da2e) SHA1(d190ac7ca2c27f11b9b4f96860b226bbea0ee403) )
	ROM_LOAD16_WORD_SWAP( "c78u.p2", 0x080000, 0x80000, CRC(5f78f0e7) SHA1(f4df30fd3515fe9f1125f470b96028052c61f57b) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",  0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",  0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",  0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",  0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",  0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",  0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3u.key",    0x00, 0x14, CRC(4a8f98c1) SHA1(11953de12ed8fbb6f60ad03b1ba5e22a429b7cfa) )
ROM_END


ROM_START( sfa3ur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78ur1.p1", 0x000000, 0x80000, CRC(b5984a19) SHA1(e225dd1d3a5d1b94adcfc5f720775e9ba321996e) )
	ROM_LOAD16_WORD_SWAP( "c78ur1.p2", 0x080000, 0x80000, CRC(7e8158ba) SHA1(a9984d7c9d02a9ebaf98cfd0dcbcf26e82e904de) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",  0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",  0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",  0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",  0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",  0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",  0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3u.key",    0x00, 0x14, CRC(4a8f98c1) SHA1(11953de12ed8fbb6f60ad03b1ba5e22a429b7cfa) )
ROM_END

ROM_START( sfa3us )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.p1", 0x000000, 0x80000, CRC(14319e29) SHA1(6de0c08f887d334d720d4c8ba7d5745f5282adfb) )
	ROM_LOAD16_WORD_SWAP( "c78.p2", 0x080000, 0x80000, CRC(65fbc272) SHA1(d320a3c0dae03ea7c32cd521b99afb0e2d62dfaf) )
	ROM_LOAD16_WORD_SWAP( "c78us.p3",  0x100000, 0x80000, CRC(e93c47d1) SHA1(18c388e55b852ad46b784e0cf3fd4c72e8bb6850) )
	ROM_LOAD16_WORD_SWAP( "c78us.p4",  0x180000, 0x80000, CRC(1bf09de3) SHA1(8bd7f5ae7dbaccc65af835f166134f1bb44e6aae) )
	ROM_LOAD16_WORD_SWAP( "c78us.p5",  0x200000, 0x80000, CRC(f6296d96) SHA1(ff9376544968c6783fd98b9a714f0c25174f027a) )
	ROM_LOAD16_WORD_SWAP( "c78us.p6",  0x280000, 0x80000, CRC(1f4008ff) SHA1(f3fe9d6560bf97e7a3a15c1c43f74ac9ef3d5fbb) )
	ROM_LOAD16_WORD_SWAP( "c78us.p7",  0x300000, 0x80000, CRC(822fc451) SHA1(49ec9e3f33d6023b59b350a79fe2299f6ac90251) )
	ROM_LOAD16_WORD_SWAP( "c78us.p8",  0x380000, 0x80000, CRC(92713468) SHA1(9855bb1dd54190e29c5935be4a6529ac0936a628))

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3u.key",    0x00, 0x14, CRC(4a8f98c1) SHA1(11953de12ed8fbb6f60ad03b1ba5e22a429b7cfa) )
ROM_END

ROM_START( sfz3j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78j.p1", 0x000000, 0x80000, CRC(cadf4a51) SHA1(a0511512f55c0befa4a905ceff8c6f5775cf40ba) )
	ROM_LOAD16_WORD_SWAP( "c78j.p2", 0x080000, 0x80000, CRC(fcb31228) SHA1(093f40083b5a4e4cae433d5856c48014063fe6ad) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",  0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",  0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",  0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",  0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",  0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",  0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz3j.key",    0x00, 0x14, CRC(d30cca8d) SHA1(b05869902d4d5968d5f79ed6165eb4b78e1ddcdd) )
ROM_END

ROM_START( sfz3jr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p1", 0x000000, 0x80000, CRC(6ee0beae) SHA1(243436fb64628f70cd130c7029d365ae97f3f42d) )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p2", 0x080000, 0x80000, CRC(a6e2978d) SHA1(27e350e78aa204670c0ee6c60baddee46a92a584) )
	ROM_LOAD16_WORD_SWAP( "c78r1.p3",  0x100000, 0x80000, CRC(05964b7d) SHA1(ac9fa2c69c712a01647f0572381d875b1eb90886) )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p4",  0x180000, 0x80000, CRC(78ce2179) SHA1(98a6f55bbdc45167fcc04cd6c3b7d71ffab31911) )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p5",  0x200000, 0x80000, CRC(398bf52f) SHA1(2c8880b65b83724b956294b903b5038091b543c5) )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p6",  0x280000, 0x80000, CRC(866d0588) SHA1(f2e9ca1bb606e4d2e3c9b62dd80074670a2e8e45) )
	ROM_LOAD16_WORD_SWAP( "c78jr1.p7",  0x300000, 0x80000, CRC(2180892c) SHA1(65a44c612b1c6dd527b306c262caa5040897ce7b) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",   0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz3j.key",    0x00, 0x14, CRC(d30cca8d) SHA1(b05869902d4d5968d5f79ed6165eb4b78e1ddcdd) )
ROM_END

ROM_START( sfz3jr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78jr2.p1", 0x000000, 0x80000, CRC(f7cb4b13) SHA1(5f86d23cf3725d9440200732405b437545ac8dd7) )
	ROM_LOAD16_WORD_SWAP( "c78jr2.p2", 0x080000, 0x80000, CRC(0846c29d) SHA1(f2e96b4f6e0187c382411276ff3a485cdc2df289) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",  0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",  0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",  0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",  0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",  0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",  0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz3j.key",    0x00, 0x14, CRC(d30cca8d) SHA1(b05869902d4d5968d5f79ed6165eb4b78e1ddcdd) )
ROM_END

ROM_START( sfz3a )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78a.p1", 0x000000, 0x80000, CRC(d7e140d6) SHA1(7eae4dc61432e1aaf73194377f787093379d53a9) )
	ROM_LOAD16_WORD_SWAP( "c78a.p2", 0x080000, 0x80000, CRC(e06869a2) SHA1(9442f7dda95d20c5eff549bfdd60b89eea5483a9) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",  0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",  0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",  0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",  0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",  0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",  0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz3a.key",    0x00, 0x14, CRC(09045d61) SHA1(0cfeb0b53973fff08b673a10b1f7e6e3b46aa214) )
ROM_END

ROM_START( sfz3ar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78ar1.p1", 0x000000, 0x80000, CRC(29c681fd) SHA1(5ee4c4e282789e4cdba5a317c7049e8c0d8b774b) )
	ROM_LOAD16_WORD_SWAP( "c78ar1.p2",  0x080000, 0x80000, CRC(9ddd1484) SHA1(d484b93d1653f522ee33285e58139167b3214902) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",   0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",   0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",   0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",   0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",   0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",   0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfz3a.key",    0x00, 0x14, CRC(09045d61) SHA1(0cfeb0b53973fff08b673a10b1f7e6e3b46aa214) )
ROM_END

ROM_START( sfa3h )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78h.p1", 0x000000, 0x80000, CRC(b3b563a3) SHA1(27863508fedeb5d85e78b8d64361f0000d00e0a5) )
	ROM_LOAD16_WORD_SWAP( "c78h.p2", 0x080000, 0x80000, CRC(47891fec) SHA1(c17421785a0f65edebb6abe54ff161ace4aa67e5) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",  0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",  0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",  0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",  0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",  0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",  0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3h.key",    0x00, 0x14, CRC(1b34998c) SHA1(d20e2b1bb2b90278a7667d525a022c6c05b8c778) )
ROM_END

ROM_START( sfa3hr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78hr1.p1", 0x000000, 0x80000, CRC(4b16cb3e) SHA1(2e1f45c9076ce231a545146d475f410397f20e27) )
	ROM_LOAD16_WORD_SWAP( "c78hr1.p2", 0x080000, 0x80000, CRC(88ad2e6a) SHA1(2a1de667ebeffd247ab9ee11687813b3397fc8de) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",  0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",  0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",  0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",  0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",  0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",  0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3h.key",    0x00, 0x14, CRC(1b34998c) SHA1(d20e2b1bb2b90278a7667d525a022c6c05b8c778) )
ROM_END

ROM_START( sfa3b )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78b.p1", 0x000000, 0x80000, CRC(046c9b4d) SHA1(aa2b19f2d4a9bab6e273635b43da7538025f9d77) )
	ROM_LOAD16_WORD_SWAP( "c78b.p2", 0x080000, 0x80000, CRC(da211919) SHA1(bffeca36c9c78168f44e288ba34bb682a1626f8f) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",  0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",  0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",  0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",  0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",  0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",  0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sfa3b.key",    0x00, 0x14, CRC(2d0a1351) SHA1(9973f186967a4737060978b2ee5d0cb2da4aa78a) )
ROM_END

ROM_START( sgemf )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c74u.p1", 0x000000, 0x80000, CRC(ac2e8566) SHA1(5975aae46bded231c0f478f40c7257434ade36b0) )
	ROM_LOAD16_WORD_SWAP( "c74.p2",  0x080000, 0x80000, CRC(f4314c96) SHA1(c40ed74039bf0096eb3648b7243a8e697638e0a6) )
	ROM_LOAD16_WORD_SWAP( "c74.p3",  0x100000, 0x80000, CRC(215655f6) SHA1(242c0f4401520f2a3b0deafc3a807b18b987e496) )
	ROM_LOAD16_WORD_SWAP( "c74.p4",  0x180000, 0x80000, CRC(ea6f13ea) SHA1(1bc924a8a9da1d2ad7667685cdb92fe317a39aba) )
	ROM_LOAD16_WORD_SWAP( "c74.p5",  0x200000, 0x80000, CRC(5ac6d5ea) SHA1(9ce8e4668b565658597a868830545fb75a5eeaa6) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c74.c1",   0x0000000, 0x400000, CRC(22d72ab9) SHA1(653efd95c34b4b9d2ab0d219f41a99ca84e12214) )
	ROM_LOAD64_WORD( "c74.c2",   0x0000002, 0x400000, CRC(16a4813c) SHA1(bf5fce6008214f353414d1b64bea4ed0c7673670) )
	ROM_LOAD64_WORD( "c74.c3",   0x0000004, 0x400000, CRC(1097e035) SHA1(4bd51e4e9447af27d2cac1f6d2201e37c949912b) )
	ROM_LOAD64_WORD( "c74.c4",   0x0000006, 0x400000, CRC(d362d874) SHA1(30c42af18440496cc05e4418e4efa41172ae4ced) )
	ROM_LOAD64_WORD( "c74.c5",   0x1000000, 0x100000, CRC(0383897c) SHA1(aba14afa1d0c971afcee4317f480e88117d77b5e) )
	ROM_LOAD64_WORD( "c74.c6",   0x1000002, 0x100000, CRC(76f91084) SHA1(3d1e32467f2aa5dd6fb96bd5c866ecc9691660fc) )
	ROM_LOAD64_WORD( "c74.c7",   0x1000004, 0x100000, CRC(756c3754) SHA1(be2f709b90222a567f198f851cf07ffb0ad433d7) )
	ROM_LOAD64_WORD( "c74.c8",   0x1000006, 0x100000, CRC(9ec9277d) SHA1(b7ceeaca30dfcdf498b61a6961f0aa1a068b8ec4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c74.m1",   0x00000, 0x08000, CRC(254e5f33) SHA1(c413ec0630b9bdd15e64f42893eba8958a09b573) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c74.m2",   0x28000, 0x20000, CRC(6902f4f9) SHA1(9bfe4ddade3c666076d26a2b545120f6d059fd7c) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c74.q1",   0x000000, 0x400000, CRC(a5dea005) SHA1(3ae79baf6ff5bd527f82b26f164c7e3c65423ae2) )
	ROM_LOAD16_WORD_SWAP( "c74.q2",   0x400000, 0x400000, CRC(4ce235fe) SHA1(795b94557e954cc0e45fd3778b609064d57a34a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sgemf.key",    0x00, 0x14, CRC(3d604021) SHA1(f6189a7dcdd110f4d624cddfe2e51e49c40a0fdb) )
ROM_END

ROM_START( pfghtj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c74j.p1", 0x000000, 0x80000, CRC(681da43e) SHA1(1bd4b6b395ac7208c0208b254455276719e98c4b) )
	ROM_LOAD16_WORD_SWAP( "c74.p2",  0x080000, 0x80000, CRC(f4314c96) SHA1(c40ed74039bf0096eb3648b7243a8e697638e0a6) )
	ROM_LOAD16_WORD_SWAP( "c74.p3",  0x100000, 0x80000, CRC(215655f6) SHA1(242c0f4401520f2a3b0deafc3a807b18b987e496) )
	ROM_LOAD16_WORD_SWAP( "c74.p4",  0x180000, 0x80000, CRC(ea6f13ea) SHA1(1bc924a8a9da1d2ad7667685cdb92fe317a39aba) )
	ROM_LOAD16_WORD_SWAP( "c74.p5",  0x200000, 0x80000, CRC(5ac6d5ea) SHA1(9ce8e4668b565658597a868830545fb75a5eeaa6) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c74.c1",   0x0000000, 0x400000, CRC(22d72ab9) SHA1(653efd95c34b4b9d2ab0d219f41a99ca84e12214) )
	ROM_LOAD64_WORD( "c74.c2",   0x0000002, 0x400000, CRC(16a4813c) SHA1(bf5fce6008214f353414d1b64bea4ed0c7673670) )
	ROM_LOAD64_WORD( "c74.c3",   0x0000004, 0x400000, CRC(1097e035) SHA1(4bd51e4e9447af27d2cac1f6d2201e37c949912b) )
	ROM_LOAD64_WORD( "c74.c4",   0x0000006, 0x400000, CRC(d362d874) SHA1(30c42af18440496cc05e4418e4efa41172ae4ced) )
	ROM_LOAD64_WORD( "c74.c5",   0x1000000, 0x100000, CRC(0383897c) SHA1(aba14afa1d0c971afcee4317f480e88117d77b5e) )
	ROM_LOAD64_WORD( "c74.c6",   0x1000002, 0x100000, CRC(76f91084) SHA1(3d1e32467f2aa5dd6fb96bd5c866ecc9691660fc) )
	ROM_LOAD64_WORD( "c74.c7",   0x1000004, 0x100000, CRC(756c3754) SHA1(be2f709b90222a567f198f851cf07ffb0ad433d7) )
	ROM_LOAD64_WORD( "c74.c8",   0x1000006, 0x100000, CRC(9ec9277d) SHA1(b7ceeaca30dfcdf498b61a6961f0aa1a068b8ec4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c74.m1",   0x00000, 0x08000, CRC(254e5f33) SHA1(c413ec0630b9bdd15e64f42893eba8958a09b573) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c74.m2",   0x28000, 0x20000, CRC(6902f4f9) SHA1(9bfe4ddade3c666076d26a2b545120f6d059fd7c) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c74.q1",   0x000000, 0x400000, CRC(a5dea005) SHA1(3ae79baf6ff5bd527f82b26f164c7e3c65423ae2) )
	ROM_LOAD16_WORD_SWAP( "c74.q2",   0x400000, 0x400000, CRC(4ce235fe) SHA1(795b94557e954cc0e45fd3778b609064d57a34a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "pfghtj.key",   0x00, 0x14, CRC(62297638) SHA1(efe090ffa8992823cfd787ade736df5487d69ca7) )
ROM_END

ROM_START( sgemfa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c74a.p1", 0x000000, 0x80000, CRC(e17c089a) SHA1(59529957aeb430df48a88414637e67848fdaaaca) )
	ROM_LOAD16_WORD_SWAP( "c74.p2",  0x080000, 0x80000, CRC(f4314c96) SHA1(c40ed74039bf0096eb3648b7243a8e697638e0a6) )
	ROM_LOAD16_WORD_SWAP( "c74.p3",  0x100000, 0x80000, CRC(215655f6) SHA1(242c0f4401520f2a3b0deafc3a807b18b987e496) )
	ROM_LOAD16_WORD_SWAP( "c74.p4",  0x180000, 0x80000, CRC(ea6f13ea) SHA1(1bc924a8a9da1d2ad7667685cdb92fe317a39aba) )
	ROM_LOAD16_WORD_SWAP( "c74.p5",  0x200000, 0x80000, CRC(5ac6d5ea) SHA1(9ce8e4668b565658597a868830545fb75a5eeaa6) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c74.c1",   0x0000000, 0x400000, CRC(22d72ab9) SHA1(653efd95c34b4b9d2ab0d219f41a99ca84e12214) )
	ROM_LOAD64_WORD( "c74.c2",   0x0000002, 0x400000, CRC(16a4813c) SHA1(bf5fce6008214f353414d1b64bea4ed0c7673670) )
	ROM_LOAD64_WORD( "c74.c3",   0x0000004, 0x400000, CRC(1097e035) SHA1(4bd51e4e9447af27d2cac1f6d2201e37c949912b) )
	ROM_LOAD64_WORD( "c74.c4",   0x0000006, 0x400000, CRC(d362d874) SHA1(30c42af18440496cc05e4418e4efa41172ae4ced) )
	ROM_LOAD64_WORD( "c74.c5",   0x1000000, 0x100000, CRC(0383897c) SHA1(aba14afa1d0c971afcee4317f480e88117d77b5e) )
	ROM_LOAD64_WORD( "c74.c6",   0x1000002, 0x100000, CRC(76f91084) SHA1(3d1e32467f2aa5dd6fb96bd5c866ecc9691660fc) )
	ROM_LOAD64_WORD( "c74.c7",   0x1000004, 0x100000, CRC(756c3754) SHA1(be2f709b90222a567f198f851cf07ffb0ad433d7) )
	ROM_LOAD64_WORD( "c74.c8",   0x1000006, 0x100000, CRC(9ec9277d) SHA1(b7ceeaca30dfcdf498b61a6961f0aa1a068b8ec4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c74.m1",   0x00000, 0x08000, CRC(254e5f33) SHA1(c413ec0630b9bdd15e64f42893eba8958a09b573) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c74.m2",   0x28000, 0x20000, CRC(6902f4f9) SHA1(9bfe4ddade3c666076d26a2b545120f6d059fd7c) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c74.q1",   0x000000, 0x400000, CRC(a5dea005) SHA1(3ae79baf6ff5bd527f82b26f164c7e3c65423ae2) )
	ROM_LOAD16_WORD_SWAP( "c74.q2",   0x400000, 0x400000, CRC(4ce235fe) SHA1(795b94557e954cc0e45fd3778b609064d57a34a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sgemfa.key",   0x00, 0x14, CRC(dd513738) SHA1(1a5f1adac6ebf1bf212d59376afacb459f5be23d) )
ROM_END

ROM_START( sgemfh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c74h.p1", 0x000000, 0x80000, CRC(e9103347) SHA1(7a32a151146a15bf5fb5ed993fee2f616077a58c) )
	ROM_LOAD16_WORD_SWAP( "c74.p2",  0x080000, 0x80000, CRC(f4314c96) SHA1(c40ed74039bf0096eb3648b7243a8e697638e0a6) )
	ROM_LOAD16_WORD_SWAP( "c74.p3",  0x100000, 0x80000, CRC(215655f6) SHA1(242c0f4401520f2a3b0deafc3a807b18b987e496) )
	ROM_LOAD16_WORD_SWAP( "c74.p4",  0x180000, 0x80000, CRC(ea6f13ea) SHA1(1bc924a8a9da1d2ad7667685cdb92fe317a39aba) )
	ROM_LOAD16_WORD_SWAP( "c74.p5",  0x200000, 0x80000, CRC(5ac6d5ea) SHA1(9ce8e4668b565658597a868830545fb75a5eeaa6) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c74.c1",   0x0000000, 0x400000, CRC(22d72ab9) SHA1(653efd95c34b4b9d2ab0d219f41a99ca84e12214) )
	ROM_LOAD64_WORD( "c74.c2",   0x0000002, 0x400000, CRC(16a4813c) SHA1(bf5fce6008214f353414d1b64bea4ed0c7673670) )
	ROM_LOAD64_WORD( "c74.c3",   0x0000004, 0x400000, CRC(1097e035) SHA1(4bd51e4e9447af27d2cac1f6d2201e37c949912b) )
	ROM_LOAD64_WORD( "c74.c4",   0x0000006, 0x400000, CRC(d362d874) SHA1(30c42af18440496cc05e4418e4efa41172ae4ced) )
	ROM_LOAD64_WORD( "c74.c5",   0x1000000, 0x100000, CRC(0383897c) SHA1(aba14afa1d0c971afcee4317f480e88117d77b5e) )
	ROM_LOAD64_WORD( "c74.c6",   0x1000002, 0x100000, CRC(76f91084) SHA1(3d1e32467f2aa5dd6fb96bd5c866ecc9691660fc) )
	ROM_LOAD64_WORD( "c74.c7",   0x1000004, 0x100000, CRC(756c3754) SHA1(be2f709b90222a567f198f851cf07ffb0ad433d7) )
	ROM_LOAD64_WORD( "c74.c8",   0x1000006, 0x100000, CRC(9ec9277d) SHA1(b7ceeaca30dfcdf498b61a6961f0aa1a068b8ec4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c74.m1",   0x00000, 0x08000, CRC(254e5f33) SHA1(c413ec0630b9bdd15e64f42893eba8958a09b573) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c74.m2",   0x28000, 0x20000, CRC(6902f4f9) SHA1(9bfe4ddade3c666076d26a2b545120f6d059fd7c) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c74.q1",   0x000000, 0x400000, CRC(a5dea005) SHA1(3ae79baf6ff5bd527f82b26f164c7e3c65423ae2) )
	ROM_LOAD16_WORD_SWAP( "c74.q2",   0x400000, 0x400000, CRC(4ce235fe) SHA1(795b94557e954cc0e45fd3778b609064d57a34a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "sgemfh.key",   0x00, 0x14, CRC(f97f4b7d) SHA1(19d43a7fc3c39fe71c9b28880d43dd4a6c2ebdbb) )
ROM_END

ROM_START( spf2t )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.p1", 0x000000, 0x80000, CRC(2af51954) SHA1(51f8797918391e772cf3cc27074ed6ca419806bd) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",  0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "spf2t.key",    0x00, 0x14, CRC(4c4dc7e3) SHA1(db4f257f6006b2f3cfa7cc70917228d639324513) )
ROM_END

ROM_START( spf2tu )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65u.p1", 0x000000, 0x80000, CRC(346e62ef) SHA1(9db5ea0aac2d459be957f8b6e2e0d18421587d4d) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",   0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "spf2tu.key",    0x00, 0x14, CRC(5d7b15e8) SHA1(c14c6b148761856bf83b31b52df83be461ac8d62) )
ROM_END


ROM_START( spf2xj )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65j.p1", 0x000000, 0x80000, CRC(2070554a) SHA1(fa818e6bd2e11667345d3d8f2397b60802ef72f9) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",   0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "spf2xj.key",   0x00, 0x14, CRC(dc39fd34) SHA1(d18f015070d08e772677ef79b542f12664cfba53) )
ROM_END

ROM_START( spf2ta )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65a.p1",  0x000000, 0x80000, CRC(3cecfa78) SHA1(c315531de87f7dc579f744e84ad2dbf068e61f46) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",   0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "spf2ta.key",   0x00, 0x14, CRC(61e93a18) SHA1(b42e89be4bd36aa57dc3c9b134d8a021aca0814b) )
ROM_END

ROM_START( spf2th )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65h.p1",  0x000000, 0x80000, CRC(20510f2d) SHA1(dcdfc0f6b849499732b76811edb682fea758f530) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",   0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "spf2th.key",   0x00, 0x14, CRC(292db449) SHA1(e0b82bd471ceadcee1e52ff5a4a8835d53a84a66) )
ROM_END

ROM_START( ssf2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48.p1",  0x000000, 0x80000, CRC(af654792) SHA1(a8e84154a639c036413f1423aaf2312a5affa55a) )
	ROM_LOAD16_WORD_SWAP( "c48.p2",  0x080000, 0x80000, CRC(b082aa67) SHA1(ca26b4bb1947cb30eaf6b61f606b859d18da4c4c) )
	ROM_LOAD16_WORD_SWAP( "c48.p3",  0x100000, 0x80000, CRC(02b9c137) SHA1(ba624441e1b4bfb67c71f6a116fe43539eaa4a15) )
	ROM_LOAD16_WORD_SWAP( "c48.p4",  0x180000, 0x80000, CRC(1c8e44a8) SHA1(8febd060cb782e09dc6216bcaee202ed84f35338) )
	ROM_LOAD16_WORD_SWAP( "c48.p5",  0x200000, 0x80000, CRC(2409001d) SHA1(f532ebb2efbb8f8ba311d10ff897490352c87f97) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48.m1",   0x00000, 0x08000, CRC(71fcdfc9) SHA1(c816f47ce10b8e85504638ada9117ec26ddaab77) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2.key",     0x00, 0x14, CRC(e469ccbb) SHA1(43d49107e96a4a14f1f25931ad3ae6065958150e) )
ROM_END

ROM_START( ssf2r1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48r1.p1",  0x000000, 0x80000, CRC(a597745d) SHA1(5b12e09c14f0ea93b668b97ca2d27a686c85f641) )
	ROM_LOAD16_WORD_SWAP( "c48.p2",  0x080000, 0x80000, CRC(b082aa67) SHA1(ca26b4bb1947cb30eaf6b61f606b859d18da4c4c) )
	ROM_LOAD16_WORD_SWAP( "c48.p3",  0x100000, 0x80000, CRC(02b9c137) SHA1(ba624441e1b4bfb67c71f6a116fe43539eaa4a15) )
	ROM_LOAD16_WORD_SWAP( "c48r1.p4",  0x180000, 0x80000, CRC(70d470c5) SHA1(ba03c8f4c76f72f4483e91547e03d1a0cf6db485) )
	ROM_LOAD16_WORD_SWAP( "c48.p5",  0x200000, 0x80000, CRC(2409001d) SHA1(f532ebb2efbb8f8ba311d10ff897490352c87f97) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2.key",     0x00, 0x14, CRC(e469ccbb) SHA1(43d49107e96a4a14f1f25931ad3ae6065958150e) )
ROM_END

ROM_START( ssf2u )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48u.p1",  0x000000, 0x80000, CRC(72f29c33) SHA1(c24769ca9568d4f6847979929b2a059e57dae6b3) )
	ROM_LOAD16_WORD_SWAP( "c48u.p2",  0x080000, 0x80000, CRC(935cea44) SHA1(1360254debf179919def1485b5758f529c94f65a) )
	ROM_LOAD16_WORD_SWAP( "c48u.p3",  0x100000, 0x80000, CRC(a0acb28a) SHA1(55c0c0ea9b9e6ef8d7c12f888cf42b6418bbf82e) )
	ROM_LOAD16_WORD_SWAP( "c48u.p4",  0x180000, 0x80000, CRC(47413dcf) SHA1(1a94e38ee899e6356ad22bde4f85e99dd3b6a934) )
	ROM_LOAD16_WORD_SWAP( "c48u.p5",  0x200000, 0x80000, CRC(e6066077) SHA1(889e2cad30b16bfaf0c54f3a38d04dd02deac6f9) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2u.key",    0x00, 0x14, CRC(2f4f8e9d) SHA1(93f7570846b76464b2ec7d37a1ee1148ef1ce76e) )
ROM_END

ROM_START( ssf2us2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48us2.p1", 0x000000, 0x80000, CRC(d48d35c9) SHA1(daa54f2ae6f5015330195aa6f568442bc777beb7) )
	ROM_LOAD16_WORD_SWAP( "c48u.p2",  0x080000, 0x80000, CRC(935cea44) SHA1(1360254debf179919def1485b5758f529c94f65a) )
	ROM_LOAD16_WORD_SWAP( "c48u.p3",  0x100000, 0x80000, CRC(a0acb28a) SHA1(55c0c0ea9b9e6ef8d7c12f888cf42b6418bbf82e) )
	ROM_LOAD16_WORD_SWAP( "c48u.p4",  0x180000, 0x80000, CRC(47413dcf) SHA1(1a94e38ee899e6356ad22bde4f85e99dd3b6a934) )
	ROM_LOAD16_WORD_SWAP( "c48us2.p5", 0x200000, 0x80000, CRC(460df588) SHA1(3f20d4ac509d03b276752c1f777178a7234ff821) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2u.key",    0x00, 0x14, CRC(2f4f8e9d) SHA1(93f7570846b76464b2ec7d37a1ee1148ef1ce76e) )
ROM_END

ROM_START( ssf2a )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48a.p1",  0x000000, 0x80000, CRC(83a059bf) SHA1(3279a792fb884f856cd5bac59eaae7d9e3be286c) )
	ROM_LOAD16_WORD_SWAP( "c48a.p2",  0x080000, 0x80000, CRC(5d873642) SHA1(74e3541ed586454a8b56e331bc9ffdb8d69f7983) )
	ROM_LOAD16_WORD_SWAP( "c48a.p3",  0x100000, 0x80000, CRC(f8fb4de2) SHA1(e3cde329405d4d59b7c234a30a7c178afb22deef) )
	ROM_LOAD16_WORD_SWAP( "c48a.p4",  0x180000, 0x80000, CRC(3185d19d) SHA1(9a354b0ee6243a3aaaa0027cce438dcfd9f93a74) )
	ROM_LOAD16_WORD_SWAP( "c48a.p5",  0x200000, 0x80000, CRC(36e29217) SHA1(86563b42676c923c6e3d760e22621e687de3a991) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2a.key",    0x00, 0x14, CRC(5fb6013f) SHA1(d027d04136e208ace6d31163c5e6de18e96694e6) )
ROM_END

ROM_START( ssf2ar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48ar1.p1", 0x000000, 0x80000, CRC(d2a3c520) SHA1(514131f0d8a7c6b5bf68630250e4b1b5983d490d) )
	ROM_LOAD16_WORD_SWAP( "c48a.p2",   0x080000, 0x80000, CRC(5d873642) SHA1(74e3541ed586454a8b56e331bc9ffdb8d69f7983) )
	ROM_LOAD16_WORD_SWAP( "c48a.p3",   0x100000, 0x80000, CRC(f8fb4de2) SHA1(e3cde329405d4d59b7c234a30a7c178afb22deef) )
	ROM_LOAD16_WORD_SWAP( "c48ar1.p4", 0x180000, 0x80000, CRC(aa8acee7) SHA1(e696b0391e41728f0cc7f190681c5fa7c96a3f81) )
	ROM_LOAD16_WORD_SWAP( "c48a.p5",   0x200000, 0x80000, CRC(36e29217) SHA1(86563b42676c923c6e3d760e22621e687de3a991) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2a.key",    0x00, 0x14, CRC(5fb6013f) SHA1(d027d04136e208ace6d31163c5e6de18e96694e6) )
ROM_END

ROM_START( ssf2j )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48j.p1",  0x000000, 0x80000, CRC(5c2e356d) SHA1(379f1e508778adda4a4087ec52c89b2253265f82) )
	ROM_LOAD16_WORD_SWAP( "c48j.p2",  0x080000, 0x80000, CRC(013bd55c) SHA1(2482f823a980d45baeea8009dadae7f996bcdb5d) )
	ROM_LOAD16_WORD_SWAP( "c48j.p3",  0x100000, 0x80000, CRC(0918d19a) SHA1(c23be61dd193058eb1391d39fbc22fbcf0640ee0) )
	ROM_LOAD16_WORD_SWAP( "c48j.p4",  0x180000, 0x80000, CRC(014e0c6d) SHA1(4a5689a05900564c2544c95741cd450ce55da0a7) )
	ROM_LOAD16_WORD_SWAP( "c48j.p5",  0x200000, 0x80000, CRC(eb6a9b1b) SHA1(daedb669b0025f6efb0f3302a40d88dcde2fc76f) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2j.key",    0x00, 0x14, CRC(bca45cc2) SHA1(5e9ccfde4b476efdce4e4fcbaf8facefe41a4c89) )
ROM_END

ROM_START( ssf2jr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48jr1.p1", 0x000000, 0x80000, CRC(0bbf1304) SHA1(be93b559ebfcc0fd72cde787c5ea4f50eac52bbf) )
	ROM_LOAD16_WORD_SWAP( "c48j.p2",   0x080000, 0x80000, CRC(013bd55c) SHA1(2482f823a980d45baeea8009dadae7f996bcdb5d) )
	ROM_LOAD16_WORD_SWAP( "c48j.p3",   0x100000, 0x80000, CRC(0918d19a) SHA1(c23be61dd193058eb1391d39fbc22fbcf0640ee0) )
	ROM_LOAD16_WORD_SWAP( "c48jr1.p4", 0x180000, 0x80000, CRC(d808a6cd) SHA1(214a4281abacdf6b74b7f51379a93cc64b4c1d7d) )
	ROM_LOAD16_WORD_SWAP( "c48j.p5",   0x200000, 0x80000, CRC(eb6a9b1b) SHA1(daedb669b0025f6efb0f3302a40d88dcde2fc76f) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2j.key",    0x00, 0x14, CRC(bca45cc2) SHA1(5e9ccfde4b476efdce4e4fcbaf8facefe41a4c89) )
ROM_END

ROM_START( ssf2jr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48jr2.p1", 0x000000, 0x80000, CRC(7eb0efed) SHA1(c389301cf26cc72ef10c20a7d37223622d05e9ca) )
	ROM_LOAD16_WORD_SWAP( "c48jr2.p2", 0x080000, 0x80000, CRC(d7322164) SHA1(b83c8523d152384a3eb9f459685b11c6e77cd6d4) )
	ROM_LOAD16_WORD_SWAP( "c48j.p3",   0x100000, 0x80000, CRC(0918d19a) SHA1(c23be61dd193058eb1391d39fbc22fbcf0640ee0) )
	ROM_LOAD16_WORD_SWAP( "c48jr1.p4", 0x180000, 0x80000, CRC(d808a6cd) SHA1(214a4281abacdf6b74b7f51379a93cc64b4c1d7d) )
	ROM_LOAD16_WORD_SWAP( "c48j.p5",   0x200000, 0x80000, CRC(eb6a9b1b) SHA1(daedb669b0025f6efb0f3302a40d88dcde2fc76f) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2j.key",    0x00, 0x14, CRC(bca45cc2) SHA1(5e9ccfde4b476efdce4e4fcbaf8facefe41a4c89) )
ROM_END

ROM_START( ssf2h )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48h.p1",  0x000000, 0x80000, CRC(b086b355) SHA1(c0ee70fa44081ebf09a72910ce1b733b019aaf71) )
	ROM_LOAD16_WORD_SWAP( "c48h.p2",  0x080000, 0x80000, CRC(1e629b29) SHA1(1c1043610cae9ea6cb3139a1c14d53a6fcd91d02) )
	ROM_LOAD16_WORD_SWAP( "c48h.p3",  0x100000, 0x80000, CRC(b5997e10) SHA1(fe9502a5dfc1dfba80d0246056eae5f3b47897a7) )
	ROM_LOAD16_WORD_SWAP( "c48h.p4",  0x180000, 0x80000, CRC(793b8fad) SHA1(e0e30dbd7a95636e592fc58577179f12b7b4ea76) )
	ROM_LOAD16_WORD_SWAP( "c48h.p5",  0x200000, 0x80000, CRC(cbb92ac3) SHA1(f5dd189757b11ea39ff8ead8d7e9bd3b97934a94) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2h.key",    0x00, 0x14, CRC(8331bc8e) SHA1(2d290871023f77cfe4eae3bd8611bb1a5f98df03) )
ROM_END

ROM_START( ssf2tb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tb.p1",  0x000000, 0x80000, CRC(496a8409) SHA1(3101689e86ab78c544524e31057478fce336ddaa) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p2",  0x080000, 0x80000, CRC(4b45c18b) SHA1(9c7ecb6fee70e317d1005bcadadf59cf11f58050) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p3",  0x100000, 0x80000, CRC(6a9c6444) SHA1(76ba626136268a48b139f6aacd6eeded94d1354d) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p4",  0x180000, 0x80000, CRC(e4944fc3) SHA1(2d77bc19140c8895eca445b6a290bc793946ccfb) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p5",  0x200000, 0x80000, CRC(2c9f4782) SHA1(de046e6bd9823129fb3d1bfff3710689816a6b0a) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tb.key",   0x00, 0x14, CRC(1ecc92b2) SHA1(22a3e7787225915833ffdef6ed2a896b079fc2f1) )
ROM_END

ROM_START( ssf2tbr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbr1.p1", 0x000000, 0x80000, CRC(1e018e34) SHA1(b9ca6655f0fc67a4f047df7ec944c9b4b85ab8ef) )
	ROM_LOAD16_WORD_SWAP( "c48tbr1.p2", 0x080000, 0x80000, CRC(ac92efaf) SHA1(340b2b6a60bf3195c0d42e214dcec49f8cc35f35) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p3", 0x100000, 0x80000, CRC(6a9c6444) SHA1(76ba626136268a48b139f6aacd6eeded94d1354d) )
	ROM_LOAD16_WORD_SWAP( "c48tbr1.p4", 0x180000, 0x80000, CRC(f442562b) SHA1(bced425b291c1b90f988e59d3639701874271cb2) )
	ROM_LOAD16_WORD_SWAP( "c48tb.p5", 0x200000, 0x80000, CRC(2c9f4782) SHA1(de046e6bd9823129fb3d1bfff3710689816a6b0a) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tb.key",   0x00, 0x14, CRC(1ecc92b2) SHA1(22a3e7787225915833ffdef6ed2a896b079fc2f1) )
ROM_END

ROM_START( ssf2tbu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbu.p1", 0x000000, 0x80000, CRC(4635b35e) SHA1(e4438812c7424d28f1caef9c2e19bc68aba81c43) )
	ROM_LOAD16_WORD_SWAP( "c48tbu.p2", 0x080000, 0x80000, CRC(2630b3dd) SHA1(c5f84fb2e93d3a4dbce6ec33cf6dae3513957837) )
	ROM_LOAD16_WORD_SWAP( "c48tbu.p3", 0x100000, 0x80000, CRC(c0293c97) SHA1(2f322e59364b9c030a3a163dc5b38778e4c7ba49) )
	ROM_LOAD16_WORD_SWAP( "c48tbu.p4", 0x180000, 0x80000, CRC(aaa33888) SHA1(c051a03c0dc55b1d16dc49ec8e4adfb5e6347c00) )
	ROM_LOAD16_WORD_SWAP( "c48tbu.p5", 0x200000, 0x80000, CRC(44c54534) SHA1(9bf7775e91b4d7dcfa397717b777798d105b02ff) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tbu.key",   0x00, 0x14, CRC(1da3ccf3) SHA1(d740581acdaccbc75308f0a8c53a3a910dc6a10f) )
ROM_END

ROM_START( ssf2tbj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p1", 0x000000, 0x80000, CRC(e78a3433) SHA1(c5a1f209e467f8f5a791b2d2f184e71650c8c9df) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p2", 0x080000, 0x80000, CRC(b4dc1906) SHA1(b29497c8562d004c6f0393eb61ba80978f4b3aff) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p3",  0x100000, 0x80000, CRC(a7e35fbc) SHA1(c59737f4dbd9ccde30b0a1e2f151a78f162ceafc) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p4", 0x180000, 0x80000, CRC(0737c30d) SHA1(8ecc492f9e638d4a2c1a9c473c0746936eb5553a) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p5",  0x200000, 0x80000, CRC(1f239515) SHA1(e5e314e7fe8d1448cc452e515415adf8aa62056d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tbj.key",  0x00, 0x14, CRC(bcc2e017) SHA1(9c375eeffa599be4994e8efd7dee9d0e67613a13) )
ROM_END

ROM_START( ssf2tbj1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbj1.p1", 0x000000, 0x80000, CRC(980d4450) SHA1(1a7a7000dc11473d06e2bb552c7a506eb0019235) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p2", 0x080000, 0x80000, CRC(b4dc1906) SHA1(b29497c8562d004c6f0393eb61ba80978f4b3aff) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p3", 0x100000, 0x80000, CRC(a7e35fbc) SHA1(c59737f4dbd9ccde30b0a1e2f151a78f162ceafc) )
	ROM_LOAD16_WORD_SWAP( "c48tbjr1.p4", 0x180000, 0x80000, CRC(cb592b30) SHA1(d9464c99f813ee50041adfc077ebe998c6e9a5f7) )
	ROM_LOAD16_WORD_SWAP( "c48tbj.p5", 0x200000, 0x80000, CRC(1f239515) SHA1(e5e314e7fe8d1448cc452e515415adf8aa62056d) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tbj.key",  0x00, 0x14, CRC(bcc2e017) SHA1(9c375eeffa599be4994e8efd7dee9d0e67613a13) )
ROM_END

ROM_START( ssf2tba )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tba.p1", 0x000000, 0x80000, CRC(8de631d2) SHA1(e9249413149c6799faaece09226cdd37906b1a63) )
	ROM_LOAD16_WORD_SWAP( "c48tba.p2", 0x080000, 0x80000, CRC(abef3042) SHA1(71062765bd1337780fb347d277ebeca5d7f91c05) )
	ROM_LOAD16_WORD_SWAP( "c48tba.p3",  0x100000, 0x80000, CRC(edfa018f) SHA1(61f80f3a1674c900df34095601db92d98bb28096) )
	ROM_LOAD16_WORD_SWAP( "c48tba.p4", 0x180000, 0x80000, CRC(2b9d1dbc) SHA1(e7136c2269c54f1fdfcdefdefc5dce0a63f38960) )
	ROM_LOAD16_WORD_SWAP( "c48tba.p5",  0x200000, 0x80000, CRC(f4a25159) SHA1(83967820a82a20030ddb1404e73b73e20605e907) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48.m1",   0x00000, 0x08000, CRC(71fcdfc9) SHA1(c816f47ce10b8e85504638ada9117ec26ddaab77) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tba.key",   0x00, 0x14, CRC(8d2740ed) SHA1(a211373e6f45ef073f36bda2d6fbfc2cd5c7866b) )
ROM_END

ROM_START( ssf2tbh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbh.p1", 0x000000, 0x80000, CRC(6db7d28b) SHA1(b37f3287c51564ee0fdab499787b37166d1eb76d) )
	ROM_LOAD16_WORD_SWAP( "c48tbh.p2", 0x080000, 0x80000, CRC(0fe7d895) SHA1(e9603eebc711641684b15d97e1a4e4dcaf5b92ae) )
	ROM_LOAD16_WORD_SWAP( "c48tbh.p3", 0x100000, 0x80000, CRC(41be4f2d) SHA1(e90061bf00cac2050e90a89bc5d1e8b0e4df35e2) )
	ROM_LOAD16_WORD_SWAP( "c48tbh.p4", 0x180000, 0x80000, CRC(d2522eb1) SHA1(711d66106161504626f18dfad5b61080cae0a0b3) )
	ROM_LOAD16_WORD_SWAP( "c48tbh.p5", 0x200000, 0x80000, CRC(b1c3a3c6) SHA1(e26020c616390c3606665d0f10345b34ec19fa46) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD("ssf2tbh.key", 0x00, 0x14, CRC(fddecf4f) SHA1(03fcad05f17edbcf037cad0e5c9acd683f432781))
ROM_END

ROM_START( ssf2t )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.p1", 0x000000, 0x80000, CRC(2fa1f396) SHA1(2aa58309811f34901554b84396556630a22ce9bc) )
	ROM_LOAD16_WORD_SWAP( "c51.p2", 0x080000, 0x80000, CRC(d0bc29c6) SHA1(d9f89bcd79cba26db2100a00dd7bd8ee6ecb75f3) )
	ROM_LOAD16_WORD_SWAP( "c51.p3", 0x100000, 0x80000, CRC(65222964) SHA1(025bb708dc5b6365cc7fe60fc3f242511ad8f384) )
	ROM_LOAD16_WORD_SWAP( "c51.p4", 0x180000, 0x80000, CRC(8fe9f531) SHA1(b5d9ed498f730fdb968992bdec33605db1a007f4) )
	ROM_LOAD16_WORD_SWAP( "c51.p5", 0x200000, 0x80000, CRC(8a7d0cb6) SHA1(27ea0cea73a93c27257bf2a170d1deaf938cc311) )
	ROM_LOAD16_WORD_SWAP( "c51.p6", 0x280000, 0x80000, CRC(74c24062) SHA1(f3eca09e0544c6aa46b0c4bead2246ab1e9a97d9) )
	ROM_LOAD16_WORD_SWAP( "c51.p7", 0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2t.key",    0x00, 0x14, CRC(524d608e) SHA1(fcd2b9a1a640a0dee4a674dc9cfb3975466f7757) )
ROM_END

ROM_START( ssf2tu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51u.p1", 0x000000, 0x80000, CRC(d6ff689e) SHA1(bea1a8aafbbbe9cb0895561a57dead1579361a8e) )
	ROM_LOAD16_WORD_SWAP( "c51u.p2", 0x080000, 0x80000, CRC(532b5ffd) SHA1(769a8a9d4e04e291ce7427b89e537bba2258ca82) )
	ROM_LOAD16_WORD_SWAP( "c51u.p3", 0x100000, 0x80000, CRC(ffa3c6de) SHA1(7cce55a3e07b5ba2e2e37e4c66a52678a1b19a63) )
	ROM_LOAD16_WORD_SWAP( "c51u.p4", 0x180000, 0x80000, CRC(83f9382b) SHA1(273ff4d4242ce22b755d35e5d2cf2517d625bdd2) )
	ROM_LOAD16_WORD_SWAP( "c51u.p5", 0x200000, 0x80000, CRC(6ab673e8) SHA1(840af0d0ce634fb98e4f89173c4f1f95ec2cf94b) )
	ROM_LOAD16_WORD_SWAP( "c51u.p6", 0x280000, 0x80000, CRC(b3c71810) SHA1(b51515f4f4aee5bbbfc8b79372d0bc6e0c140912) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",  0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tu.key",   0x00, 0x14, CRC(f7d62def) SHA1(27a797fd6fe435e14ca334947df38d56f89f79de) )
ROM_END

ROM_START( ssf2tur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51ur1.p1", 0x000000, 0x80000, CRC(86e4a335) SHA1(04db3fd519973aeb7b32be62871e0fb4605946eb) )
	ROM_LOAD16_WORD_SWAP( "c51u.p2",   0x080000, 0x80000, CRC(532b5ffd) SHA1(769a8a9d4e04e291ce7427b89e537bba2258ca82) )
	ROM_LOAD16_WORD_SWAP( "c51u.p3",   0x100000, 0x80000, CRC(ffa3c6de) SHA1(7cce55a3e07b5ba2e2e37e4c66a52678a1b19a63) )
	ROM_LOAD16_WORD_SWAP( "c51ur1.p4", 0x180000, 0x80000, CRC(e4c04c99) SHA1(01fe284363e4795e7bdf4206f54a6108fcdac18b) )
	ROM_LOAD16_WORD_SWAP( "c51ur1.p5", 0x200000, 0x80000, CRC(d8199e41) SHA1(aa5647446f7e076cdf895dd5cbc5b30a8d4fdac2) )
	ROM_LOAD16_WORD_SWAP( "c51u.p6",   0x280000, 0x80000, CRC(b3c71810) SHA1(b51515f4f4aee5bbbfc8b79372d0bc6e0c140912) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",    0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2tu.key",   0x00, 0x14, CRC(f7d62def) SHA1(27a797fd6fe435e14ca334947df38d56f89f79de) )
ROM_END

ROM_START( ssf2ta )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51a.p1", 0x000000, 0x80000, CRC(04b9ff34) SHA1(69feb2c9c03634e6f964dae310d7b72b4c76140d) )
	ROM_LOAD16_WORD_SWAP( "c51a.p2", 0x080000, 0x80000, CRC(16ea5f7a) SHA1(08404c6a79b9a36eceb06e0d3e1d747a21fac186) )
	ROM_LOAD16_WORD_SWAP( "c51a.p3", 0x100000, 0x80000, CRC(53d61f0c) SHA1(b30e666d0dae7b738a76a27d1d68fbb9a630c27c) )
	ROM_LOAD16_WORD_SWAP( "c51a.p4", 0x180000, 0x80000, CRC(066d09b5) SHA1(221972629b094809f7c431f86b3f3b10354487b5) )
	ROM_LOAD16_WORD_SWAP( "c51a.p5", 0x200000, 0x80000, CRC(a428257b) SHA1(620f3a264b2c82ef1af0e33310d110e1f3e6fddf) )
	ROM_LOAD16_WORD_SWAP( "c51a.p6", 0x280000, 0x80000, CRC(39be596c) SHA1(f7ab80e64cbb703535dd39b875273eefa57df489) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",  0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2ta.key",   0x00, 0x14, CRC(c11fa8e9) SHA1(9e9fcfaa40e792b0ebf4585f1f8995e5e3fee3fd) )
ROM_END

ROM_START( ssf2th )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51h.p1", 0x000000, 0x80000, CRC(fbe80dfe) SHA1(b9c5f0a0f6ae7839e348c870ac87b16dbb665bd9) )
	ROM_LOAD16_WORD_SWAP( "c51h.p2", 0x080000, 0x80000, CRC(ef9dd4b1) SHA1(3c5d76e6ae7db91f9287f341c32e3b7a75e256f6) )
	ROM_LOAD16_WORD_SWAP( "c51h.p3", 0x100000, 0x80000, CRC(09e56ecc) SHA1(a333a67994ca543826466a17342cfe4be6da0024) )
	ROM_LOAD16_WORD_SWAP( "c51h.p4", 0x180000, 0x80000, CRC(e6f210be) SHA1(87107096863fc0c3d47b6e50f95cb26706728dbc) )
	ROM_LOAD16_WORD_SWAP( "c51h.p5", 0x200000, 0x80000, CRC(900ba1a4) SHA1(4206de5f17def6a8a98fc9b0b52d3e2c4de587d3) )
	ROM_LOAD16_WORD_SWAP( "c51h.p6", 0x280000, 0x80000, CRC(c15f0424) SHA1(de9f7075f02653c4edef0e6f4a1210a9b2711d28) )
	ROM_LOAD16_WORD_SWAP( "c51h.p7", 0x300000, 0x80000, CRC(5b92b3f9) SHA1(20aabb87cceecf0e8ccd308c5853b223fb931750) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2th.key",   0x00, 0x14, CRC(f6ce6a35) SHA1(fc0704cb7f2da70f8d8b83d323c8eab851dfae89) )
ROM_END

ROM_START( ssf2xj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51j.p1", 0x000000, 0x80000, CRC(50b52b37) SHA1(dc0330fcbffbc0077faa20d34a28d6a49b04639e) )
	ROM_LOAD16_WORD_SWAP( "c51j.p2", 0x080000, 0x80000, CRC(af7767b4) SHA1(61e7364408bf07c01634913c112b6245acce48ab) )
	ROM_LOAD16_WORD_SWAP( "c51j.p3", 0x100000, 0x80000, CRC(f4ff18f5) SHA1(aa713c9e1a2eba35bf1c9b40bb262ff7e46b9ce4) )
	ROM_LOAD16_WORD_SWAP( "c51j.p4", 0x180000, 0x80000, CRC(413477c2) SHA1(63bf120566db0c23cc726e7cdbf78428582c694f) )
	ROM_LOAD16_WORD_SWAP( "c51j.p5", 0x200000, 0x80000, CRC(a18b3d83) SHA1(f08dd30a1b864fdd5d05d58a0b43e65077702d32) )
	ROM_LOAD16_WORD_SWAP( "c51j.p6", 0x280000, 0x80000, CRC(2de76f10) SHA1(8cbe96dfeaa41306caa2819b82272ce3b0b9f926) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",  0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2xj.key",   0x00, 0x14, CRC(160d1424) SHA1(187baff83b54acfdcedd9b9222a4cc768040c622) )
ROM_END


ROM_START( ssf2xjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51jr1.p1", 0x000000, 0x80000, CRC(a7417b79) SHA1(189c3ed546bb2844e9fa9fe7e9aacef728bc8939) )
	ROM_LOAD16_WORD_SWAP( "c51j.p2",   0x080000, 0x80000, CRC(af7767b4) SHA1(61e7364408bf07c01634913c112b6245acce48ab) )
	ROM_LOAD16_WORD_SWAP( "c51j.p3",   0x100000, 0x80000, CRC(f4ff18f5) SHA1(aa713c9e1a2eba35bf1c9b40bb262ff7e46b9ce4) )
	ROM_LOAD16_WORD_SWAP( "c51jr1.p4", 0x180000, 0x80000, CRC(260d0370) SHA1(5339cf87000caef74d491815391be59cfd701c8b) )
	ROM_LOAD16_WORD_SWAP( "c51jr1.p5", 0x200000, 0x80000, CRC(1324d02a) SHA1(c23a6ea09819bd33b6e2f58aa28c317ce53a46a0) )
	ROM_LOAD16_WORD_SWAP( "c51j.p6",   0x280000, 0x80000, CRC(2de76f10) SHA1(8cbe96dfeaa41306caa2819b82272ce3b0b9f926) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",    0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2xj.key",   0x00, 0x14, CRC(160d1424) SHA1(187baff83b54acfdcedd9b9222a4cc768040c622) )
ROM_END

// interesting version, yellow case (rental?), shows OP instead of AM on startup, uses a different (unique) key to the usual Japan set
ROM_START( ssf2xjr1r )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51o.p1", 0x000000, 0x80000, CRC(2ba33dc6) SHA1(ab31046c1604eae30e55eb0e03804c98765d578e) )
	ROM_LOAD16_WORD_SWAP( "c51o.p2", 0x080000, 0x80000, CRC(ba663dd7) SHA1(46baf0a48e4fd807098aa48e34625964b279a223) )
	ROM_LOAD16_WORD_SWAP( "c51o.p3", 0x100000, 0x80000, CRC(1321625c) SHA1(af5bce813bc2d8d406c7cb30ba5cb61b17b74efd) )
	ROM_LOAD16_WORD_SWAP( "c51o.p4", 0x180000, 0x80000, CRC(0cc490ed) SHA1(59c708ff3824964bddaa508e2aea2d6f26742d40) )
	ROM_LOAD16_WORD_SWAP( "c51o.p5", 0x200000, 0x80000, CRC(64b9015e) SHA1(1b22029e2c8304e5dcda01c2e48c24e89d93b13b) )
	ROM_LOAD16_WORD_SWAP( "c51o.p6", 0x280000, 0x80000, CRC(b60f4b58) SHA1(1db3876daef85713a22dbc42f76a26a47fa26079) )
	ROM_LOAD16_WORD_SWAP( "c51.p7",  0x300000, 0x80000, CRC(642fae3f) SHA1(746df99b826b9837bba267104132161153c1daff) )

	ROM_REGION( 0x1000000, "gfx", 0 )  // identical content, different board layout
	ROM_LOAD64_WORD( "c51o.c01",   0x000000, 0x080000, CRC(e9123f9f) SHA1(a44ea524cf4fba9c27d47b404f9e5f8ef0894365) )
	ROM_LOAD64_WORD( "c51o.c05",   0x200000, 0x080000, CRC(2f8201f3) SHA1(630beb3b5ca018582d77f2ecfb72451ef934de43) )
	ROM_LOAD64_WORD( "c51o.c09",   0x400000, 0x080000, CRC(0fa334b4) SHA1(ae14a6be82e445c90af0ec6d439f0f0368dd0c56) )
	ROM_LOAD64_WORD( "c51o.c13",   0x600000, 0x080000, CRC(b76740d3) SHA1(5d0e1624bc2b04483ad5ebc06dd10824d9f5599e) )
	ROM_LOAD64_WORD( "c51o.c17",   0x800000, 0x080000, CRC(14f058ec) SHA1(6c7f455e117f5419d7e2324cfd67f8f9c243be58) )
	ROM_LOAD64_WORD( "c51o.c21",   0xa00000, 0x080000, CRC(800c3ae9) SHA1(e4e7679495665811c043ef6cd9c949d95902e33e) )
	ROM_LOAD64_WORD( "c51o.c25",   0xc00000, 0x080000, CRC(06cf540b) SHA1(b0e68c2d199dcc9c9ea1f1926ee2691cced434d4) )
	ROM_LOAD64_WORD( "c51o.c29",   0xe00000, 0x080000, CRC(71084e42) SHA1(9b8755b4c5a0c36ebcf6b12801d21fb7ef011638) )
	ROM_LOAD64_WORD( "c51o.c02",   0x000002, 0x080000, CRC(6eb3ee4d) SHA1(7bb3aa3dc1a6e2ddde985463185035f63cd1f9e3) )
	ROM_LOAD64_WORD( "c51o.c06",   0x200002, 0x080000, CRC(2bcf1eda) SHA1(64da42c81d42031f90636bf223277973ac3869b3) )
	ROM_LOAD64_WORD( "c51o.c10",   0x400002, 0x080000, CRC(3330cc11) SHA1(d118fee0962470954638921e27f31ef8c1190ea5) )
	ROM_LOAD64_WORD( "c51o.c14",   0x600002, 0x080000, CRC(96e2ead3) SHA1(47dfeea824d607e610201c816a81be33ee037993) )
	ROM_LOAD64_WORD( "c51o.c18",   0x800002, 0x080000, CRC(e356a275) SHA1(36b750e5b9e4a3ec6c19af5865d4d262e2b67188) )
	ROM_LOAD64_WORD( "c51o.c22",   0xa00002, 0x080000, CRC(fec5698b) SHA1(53648953af987bcae5a9a5445c30f2ec594bfc30) )
	ROM_LOAD64_WORD( "c51o.c26",   0xc00002, 0x080000, CRC(69da0751) SHA1(fbc92b06348a15eeffd2f122cb39ff05a8025023) )
	ROM_LOAD64_WORD( "c51o.c30",   0xe00002, 0x080000, CRC(cc53ec15) SHA1(4caaa975fcc2fb4f5f63ff2c373a42a5e2dbd724) )
	ROM_LOAD64_WORD( "c51o.c03",   0x000004, 0x080000, CRC(cf0d44a8) SHA1(02dc2ca7a7fed1af4f13298b5af7af986f085b60) )
	ROM_LOAD64_WORD( "c51o.c07",   0x200004, 0x080000, CRC(56a153a4) SHA1(29b9d53ab0d86a1d0e72d33a7c7968cc54f3f90b) )
	ROM_LOAD64_WORD( "c51o.c11",   0x400004, 0x080000, CRC(5484e5f6) SHA1(70b085f280f562e624eb0b6ee22ba6c15b71cee9) )
	ROM_LOAD64_WORD( "c51o.c15",   0x600004, 0x080000, CRC(fce6b7f5) SHA1(1d323f8aa522d7646dd23d841f6e4c2b264e5b27) )
	ROM_LOAD64_WORD( "c51o.c19",   0x800004, 0x080000, CRC(042d7970) SHA1(dd4fae1666ceca24545ea665ad5d906f9066b2d6) )
	ROM_LOAD64_WORD( "c51o.c23",   0xa00004, 0x080000, CRC(88c472e6) SHA1(a4681364b2434e40a2c881bf0aafb8eff39d823b) )
	ROM_LOAD64_WORD( "c51o.c27",   0xc00004, 0x080000, CRC(a7d66348) SHA1(b9f8620106219bdb13cd7c497f1cac80514aa565) )
	ROM_LOAD64_WORD( "c51o.c31",   0xe00004, 0x080000, CRC(cf9119c8) SHA1(dccd5931e21568ef69f71798b07b681f1933027b) )
	ROM_LOAD64_WORD( "c51o.c04",   0x000006, 0x080000, CRC(6d374ad9) SHA1(1070e829e20b11f02bd64b213b08bd8487602f29) )
	ROM_LOAD64_WORD( "c51o.c08",   0x200006, 0x080000, CRC(34cf8bcf) SHA1(ad19cdc16ebedb6614d7fa4228f14f3c3d2ee166) )
	ROM_LOAD64_WORD( "c51o.c12",   0x400006, 0x080000, CRC(d796ea3f) SHA1(96887ddfefa267ed7628df1aa742285c62c97b0d) )
	ROM_LOAD64_WORD( "c51o.c16",   0x600006, 0x080000, CRC(c85fb7e3) SHA1(0440813b52704285a591e3152643967c2cde8c63) )
	ROM_LOAD64_WORD( "c51o.c20",   0x800006, 0x080000, CRC(6c50c2b5) SHA1(027963231450e075f9b2b7136f6499baca96594d) )
	ROM_LOAD64_WORD( "c51o.c24",   0xa00006, 0x080000, CRC(59549f63) SHA1(019677eba92eb1cf463e15cd98383aab04de63bf) )
	ROM_LOAD64_WORD( "c51o.c28",   0xc00006, 0x080000, CRC(86c97869) SHA1(179117e7c887685e322f97a21461eb05de44d1ee) )
	ROM_LOAD64_WORD( "c51o.c32",   0xe00006, 0x080000, CRC(1c0e1989) SHA1(fac63bae9129a656c12e5665dc0eaddc9017821f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 ) // identical content, different board layout
	ROM_LOAD16_WORD_SWAP( "c51o.q1",   0x000000, 0x080000, CRC(9eda6954) SHA1(28e007e8dcea127ffd82fe243a4729fa765347d6) )
	ROM_LOAD16_WORD_SWAP( "c51o.q2",   0x080000, 0x080000, CRC(355f6589) SHA1(86f7243bc8ed22760658779d0f8810b1e1f9cdd6) )
	ROM_LOAD16_WORD_SWAP( "c51o.q3",   0x100000, 0x080000, CRC(d5d08a05) SHA1(78549077a105384c7b1b609dc2e9e115dea4a220) )
	ROM_LOAD16_WORD_SWAP( "c51o.q4",   0x180000, 0x080000, CRC(930725eb) SHA1(5ff27e89f4c203207d2a762776fa014a31ab5409) )
	ROM_LOAD16_WORD_SWAP( "c51o.q5",   0x200000, 0x080000, CRC(827abf3c) SHA1(51d9ed9b182bcf24b0ccc3e99a781750609fec58) )
	ROM_LOAD16_WORD_SWAP( "c51o.q6",   0x280000, 0x080000, CRC(3919c0e5) SHA1(23b21b13775c62f0c8ee6b9db403196397981b27) )
	ROM_LOAD16_WORD_SWAP( "c51o.q7",   0x300000, 0x080000, CRC(1ba9bfa6) SHA1(e010e66aea28b2688ccf6feaf678728bab89a852) )
	ROM_LOAD16_WORD_SWAP( "c51o.q8",   0x380000, 0x080000, CRC(0c89a272) SHA1(55eb7322c8498e0897165a689feb0406caa09db6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "ssf2xjr1r.key",  0x00, 0x14, CRC(82c86e63) SHA1(e99efc6a5652fb96489ffcd7bcc6a1726b08d56b) )
ROM_END

ROM_START( vhunt2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c75.p1",  0x000000, 0x80000, CRC(9ae8f186) SHA1(f4e3a1b6ae823737d3b18561469f206921b48587) )
	ROM_LOAD16_WORD_SWAP( "c75.p2",  0x080000, 0x80000, CRC(e2fabf53) SHA1(78c53f8e984b00245486b751515248879df77437) )
	ROM_LOAD16_WORD_SWAP( "c75.p3",  0x100000, 0x80000, CRC(de34f624) SHA1(60bbbd1765e76839b01c38765da2368c5188ec61) )
	ROM_LOAD16_WORD_SWAP( "c75.p4",  0x180000, 0x80000, CRC(6a3b9897) SHA1(4f3b37004db8a3d3dde709b51c94c392615134b5) )
	ROM_LOAD16_WORD_SWAP( "c75.p5",  0x200000, 0x80000, CRC(b021c029) SHA1(de4299197600608e83fe50775e3f352f5add844d) )
	ROM_LOAD16_WORD_SWAP( "c75.p6",  0x280000, 0x80000, CRC(ac873dff) SHA1(ad9a085b8403801035683b6f63eee33daf4e97ae) )
	ROM_LOAD16_WORD_SWAP( "c75.p7",  0x300000, 0x80000, CRC(eaefce9c) SHA1(d842a824f0d0adc13a86f780084164c1273c45a4) )
	ROM_LOAD16_WORD_SWAP( "c75.p8",  0x380000, 0x80000, CRC(11730952) SHA1(2966b80b99ab065614a6ddb546110f482b998e32) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c75.c1",   0x0000000, 0x400000, CRC(3b02ddaa) SHA1(a73b0554afbfc7ace41bdf8e6cafd4c1ef0b0a08) )
	ROM_LOAD64_WORD( "c75.c2",   0x0000002, 0x400000, CRC(4e40de66) SHA1(e8b80eadffad6070aa04c8ab426311c44e7c5507) )
	ROM_LOAD64_WORD( "c75.c3",   0x0000004, 0x400000, CRC(b31d00c9) SHA1(7e7be64690663f52d10c8946aabec4250c8a8740) )
	ROM_LOAD64_WORD( "c75.c4",   0x0000006, 0x400000, CRC(149be3ab) SHA1(afc8e96e6aa3cf1db6dfd8075030a6c50b4419a9) )
	ROM_LOAD64_WORD( "c75.c5",   0x1000000, 0x400000, CRC(cd09bd63) SHA1(e582b20a948ae54f52590496051688dbfae2bc9c) )
	ROM_LOAD64_WORD( "c75.c6",   0x1000002, 0x400000, CRC(e0182c15) SHA1(a924d53ab39f4d85173bdb92a197dde2db0dc3f7) )
	ROM_LOAD64_WORD( "c75.c7",   0x1000004, 0x400000, CRC(778dc4f6) SHA1(8d0cd1c387b4b6ac7f92bb2e5a25983856328cdc) )
	ROM_LOAD64_WORD( "c75.c8",   0x1000006, 0x400000, CRC(605d9d1d) SHA1(99bc27557741527ca678d7b6307164bc04ebedc6) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c75.m1",  0x00000, 0x08000, CRC(67b9f779) SHA1(3994c65f888004b56ea9f478b1feaa81e306347e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c75.m2",  0x28000, 0x20000, CRC(aaf15fcb) SHA1(6f61daa162c835165a8aabaf1d0ea8816fbfbd40) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c75.q1",  0x000000, 0x400000, CRC(38922efd) SHA1(8cfb36bdce3a524d0a81fec12ca0cba82222fa30) )
	ROM_LOAD16_WORD_SWAP( "c75.q2",  0x400000, 0x400000, CRC(6e2430af) SHA1(b475faf943bec4171ba0130f287e1948743ca273) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhunt2.key",   0x00, 0x14, CRC(61306b20) SHA1(f72cf44837f6d934dd6bc877c3c98db388e72a67) )
ROM_END

ROM_START( vhunt2r1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c75r1.p1", 0x000000, 0x80000, CRC(1a5feb13) SHA1(a6dd6af2601e2da14032bcbf17e9f79c7a4ba2db) )
	ROM_LOAD16_WORD_SWAP( "c75r1.p2", 0x080000, 0x80000, CRC(434611a5) SHA1(ee093017405db6c16bfee3fe446bae659c6accc2) )
	ROM_LOAD16_WORD_SWAP( "c75.p3", 0x100000, 0x80000, CRC(de34f624) SHA1(60bbbd1765e76839b01c38765da2368c5188ec61) )
	ROM_LOAD16_WORD_SWAP( "c75.p4", 0x180000, 0x80000, CRC(6a3b9897) SHA1(4f3b37004db8a3d3dde709b51c94c392615134b5) )
	ROM_LOAD16_WORD_SWAP( "c75.p5", 0x200000, 0x80000, CRC(b021c029) SHA1(de4299197600608e83fe50775e3f352f5add844d) )
	ROM_LOAD16_WORD_SWAP( "c75.p6", 0x280000, 0x80000, CRC(ac873dff) SHA1(ad9a085b8403801035683b6f63eee33daf4e97ae) )
	ROM_LOAD16_WORD_SWAP( "c75.p7", 0x300000, 0x80000, CRC(eaefce9c) SHA1(d842a824f0d0adc13a86f780084164c1273c45a4) )
	ROM_LOAD16_WORD_SWAP( "c75.p8", 0x380000, 0x80000, CRC(11730952) SHA1(2966b80b99ab065614a6ddb546110f482b998e32) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c75.c1",   0x0000000, 0x400000, CRC(3b02ddaa) SHA1(a73b0554afbfc7ace41bdf8e6cafd4c1ef0b0a08) )
	ROM_LOAD64_WORD( "c75.c2",   0x0000002, 0x400000, CRC(4e40de66) SHA1(e8b80eadffad6070aa04c8ab426311c44e7c5507) )
	ROM_LOAD64_WORD( "c75.c3",   0x0000004, 0x400000, CRC(b31d00c9) SHA1(7e7be64690663f52d10c8946aabec4250c8a8740) )
	ROM_LOAD64_WORD( "c75.c4",   0x0000006, 0x400000, CRC(149be3ab) SHA1(afc8e96e6aa3cf1db6dfd8075030a6c50b4419a9) )
	ROM_LOAD64_WORD( "c75.c5",   0x1000000, 0x400000, CRC(cd09bd63) SHA1(e582b20a948ae54f52590496051688dbfae2bc9c) )
	ROM_LOAD64_WORD( "c75.c6",   0x1000002, 0x400000, CRC(e0182c15) SHA1(a924d53ab39f4d85173bdb92a197dde2db0dc3f7) )
	ROM_LOAD64_WORD( "c75.c7",   0x1000004, 0x400000, CRC(778dc4f6) SHA1(8d0cd1c387b4b6ac7f92bb2e5a25983856328cdc) )
	ROM_LOAD64_WORD( "c75.c8",   0x1000006, 0x400000, CRC(605d9d1d) SHA1(99bc27557741527ca678d7b6307164bc04ebedc6) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c75.m1",  0x00000, 0x08000, CRC(67b9f779) SHA1(3994c65f888004b56ea9f478b1feaa81e306347e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c75.m2",  0x28000, 0x20000, CRC(aaf15fcb) SHA1(6f61daa162c835165a8aabaf1d0ea8816fbfbd40) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c75.q1",  0x000000, 0x400000, CRC(38922efd) SHA1(8cfb36bdce3a524d0a81fec12ca0cba82222fa30) )
	ROM_LOAD16_WORD_SWAP( "c75.q2",  0x400000, 0x400000, CRC(6e2430af) SHA1(b475faf943bec4171ba0130f287e1948743ca273) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vhunt2.key",   0x00, 0x14, CRC(61306b20) SHA1(f72cf44837f6d934dd6bc877c3c98db388e72a67) )
ROM_END

ROM_START( vsav )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.p1", 0x000000, 0x80000, CRC(f5962a8c) SHA1(e37d48b78186c7c097894d6c17faf7c9333f61eb) )
	ROM_LOAD16_WORD_SWAP( "c71.p2", 0x080000, 0x80000, CRC(21b40ea2) SHA1(6790fa3e618850f518cbd470f44434a71be6f29f) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",  0x100000, 0x80000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",  0x180000, 0x80000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",  0x200000, 0x80000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",  0x280000, 0x80000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",  0x300000, 0x80000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",  0x380000, 0x80000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsav.key",     0x00, 0x14, CRC(a6e3b164) SHA1(79fff0e29f32e1ba21bdcc57d96f2c8e2997a2d1) )
ROM_END

ROM_START( vsavu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71u.p1", 0x000000, 0x80000, CRC(1f295274) SHA1(c926d8af4fccee5104507ee0196b05dcd419ee20) )
	ROM_LOAD16_WORD_SWAP( "c71u.p2", 0x080000, 0x80000, CRC(c46adf81) SHA1(85ffb9b3282874d6ce9318a88429666e98f67cea) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",  0x100000, 0x80000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",  0x180000, 0x80000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",  0x200000, 0x80000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",  0x280000, 0x80000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",  0x300000, 0x80000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",  0x380000, 0x80000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsavu.key",    0x00, 0x14, CRC(ff21b9d7) SHA1(214ea9a3bbb8e1c133f77844dbe7a4dea94e35e1) )
ROM_END

ROM_START( vsavj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71j.p1", 0x000000, 0x80000, CRC(2a2e74a4) SHA1(e9e7bce3c2ad0c9eebcbcd5139979d1fa19187ad) )
	ROM_LOAD16_WORD_SWAP( "c71j.p2", 0x080000, 0x80000, CRC(1c2427bc) SHA1(9047ac0ccd875d91a8ceafdf1ccf9d21c4c71644) )
	ROM_LOAD16_WORD_SWAP( "c71j.p3", 0x100000, 0x80000, CRC(95ce88d5) SHA1(ba5e64c2551d97a71d2f4d7a78663aede4b722e8) )
	ROM_LOAD16_WORD_SWAP( "c71j.p4", 0x180000, 0x80000, CRC(2c4297e0) SHA1(3a7103456ba3937f63c28dd42020cac1955b5741) )
	ROM_LOAD16_WORD_SWAP( "c71j.p5", 0x200000, 0x80000, CRC(a38aaae7) SHA1(0a5719eb2b0bbde955f605b1057ed6a8eb54ad80) )
	ROM_LOAD16_WORD_SWAP( "c71j.p6", 0x280000, 0x80000, CRC(5773e5c9) SHA1(551afc5d921f9ef1fe928ca83d072b6a6105ab0e) )
	ROM_LOAD16_WORD_SWAP( "c71j.p7", 0x300000, 0x80000, CRC(d064f8b9) SHA1(09f77f7b466c147a5d894a4ec3b40bd068dfab26) )
	ROM_LOAD16_WORD_SWAP( "c71j.p8", 0x380000, 0x80000, CRC(434518e9) SHA1(ce1c8557a9e6c5451ab41a96f01b0cd4ba02ea3e) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsavj.key",    0x00, 0x14, CRC(36d28ab8) SHA1(7de11dbe4a7ea0e8b68ab332f80e32cb6c1bf48e) )
ROM_END

ROM_START( vsava )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71a.p1", 0x000000, 0x80000, CRC(44c1198f) SHA1(35714b2f6ebeafea93be6467b5b22ea41b9f3826) )
	ROM_LOAD16_WORD_SWAP( "c71a.p2", 0x080000, 0x80000, CRC(2218b781) SHA1(5dd28cc1b70b2953fbd4d5fd14abeeb3b83b193e) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",  0x100000, 0x80000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",  0x180000, 0x80000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",  0x200000, 0x80000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",  0x280000, 0x80000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",  0x300000, 0x80000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",  0x380000, 0x80000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsava.key",    0x00, 0x14, CRC(8a3520f4) SHA1(3865972f2400b035ac71b8144b0437bade8dde22) )
ROM_END

ROM_START( vsavh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71h.p1", 0x000000, 0x80000, CRC(7cc62df8) SHA1(716ad31f0e253868a5b1b89943ddc980f130d5b7) )
	ROM_LOAD16_WORD_SWAP( "c71h.p2", 0x080000, 0x80000, CRC(d716f3b5) SHA1(7900440071eafa4d1559b1fa8faefaa0588a65d5) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",  0x100000, 0x80000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",  0x180000, 0x80000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",  0x200000, 0x80000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",  0x280000, 0x80000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",  0x300000, 0x80000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",  0x380000, 0x80000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsavh.key",    0x00, 0x14, CRC(a7dd6409) SHA1(5c60390e540ea538b1cd0b31635a8ffa1844fa24) )
ROM_END

ROM_START( vsavb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71b.p1", 0x000000, 0x080000, CRC(22ffba4b) SHA1(2c214c43115fc0978d5ebb814a19b8ee23faab3f) )
	ROM_LOAD16_WORD_SWAP( "c71b.p2", 0x080000, 0x080000, CRC(88624909) SHA1(052c8f49ac3bc3d231cfea38c5bad89c3380834e) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",  0x100000, 0x080000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",  0x180000, 0x080000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",  0x200000, 0x080000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",  0x280000, 0x080000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",  0x300000, 0x080000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",  0x380000, 0x080000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsavb.key",    0x00, 0x14, CRC(5274e635) SHA1(4a36bc9cc4497bdfaecf7dd638b5deace131c111) )
ROM_END

ROM_START( vsav2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c76.p1", 0x000000, 0x80000, CRC(89fd86b4) SHA1(a52f40618d7f12f1df5862ad8e15fea60bef22a2) )
	ROM_LOAD16_WORD_SWAP( "c76.p2", 0x080000, 0x80000, CRC(107c091b) SHA1(bf5c2e4339e1a66b3c819900cc9b723a537adf6b) )
	ROM_LOAD16_WORD_SWAP( "c76.p3", 0x100000, 0x80000, CRC(61979638) SHA1(4d5625a9a06926c1a42c8f6e3a4c943f17750ec2) )
	ROM_LOAD16_WORD_SWAP( "c76.p4", 0x180000, 0x80000, CRC(f37c5bc2) SHA1(d8c1040a6ee6b9fc677a6a32b99bf02b6a707812) )
	ROM_LOAD16_WORD_SWAP( "c76.p5", 0x200000, 0x80000, CRC(8f885809) SHA1(69dac07e1f483b6478f792d20a137d6a081fbea3) )
	ROM_LOAD16_WORD_SWAP( "c76.p6", 0x280000, 0x80000, CRC(2018c120) SHA1(de1184ab771c6f075cdefa744a28b09f78d91643) )
	ROM_LOAD16_WORD_SWAP( "c76.p7", 0x300000, 0x80000, CRC(fac3c217) SHA1(0e9dd54e401e6d7c4fe81107ffd27e42ca810fcb) )
	ROM_LOAD16_WORD_SWAP( "c76.p8", 0x380000, 0x80000, CRC(eb490213) SHA1(bf0416df66a33c7a4678ab4a047de334dfd3b31e) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c76.c1",   0x0000000, 0x400000, CRC(5c852f52) SHA1(528ce7fc9a0451e2e2d221dbf5e4a5796584e053) )
	ROM_LOAD64_WORD( "c76.c2",   0x0000002, 0x400000, CRC(a20f58af) SHA1(e873ad3e0fc8a06a5029113faf991f5c1b765316) )
	ROM_LOAD64_WORD( "c76.c3",   0x0000004, 0x400000, CRC(39db59ad) SHA1(da94f1529da82a6bf2129f51548412e1ab2b001a) )
	ROM_LOAD64_WORD( "c76.c4",   0x0000006, 0x400000, CRC(00c763a7) SHA1(0ff528e12e255ebf699101ac71f05b1f6bef7165) )
	ROM_LOAD64_WORD( "c76.c5",   0x1000000, 0x400000, CRC(cd09bd63) SHA1(e582b20a948ae54f52590496051688dbfae2bc9c) )
	ROM_LOAD64_WORD( "c76.c6",   0x1000002, 0x400000, CRC(e0182c15) SHA1(a924d53ab39f4d85173bdb92a197dde2db0dc3f7) )
	ROM_LOAD64_WORD( "c76.c7",   0x1000004, 0x400000, CRC(778dc4f6) SHA1(8d0cd1c387b4b6ac7f92bb2e5a25983856328cdc) )
	ROM_LOAD64_WORD( "c76.c8",   0x1000006, 0x400000, CRC(605d9d1d) SHA1(99bc27557741527ca678d7b6307164bc04ebedc6) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c76.m1",   0x00000, 0x08000, CRC(35190139) SHA1(07f8e53ea398461de5dcda9814dde7c09faf9f65) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c76.m2",   0x28000, 0x20000, CRC(c32dba09) SHA1(1fe337ff334fab79847f9677ba0e168e93daa1c8) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c76.q1",   0x000000, 0x400000, CRC(d67e47b7) SHA1(15a3f6779eccb10551ed94edf7e6e406a79b3de7) )
	ROM_LOAD16_WORD_SWAP( "c76.q2",   0x400000, 0x400000, CRC(6d020a14) SHA1(e98f862fac1e357c90949768bb2646263d9981a0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "vsav2.key",    0x00, 0x14, CRC(289028ce) SHA1(35779f0284dc15591493c8ec75ecda801148f3e0) )
ROM_END

ROM_START( xmcota )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.p1",  0x000000, 0x80000, CRC(5a726d13) SHA1(046d068f1f3c3d2a0b73df5b1f69a6afdde8d030) )
	ROM_LOAD16_WORD_SWAP( "c56.p2",  0x080000, 0x80000, CRC(06a83f3a) SHA1(2f30de88fccb104bfd038310483a63de7bee4b94) )
	ROM_LOAD16_WORD_SWAP( "c56.p3",  0x100000, 0x80000, CRC(87b0ed0f) SHA1(f4d78fdd9fcf864e909d9a2bb351b49a5f8ec7a0) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",  0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",  0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",  0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",  0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56.p8",  0x380000, 0x80000, CRC(cb36b0a4) SHA1(f21e3f2da405dfe43843ad32d381ea51f5d2fdd7) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcota.key",   0x00, 0x14, CRC(6665bbfb) SHA1(beb8ac662ac901edf1a0e8a7999c24df64b8d219) )
ROM_END

ROM_START( xmcotar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56r1.p1", 0x000000, 0x80000, CRC(a9a09b09) SHA1(e316f443d393139894592dbb1b676f3a2385ed14) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p2", 0x080000, 0x80000, CRC(52fa2106) SHA1(6904eef0fb11e44046e160a1c0ff6ea48337f630) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3", 0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8", 0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcota.key",   0x00, 0x14, CRC(6665bbfb) SHA1(beb8ac662ac901edf1a0e8a7999c24df64b8d219) )
ROM_END

ROM_START( xmcotau )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56u.p1",  0x000000, 0x80000, CRC(0bafeb0e) SHA1(170c819bd7ffafefb9b2a587509bdf2c0415474b) )
	ROM_LOAD16_WORD_SWAP( "c56u.p2",  0x080000, 0x80000, CRC(c29bdae3) SHA1(c605a4fd90336459c7b24cd7b2b243eef10f6407) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3", 0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8", 0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotau.key",  0x00, 0x14, CRC(623d3357) SHA1(27e8209e5a0917b127f0e65c53e95c28d78e7a0e) )
ROM_END

ROM_START( xmcotah )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56h.p1",  0x000000, 0x80000, CRC(e4b85a90) SHA1(1eaf94ce42438eea45cd5c813f2859abf258dd3a) )
	ROM_LOAD16_WORD_SWAP( "c56h.p2",  0x080000, 0x80000, CRC(7dfe1406) SHA1(4ddc0a8947d78ce587220f8188c8a8f00c7372c4) )
	ROM_LOAD16_WORD_SWAP( "c56.p3",   0x100000, 0x80000, CRC(87b0ed0f) SHA1(f4d78fdd9fcf864e909d9a2bb351b49a5f8ec7a0) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56.p8",   0x380000, 0x80000, CRC(cb36b0a4) SHA1(f21e3f2da405dfe43843ad32d381ea51f5d2fdd7) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotah.key",  0x00, 0x14, CRC(c9a45a5a) SHA1(6af21fe3c46133da9e0854375ff7ab2eaec64ce3) )
ROM_END

ROM_START( xmcotahr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56hr1.p1", 0x000000, 0x80000, CRC(63b0a84f) SHA1(4a0d604ff11e68313a0711702803390cb0d2f234) )
	ROM_LOAD16_WORD_SWAP( "c56hr1.p2", 0x080000, 0x80000, CRC(b1b9b727) SHA1(b04ae3b7aab88cbb1a55068343ff9a9806d331a4) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3", 0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8", 0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotah.key",  0x00, 0x14, CRC(c9a45a5a) SHA1(6af21fe3c46133da9e0854375ff7ab2eaec64ce3) )
ROM_END

ROM_START( xmcotaj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56j.p1",  0x000000, 0x80000, CRC(0df29f5f) SHA1(83993ea90e7a602c3db137d08c008dcd9bee3055) )
	ROM_LOAD16_WORD_SWAP( "c56j.p2",  0x080000, 0x80000, CRC(4a65833b) SHA1(cd899674ba6448fb3841247d3f434e82b19c5399) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3", 0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8", 0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaj.key",  0x00, 0x14, CRC(d278b4ac) SHA1(06d94e8a203172792f405c73f9cbb37f4738429f) )
ROM_END

ROM_START( xmcotaj1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56j1.p1",   0x000000, 0x80000, CRC(79086d62) SHA1(d99a48bd40593aa9c1ff1fcbfc40cfe3bc882fc5) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p2",   0x080000, 0x80000, CRC(38eed613) SHA1(86ecb58bf03adda705a39a9779862fe2c85cadad) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p3",   0x100000, 0x80000, CRC(c3ed62a2) SHA1(4e3317d7ca981e33318822103a16e59f4ce20deb) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p4",   0x180000, 0x80000, CRC(f03c52e1) SHA1(904b2312ee594f5ece0484cad0eed25cc758185e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p5",   0x200000, 0x80000, CRC(325626b1) SHA1(3f3a0aabbe5ffad8136ac91e0de785103b16059b) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p6",   0x280000, 0x80000, CRC(7194ea10) SHA1(40a5892d816f24cbfd4c310792eeabf689c6fa7e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p7",   0x300000, 0x80000, CRC(ae946df3) SHA1(733671f76d766bda7110df9d338791cc5202b050) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p8",   0x380000, 0x80000, CRC(32a6be1d) SHA1(8f5fcb33b528abed670b4fc3fa62431a6e033c56) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaj.key",  0x00, 0x14, CRC(d278b4ac) SHA1(06d94e8a203172792f405c73f9cbb37f4738429f) )
ROM_END

ROM_START( xmcotaj2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56j2.p1",   0x000000, 0x80000, CRC(c8175fb3) SHA1(ea25bd165f8794324a1e07719312798cf9742924) )
	ROM_LOAD16_WORD_SWAP( "c56j2.p2",   0x080000, 0x80000, CRC(54b3fba3) SHA1(47eaff5d36a45e4196f87ed3d02e54d5407e7962) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p3",   0x100000, 0x80000, CRC(c3ed62a2) SHA1(4e3317d7ca981e33318822103a16e59f4ce20deb) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p4",   0x180000, 0x80000, CRC(f03c52e1) SHA1(904b2312ee594f5ece0484cad0eed25cc758185e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p5",   0x200000, 0x80000, CRC(325626b1) SHA1(3f3a0aabbe5ffad8136ac91e0de785103b16059b) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p6",   0x280000, 0x80000, CRC(7194ea10) SHA1(40a5892d816f24cbfd4c310792eeabf689c6fa7e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p7",   0x300000, 0x80000, CRC(ae946df3) SHA1(733671f76d766bda7110df9d338791cc5202b050) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p8",   0x380000, 0x80000, CRC(32a6be1d) SHA1(8f5fcb33b528abed670b4fc3fa62431a6e033c56) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaj.key",  0x00, 0x14, CRC(d278b4ac) SHA1(06d94e8a203172792f405c73f9cbb37f4738429f) )
ROM_END

ROM_START( xmcotaj3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56j3.p1",   0x000000, 0x80000, CRC(00761611) SHA1(e780dbe1d21a0d5b6981f0395942c9fa59688113) )
	ROM_LOAD16_WORD_SWAP( "c56j3.p2",   0x080000, 0x80000, CRC(614d3f60) SHA1(2272ae243557562a0bc85d2cd2b37dd876f6902c) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p3",   0x100000, 0x80000, CRC(c3ed62a2) SHA1(4e3317d7ca981e33318822103a16e59f4ce20deb) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p4",   0x180000, 0x80000, CRC(f03c52e1) SHA1(904b2312ee594f5ece0484cad0eed25cc758185e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p5",   0x200000, 0x80000, CRC(325626b1) SHA1(3f3a0aabbe5ffad8136ac91e0de785103b16059b) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p6",   0x280000, 0x80000, CRC(7194ea10) SHA1(40a5892d816f24cbfd4c310792eeabf689c6fa7e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p7",   0x300000, 0x80000, CRC(ae946df3) SHA1(733671f76d766bda7110df9d338791cc5202b050) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p8",   0x380000, 0x80000, CRC(32a6be1d) SHA1(8f5fcb33b528abed670b4fc3fa62431a6e033c56) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaj.key",  0x00, 0x14, CRC(d278b4ac) SHA1(06d94e8a203172792f405c73f9cbb37f4738429f) )
ROM_END

ROM_START( xmcotajr )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56jr.p1",  0x000000, 0x80000, CRC(7ab19acf) SHA1(ca02e58f1d713ee74c6c1515772da0ca26f9deb9) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p2",  0x080000, 0x80000, CRC(7615dd21) SHA1(f393c985ae1c7f378f9184fd2c8530b7494ba06d) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p3",  0x100000, 0x80000, CRC(0303d672) SHA1(4816b5ac6a9bf78665112d54a8f3569d590721b2) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p4",  0x180000, 0x80000, CRC(332839a5) SHA1(c7b80fad1130cc025de3fad372b727d360adc47b) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p5",  0x200000, 0x80000, CRC(6255e8d5) SHA1(159f7983b93ee82c2012a3a6a9f451a521f98ed6) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p6",  0x280000, 0x80000, CRC(b8ebe77c) SHA1(3ef06f19f2ba0aee8be9d9a9f0b1742f9ee1282a) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p7",  0x300000, 0x80000, CRC(5440d950) SHA1(d5338718964b6f36655ddd62dbbe2bbfb44db114) )
	ROM_LOAD16_WORD_SWAP( "c56jr.p8",  0x380000, 0x80000, CRC(b8296966) SHA1(b13496956d8288302bf5c9a7478d4791e41e1bfd) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56jr.m1",   0x00000, 0x08000, CRC(7178336e) SHA1(d94cddcc144336fa3ee2778b3531badcc4646e9d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56jr.m2",   0x28000, 0x20000, CRC(0ec58501) SHA1(3af500049f901897086bd35b83ca83f4bbc8b3f6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaj.key",  0x00, 0x14, CRC(d278b4ac) SHA1(06d94e8a203172792f405c73f9cbb37f4738429f) )
ROM_END

ROM_START( xmcotaa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56a.p1",  0x000000, 0x80000, CRC(f1ade6e7) SHA1(b212b6107a7ab01db3a60ccc7e1376fdba6f2e6a) )
	ROM_LOAD16_WORD_SWAP( "c56a.p2",  0x080000, 0x80000, CRC(b5a8843d) SHA1(e45381725a7356760dc71e027f3b7059e5919621) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3", 0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",   0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",   0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",   0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",   0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8", 0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaa.key",  0x00, 0x14, CRC(3fdd2d42) SHA1(effcfdf03e71d386356c32c2803c8c841871e24c) )
ROM_END

ROM_START( xmcotaar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56ar1.p1",  0x000000, 0x80000, CRC(2280d03f) SHA1(554753ff7abd5b66d2ffede64e4c459f3df42209) )
	ROM_LOAD16_WORD_SWAP( "c56ar1.p2",  0x080000, 0x80000, CRC(ebbbf11e) SHA1(7f6c4555330923641310fae2beda1047a1c711b4) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p3",   0x100000, 0x80000, CRC(c3ed62a2) SHA1(4e3317d7ca981e33318822103a16e59f4ce20deb) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p4",   0x180000, 0x80000, CRC(f03c52e1) SHA1(904b2312ee594f5ece0484cad0eed25cc758185e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p5",   0x200000, 0x80000, CRC(325626b1) SHA1(3f3a0aabbe5ffad8136ac91e0de785103b16059b) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p6",   0x280000, 0x80000, CRC(7194ea10) SHA1(40a5892d816f24cbfd4c310792eeabf689c6fa7e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p7",   0x300000, 0x80000, CRC(ae946df3) SHA1(733671f76d766bda7110df9d338791cc5202b050) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p8",   0x380000, 0x80000, CRC(32a6be1d) SHA1(8f5fcb33b528abed670b4fc3fa62431a6e033c56) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaa.key",  0x00, 0x14, CRC(3fdd2d42) SHA1(effcfdf03e71d386356c32c2803c8c841871e24c) )
ROM_END

ROM_START( xmcotaar2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56ar2.p1",  0x000000, 0x80000, CRC(7df8b27e) SHA1(2f0ce6b10857e04ddaf7a76edf126282c53511b3) )
	ROM_LOAD16_WORD_SWAP( "c56ar2.p2",  0x080000, 0x80000, CRC(b44e30a7) SHA1(27b0a8b06aa11673dd145717c6286eb27186cf79) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p3",   0x100000, 0x80000, CRC(c3ed62a2) SHA1(4e3317d7ca981e33318822103a16e59f4ce20deb) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p4",   0x180000, 0x80000, CRC(f03c52e1) SHA1(904b2312ee594f5ece0484cad0eed25cc758185e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p5",   0x200000, 0x80000, CRC(325626b1) SHA1(3f3a0aabbe5ffad8136ac91e0de785103b16059b) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p6",   0x280000, 0x80000, CRC(7194ea10) SHA1(40a5892d816f24cbfd4c310792eeabf689c6fa7e) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p7",   0x300000, 0x80000, CRC(ae946df3) SHA1(733671f76d766bda7110df9d338791cc5202b050) )
	ROM_LOAD16_WORD_SWAP( "c56j1.p8",   0x380000, 0x80000, CRC(32a6be1d) SHA1(8f5fcb33b528abed670b4fc3fa62431a6e033c56) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotaa.key",  0x00, 0x14, CRC(3fdd2d42) SHA1(effcfdf03e71d386356c32c2803c8c841871e24c) )
ROM_END

ROM_START( xmcotab )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56b.p1", 0x000000, 0x80000, CRC(ab48bcb0) SHA1(856168752cdd4d248eb43b2794e989a3409dc8b0) )
	ROM_LOAD16_WORD_SWAP( "c56b.p2", 0x080000, 0x80000, CRC(8d8fcbb1) SHA1(f98712374006d44e6121abc65ba2c78c5ebf6de5) )
	ROM_LOAD16_WORD_SWAP( "c56.p3",  0x100000, 0x80000, CRC(87b0ed0f) SHA1(f4d78fdd9fcf864e909d9a2bb351b49a5f8ec7a0) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",  0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",  0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",  0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",  0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56.p8",  0x380000, 0x80000, CRC(cb36b0a4) SHA1(f21e3f2da405dfe43843ad32d381ea51f5d2fdd7) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmcotab.key",  0x00, 0x14, CRC(bb70f10f) SHA1(99f9d1eccd81c906b6adb08b5f1547636e649298) )
ROM_END

ROM_START( xmvsf )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.p1", 0x000000, 0x80000, CRC(db06413f) SHA1(c6d8aa1e43fc541e5b4e938258f27ab9ee30ca33) )
	ROM_LOAD16_WORD_SWAP( "c69.p2", 0x080000, 0x80000, CRC(ef015aef) SHA1(d3504cb8c38f720b1f4528157266db60c8c6c075) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsf.key",    0x00, 0x14, CRC(d5c07311) SHA1(1b401ffc241436c4869486c174774b67e3bf3df8) )
ROM_END

ROM_START( xmvsfr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69r1.p1", 0x000000, 0x80000, CRC(5ae5bd3b) SHA1(f687f018008cef24f86f53373c3f5547741a4c5b) )
	ROM_LOAD16_WORD_SWAP( "c69r1.p2", 0x080000, 0x80000, CRC(5eb9c02e) SHA1(25a392913213b98ce1bbd463bf5e5e10729bde0c) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",   0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",   0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsf.key",    0x00, 0x14, CRC(d5c07311) SHA1(1b401ffc241436c4869486c174774b67e3bf3df8) )
ROM_END

ROM_START( xmvsfu )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69u.p1", 0x000000, 0x80000, CRC(8739ef61) SHA1(2eb5912d3026bed0f720d28e1bf3a7ceb5b80803) )
	ROM_LOAD16_WORD_SWAP( "c69u.p2", 0x080000, 0x80000, CRC(e11d35c1) SHA1(d838199b2767d9f02fa0f103c5d587a4c78c0d21) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfu.key",   0x00, 0x14, CRC(eca13458) SHA1(4de2691de7de104dbba1f10b3f738e6f2b708a15) )
ROM_END

ROM_START( xmvsfur1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69ur1.p1", 0x000000, 0x80000, CRC(5481155a) SHA1(799a2488684cbead33206498d13261b79624a46e) )
	ROM_LOAD16_WORD_SWAP( "c69ur1.p2", 0x080000, 0x80000, CRC(1e236388) SHA1(329c08103840fadbc4176785c4b24013a7a2b1bc) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",   0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",   0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfu.key",   0x00, 0x14, CRC(eca13458) SHA1(4de2691de7de104dbba1f10b3f738e6f2b708a15) )
ROM_END

ROM_START( xmvsfur2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69ur2.p1", 0x000000, 0x80000, CRC(bd8b152f) SHA1(6b029b7314ce2516c67e5a71508f86aa01d30ab8) )
	ROM_LOAD16_WORD_SWAP( "c69ur2.p2", 0x080000, 0x80000, CRC(7c7d1da3) SHA1(96dd1f83c8f8053177b91ad31c4c051b28dd0208) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfu.key",   0x00, 0x14, CRC(eca13458) SHA1(4de2691de7de104dbba1f10b3f738e6f2b708a15) )
ROM_END

ROM_START( xmvsfj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69j.p1", 0x000000, 0x80000, CRC(2a167526) SHA1(779d1be2149b1feb2c5f710ed70b162e11bd0de4) )
	ROM_LOAD16_WORD_SWAP( "c69j.p2", 0x080000, 0x80000, CRC(d993436b) SHA1(b3279f4fe3728700adf086bf27133c5a74c6302c) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfj.key",   0x00, 0x14, CRC(87576cda) SHA1(65905400f4462c175baa93b43e015e8596def31b) )
ROM_END

ROM_START( xmvsfjr1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69jr1.p1", 0x000000, 0x80000, CRC(ef24da96) SHA1(8f4a2a626a059bcf36048770153a9ffc85bba304) )
	ROM_LOAD16_WORD_SWAP( "c69jr1.p2", 0x080000, 0x80000, CRC(70a59b35) SHA1(786d9b243373024735848f785503c6aa883b1c2f) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfj.key",   0x00, 0x14, CRC(87576cda) SHA1(65905400f4462c175baa93b43e015e8596def31b) )
ROM_END

ROM_START( xmvsfjr2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69jr2.p1", 0x000000, 0x80000, CRC(beb81de9) SHA1(fce0d43b193a521d026be6508a91be6e2d03f480) )
	ROM_LOAD16_WORD_SWAP( "c69jr2.p2", 0x080000, 0x80000, CRC(23d11271) SHA1(45e4ac52001f0c2b6cd6e07413b5e503c2b90329) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfj.key",   0x00, 0x14, CRC(87576cda) SHA1(65905400f4462c175baa93b43e015e8596def31b) )
ROM_END

ROM_START( xmvsfjr3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69jr3.p1", 0x000000, 0x80000, CRC(180656a1) SHA1(aec2dfcfe8bcab03a48f749977e6f08fc21558bc) )
	ROM_LOAD16_WORD_SWAP( "c69jr3.p2", 0x080000, 0x80000, CRC(5832811c) SHA1(e900b343241310d4dd1b45f42573e1e90f2dcbda) )
	ROM_LOAD16_WORD_SWAP( "c69jr3.p3",   0x100000, 0x80000, CRC(030e0e1e) SHA1(164e3023bb1965768448e1bf6c45ff9e0ac964c7) )
	ROM_LOAD16_WORD_SWAP( "c69jr3.p4",   0x180000, 0x80000, CRC(5d04a8ff) SHA1(3b5a524f3f1c4b540c88275418bdaf50c7186713) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfj.key",   0x00, 0x14, CRC(87576cda) SHA1(65905400f4462c175baa93b43e015e8596def31b) )
ROM_END

ROM_START( xmvsfa )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69a.p1", 0x000000, 0x80000, CRC(d0cca7a8) SHA1(70e0dd0725a52208e9e71fed82fba1d851a6bb42) )
	ROM_LOAD16_WORD_SWAP( "c69a.p2", 0x080000, 0x80000, CRC(8c8e76fd) SHA1(ac1c8200951131bea0bda417b6bc2f77130b5fdd) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfa.key",   0x00, 0x14, CRC(44941468) SHA1(1938dee4bab79200576777a70b814ff1179135c4) )
ROM_END

ROM_START( xmvsfar1 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69ar1.p1",  0x000000, 0x80000, CRC(520054df) SHA1(98fd30aeb4cc9120607434f258a1c00204d75d63) )
	ROM_LOAD16_WORD_SWAP( "c69ar1.p2",  0x080000, 0x80000, CRC(13086e55) SHA1(0d9a4b2a4278a98423155702c66df3f2e1e8ef56) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfa.key",   0x00, 0x14, CRC(44941468) SHA1(1938dee4bab79200576777a70b814ff1179135c4) )
ROM_END

ROM_START( xmvsfar2 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69ar2.p1", 0x000000, 0x80000, CRC(9bdde21c) SHA1(56c295d9e908a1496d6a08ff1cd10c87de1d4ff5) )
	ROM_LOAD16_WORD_SWAP( "c69ar2.p2", 0x080000, 0x80000, CRC(33300edf) SHA1(8ec4203c1bc23a6284c47ee873b151da9f072edc) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfa.key",   0x00, 0x14, CRC(44941468) SHA1(1938dee4bab79200576777a70b814ff1179135c4) )
ROM_END

ROM_START( xmvsfar3 )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69ar3.p1", 0x000000, 0x80000, CRC(2b164fd7) SHA1(90eefa309202978c914897f30b2e6caf23fcd9f3) )
	ROM_LOAD16_WORD_SWAP( "c69ar3.p2", 0x080000, 0x80000, CRC(2d32f039) SHA1(515bdb1ad99106a5b8aa3a94be4fc4ceb31a6711) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfa.key",   0x00, 0x14, CRC(44941468) SHA1(1938dee4bab79200576777a70b814ff1179135c4) )
ROM_END

ROM_START( xmvsfh )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69h.p1", 0x000000, 0x80000, CRC(d4fffb04) SHA1(989ed975cfc1318998c2da26f450949bdac41d0c) )
	ROM_LOAD16_WORD_SWAP( "c69h.p2", 0x080000, 0x80000, CRC(1b4ea638) SHA1(7523be63c1eef153e47fc8e1c10eb99ab40b94a0) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfh.key",   0x00, 0x14, CRC(f632a36b) SHA1(b3914e72d391e0d772542f7863680232fe3bc6f7) )
ROM_END

ROM_START( xmvsfb )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69b.p1", 0x000000, 0x80000, CRC(05baccca) SHA1(7124e49e2180f77674ae014257a816cd4409d613) )
	ROM_LOAD16_WORD_SWAP( "c69b.p2", 0x080000, 0x80000, CRC(e350c755) SHA1(5e615fd4b9954410c05b34151fae70d910340a6c) )
	ROM_LOAD16_WORD_SWAP( "c69.p3",  0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",  0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
	ROM_LOAD16_WORD_SWAP( "c69.p5",   0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",   0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",   0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "xmvsfb.key",   0x00, 0x14, CRC(f0384798) SHA1(a221b888d9ed1f9d1da0aab1b4749c3c6d2586f6) )
ROM_END


/*************************************
 *
 *  Games initialization
 *
 *************************************/

void cps2_state::init_digital_volume()
{
	m_cps2digitalvolumelevel = 39; /* maximum */
	m_cps2disabledigitalvolume = 0;

	/* create a timer to update our volume state from the fake switches - read it every 6 frames or so to enable some granularity */
	m_digital_volume_timer = machine().scheduler().timer_alloc(timer_expired_delegate(FUNC(cps2_state::cps2_update_digital_volume),this));
	m_digital_volume_timer->adjust(attotime::from_msec(100), 0, attotime::from_msec(100));
}



void cps2_state::init_cps2()
{
	/* Decrypt the game - see machine/cps2crypt.cpp */
	init_cps2crypt();
	init_cps2nc();
}

void cps2_state::init_cps2nc()
{
	/* Initialize some video elements */
	init_cps2_video();

	m_cps2networkpresent = 0;

	init_digital_volume();

	m_maincpu->set_clock_scale(0.7375f); /* RAM access waitstates etc. aren't emulated - slow the CPU to compensate */
}


void cps2_state::init_ssf2tb()
{
	init_cps2();

	m_cps2networkpresent = 0;

	/* we don't emulate the network board, so don't say it's present for now, otherwise the game will
	   attempt to boot in tournament mode and fail */
	//m_cps2networkpresent = 1;

}

void cps2_state::init_pzloop2()
{
	init_cps2();

	m_readpaddle = 0;
	m_cps2_dial_type = 1;

	save_item(NAME(m_readpaddle));

	m_maincpu->space(AS_PROGRAM).install_read_handler(0x804000, 0x804001, read16smo_delegate(*this, FUNC(cps2_state::joy_or_paddle_r)));
}

void cps2_state::init_singbrd()
{
	init_cps2();

	/* the single board games don't have a digital volume switch */
	m_cps2disabledigitalvolume = 1;
	m_digital_volume_timer->adjust(attotime::never, 0, attotime::never);
}

u16 cps2_state::gigaman2_dummyqsound_r(offs_t offset)
{
	return m_gigaman2_dummyqsound_ram[offset];
}

void cps2_state::gigaman2_dummyqsound_w(offs_t offset, u16 data)
{
	m_gigaman2_dummyqsound_ram[offset] = data;
}

/* rearrange the graphics data into the normal order */
void cps2_state::gigaman2_gfx_reorder()
{
	int i;
	int length = memregion( "gfx" )->bytes();
	u16 *rom = (u16 *)memregion("gfx")->base();
	std::vector<u16> buf( length );

	memcpy (&buf[0], rom, length);

	for (i = 0; i < length/2; i++) {
		rom[i] = buf[((i & ~7) >> 2) | ((i & 4) << 18) | ((i & 2) >> 1) | ((i & 1) << 21)];
	}
}

void cps2_state::init_gigaman2()
{
	address_space &space = m_maincpu->space(AS_PROGRAM);

	gigaman2_gfx_reorder();

	init_cps2nc();

	m_gigaman2_dummyqsound_ram = std::make_unique<u16[]>(0x20000 / 2);
	save_pointer(NAME(m_gigaman2_dummyqsound_ram.get()), 0x20000 / 2);

	space.install_readwrite_handler(0x618000, 0x619fff, read16sm_delegate(*this, FUNC(cps2_state::gigaman2_dummyqsound_r)), write16sm_delegate(*this, FUNC(cps2_state::gigaman2_dummyqsound_w))); // no qsound..

	memcpy(m_decrypted_opcodes, memregion("maincpu")->base()+0x200000, 0x200000);

	/* no digital volume switches on this? */
	m_digital_volume_timer->adjust(attotime::never, 0, attotime::never);
}

void cps2_state::init_ecofghtr()
{
	init_cps2();

	m_readpaddle = 0;
	m_cps2_dial_type = 2;

	save_item(NAME(m_readpaddle));

	m_maincpu->space(AS_PROGRAM).install_read_handler(0x804000, 0x804001, read16smo_delegate(*this, FUNC(cps2_state::joy_or_paddle_ecofghtr_r)));

}



/*************************************
 *
 *  Game drivers
 *
 *************************************/

GAME( 1993, ssf2,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (World 931005)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2r1,     ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (World 930911)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2u,      ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (USA 930911)",                     MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2us2,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg","Super Street Fighter II: The New Challengers Super 2 (USA 930911)",             MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2a,      ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Asia 931005)",                    MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2ar1,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Asia 930914)",                    MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2j,      ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Japan 931005)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2jr1,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Japan 930911)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2jr2,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Japan 930910)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2h,      ssf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II: The New Challengers (Hispanic 930911)",                MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tb,     ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (World 931119)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbr1,   ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (World 930911)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbu,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (USA 930911)",                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbj,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (Japan 931005)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbj1,   ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (Japan 930911)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tba,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (Asia 931005)",                  MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbh,    ssf2,     cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "Capcom", "Super Street Fighter II: The Tournament Battle (Hispanic 931005)",              MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtr,   0,        cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Eco Fighters (World 931203)",                                                   MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtru,  ecofghtr, cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Eco Fighters (USA 940215)",                                                     MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtru1, ecofghtr, cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Eco Fighters (USA 931203)",                                                     MACHINE_SUPPORTS_SAVE )
GAME( 1993, uecology,   ecofghtr, cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Ultimate Ecology (Japan 931203)",                                               MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtra,  ecofghtr, cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Eco Fighters (Asia 931203)",                                                    MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtrh,  ecofghtr, cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "Capcom", "Eco Fighters (Hispanic 931203)",                                                MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtod,      0,        cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Euro 940412)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodr1,    ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Euro 940113)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodu,     ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (USA 940125)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodur1,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (USA 940113)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodj,     ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Japan 940412)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodjr1,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Japan 940125)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodjr2,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Japan 940113)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtoda,     ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Asia 940412)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodar1,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Asia 940113)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodh,     ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Hispanic 940412)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodhr1,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Hispanic 940125)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1993, ddtodhr2,   ddtod,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Tower of Doom (Hispanic 940113)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2t,      0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II Turbo (World 940223)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2ta,     ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II Turbo (Asia 940223)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2th,     ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II Turbo (Hispanic 940223)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2tu,     ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II Turbo (USA 940323)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2tur1,   ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II Turbo (USA 940223)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2xj,     ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II X: Grand Master Challenge (Japan 940311)",              MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2xjr1,   ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II X: Grand Master Challenge (Japan 940223)",              MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2xjr1r,  ssf2t,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Street Fighter II X: Grand Master Challenge (Japan 940223 rent version)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, avsp,       0,        cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Alien vs. Predator (Euro 940520)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1994, avspu,      avsp,     cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Alien vs. Predator (USA 940520)",                                               MACHINE_SUPPORTS_SAVE )
GAME( 1994, avspj,      avsp,     cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Alien vs. Predator (Japan 940520)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1994, avspa,      avsp,     cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Alien vs. Predator (Asia 940520)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1994, avsph,      avsp,     cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Alien vs. Predator (Hispanic 940520)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlk,      0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Darkstalkers: The Night Warriors (Euro 940705)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlku,     dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Darkstalkers: The Night Warriors (USA 940818)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlkur1,   dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Darkstalkers: The Night Warriors (USA 940705)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlka,     dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Darkstalkers: The Night Warriors (Asia 940705)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlkh,     dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Darkstalkers: The Night Warriors (Hispanic 940818)",                            MACHINE_SUPPORTS_SAVE )
GAME( 1994, vampj,      dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire: The Night Warriors (Japan 940705)",                                    MACHINE_SUPPORTS_SAVE ) // Partial update set? Only rom 04 is "B" revision
GAME( 1994, vampja,     dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire: The Night Warriors (Japan 940705 alt)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, vampjr1,    dstlk,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire: The Night Warriors (Japan 940630)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, ringdest,   0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Ring of Destruction: Slammasters II (Euro 940902)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1994, ringdesta,  ringdest, cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Ring of Destruction: Slammasters II (Asia 940831)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1994, ringdesth,  ringdest, cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Ring of Destruction: Slammasters II (Hispanic 940902)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1994, ringdestb,  ringdest, cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Ring of Destruction: Slammasters II (Brazil 940902)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1994, smbomb,     ringdest, cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Muscle Bomber: The International Blowout (Japan 940831)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, smbombr1,   ringdest, cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Muscle Bomber: The International Blowout (Japan 940808)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwar,     0,        cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (Euro 941024)",                                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwarr1,   armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (Euro 941011)",                                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwaru,    armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (USA 941024)",                                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwaru1,   armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (USA 940920)",                                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwarb,    armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (Brazil 941024)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1994, pgear,      armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Powered Gear: Strategic Variant Armor Equipment (Japan 941024)",                MACHINE_SUPPORTS_SAVE )
GAME( 1994, pgearr1,    armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Powered Gear: Strategic Variant Armor Equipment (Japan 940916)",                MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwara,    armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (Asia 941024)",                                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwarar1,  armwar,   cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Armored Warriors (Asia 940920)",                                                MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcota,     0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Euro 950331)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotar1,   xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Euro 950105)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotau,    xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (USA 950105)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotab,    xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Brazil 950331)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotah,    xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Hispanic 950331)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotahr1,  xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Hispanic 950105)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaj,    xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Japan 950105)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaj1,   xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Japan 941222)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaj2,   xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Japan 941219)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaj3,   xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Japan 941217)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotajr,   xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Japan 941208 rent version)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaa,    xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Asia 950105)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaar1,  xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Asia 941219)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotaar2,  xmcota,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men: Children of the Atom (Asia 941217)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarr,      0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Night Warriors: Darkstalkers' Revenge (Euro 950316)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarru,     nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Night Warriors: Darkstalkers' Revenge (USA 950406)",                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarrh,     nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Night Warriors: Darkstalkers' Revenge (Hispanic 950403)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarrb,     nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Night Warriors: Darkstalkers' Revenge (Brazil 950403)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarra,     nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Night Warriors: Darkstalkers' Revenge (Asia 950302)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, vhuntj,     nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter: Darkstalkers' Revenge (Japan 950316)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, vhuntjr1s,  nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter: Darkstalkers' Revenge (Japan 950307 stop version)",             MACHINE_SUPPORTS_SAVE )
GAME( 1995, vhuntjr1,   nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter: Darkstalkers' Revenge (Japan 950307)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, vhuntjr2,   nwarr,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter: Darkstalkers' Revenge (Japan 950302)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, cybots,     0,        cps2,     cybots,    cps2_state, init_cps2,     ROT0,   "Capcom", "Cyberbots: Fullmetal Madness (Euro 950424)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1995, cybotsu,    cybots,   cps2,     cybots,    cps2_state, init_cps2,     ROT0,   "Capcom", "Cyberbots: Fullmetal Madness (USA 950424)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1995, cybotsj,    cybots,   cps2,     cybots,    cps2_state, init_cps2,     ROT0,   "Capcom", "Cyberbots: Fullmetal Madness (Japan 950420)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfa,        0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha: Warriors' Dreams (Euro 950727)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfar1,      sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha: Warriors' Dreams (Euro 950718)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfar2,      sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha: Warriors' Dreams (Euro 950627)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfar3,      sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha: Warriors' Dreams (Euro 950605)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfau,       sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha: Warriors' Dreams (USA 950627)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfza,       sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Asia 950627)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzar1,     sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Asia 950605)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzj,       sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Japan 950727)",                                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzjr1,     sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Japan 950627)",                                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzjr2,     sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Japan 950605)",                                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzh,       sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Hispanic 950718)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzhr1,     sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Hispanic 950627)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzb,       sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Brazil 951109)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfzbr1,     sfa,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero (Brazil 950727)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, mmancp2u,   megaman,  cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man: The Power Battle (CPS2, USA 951006, SAMPLE Version)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1995, mmancp2ur1, megaman,  cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man: The Power Battle (CPS2, USA 950926, SAMPLE Version)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1995, mmancp2ur2, megaman,  cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man: The Power Battle (CPS2, USA 950925, SAMPLE Version)",                 MACHINE_SUPPORTS_SAVE )
GAME( 1995, rmancp2j,   megaman,  cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Rockman: The Power Battle (CPS2, Japan 950922)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1995, msh,        0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Euro 951024)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshu,       msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (USA 951024)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshj,       msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Japan 951117)",                                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshjr1,     msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Japan 951024)",                                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, msha,       msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Asia 951024)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshh,       msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Hispanic 951117)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshb,       msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Brazil 951117)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshbr1,     msh,      cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes (Brazil 951024)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xx,       0,        cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Euro 960104)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxu,      19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (USA 951207)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxa,      19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Asia 960104)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxar1,    19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Asia 951207)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxj,      19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Japan 960104, yellow case)",                     MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxjr1,    19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Japan 951225)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxjr2,    19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Japan 951207)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxh,      19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Hispanic 951218)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxb,      19xx,     cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "Capcom", "19XX: The War Against Destiny (Brazil 951218)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsom,      0,        cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Euro 960619)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomr1,    ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Euro 960223)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomr2,    ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Euro 960209)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomr3,    ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Euro 960208)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomu,     ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (USA 960619)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomur1,   ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (USA 960209)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomj,     ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Japan 960619)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomjr1,   ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Japan 960206)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomjr2,   ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Japan 960223)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsoma,     ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Asia 960619)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomar1,   ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Asia 960208)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomh,     ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Hispanic 960223)",                     MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomb,     ddsom,    cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "Capcom", "Dungeons & Dragons: Shadow over Mystara (Brazil 960223)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfa2,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 2 (Euro 960229)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfa2u,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 2 (USA 960430)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfa2ur1,    sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 2 (USA 960306)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2j,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Japan 960430)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2jr1,    sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Japan 960227)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2a,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Asia 960227)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2b,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Brazil 960531)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2br1,    sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Brazil 960304)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2h,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Hispanic 960304)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2n,      sfa2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 (Oceania 960229)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2al,     0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 Alpha (Asia 960826)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2alr1,   sfz2al,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 Alpha (Asia 960805)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2alj,    sfz2al,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 Alpha (Japan 960805)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2alh,    sfz2al,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 Alpha (Hispanic 960813)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2alb,    sfz2al,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 2 Alpha (Brazil 960813)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2t,      0,        cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Puzzle Fighter II Turbo (Euro 960529)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2tu,     spf2t,    cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Puzzle Fighter II Turbo (USA 960620)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2xj,     spf2t,    cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Puzzle Fighter II X (Japan 960531)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2ta,     spf2t,    cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Puzzle Fighter II Turbo (Asia 960529)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2th,     spf2t,    cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Puzzle Fighter II Turbo (Hispanic 960531)",                               MACHINE_SUPPORTS_SAVE )
GAME( 1996, megaman2,   0,        cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man 2: The Power Fighters (USA 960708)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1996, megaman2a,  megaman2, cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man 2: The Power Fighters (Asia 960708)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1996, rockman2j,  megaman2, cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Rockman 2: The Power Fighters (Japan 960708)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1996, megaman2h,  megaman2, cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Mega Man 2: The Power Fighters (Hispanic 960712)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1996, qndream,    0,        cps2,     qndream,   cps2_state, init_cps2,     ROT0,   "Capcom", "Quiz Nanairo Dreams: Nijiirochou no Kiseki (Japan 96086)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsf,      0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Euro 961004)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfr1,    xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Euro 960910)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfu,     xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (USA 961023)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfur1,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (USA 961004)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfur2,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (USA 960910)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfj,     xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Japan 961023)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfjr1,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Japan 961004)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfjr2,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Japan 960910)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfjr3,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Japan 960909)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfa,     xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Asia 961023)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfar1,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Asia 961004)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfar2,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Asia 960919)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfar3,   xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Asia 960910)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfh,     xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Hispanic 961004)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfb,     xmvsf,    cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "X-Men Vs. Street Fighter (Brazil 961023)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1997, batcir,     0,        cps2,     cps2_4p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Battle Circuit (Euro 970319)",                                                  MACHINE_SUPPORTS_SAVE )
GAME( 1997, batcira,    batcir,   cps2,     cps2_4p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Battle Circuit (Asia 970319)",                                                  MACHINE_SUPPORTS_SAVE )
GAME( 1997, batcirj,    batcir,   cps2,     cps2_4p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Battle Circuit (Japan 970319)",                                                 MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsav,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (Euro 970519)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsavu,      vsav,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (USA 970519)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsavj,      vsav,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (Japan 970519)",                            MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsava,      vsav,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (Asia 970519)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsavh,      vsav,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (Hispanic 970519)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsavb,      vsav,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior: The Lord of Vampire (Brazil 970519)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsf,     0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Euro 970625)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfu,    mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (USA 970827)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfu1,   mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (USA 970625)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfj,    mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Japan 970707)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfj1,   mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Japan 970702)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfj2,   mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Japan 970625)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfh,    mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Hispanic 970625)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfa,    mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Asia 970625)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfa1,   mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Asia 970620)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfb,    mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Brazil 970827)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfb1,   mshvsf,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Super Heroes Vs. Street Fighter (Brazil 970625)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclub,     0,        cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Euro 971017)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclub1,    csclub,   cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Euro 970722)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1997, cscluba,    csclub,   cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Asia 970722)",                                              MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclubj,    csclub,   cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Japan 970722)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclubjy,   csclub,   cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Japan 970722, yellow case)",                                MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclubh,    csclub,   cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Capcom Sports Club (Hispanic 970722)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1997, sgemf,      0,        cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Gem Fighter Mini Mix (USA 970904)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 1997, pfghtj,     sgemf,    cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Pocket Fighter (Japan 970904)",                                                 MACHINE_SUPPORTS_SAVE )
GAME( 1997, sgemfa,     sgemf,    cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Gem Fighter: Mini Mix (Asia 970904)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1997, sgemfh,     sgemf,    cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Capcom", "Super Gem Fighter: Mini Mix (Hispanic 970904)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1997, vhunt2,     0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter 2: Darkstalkers Revenge (Japan 970929)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, vhunt2r1,   vhunt2,   cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Hunter 2: Darkstalkers Revenge (Japan 970913)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsav2,      0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Vampire Savior 2: The Lord of Vampire (Japan 970913)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvsc,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Euro 980123)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscr1,     mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Euro 980112)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscu,      mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (USA 980123)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscur1,    mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (USA 971222)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscj,      mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Japan 980123)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscjr1,    mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Japan 980112)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscjsing,  mvsc,     cps2,     cps2_2p6b, cps2_state, init_singbrd,  ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Japan 980123) (Single PCB)",          MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvsca,      mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Asia 980123)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscar1,    mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Asia 980112)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvsch,      mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Hispanic 980123)",                    MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscb,      mvsc,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Marvel Vs. Capcom: Clash of Super Heroes (Brazil 980123)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 3 (Euro 980904)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3u,      sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 3 (USA 980904)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3ur1,    sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 3 (USA 980629)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3us,     sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Alpha 3 (USA 980616, SAMPLE Version)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3h,      sfa3,     cps2,     cps2_2p6bt, cps2_state, init_cps2,    ROT0,   "Capcom", "Street Fighter Alpha 3 (Hispanic 980904)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3hr1,    sfa3,     cps2,     cps2_2p6bt, cps2_state, init_cps2,    ROT0,   "Capcom", "Street Fighter Alpha 3 (Hispanic 980629)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3b,      sfa3,     cps2,     cps2_2p6bt, cps2_state, init_cps2,    ROT0,   "Capcom", "Street Fighter Alpha 3 (Brazil 980629)",                                        MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3j,      sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 3 (Japan 980904)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3jr1,    sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 3 (Japan 980727)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3jr2,    sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 3 (Japan 980629)",                                          MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3a,      sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 3 (Asia 980904)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3ar1,    sfa3,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Street Fighter Zero 3 (Asia 980701)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1999, jyangoku,   0,        cps2,     cps2_1p2b, cps2_state, init_cps2,     ROT0,   "Capcom", "Jyangokushi: Haoh no Saihai (Japan 990527)",                                    MACHINE_SUPPORTS_SAVE )
GAME( 2004, hsf2,       0,        cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Hyper Street Fighter II: The Anniversary Edition (USA 040202)",                 MACHINE_SUPPORTS_SAVE )
GAME( 2004, hsf2a,      hsf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Hyper Street Fighter II: The Anniversary Edition (Asia 040202)",                MACHINE_SUPPORTS_SAVE )
GAME( 2004, hsf2j,      hsf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Hyper Street Fighter II: The Anniversary Edition (Japan 040202)",               MACHINE_SUPPORTS_SAVE )
GAME( 2004, hsf2j1,     hsf2,     cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "Capcom", "Hyper Street Fighter II: The Anniversary Edition (Japan 031222)",               MACHINE_SUPPORTS_SAVE )

// Games released on CPS-2 hardware by Takumi

GAME( 1999, gigawing,   0,        cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Giga Wing (USA 990222)",                           MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawingj,  gigawing, cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Giga Wing (Japan 990223)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawinga,  gigawing, cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Giga Wing (Asia 990222)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawingh,  gigawing, cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Giga Wing (Hispanic 990222)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawingb,  gigawing, cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Giga Wing (Brazil 990222)",                        MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrix,    0,        cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Mars Matrix: Hyper Solid Shooting (USA 000412)",   MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrixa,   mmatrix,  cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Mars Matrix: Hyper Solid Shooting (Asia 000412)",  MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrixj,   mmatrix,  cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Mars Matrix: Hyper Solid Shooting (Japan 000412)", MACHINE_SUPPORTS_SAVE )

// Games released on CPS-2 hardware by Mitchell

GAME( 2000, mpang,      0,        cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Mighty! Pang (Euro 001010)",           MACHINE_SUPPORTS_SAVE )
GAME( 2000, mpangr1,    mpang,    cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Mighty! Pang (Euro 000925)",           MACHINE_SUPPORTS_SAVE )
GAME( 2000, mpangu,     mpang,    cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Mighty! Pang (USA 001010)",            MACHINE_SUPPORTS_SAVE )
GAME( 2000, mpangj,     mpang,    cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Mighty! Pang (Japan 001011)",          MACHINE_SUPPORTS_SAVE )
GAME( 2000, mpanga,     mpang,    cps2, cps2_2p1b, cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Mighty! Pang (Asia 001010)",           MACHINE_SUPPORTS_SAVE )
GAME( 2001, pzloop2,    0,        cps2, pzloop2,   cps2_state, init_pzloop2,  ROT0,   "Mitchell (Capcom license)", "Puzz Loop 2 (Euro 010302)",            MACHINE_SUPPORTS_SAVE )
GAME( 2001, pzloop2j,   pzloop2,  cps2, pzloop2,   cps2_state, init_pzloop2,  ROT0,   "Mitchell (Capcom license)", "Puzz Loop 2 (Japan 010226)",           MACHINE_SUPPORTS_SAVE )
GAME( 2001, pzloop2jr1, pzloop2,  cps2, pzloop2,   cps2_state, init_pzloop2,  ROT0,   "Mitchell (Capcom license)", "Puzz Loop 2 (Japan 010205)",           MACHINE_SUPPORTS_SAVE )
GAME( 2001, choko,      0,        cps2, choko,     cps2_state, init_cps2,     ROT0,   "Mitchell (Capcom license)", "Janpai Puzzle Choukou (Japan 010820)", MACHINE_SUPPORTS_SAVE )

// Games released on CPS-2 hardware by Eighting/Raizing

GAME( 2000, dimahoo,    0,        cps2, cps2_2p3b, cps2_state, init_cps2,     ROT270, "Eighting / Raizing (Capcom license)", "Dimahoo (Euro 000121)",                 MACHINE_SUPPORTS_SAVE )
GAME( 2000, dimahoou,   dimahoo,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT270, "Eighting / Raizing (Capcom license)", "Dimahoo (USA 000121)",                  MACHINE_SUPPORTS_SAVE )
GAME( 2000, gmahou,     dimahoo,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT270, "Eighting / Raizing (Capcom license)", "Great Mahou Daisakusen (Japan 000121)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, 1944,       0,        cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Eighting / Raizing (Capcom license)", "1944: The Loop Master (Euro 000620)",   MACHINE_SUPPORTS_SAVE )
GAME( 2000, 1944j,      1944,     cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Eighting / Raizing (Capcom license)", "1944: The Loop Master (Japan 000620)",  MACHINE_SUPPORTS_SAVE )
GAME( 2000, 1944u,      1944,     cps2, cps2_2p2b, cps2_state, init_cps2,     ROT0,   "Eighting / Raizing (Capcom license)", "1944: The Loop Master (USA 000620)",    MACHINE_SUPPORTS_SAVE )

// Games released on CPS-2 hardware by Cave

GAME( 2001, progear,    0,        cps2, cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Cave (Capcom license)", "Progear (USA 010117)",             MACHINE_SUPPORTS_SAVE )
GAME( 2001, progearj,   progear,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Cave (Capcom license)", "Progear no Arashi (Japan 010117)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, progeara,   progear,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Cave (Capcom license)", "Progear (Asia 010117)",            MACHINE_SUPPORTS_SAVE )

/*
 ------------------------
 Phoenix bootleg sets
 ------------------------

 The Phoenix sets were created by Razoola as a method of allowing the games to run on
 CPS2 boards where the battery had died.  When this happens the boards run non-encrypted
 code, but the memory mapping is changed.  As the original games have encrypted code
 mixed with decrypted data the program roms must be carefully modified in order to
 correctly contain only decrypted code and data, as well as modification to compensate
 for the memory map changes that occur on the dead boards.  Due nature of this process
 there were sometimes errors introduced into the 'Phoenix' sets.

 Unfortunately the 'Phoenix' sets also ended up forming the basis of a mass cps2
 bootlegging operation whereby cheap CPS2 B boards were purchased, the encryption keys
 killed, and the boards converted to more desirable games.  These started off as single
 game bootlegs of in-demand titles, but soon started also forming the basis of xx-in-1
 bootlegs running on heavily customized B-boards.  These are not legitimate Capcom
 products despite appearing to be so.

 These bootlegs are often sold as 'Phoenix Edition' after Razoola's name, 'xx-in-1', or
 simply 'Suicide-Free' to further artificially inflate the price. Buyer Beware!

 All sets are marked as bootleg because they're unauthorized modifications of the
 original Capcom rom data, and were used for bootleg conversions.

 This may not be a complete list of sets, it was taken from MamePlus.  Other sets, and
 further customized bootlegs boards are known to exist.

*/


ROM_START( 1944d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c83d.p1", 0x000000, 0x80000, CRC(28e8aae4) SHA1(b2ae11bddbf156cbf38eafdc705067bff9256752) )
	ROM_LOAD16_WORD_SWAP( "c83.p2",   0x080000, 0x80000, CRC(dba1c66e) SHA1(4764e77d4da5d19d9acded27df1e1bcba06b0fcf) )
	ROM_LOAD16_WORD_SWAP( "c83u.p3",  0x100000, 0x80000, CRC(ea813eb7) SHA1(34e0175a5f22d08c3538369b4bfd077a7427a128) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c83.c1",   0x0000000, 0x400000, CRC(c9fca741) SHA1(1781d4fc18b6d6f79b7b39d9bcace750fb61a5cf) )
	ROM_LOAD64_WORD( "c83.c2",   0x0000002, 0x400000, CRC(f809d898) SHA1(a0b6af49e1780678d808c317b875161cedddb314) )
	ROM_LOAD64_WORD( "c83.c3",   0x0000004, 0x400000, CRC(15ba4507) SHA1(bed6a82bf1dc1aa501d4c2d098115a15e18d446a) )
	ROM_LOAD64_WORD( "c83.c4",   0x0000006, 0x400000, CRC(3dd41b8c) SHA1(676078baad789e25f6e5a79de29672587be7ff00) )
	ROM_LOAD64_WORD( "c83.c5",   0x1000000, 0x100000, CRC(3fe3a54b) SHA1(0a8e5cae141d24fd8b3cb11796c44728b0acd69e) )
	ROM_LOAD64_WORD( "c83.c6",   0x1000002, 0x100000, CRC(565cd231) SHA1(0aecd433fb4ca2de1aca9fbb1e314fb1f6979321) )
	ROM_LOAD64_WORD( "c83.c7",   0x1000004, 0x100000, CRC(63ca5988) SHA1(30137fa77573c84bcc24570bccb7dba61ddb413c) )
	ROM_LOAD64_WORD( "c83.c8",   0x1000006, 0x100000, CRC(21eb8f3b) SHA1(efa69f19a958047dd91a294c88857ed3133fcbef) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c83.m1",   0x00000, 0x08000, CRC(d2e44318) SHA1(33e45f6fe9fed098a4c072b8c39406aef1a949b2) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c83.q1",   0x000000, 0x400000, CRC(243e4e05) SHA1(83281f7290ac105a3f9a7507cbc11317d45ba706) )
	ROM_LOAD16_WORD_SWAP( "c83.q2",   0x400000, 0x400000, CRC(4fcf1600) SHA1(36f18c5d92b79433bdf7088b29a244708929d48e) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( 19xxd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c62ud.p1", 0x000000, 0x80000, CRC(f81b60e5) SHA1(6f246ed7779a3a75e7ad7fa8c913c901ea423828) )
	ROM_LOAD16_WORD_SWAP( "c62ud.p2", 0x080000, 0x80000, CRC(cc44638c) SHA1(a11df36b98bb59cf64c47bab29721e80d64054ff) )
	ROM_LOAD16_WORD_SWAP( "c62ud.p3", 0x100000, 0x80000, CRC(33a168de) SHA1(11e1977cda6269f1a7d6a0f53b73e3094649bd2f) )
	ROM_LOAD16_WORD_SWAP( "c62ud.p4", 0x180000, 0x80000, CRC(e0111282) SHA1(233bf939f79e5dd861a01d6d76ae57861d0438fd) )
	ROM_LOAD16_WORD_SWAP( "c62.p5",   0x200000, 0x80000, CRC(61c0296c) SHA1(9e225beccffd14bb53a32f8c0f2aef7f331dae30) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c62.c1",   0x0000000, 0x080000, CRC(427aeb18) SHA1(901029b5423e4bda85f592735036c06b7d426680) )
	ROM_LOAD64_WORD( "c62.c2",   0x0000002, 0x080000, CRC(63bdbf54) SHA1(9beb64ef0a8c92490848599d5d979bf42532609d) )
	ROM_LOAD64_WORD( "c62.c3",   0x0000004, 0x080000, CRC(2dfe18b5) SHA1(8a44364d9af6b9e1664b44b9235dc172182c9eb8) )
	ROM_LOAD64_WORD( "c62.c4",   0x0000006, 0x080000, CRC(cbef9579) SHA1(172413f220b242411218c7865e04014ec6417537) )
	ROM_LOAD64_WORD( "c62.c5",   0x0800000, 0x200000, CRC(e916967c) SHA1(3f937022166149a80585f91388de521055ca88ca) )
	ROM_LOAD64_WORD( "c62.c6",   0x0800002, 0x200000, CRC(6e75f3db) SHA1(4e1c8466eaa612102d0807d2e8bf1004e97476ea) )
	ROM_LOAD64_WORD( "c62.c7",   0x0800004, 0x200000, CRC(2213e798) SHA1(b1a9d5547f3f6c3ab59e8b761d224793c6ca33cb) )
	ROM_LOAD64_WORD( "c62.c8",   0x0800006, 0x200000, CRC(ab9d5b96) SHA1(52b755da401fde90c13181b02ab33e5e4b2aa1f7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c62.m1",   0x00000, 0x08000, CRC(ef55195e) SHA1(813f465f2d392f6abeadbf661c54cf51171fa006) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c62.q1",   0x000000, 0x200000, CRC(d38beef3) SHA1(134e961b926a97cca5e45d3558efb98f6f278e08) )
	ROM_LOAD16_WORD_SWAP( "c62.q2",   0x200000, 0x200000, CRC(d47c96e2) SHA1(3c1b5563f8e7ee1c450b3592fcb319e928caec3c) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz2ad )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jad.p1", 0x000000, 0x80000, CRC(017f8fab) SHA1(5546d935c569464c29999914c697c2a171659f42) )
	ROM_LOAD16_WORD_SWAP( "c64jad.p2", 0x080000, 0x80000, CRC(f50e5ea2) SHA1(ec75aa69bd18cdfac4f5783d9c6c3691bb4914c9) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p3",   0x100000, 0x80000, CRC(98e8e992) SHA1(41745b63e6b3888081d189b8315ed3b7526b3d20) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",   0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64ja.p5",  0x200000, 0x80000, CRC(0aed2494) SHA1(7beb1a394f17cd78a27128292b626aae28754ca2) )
	ROM_LOAD16_WORD_SWAP( "c64.p6",   0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( avspd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c52d.p1", 0x000000, 0x80000, CRC(66aa8aad) SHA1(eb1928393d0dd4cc1a96c00324508f83f36a7622) )
	ROM_LOAD16_WORD_SWAP( "c52d.p2", 0x080000, 0x80000, CRC(579306c2) SHA1(cabee3fdb624e681013a5a57d2a37339b96518fb) )
	ROM_LOAD16_WORD_SWAP( "c52.p3",  0x100000, 0x80000, CRC(fbfb5d7a) SHA1(5549bc9d780753bc9c10fba82588e5c3d4a2acb2) )
	ROM_LOAD16_WORD_SWAP( "c52d.p4", 0x180000, 0x80000, CRC(63094539) SHA1(f1b776cf4334fa7fa1ee0e5ce81a5996b930996b) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c52.c1",   0x0000000, 0x200000, CRC(8f8b5ae4) SHA1(457ce959aa5db3a003de7dda2b3799b2f1ae279b) )
	ROM_LOAD64_WORD( "c52.c2",   0x0000002, 0x200000, CRC(b00280df) SHA1(bc1291a4a222d410bc99b6f1ed392067d9c3999e) )
	ROM_LOAD64_WORD( "c52.c3",   0x0000004, 0x200000, CRC(94403195) SHA1(efaad001527a5eba8f626aea9037ac6ef9a2c295) )
	ROM_LOAD64_WORD( "c52.c4",   0x0000006, 0x200000, CRC(e1981245) SHA1(809ccb7f10262e227d5e9d9f710e06f0e751f550) )
	ROM_LOAD64_WORD( "c52.c5",   0x0800000, 0x200000, CRC(ebba093e) SHA1(77aaf4197d1dae3321cf9c6d2b7967ee54cf3f30) )
	ROM_LOAD64_WORD( "c52.c6",   0x0800002, 0x200000, CRC(fb228297) SHA1(ebd02a4ba085dc70c0603662e14d61625fa04648) )
	ROM_LOAD64_WORD( "c52.c7",   0x0800004, 0x200000, CRC(34fb7232) SHA1(8b1f15bfa758a61e6ad519af24ca774edc70d194) )
	ROM_LOAD64_WORD( "c52.c8",   0x0800006, 0x200000, CRC(f90baa21) SHA1(20a900819a9d321316e3dfd241210725d7191ecf) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c52.m1",   0x00000, 0x08000, CRC(2d3b4220) SHA1(2b2d04d4282550fa9f6e1ad8528f20d1f2ac02eb) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c52.q1",   0x000000, 0x200000, CRC(83499817) SHA1(e65b0ebd61ddc748842a9d4d92404b5305307623) )
	ROM_LOAD16_WORD_SWAP( "c52.q2",   0x200000, 0x200000, CRC(f4110d49) SHA1(f27538776cc1ba8213f19f98728ed8c02508d3ac) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( batcird )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c70d.p1", 0x000000, 0x80000, CRC(0737db6d) SHA1(e7f02f568a8013d5abf32e0e7dc120c004adb6c9) )
	ROM_LOAD16_WORD_SWAP( "c70d.p2", 0x080000, 0x80000, CRC(ef1a8823) SHA1(9326e031b937af18b98dc3f236caaac632dac66d) )
	ROM_LOAD16_WORD_SWAP( "c70d.p3", 0x100000, 0x80000, CRC(20bdbb14) SHA1(fe3a202741ca657b2b67e89050788b67d709a36d) )
	ROM_LOAD16_WORD_SWAP( "c70d.p4", 0x180000, 0x80000, CRC(b4d8f5bc) SHA1(dc5ca580ecfb051ded551663ea4e9f161f820f81) )
	ROM_LOAD16_WORD_SWAP( "c70.p5",   0x200000, 0x80000, CRC(7322d5db) SHA1(473be1f1bf603bdd82451661a6206507f50ed2b6) )
	ROM_LOAD16_WORD_SWAP( "c70.p6",   0x280000, 0x80000, CRC(6aac85ab) SHA1(ad02d4185c2b3664fb96350d8ad317d3939a7554) )
	ROM_LOAD16_WORD_SWAP( "c70.p7",   0x300000, 0x80000, CRC(1203db08) SHA1(fdbea14618b277132f9e010ef36c134a8ea42162) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c70.c1",   0x000000, 0x400000, CRC(dc705bad) SHA1(96e37147674bf9cd21c770897da59daac25d921a) )
	ROM_LOAD64_WORD( "c70.c2",   0x000002, 0x400000, CRC(e5779a3c) SHA1(bbd7fbe061e751388d2f02434144daf9b1e36640) )
	ROM_LOAD64_WORD( "c70.c3",   0x000004, 0x400000, CRC(b33f4112) SHA1(e501fd921c8bcede69946b029e05d422714c1040) )
	ROM_LOAD64_WORD( "c70.c4",   0x000006, 0x400000, CRC(a6fcdb7e) SHA1(7a28d5d7aa036d23d97fad17d0cdb8210dc8153a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c70.m1",   0x00000, 0x08000, CRC(1e194310) SHA1(3b29de0aca9dbca59d6b50fb2509e2a913c6b0af) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c70.m2",   0x28000, 0x20000, CRC(01aeb8e6) SHA1(50a5d1cce0caf7c5143d4904431e8f41e2a57464) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c70.q1",   0x000000, 0x200000, CRC(c27f2229) SHA1(df2459493af40937b6656a16fad43ff51bed2204) )
	ROM_LOAD16_WORD_SWAP( "c70.q2",   0x200000, 0x200000, CRC(418a2e33) SHA1(0642ddff2ab9255f154419da24ba644ed63f34ab) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ddtodd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c50d.p1", 0x000000, 0x80000, CRC(843330f4) SHA1(3f77876ac5d61b595c22697724a16034b16954af) )
	ROM_LOAD16_WORD_SWAP( "c50d.p2", 0x080000, 0x80000, CRC(306f14fc) SHA1(b684ee72acc3087ada20b2d13202366c3cff1014) )
	ROM_LOAD16_WORD_SWAP( "c50d.p3", 0x100000, 0x80000, CRC(8c6b8328) SHA1(ab5d2c608bd3cdb298a9743d819616bf2df02ddd) )
	ROM_LOAD16_WORD_SWAP( "c50.p4",   0x180000, 0x80000, CRC(6225495a) SHA1(a9a02abb072e3482ac92d7aed8ce9a5bcf636bc0) )
	ROM_LOAD16_WORD_SWAP( "c50d.p5",  0x200000, 0x80000, CRC(0f0df6cc) SHA1(5da5f989ed71faf5e2950fdf9650d94918616ae4) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c50.c1",   0x000000, 0x200000, CRC(da3cb7d6) SHA1(d59bb53d5f32889eb6eb7f8b1c8781948c97283d) )
	ROM_LOAD64_WORD( "c50.c2",   0x000002, 0x200000, CRC(92b63172) SHA1(9bed7dbbb17729f2ad3d318396f5335c0bd39937) )
	ROM_LOAD64_WORD( "c50.c3",   0x000004, 0x200000, CRC(b98757f5) SHA1(3eead22e097906bf0e1e151cd0a9c75abc5a32d4) )
	ROM_LOAD64_WORD( "c50.c4",   0x000006, 0x200000, CRC(8121ce46) SHA1(40c4dc969318d38f0c6d5401c9c64371f51aa12c) )
	ROM_LOAD64_WORD( "c50.c5",   0x800000, 0x100000, CRC(837e6f3f) SHA1(c060183474fba0e82d765b9f282b84838550dff6) )
	ROM_LOAD64_WORD( "c50.c6",   0x800002, 0x100000, CRC(f0916bdb) SHA1(9354d258dd26cbbf12c78ecfc277c357cbdb360e) )
	ROM_LOAD64_WORD( "c50.c7",   0x800004, 0x100000, CRC(cef393ef) SHA1(830b33c86cc24776d17ad65fa89a3b16c40446a1) )
	ROM_LOAD64_WORD( "c50.c8",   0x800006, 0x100000, CRC(8953fe9e) SHA1(f4795beb006335d13e3934aa9760e775eb0bb950) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c50.m1",   0x00000, 0x08000, CRC(3f5e2424) SHA1(4aa744576bc6752c43a90a27a816ebd90076b248) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c50.q1",   0x000000, 0x200000, CRC(0c499b67) SHA1(a8ebd8a1cd6dece8344b7cb0439d85843fb97616) )
	ROM_LOAD16_WORD_SWAP( "c50.q2",   0x200000, 0x200000, CRC(2f0b5a4e) SHA1(8d1ebbb811aa469b0f0d29d719d2b9af28fb63a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ddsomud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c63ud.p1", 0x000000, 0x80000, CRC(816f695a) SHA1(5e590d36c04ceac1b48954c0554a733b4c3a5277) )
	ROM_LOAD16_WORD_SWAP( "c63ud.p2", 0x080000, 0x80000, CRC(7cc81c6b) SHA1(2619cb25d5ae62adc5c2292e25c0d33674f5cc13) )
	ROM_LOAD16_WORD_SWAP( "c63u.p3", 0x100000, 0x80000, CRC(5eb1991c) SHA1(429a60b5396ff4192904867fbe0524268f0edbcb) )
	ROM_LOAD16_WORD_SWAP( "c63u.p4", 0x180000, 0x80000, CRC(c26b5e55) SHA1(9590206f30459941880ff4b56c7f276cc78e3a22) )
	ROM_LOAD16_WORD_SWAP( "c63u.p5", 0x200000, 0x80000, CRC(909a0b8b) SHA1(58bda17c36063a79df8b5031755c7909a9bda221) )
	ROM_LOAD16_WORD_SWAP( "c63u.p6", 0x280000, 0x80000, CRC(e53c4d01) SHA1(bad872e4e793a39f68bc0e580772e982714b5876) )
	ROM_LOAD16_WORD_SWAP( "c63u.p7", 0x300000, 0x80000, CRC(5f86279f) SHA1(c2a454e5f821b1cdd49f2cf0602e9bfb7ba63340) )
	ROM_LOAD16_WORD_SWAP( "c63ud.p8",   0x380000, 0x80000, CRC(0c172f8f) SHA1(4f0ad9ab401f9f2d7d8f605a5ef78add4f4ced38) )

	ROM_REGION( 0x1800000, "gfx", 0 )
	ROM_LOAD64_WORD( "c63.c1",   0x0000000, 0x400000, CRC(a46b4e6e) SHA1(fb90f42868c581c481b4ceff9f692753fb186b30) )
	ROM_LOAD64_WORD( "c63.c2",   0x0000002, 0x400000, CRC(d5fc50fc) SHA1(bc692f17b18bb47a724cd5152377cd5ccd6e184a) )
	ROM_LOAD64_WORD( "c63.c3",   0x0000004, 0x400000, CRC(837c0867) SHA1(3d6db290a8f76299a23543f0ccf6a7905e1088ac) )
	ROM_LOAD64_WORD( "c63.c4",   0x0000006, 0x400000, CRC(bb0ec21c) SHA1(e43ccc1cf63ccd2b504cc9fd701af849a7321914) )
	ROM_LOAD64_WORD( "c63.c5",   0x1000000, 0x200000, CRC(6d824ce2) SHA1(0ccfe6c8a944937718e28a1a373b5822c7b7001b) )
	ROM_LOAD64_WORD( "c63.c6",   0x1000002, 0x200000, CRC(79682ae5) SHA1(ee84f4791c29ce9e2bae06ba3ec47ff4d2cd7054) )
	ROM_LOAD64_WORD( "c63.c7",   0x1000004, 0x200000, CRC(acddd149) SHA1(7f50de9b2d1cc733594c642be1804190519caffa) )
	ROM_LOAD64_WORD( "c63.c8",   0x1000006, 0x200000, CRC(117fb0c0) SHA1(15c01fa1a71b6469b0e1bde0ce5835c5ff9d938c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c63.m1",   0x00000, 0x08000, CRC(99d657e5) SHA1(1528dd6b07a0e79951a35c0457c8a9c9770e9c78) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c63.m2",   0x28000, 0x20000, CRC(117a3824) SHA1(14f3a12170b601c5466c93af9d2f24e0b386b4e4) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c63.q1",   0x000000, 0x200000, CRC(98d0c325) SHA1(7406e8d943d77c468eb418c4113261f4ab973bbf) )
	ROM_LOAD16_WORD_SWAP( "c63.q2",   0x200000, 0x200000, CRC(5ea2e7fa) SHA1(0e6a9fd007f637adcb2226c902394f07de45e803) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ecofghtrd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c49d.p1", 0x000000, 0x80000, CRC(ac725d2b) SHA1(f77a37d160aed34319ef810728de88a87bccb7a5) )
	ROM_LOAD16_WORD_SWAP( "c49d.p2", 0x080000, 0x80000, CRC(f800138d) SHA1(a8c05da5d33c338a82fed4f7b59c6ba8c10e8597) )
	ROM_LOAD16_WORD_SWAP( "c49d.p3", 0x100000, 0x80000, CRC(eb6a12f2) SHA1(5253a193a38b566eefbb8644511f283ac4a69850) )
	ROM_LOAD16_WORD_SWAP( "c49d.p4", 0x180000, 0x80000, CRC(8380ec9a) SHA1(4f3a95a2580f89ae214b588df9258bd9dc06f993) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c49.c1",   0x000000, 0x200000, CRC(dcaf1436) SHA1(ba124cc0bb10c1d1c07592a3623add4ed054182e) )
	ROM_LOAD64_WORD( "c49.c2",   0x000002, 0x200000, CRC(2807df41) SHA1(66a9800af435055737ce50a0b0ced7c5718c2004) )
	ROM_LOAD64_WORD( "c49.c3",   0x000004, 0x200000, CRC(8a708d02) SHA1(95ec527edc904a66e325667521b4d07d72579211) )
	ROM_LOAD64_WORD( "c49.c4",   0x000006, 0x200000, CRC(de7be0ef) SHA1(bf8df9a31f8923f4b726ea12fe8327368463ebe1) )
	ROM_LOAD64_WORD( "c49.c5",   0x800000, 0x100000, CRC(1a003558) SHA1(64bbd89e65dc0cf6f4ab5ea93a4cc6312d0d0802) )
	ROM_LOAD64_WORD( "c49.c6",   0x800002, 0x100000, CRC(4ff8a6f9) SHA1(03968a301417e8843d42d4e0db42aa0a3a38664b) )
	ROM_LOAD64_WORD( "c49.c7",   0x800004, 0x100000, CRC(b167ae12) SHA1(48c552d02caad27d680aa51170560794f2a51478) )
	ROM_LOAD64_WORD( "c49.c8",   0x800006, 0x100000, CRC(1064bdc2) SHA1(c51f75ac8d3f02a771feda0a933314a928555c4e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c49.m1",   0x00000, 0x08000, CRC(c235bd15) SHA1(feb7cd7db9dc0b9887b33eed9796bb0205fb719d) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c49.q1",   0x000000, 0x200000, CRC(81b25d39) SHA1(448adfcc7d98873a48c710d857225cdd1580e5c9) )
	ROM_LOAD16_WORD_SWAP( "c49.q2",   0x200000, 0x200000, CRC(27729e52) SHA1(a55c8159adf766dda70cb047f5ac85ce6bc0a3f3) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( gigawingjd )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79jd.p1", 0x000000, 0x80000, CRC(cb1c756e) SHA1(f8b37120b429e14aad6e7d2106f8f8c422f2ce2b) )
	ROM_LOAD16_WORD_SWAP( "c79jd.p2", 0x080000, 0x80000, CRC(fa158e04) SHA1(bd0f0351fabe376944c28e327bcf83a8d9229441) )
	ROM_LOAD16_WORD_SWAP( "c79jd.p3", 0x100000, 0x80000, CRC(1c5bc4e7) SHA1(7f338cb45686b24b9136dd1e575fd842f9fd0b05) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( gigawingd )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c79d.p1", 0x000000, 0x80000, CRC(dde92dfa) SHA1(37b052438663d771c256c6cc61cd3f15c560f1b8) )
	ROM_LOAD16_WORD_SWAP( "c79d.p2", 0x080000, 0x80000, CRC(e0509ae2) SHA1(6962cda86351194fe901edd2b99d9da622c00c36) )
	ROM_LOAD16_WORD_SWAP( "c79d.p3", 0x100000, 0x80000, CRC(722d0042) SHA1(8587e42d4781c503bc8871b88d3f85e21659c50e) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c79.c1",   0x000000, 0x400000, CRC(105530a4) SHA1(3be06c032985ea6bd3805d73a407bf748385087b) )
	ROM_LOAD64_WORD( "c79.c2",   0x000002, 0x400000, CRC(9e774ab9) SHA1(adea1e844f3d9ccd5ad116ff8277f16a96e68d76) )
	ROM_LOAD64_WORD( "c79.c3",   0x000004, 0x400000, CRC(466e0ba4) SHA1(9563455b95d36fafe508290659088b153539cfdf) )
	ROM_LOAD64_WORD( "c79.c4",   0x000006, 0x400000, CRC(840c8dea) SHA1(ea04afce17f00b45d3d2cd5140d0dd7ab4bccc00) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c79.m1",   0x00000, 0x08000, CRC(4c6351d5) SHA1(cef81fb7c4b8cb2ef1f8f3c27982aefbcbe38160) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c79.q1",   0x000000, 0x400000, CRC(e172acf5) SHA1(d7b0963d66165f3607d887741c5e7ab952bcf2ff) )
	ROM_LOAD16_WORD_SWAP( "c79.q2",   0x400000, 0x400000, CRC(4bee4e8f) SHA1(c440b5a38359ec3b8002f39690b79bf78703f5d0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( hsf2d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c88d.p1", 0x000000, 0x80000, CRC(0153d371) SHA1(137f89b4ca41346abd8d1ef4a17605f6622b741e) )
	ROM_LOAD16_WORD_SWAP( "c88d.p2", 0x080000, 0x80000, CRC(0276b78a) SHA1(3f5502f77eb9889ca4658eb323579e05b35c9868) )
	ROM_LOAD16_WORD_SWAP( "c88.p3",   0x100000, 0x80000, CRC(dde34a35) SHA1(f5be2d2916db6e86e0886d61d55bddf138273ebc) )
	ROM_LOAD16_WORD_SWAP( "c88.p4",   0x180000, 0x80000, CRC(f4e56dda) SHA1(c6490707c2a416ab88612c2d73abbe5853d8cb92) )
	ROM_LOAD16_WORD_SWAP( "c88.p5",   0x200000, 0x80000, CRC(ee4420fc) SHA1(06cf76660b0c794d2460c52d9fe8334fff51e9de) )
	ROM_LOAD16_WORD_SWAP( "c88.p6",   0x280000, 0x80000, CRC(c9441533) SHA1(bf178fac1f060fcce3ff9118333c8517dadc9429) )
	ROM_LOAD16_WORD_SWAP( "c88.p7",   0x300000, 0x80000, CRC(3fc638a8) SHA1(2a42877b26c8abc437da46225701f0bba6e40058) )
	ROM_LOAD16_WORD_SWAP( "c88.p8",   0x380000, 0x80000, CRC(20d0f9e4) SHA1(80a5eeef9472e327b0d4ee26434bad109a9434ea) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c88.c1",   0x0000000, 0x800000, CRC(a6ecab17) SHA1(6749a4c8dc81f4b10f910c31c82cf6674e2a44eb) )
	ROM_LOAD64_WORD( "c88.c2",   0x0000002, 0x800000, CRC(10a0ae4d) SHA1(701b4900fbc8bef20efa1a706891c8df4bf14641) )
	ROM_LOAD64_WORD( "c88.c3",   0x0000004, 0x800000, CRC(adfa7726) SHA1(8d36ec125a8c91abfe5213893d794f8bc11c8acd) )
	ROM_LOAD64_WORD( "c88.c4",   0x0000006, 0x800000, CRC(bb3ae322) SHA1(ecd289d7a0fe365fdd7c5527cb17796002beb553) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c88.m1",   0x00000, 0x08000, CRC(c1a13786) SHA1(c7392c7efb15ea4042e75bd9007e974293d8935d) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c88.m2",   0x28000, 0x20000, CRC(2d8794aa) SHA1(c634affdc2568020cce6af97b4fa79925d9943f3) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c88.q1",   0x000000, 0x800000, CRC(0e15c359) SHA1(176108b0d76d821a849324680aba0cd04b5016c1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( megamn2d )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c66d.p1", 0x000000, 0x80000, CRC(d3635f25) SHA1(69a5a44bdc040754efa7fb96a267f8dcc2c5a23f) )
	ROM_LOAD16_WORD_SWAP( "c66d.p2", 0x080000, 0x80000, CRC(768a1705) SHA1(9b0f206ef15d72c9d0cb496845353ce7fdf2d25e) )
	ROM_LOAD16_WORD_SWAP( "c66.p3",  0x100000, 0x80000, CRC(02ee9efc) SHA1(1b80c40389b51a03b930051f232630616c12e6c5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c66.c1",  0x800000, 0x200000, CRC(9b1f00b4) SHA1(c1c5c2d9d00121425ae6598444d704f420ef4eef) )
	ROM_LOAD64_WORD( "c66.c2",  0x800002, 0x200000, CRC(c2bb0c24) SHA1(38724c49d9db49765a4ed9bc2dc8f57cec45ec7c) )
	ROM_LOAD64_WORD( "c66.c3",  0x800004, 0x200000, CRC(12257251) SHA1(20cb58afda0e6200991277817485340a6a41ae2b) )
	ROM_LOAD64_WORD( "c66.c4",  0x800006, 0x200000, CRC(f9b6e786) SHA1(aeb4acff7208e66a35198143fd2478039fdaa3a6) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c66.m1",  0x00000, 0x08000, CRC(d18e7859) SHA1(0939fac70042d0b4db5c2fdcef1f79b95febd45e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c66.m2",   0x28000, 0x20000, CRC(c463ece0) SHA1(5c3e41eb61610b3f8c431206f6672907e3a0bdb0) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c66.q1",   0x000000, 0x200000, CRC(2106174d) SHA1(0a35d9ca8ebcad74904b20648d5320f839d6377e) )
	ROM_LOAD16_WORD_SWAP( "c66.q2",   0x200000, 0x200000, CRC(546c1636) SHA1(f96b172ab899f2c6ee17a5dd1fb61af9432e3cd2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( mvscud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c77ud.p1", 0x000000, 0x80000, CRC(75cde3e5) SHA1(5453056da68cdd4675cb585a9c6ae85e073193f5) )
	ROM_LOAD16_WORD_SWAP( "c77ud.p2", 0x080000, 0x80000, CRC(b32ea484) SHA1(742b35a45eadea3457bfb93808cbba599e9744e3) )
	ROM_LOAD16_WORD_SWAP( "c77.p3",   0x100000, 0x80000, CRC(2d8c8e86) SHA1(b07d640a734c5d336054ed05195786224c9a6cd4) )
	ROM_LOAD16_WORD_SWAP( "c77.p4",   0x180000, 0x80000, CRC(8528e1f5) SHA1(cd065c05268ab581b05676da544baf6af642acac) )
	ROM_LOAD16_WORD_SWAP( "c77ud.p5",  0x200000, 0x80000, CRC(205293e9) SHA1(b47a7057abd18d85ff9d86483ecb7ab783b3e791) )
	ROM_LOAD16_WORD_SWAP( "c77.p6",    0x280000, 0x80000, CRC(bc002fcd) SHA1(0b6735a071a9274f7ab25c743271fc30411fe819) )
	ROM_LOAD16_WORD_SWAP( "c77.p7",    0x300000, 0x80000, CRC(c67b26df) SHA1(6e9969246c57269d7ba0992a5cc319c8910bf8a9) )
	ROM_LOAD16_WORD_SWAP( "c77.p8",    0x380000, 0x80000, CRC(0fdd1e26) SHA1(5fa684d823b4f4eec61ed9e9b8938af5272ae1ed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c77.c1",   0x0000000, 0x400000, CRC(fa5f74bc) SHA1(79a619248938a85ce4f7794a704647b9cf564fbc) )
	ROM_LOAD64_WORD( "c77.c2",   0x0000002, 0x400000, CRC(71938a8f) SHA1(6982f7203458c1c46a1c1c13c0d0f2a5e109d271) )
	ROM_LOAD64_WORD( "c77.c3",   0x0000004, 0x400000, CRC(92741d07) SHA1(ddfd70eab7c983ab452194b1860059f8ad694459) )
	ROM_LOAD64_WORD( "c77.c4",   0x0000006, 0x400000, CRC(bcb72fc6) SHA1(46ab98dcdf6f5d611646a22a7355939ef5b2bbe5) )
	ROM_LOAD64_WORD( "c77.c5",   0x1000000, 0x400000, CRC(7f1df4e4) SHA1(ede92b31c1fe87f91b4fe74ac211f2fb5f863bc2) )
	ROM_LOAD64_WORD( "c77.c6",   0x1000002, 0x400000, CRC(90bd3203) SHA1(ed83208c486ea0f407b7e5d16a8cf242a6f73774) )
	ROM_LOAD64_WORD( "c77.c7",   0x1000004, 0x400000, CRC(67aaf727) SHA1(e0e69104e31d2c41e18c0d24e9ab962406a7ca9a) )
	ROM_LOAD64_WORD( "c77.c8",   0x1000006, 0x400000, CRC(8b0bade8) SHA1(c5732361bb4bf284c4d12a82ac2c5750b1f9d441) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c77.m1",   0x00000, 0x08000, CRC(41629e95) SHA1(36925c05b5fdcbe43283a882d021e5360c947061) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c77.m2",   0x28000, 0x20000, CRC(963abf6b) SHA1(6b784870e338701cefabbbe4669984b5c4e8a9a5) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c77.q1",   0x000000, 0x400000, CRC(850fe663) SHA1(81e519d05a08855f242ea2e17ee0859b449db895) )
	ROM_LOAD16_WORD_SWAP( "c77.q2",   0x400000, 0x400000, CRC(7ccb1896) SHA1(74caadf3282fcc6acffb1bbe3734106f81124121) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( nwarrud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c57ud.p1", 0x000000, 0x80000, CRC(20d4d5a8) SHA1(420396c77a9f77feaba4be2e0faaab995842fb57) )
	ROM_LOAD16_WORD_SWAP( "c57ud.p2", 0x080000, 0x80000, CRC(61be9b42) SHA1(679381f279f51f28fdc4c4dbefabe6139855303b) )
	ROM_LOAD16_WORD_SWAP( "c57ud.p3", 0x100000, 0x80000, CRC(1ba906d8) SHA1(9e1db8af4070c68f25b8fb898e9d29a97f775e4c) )
	ROM_LOAD16_WORD_SWAP( "c57u.p4",  0x180000, 0x80000, CRC(08c04cdb) SHA1(b78d87631a13c26cc1580d2ecc0d137105c23f0a) )
	ROM_LOAD16_WORD_SWAP( "c57u.p5",  0x200000, 0x80000, CRC(b5a5ab19) SHA1(f7b35b8cba81f88a6bdfea7e2dc12eca480c276c) )
	ROM_LOAD16_WORD_SWAP( "c57u.p6",  0x280000, 0x80000, CRC(51bb20fb) SHA1(a98c569dd45b4bd2275f9bd1df060d6eaead53df) )
	ROM_LOAD16_WORD_SWAP( "c57u.p7",  0x300000, 0x80000, CRC(41a64205) SHA1(1f5af658b7c3fb09cab3dd10d6dc433a0605f81a) )
	ROM_LOAD16_WORD_SWAP( "c57ud.p8", 0x380000, 0x80000, CRC(9619adad) SHA1(abbbe28659c031f34be23a38950d9b56f7f7ca86) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c57.c5",   0x1000000, 0x400000, CRC(7a0e1add) SHA1(6b28a91bd59bba97886fdea30116a5b1071109ed) )
	ROM_LOAD64_WORD( "c57.c6",   0x1000002, 0x400000, CRC(2f41ca75) SHA1(f4a67e60b62001e6fe75cb05b9c81040a8a09f54) )
	ROM_LOAD64_WORD( "c57.c7",   0x1000004, 0x400000, CRC(64498eed) SHA1(d64e54a9ad1cbb927b7bac2eb16e1487834c5706) )
	ROM_LOAD64_WORD( "c57.c8",   0x1000006, 0x400000, CRC(17f2433f) SHA1(0cbf8c96f92016fefb4a9c668ce5fd260342d712) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c57.m1",   0x00000, 0x08000, CRC(5045dcac) SHA1(fd1a6586fbdd48a707df1fa52309b4cf50e3cc4c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c57.m2",   0x28000, 0x20000, CRC(86b60e59) SHA1(197d07ced8b9850729c83fa59b7afc283500bdee) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c57.q1",   0x000000, 0x200000, CRC(e1837d33) SHA1(e3cb69f64767bacbec7286d0b4cd0ce7a0ba13d8) )
	ROM_LOAD16_WORD_SWAP( "c57.q2",   0x200000, 0x200000, CRC(fbd3cd90) SHA1(4813c25802ad71b77ca04fd8f3a86344f99f0d6a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ringdstd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c54d.p1", 0x000000, 0x80000, CRC(f6fba4cd) SHA1(aecea7a4c3ed2153017b465b075c7558d914cdc8) )
	ROM_LOAD16_WORD_SWAP( "c54d.p2", 0x080000, 0x80000, CRC(193bc493) SHA1(d2c6af56d3514c6d392aa3f2ea4778bbe5ef513f) )
	ROM_LOAD16_WORD_SWAP( "c54d.p3", 0x100000, 0x80000, CRC(168cccbb) SHA1(c3e1695e013f1197f7093aa499482ff8d7f75b5b) )
	ROM_LOAD16_WORD_SWAP( "c54d.p4", 0x180000, 0x80000, CRC(04673262) SHA1(6ebdfb2adfa0039f6db4d1e81e9ea0a692be4fdc) )
	ROM_LOAD16_WORD_SWAP( "c54.p5",  0x200000, 0x80000, CRC(b9a11577) SHA1(e9b58ef8acd1fedd3c9e0a3489593c7e931106c0) )
	ROM_LOAD16_WORD_SWAP( "c54.p6",  0x280000, 0x80000, CRC(f931b76b) SHA1(0b7e8d1278dcba89f0063bd09cda96d6ae1bc282) )

	ROM_REGION( 0x1200000, "gfx", 0 )
	ROM_LOAD64_WORD( "c54.c01",   0x0000000, 0x200000, CRC(d9b2d1de) SHA1(e8658983070dadcd1300a680a42c8431579e2b4f) )
	ROM_LOAD64_WORD( "c54.c02",   0x0000002, 0x200000, CRC(9a766d92) SHA1(afdf88afbec527268d63c11ea32f861b52e11489) )
	ROM_LOAD64_WORD( "c54.c03",   0x0000004, 0x200000, CRC(51800f0f) SHA1(9526cd69a23340a81841271b51de03d9bf2b979f) )
	ROM_LOAD64_WORD( "c54.c04",   0x0000006, 0x200000, CRC(35757e96) SHA1(c915f3b9e4fdec3defc7eecb2c1f7377e6072228) )
	ROM_LOAD64_WORD( "c54.c05",   0x0800000, 0x200000, CRC(e5bfd0e7) SHA1(327e626df4c2152f921f15535c01dda6c4437527) )
	ROM_LOAD64_WORD( "c54.c06",   0x0800002, 0x200000, CRC(c56c0866) SHA1(1e2218e852ae72a9a95861dd37129fe78d4b1329) )
	ROM_LOAD64_WORD( "c54.c07",   0x0800004, 0x200000, CRC(4ded3910) SHA1(d883541ce4d83f4e7ab095f2ef273408d9911f9a) )
	ROM_LOAD64_WORD( "c54.c08",   0x0800006, 0x200000, CRC(26ea1ec5) SHA1(22be249b1f73272feacf4026f09fc877f5d86353) )
	ROM_LOAD64_WORD( "c54.c09",   0x1000000, 0x080000, CRC(0a08c5fc) SHA1(ff3fad4fbc98e3013291c7ba7ee5e057a2628b36) )
	ROM_LOAD64_WORD( "c54.c10",   0x1000002, 0x080000, CRC(0911b6c4) SHA1(e7a7061b192658724d98cae8693f63dd5bc40c00) )
	ROM_LOAD64_WORD( "c54.c11",   0x1000004, 0x080000, CRC(82d6c4ec) SHA1(ed8ed02a00f59a048b9891ec2a77720bb6a5e03d) )
	ROM_LOAD64_WORD( "c54.c12",   0x1000006, 0x080000, CRC(9b48678b) SHA1(4fa300d356c538947983ae85bb5c5cfd1fb835e7) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c54.m1",   0x00000, 0x08000, CRC(0abc229a) SHA1(967f574e6358dfc1b01e6a4a4df1a8f34eb3d814) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c54.m2",   0x28000, 0x20000, CRC(d051679a) SHA1(583c2521a30db1740d95dd94a38751fbeff3aae5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c54.q1",   0x000000, 0x200000, CRC(c56935f9) SHA1(ca1705e48e31ddc13505e6297bceca2bec1bb209) )
	ROM_LOAD16_WORD_SWAP( "c54.q2",   0x200000, 0x200000, CRC(955b0782) SHA1(ee09500e7b44e923126533613bfe26cdabc7ab5f) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfad )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c59d.p1", 0x000000, 0x80000, CRC(a1a54827) SHA1(85dd1a6f807af56b63cd2880e6e32794eb11c61e) )
	ROM_LOAD16_WORD_SWAP( "c59.p2",   0x080000, 0x80000, CRC(8b73b0e5) SHA1(5318761f615c21395366b5333e75eaaa73ef2073) )
	ROM_LOAD16_WORD_SWAP( "c59.p3",   0x100000, 0x80000, CRC(0810544d) SHA1(5f39bda3e7b16508eb58e5a2e0cc58c09cf428ce) )
	ROM_LOAD16_WORD_SWAP( "c59.p4",    0x180000, 0x80000, CRC(806e8f38) SHA1(b6d6912aa8f2f590335d7ff9a8214648e7131ebb) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c59.c1",  0x800000, 0x200000, CRC(90fefdb3) SHA1(5eb28c8de57acfeaefebdd01509c7d9ba5244705) )
	ROM_LOAD64_WORD( "c59.c2",  0x800002, 0x200000, CRC(5354c948) SHA1(07588f1ba6addc04fef3274c971174aaf3e632ab) )
	ROM_LOAD64_WORD( "c59.c3",  0x800004, 0x200000, CRC(41a1e790) SHA1(ce25dad542308691dbe9606b26279bbd59ea4b81) )
	ROM_LOAD64_WORD( "c59.c4",  0x800006, 0x200000, CRC(a549df98) SHA1(f054e95df650a891ef56da8bfb31cb2c945a9aed) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c59.m1",   0x00000, 0x08000, CRC(ffffec7d) SHA1(75b4aef001b72a0f571b51b2b97803facc1832dd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c59.m2",   0x28000, 0x20000, CRC(45f46a08) SHA1(e32dbd27b52ab708278045b5a829376e55a4ca81) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c59.q1",   0x000000, 0x200000, CRC(c4b093cd) SHA1(256526bb693a0b72443f047e060304c9b739acd1) )
	ROM_LOAD16_WORD_SWAP( "c59.q2",   0x200000, 0x200000, CRC(8bdbc4b4) SHA1(0e21c9a75a17a7e7dfd8bb51098c2b9dc4c933ec) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( mshud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c61ud.p1",0x000000, 0x80000, CRC(c1d8c4c6) SHA1(9d367d23a438fd22edce58ce814a4b3b36e5fc2a) )
	ROM_LOAD16_WORD_SWAP( "c61ud.p2",0x080000, 0x80000, CRC(e73dda16) SHA1(f86cd74bdfa82bf0249770694de9419ffc3d3f63) )
	ROM_LOAD16_WORD_SWAP( "c61ud.p3",0x100000, 0x80000, CRC(3b493e84) SHA1(875e616270e839218c924e09627bcf79211ee694) )
	ROM_LOAD16_WORD_SWAP( "c61.p4",  0x180000, 0x80000, CRC(803e3fa4) SHA1(0acdeda65002521bf24130cbf06f9faa1dcef9e5) )
	ROM_LOAD16_WORD_SWAP( "c61.p5",  0x200000, 0x80000, CRC(c45f8e27) SHA1(4d28e0782c31ce56e728ac6ef5edd10437f00637) )
	ROM_LOAD16_WORD_SWAP( "c61.p6",  0x280000, 0x80000, CRC(9ca6f12c) SHA1(26ad682667b983b805e1f577426e5fca8ee3c82b) )
	ROM_LOAD16_WORD_SWAP( "c61.p7",  0x300000, 0x80000, CRC(82ec27af) SHA1(caf76268063ba91d28e8af684d60c2d71f29b9b9) )
	ROM_LOAD16_WORD_SWAP( "c61.p8",  0x380000, 0x80000, CRC(8d931196) SHA1(983e62efcdb4c8db6bce6acf4f86acb9447b565d) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c61.c1",   0x0000000, 0x400000, CRC(09d14566) SHA1(c96463654043f22da5e844c6da17aa9273dc3439) )
	ROM_LOAD64_WORD( "c61.c2",   0x0000002, 0x400000, CRC(ee962057) SHA1(24e359accb5f71a5863d7bad4088719fa547f88c) )
	ROM_LOAD64_WORD( "c61.c3",   0x0000004, 0x400000, CRC(604ece14) SHA1(880fb62b33ba4cceb38635e4ec056fac11a3c70f) )
	ROM_LOAD64_WORD( "c61.c4",   0x0000006, 0x400000, CRC(94a731e8) SHA1(1e784a3412e7361e3001494e1daf840ef8c20449) )
	ROM_LOAD64_WORD( "c61.c5",   0x1000000, 0x400000, CRC(4197973e) SHA1(93aeea1a480b5f452c8a40ae3fff956796b859fa) )
	ROM_LOAD64_WORD( "c61.c6",   0x1000002, 0x400000, CRC(438da4a0) SHA1(ca93b14c3a570f9dd582efbb3f0536a92e535042) )
	ROM_LOAD64_WORD( "c61.c7",   0x1000004, 0x400000, CRC(4db92d94) SHA1(f1b25ccc0627139ad5b287a8f2ab3b4a2fb8b8e4) )
	ROM_LOAD64_WORD( "c61.c8",   0x1000006, 0x400000, CRC(a2b0c6c0) SHA1(71016c01c1a706b73cf5b9ac7e384a030c6cf08d) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c61.m1",   0x00000, 0x08000, CRC(c976e6f9) SHA1(281025e5aaf97c0aeddc8bd0f737d092daadad9e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c61.m2",   0x28000, 0x20000, CRC(ce67d0d9) SHA1(324226597cc5a11603f04085fef7715a314ecc05) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c61.q1",   0x000000, 0x200000, CRC(37ac6d30) SHA1(ec67421fbf4a08a686e76792cb35e9cbf04d022d) )
	ROM_LOAD16_WORD_SWAP( "c61.q2",   0x200000, 0x200000, CRC(de092570) SHA1(a03d0df901f6ea79685eaed67db65bee14ec29c6) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz2ald )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c67d.p1", 0x000000, 0x80000, CRC(89f9483b) SHA1(5da4f82ddc79d9b5697ba2fea96dcaf460118248) )
	ROM_LOAD16_WORD_SWAP( "c67d.p2", 0x080000, 0x80000, CRC(aef27ae5) SHA1(5877a05e7355195d686a4d0a97062aacbd3277ee) )
	ROM_LOAD16_WORD_SWAP( "c67.p3",  0x100000, 0x80000, CRC(f053a55e) SHA1(f98a8af5cd33a543a5596d59381f9adafed38854) )
	ROM_LOAD16_WORD_SWAP( "c67.p4",  0x180000, 0x80000, CRC(cfc0e7a8) SHA1(31ed58451c7a6ac88a8fccab369167694698f044) )
	ROM_LOAD16_WORD_SWAP( "c67.p5",  0x200000, 0x80000, CRC(5feb8b20) SHA1(13c79c9b72c3abf0a0b75d507d91ece71e460c06) )
	ROM_LOAD16_WORD_SWAP( "c67.p6",  0x280000, 0x80000, CRC(6eb6d412) SHA1(c858fec9c1dfea70dfcca629c1c24306f8ae6d81) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfa3ud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78ud.p1", 0x000000, 0x80000, CRC(6db8add7) SHA1(f6dc8ed330254acab16e42a8e729bca7713c8dc1) )
	ROM_LOAD16_WORD_SWAP( "c78ud.p2", 0x080000, 0x80000, CRC(d9c65a26) SHA1(8293ef112eaa534b58a3b56721af5d2f53fd1576) )
	ROM_LOAD16_WORD_SWAP( "c78.p3",   0x100000, 0x80000, CRC(57fd0a40) SHA1(bc2d5f4d57117bbf58b1adb088e00424ef489e92) )
	ROM_LOAD16_WORD_SWAP( "c78.p4",   0x180000, 0x80000, CRC(f6305f8b) SHA1(3fd1ebdbad96103aca604e950b488e52460a71ec) )
	ROM_LOAD16_WORD_SWAP( "c78.p5",   0x200000, 0x80000, CRC(6eab0f6f) SHA1(f8d093dda65cf4e8a3000dc1b96355bb03dcb495) )
	ROM_LOAD16_WORD_SWAP( "c78.p6",   0x280000, 0x80000, CRC(910c4a3b) SHA1(dbd41280f9b16ad6a5b8f12092549970349395f1) )
	ROM_LOAD16_WORD_SWAP( "c78.p7",   0x300000, 0x80000, CRC(b29e5199) SHA1(c6c215eb5aa37f678a9cafcbd8620969fb5ca12f) )
	ROM_LOAD16_WORD_SWAP( "c78.p8",   0x380000, 0x80000, CRC(deb2ff52) SHA1(0aa4722aad68a04164946c78bf05752f947b4322) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( spf2xjd )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65jd.p1", 0x000000, 0x80000, CRC(5e85ed08) SHA1(8242030daf24d8058368d2e7bfe8199db62966c3) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",    0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( vsavd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c71d.p1", 0x000000, 0x80000, CRC(97d805e3) SHA1(394398fbeb0b375a79cefb48c57d14b330311247) )
	ROM_LOAD16_WORD_SWAP( "c71d.p2", 0x080000, 0x80000, CRC(5e07fdce) SHA1(897d1c10fb0de2a62360d9b43cb78faaa9976eb9) )
	ROM_LOAD16_WORD_SWAP( "c71.p3",   0x100000, 0x80000, CRC(4118e00f) SHA1(94ce8abc5ff547667f4c6022d84d0ed4cd062d7e) )
	ROM_LOAD16_WORD_SWAP( "c71.p4",   0x180000, 0x80000, CRC(2f4fd3a9) SHA1(48549ff0121312ea4a18d0fa167a32f905c14c9f) )
	ROM_LOAD16_WORD_SWAP( "c71.p5",   0x200000, 0x80000, CRC(cbda91b8) SHA1(31b20aa92422384b1d7a4706ad4c01ea2bd0e0d1) )
	ROM_LOAD16_WORD_SWAP( "c71.p6",   0x280000, 0x80000, CRC(6ca47259) SHA1(485d8f3a132ccb3f7930cae74de8662d2d44e412) )
	ROM_LOAD16_WORD_SWAP( "c71.p7",   0x300000, 0x80000, CRC(f4a339e3) SHA1(abd101a55f7d9ddb8aba04fe8d3f0f5d2006c925) )
	ROM_LOAD16_WORD_SWAP( "c71.p8",   0x380000, 0x80000, CRC(fffbb5b8) SHA1(38aecb820bd1cbd17287848c3ffb013e1d464ddf) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c71.c1",   0x0000000, 0x400000, CRC(fd8a11eb) SHA1(21b9773959e17976ff46b75a6a405042836b2c5f) )
	ROM_LOAD64_WORD( "c71.c2",   0x0000002, 0x400000, CRC(dd1e7d4e) SHA1(30476e061cdebdb1838b83f4ebd5efae12b7dbfb) )
	ROM_LOAD64_WORD( "c71.c3",   0x0000004, 0x400000, CRC(6b89445e) SHA1(2abd489839d143c46e25f4fc3db476b70607dc03) )
	ROM_LOAD64_WORD( "c71.c4",   0x0000006, 0x400000, CRC(3830fdc7) SHA1(ebd3f559c254d349e256c9feb3477f1ed7518206) )
	ROM_LOAD64_WORD( "c71.c5",   0x1000000, 0x400000, CRC(c1a28e6c) SHA1(012803af33174c0602649d2a2d84f6ee79f54ad2) )
	ROM_LOAD64_WORD( "c71.c6",   0x1000002, 0x400000, CRC(194a7304) SHA1(a19a9a6fb829953b054dc5c3b0dc017f60d37928) )
	ROM_LOAD64_WORD( "c71.c7",   0x1000004, 0x400000, CRC(df9a9f47) SHA1(ce29ff00cf4b6fdd9b3b1ed87823534f1d364eab) )
	ROM_LOAD64_WORD( "c71.c8",   0x1000006, 0x400000, CRC(c22fc3d9) SHA1(df7538c05b03a4ad94d369f8083799979e6fac42) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c71.m1",   0x00000, 0x08000, CRC(f778769b) SHA1(788ce1ad8a322179f634df9e62a31ad776b96762) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c71.m2",   0x28000, 0x20000, CRC(cc09faa1) SHA1(2962ef0ceaf7e7279de3c421ea998763330eb43e) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c71.q1",   0x000000, 0x400000, CRC(e80e956e) SHA1(74181fca4b764fb3c56ceef2cb4c6fd6c18ec4b6) )
	ROM_LOAD16_WORD_SWAP( "c71.q2",   0x400000, 0x400000, CRC(9cd71557) SHA1(7059db25698a0b286314c5961c618f6d2e6f24a1) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( xmcotar1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c56r1d.p1", 0x000000, 0x80000, CRC(bef56003) SHA1(4264f9d7236b00e513664932685b3d93ea636f21) )
	ROM_LOAD16_WORD_SWAP( "c56r1d.p2", 0x080000, 0x80000, CRC(b1a21fa6) SHA1(cbb577b180f28e2af5d6518679f3b16967129ef5) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p3",  0x100000, 0x80000, CRC(ac0d7759) SHA1(650d4474b13f16af7910a0f721fcda2ddb2414fd) )
	ROM_LOAD16_WORD_SWAP( "c56.p4",    0x180000, 0x80000, CRC(1b86a328) SHA1(2469cd705139ee9f1142e6e379e68d0c9675b37e) )
	ROM_LOAD16_WORD_SWAP( "c56.p5",    0x200000, 0x80000, CRC(2c142a44) SHA1(7624875f9c39b361fc83e52e87e0fd5e96279713) )
	ROM_LOAD16_WORD_SWAP( "c56.p6",    0x280000, 0x80000, CRC(f712d44f) SHA1(0d18d4a4eacad94a66beca6ec509ac7f690c6882) )
	ROM_LOAD16_WORD_SWAP( "c56.p7",    0x300000, 0x80000, CRC(9241cae8) SHA1(bb6980abf25aaf3eb14e230ca6942f3e2ab2c660) )
	ROM_LOAD16_WORD_SWAP( "c56r1.p8",  0x380000, 0x80000, CRC(53c0eab9) SHA1(e3b1ec1fd517735f7801cfebb257c43185c6d3fb) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c56.c1",   0x0000000, 0x400000, CRC(bf4df073) SHA1(4d2740c3a827f0ec2cf75ad99c65e393c6a11c23) )
	ROM_LOAD64_WORD( "c56.c2",   0x0000002, 0x400000, CRC(4d7e4cef) SHA1(50b8797b8099a8d76ad063ba1201a13dbb88ae3a) )
	ROM_LOAD64_WORD( "c56.c3",   0x0000004, 0x400000, CRC(513eea17) SHA1(a497477ad9ac13180911d8745ef6ee1955c0b877) )
	ROM_LOAD64_WORD( "c56.c4",   0x0000006, 0x400000, CRC(d23897fc) SHA1(1e31627999736652252164d32662779a1ac6ca29) )
	ROM_LOAD64_WORD( "c56.c5",   0x1000000, 0x400000, CRC(778237b7) SHA1(89a759ec383518ec52f5059d10ec342f2247aa20) )
	ROM_LOAD64_WORD( "c56.c6",   0x1000002, 0x400000, CRC(67b36948) SHA1(692fb6e4096b880aa22996d554b160f664bbd907) )
	ROM_LOAD64_WORD( "c56.c7",   0x1000004, 0x400000, CRC(015a7c4c) SHA1(cccc95dafd076a1a9fa004710006149c42d058ba) )
	ROM_LOAD64_WORD( "c56.c8",   0x1000006, 0x400000, CRC(9dde2758) SHA1(17ba259cad03c7b5d56c0a5eda9ab53521665729) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c56.m1",  0x00000, 0x08000, CRC(40f479ea) SHA1(f29e15f537675305264ae2138a0a537fb9e2008b) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c56.m2",  0x28000, 0x20000, CRC(39d9b5ad) SHA1(af502debfd36100d4fc971ed25fdf9d7121d6f18) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c56.q1",   0x000000, 0x200000, CRC(c848a6bc) SHA1(ac8ac564d3c43225822f8bc330eba9f35b24b0a4) )
	ROM_LOAD16_WORD_SWAP( "c56.q2",   0x200000, 0x200000, CRC(729c188f) SHA1(3279774ad8aebbcf0fc779cdfcbe21044dd192ad) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( xmvsfu1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c69u1d.p1", 0x000000, 0x80000, CRC(4e2e76b7) SHA1(812ebe4063a1c5d8c86200a51e6ab00e57e02869) )
	ROM_LOAD16_WORD_SWAP( "c69u1d.p2", 0x080000, 0x80000, CRC(290c61a7) SHA1(f0d409048c9d477ee98e6df92febcd4492a291ee) )
//  ROM_LOAD16_WORD_SWAP( "c69d.p3a",  0x100000, 0x80000, CRC(de347b11) SHA1(297ae207811df9a4973de1df00b2efaa14a0137d) ) // bad dump?
	ROM_LOAD16_WORD_SWAP( "c69.p3",   0x100000, 0x80000, CRC(7db6025d) SHA1(2d74f48f83f45359bfaca28ab686625766af12ee) )
	ROM_LOAD16_WORD_SWAP( "c69.p4",   0x180000, 0x80000, CRC(e8e2c75c) SHA1(929408cb5d98e95cec75ea58e4701b0cbdbcd016) )
//  ROM_LOAD16_WORD_SWAP( "c69d.p5",   0x200000, 0x80000, CRC(f761ded7) SHA1(e49277398734dea044e7c8ec16800db196905e6f) ) // bad dump ?
	ROM_LOAD16_WORD_SWAP( "c69.p5",    0x200000, 0x80000, CRC(08f0abed) SHA1(ef16c376232dba63b0b9bc3aa0640f9001ccb68a) )
	ROM_LOAD16_WORD_SWAP( "c69.p6",    0x280000, 0x80000, CRC(81929675) SHA1(19cf7afbc1daaefec40195e40ba74970f3906a1c) )
	ROM_LOAD16_WORD_SWAP( "c69.p7",    0x300000, 0x80000, CRC(9641f36b) SHA1(dcba3482d1ba37ccfb30d402793ee063c6621aed) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c69.c1",   0x0000000, 0x400000, CRC(f6684efd) SHA1(c0a2f3a9e82ab8b084a500aec71ac633e947328c) )
	ROM_LOAD64_WORD( "c69.c2",   0x0000002, 0x400000, CRC(29109221) SHA1(898b8f678fd03c462ce0d8eb7fb3441ef601085b) )
	ROM_LOAD64_WORD( "c69.c3",   0x0000004, 0x400000, CRC(92db3474) SHA1(7b6f4c8ebfdac167b25f35029068b6253c141fe6) )
	ROM_LOAD64_WORD( "c69.c4",   0x0000006, 0x400000, CRC(3733473c) SHA1(6579da7145c95b3ad00844a5fc8c2e22c23365e2) )
	ROM_LOAD64_WORD( "c69.c5",   0x1000000, 0x400000, CRC(bcac2e41) SHA1(838ff24f7e8543a787a55a5d592c9517ce3b8b93) )
	ROM_LOAD64_WORD( "c69.c6",   0x1000002, 0x400000, CRC(ea04a272) SHA1(cd7c79037b5b4a39bef5156433e984dc4dc2c081) )
	ROM_LOAD64_WORD( "c69.c7",   0x1000004, 0x400000, CRC(b0def86a) SHA1(da3a6705ea7050fc5c2c10d33400ed67be9f455d) )
	ROM_LOAD64_WORD( "c69.c8",   0x1000006, 0x400000, CRC(4b40ff9f) SHA1(9a981d442132efff09a27408d74646ba357c7357) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c69.m1",   0x00000, 0x08000, CRC(3999e93a) SHA1(fefcff8a9a5c83df7655a16187cf9ba3e7efbb25) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c69.m2",   0x28000, 0x20000, CRC(101bdee9) SHA1(75920e88bf46fcd33a7957777a1d799818ffb0d6) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c69.q1",   0x000000, 0x200000, CRC(9cadcdbc) SHA1(64d3bd53b04daec84c9af4aa3ff010867b3d306d) )
	ROM_LOAD16_WORD_SWAP( "c69.q2",   0x200000, 0x200000, CRC(7b11e460) SHA1(a581c84acaaf0ce056841c15a6f36889e88be68d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( dstlku1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c53ud.p1",  0x000000, 0x80000, CRC(47b7a680) SHA1(ea51d784f1ed49b70aa8addce987a7c09a5c653c) )
	ROM_LOAD16_WORD_SWAP( "c53ud.p2",  0x080000, 0x80000, CRC(3b7a4939) SHA1(020f9768f4d3afb193b1d5a824674f7fb3434369) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p3", 0x100000, 0x80000, CRC(673ed50a) SHA1(7dff27dba1da55a18eb459e4a2d679cf699f2804) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p4", 0x180000, 0x80000, CRC(f2377be7) SHA1(4520d44f94a03bd40c27062344e56ba8718c2fb8) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p5", 0x200000, 0x80000, CRC(d8f498c4) SHA1(569d9c309e9d95d2501a7c0a2c1291b49320d767) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p6", 0x280000, 0x80000, CRC(e6a8a1a0) SHA1(adf621e12623a2af4dbf0858a8fa3816e7c7073b) )
	ROM_LOAD16_WORD_SWAP( "c53ud.p7",  0x300000, 0x80000, CRC(8b333a19) SHA1(5274510491433ac4a0ba3a0a120e95205d291ffe) )
	ROM_LOAD16_WORD_SWAP( "c53ur1.p8", 0x380000, 0x80000, CRC(c1a3d9be) SHA1(82b4ce3325a7ecf3a60dd781f9b224fdde8daa65) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c53.c1",   0x0000000, 0x400000, CRC(c51baf99) SHA1(2fb6642908e542e404391eb17392f8270e87bf48) )
	ROM_LOAD64_WORD( "c53.c2",   0x0000002, 0x400000, CRC(3ce83c77) SHA1(93369b23c6d7d834297434691bb047ee3dd9539c) )
	ROM_LOAD64_WORD( "c53.c3",   0x0000004, 0x400000, CRC(4f2408e0) SHA1(cd49c6b3c7e6470c6058f98ccc5210b052bb13e2) )
	ROM_LOAD64_WORD( "c53.c4",   0x0000006, 0x400000, CRC(9ff60250) SHA1(d69ba4dc6bd37d003245f0cf3211d6e2623005b8) )
	ROM_LOAD64_WORD( "c53.c5",   0x1000000, 0x100000, CRC(bd87243c) SHA1(87b33aeb72514e1228ffc27ec6dd534f14882760) )
	ROM_LOAD64_WORD( "c53.c6",   0x1000002, 0x100000, CRC(afec855f) SHA1(cd117833b8d475489b90ff44b57e2c5cb1af3af5) )
	ROM_LOAD64_WORD( "c53.c7",   0x1000004, 0x100000, CRC(3a033625) SHA1(294238f30cba5cf4f8f1de951d54c2077bd95de9) )
	ROM_LOAD64_WORD( "c53.c8",   0x1000006, 0x100000, CRC(2bff6a89) SHA1(8f4e131e5ce0af48fb89f98026d9f0356c7c301f) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c53.m1",   0x00000, 0x08000, CRC(64b685d5) SHA1(6c180e7420db754eca5cad17a40f5a64f5c3bd15) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c53.m2",   0x28000, 0x20000, CRC(cf7c97c7) SHA1(109a4b56ecd59be9c3f5869de99d40619bdaef21) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c53.q1",   0x000000, 0x200000, CRC(4a39deb2) SHA1(7e63e615869958db66a4e52a0272afee5a10e446) )
	ROM_LOAD16_WORD_SWAP( "c53.q2",   0x200000, 0x200000, CRC(1a3e5c03) SHA1(c5a556e125d6c3d68da745b4d56cd7a851f2a23d) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( progearud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85ud.p1", 0x000000, 0x80000, CRC(ba22b9c5) SHA1(3bacef465ad137b7d618ad7dbe4a0f29b3422a31) )
	ROM_LOAD16_WORD_SWAP( "c85ud.p2", 0x080000, 0x80000, CRC(df3927ef) SHA1(2869f3f7c1ae76e9b3ce5c66eed277ee8484aeba) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c85.c1",   0x0000000, 0x200000,  CRC(452f98b0) SHA1(a10e615c32098f6d25becd466da8faa967523a7b) )
	ROM_LOAD64_BYTE( "c85.c2",   0x0000001, 0x200000,  CRC(9e672092) SHA1(fce0b8b43a1c069262f4e3e81c1a04621e232c88) )
	ROM_LOAD64_BYTE( "c85.c3",   0x0000002, 0x200000,  CRC(ae9ddafe) SHA1(afbb26fed6cd0cb5c0099a10d35aeb453318c14d) )
	ROM_LOAD64_BYTE( "c85.c4",   0x0000003, 0x200000,  CRC(94d72d94) SHA1(df6a3fe49c008f73b160eb6f2a44dc371ff73cba) )
	ROM_LOAD64_BYTE( "c85.c5",   0x0000004, 0x200000,  CRC(48a1886d) SHA1(ebf44b42d784924e08a832a7e5f66a887bab244b) )
	ROM_LOAD64_BYTE( "c85.c6",   0x0000005, 0x200000,  CRC(172d7e37) SHA1(0eaedd24cd3fa87b6f35fbd63078d40c493c92d0) )
	ROM_LOAD64_BYTE( "c85.c7",   0x0000006, 0x200000,  CRC(9ee33d98) SHA1(85d1bd31940e35ac8c732165020881a2d65cd6b1) )
	ROM_LOAD64_BYTE( "c85.c8",   0x0000007, 0x200000,  CRC(848dee32) SHA1(c591288e86ad1624d0fe66563808af9fac786e64) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.q1",   0x000000, 0x200000, CRC(c0aac80c) SHA1(91784d35d4f7e113529bb5be6081b67094b150ea) )
	ROM_LOAD16_WORD_SWAP( "c85.q2",   0x200000, 0x200000, CRC(37a65d86) SHA1(374d562a4648734f82aa2ddb6d258e870896dd45) )
	ROM_LOAD16_WORD_SWAP( "c85.q3",   0x400000, 0x200000, CRC(d3f1e934) SHA1(5dcea28c873d0d472f5b94e07d97cd77ace2b252) )
	ROM_LOAD16_WORD_SWAP( "c85.q4",   0x600000, 0x200000, CRC(8b39489a) SHA1(fd790efaf37dc2c4c16f657941044e3e2d3c2711) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( progearjd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85jd.p1", 0x000000, 0x80000, CRC(0271f3a3) SHA1(caf5ce19aa3471e65931ebd7ef06693e9fdf11a7) )
	ROM_LOAD16_WORD_SWAP( "c85jd.p2", 0x080000, 0x80000, CRC(be4b7799) SHA1(0414922296079ca9380a41bf3c628ce67acec06d) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c85.c1",   0x0000000, 0x200000,  CRC(452f98b0) SHA1(a10e615c32098f6d25becd466da8faa967523a7b) )
	ROM_LOAD64_BYTE( "c85.c2",   0x0000001, 0x200000,  CRC(9e672092) SHA1(fce0b8b43a1c069262f4e3e81c1a04621e232c88) )
	ROM_LOAD64_BYTE( "c85.c3",   0x0000002, 0x200000,  CRC(ae9ddafe) SHA1(afbb26fed6cd0cb5c0099a10d35aeb453318c14d) )
	ROM_LOAD64_BYTE( "c85.c4",   0x0000003, 0x200000,  CRC(94d72d94) SHA1(df6a3fe49c008f73b160eb6f2a44dc371ff73cba) )
	ROM_LOAD64_BYTE( "c85.c5",   0x0000004, 0x200000,  CRC(48a1886d) SHA1(ebf44b42d784924e08a832a7e5f66a887bab244b) )
	ROM_LOAD64_BYTE( "c85.c6",   0x0000005, 0x200000,  CRC(172d7e37) SHA1(0eaedd24cd3fa87b6f35fbd63078d40c493c92d0) )
	ROM_LOAD64_BYTE( "c85.c7",   0x0000006, 0x200000,  CRC(9ee33d98) SHA1(85d1bd31940e35ac8c732165020881a2d65cd6b1) )
	ROM_LOAD64_BYTE( "c85.c8",   0x0000007, 0x200000,  CRC(848dee32) SHA1(c591288e86ad1624d0fe66563808af9fac786e64) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85.q1",   0x000000, 0x200000, CRC(c0aac80c) SHA1(91784d35d4f7e113529bb5be6081b67094b150ea) )
	ROM_LOAD16_WORD_SWAP( "c85.q2",   0x200000, 0x200000, CRC(37a65d86) SHA1(374d562a4648734f82aa2ddb6d258e870896dd45) )
	ROM_LOAD16_WORD_SWAP( "c85.q3",   0x400000, 0x200000, CRC(d3f1e934) SHA1(5dcea28c873d0d472f5b94e07d97cd77ace2b252) )
	ROM_LOAD16_WORD_SWAP( "c85.q4",   0x600000, 0x200000, CRC(8b39489a) SHA1(fd790efaf37dc2c4c16f657941044e3e2d3c2711) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( progearjbl )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c85jbl.p1", 0x000000, 0x80000, CRC(4fef676c) SHA1(f50f32403315f77e115141bbee6a1b9a800821eb) )
	ROM_LOAD16_WORD_SWAP( "c85jbl.p2", 0x080000, 0x80000, CRC(a069bd3b) SHA1(6b7e20c883221da9b8eccb4f86017bb93e1fc11f) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c85bl.c1",   0x0000000, 0x400000, CRC(5194c198) SHA1(f9ae16cad8b30547b8dd3a1ea1d6e845d76ccefe) )
	ROM_LOAD64_WORD( "c85bl.c2",   0x0000002, 0x400000, CRC(b794e83f) SHA1(0332fa798b2955464ab336929866c9185bd4b520) )
	ROM_LOAD64_WORD( "c85bl.c3",   0x0000004, 0x400000, CRC(87f22918) SHA1(7f048d66593f385526bd2c669cf6b8ad58d12bbf) )
	ROM_LOAD64_WORD( "c85bl.c4",   0x0000006, 0x400000, CRC(65ffb45b) SHA1(9c21187519c1ec95c5a960b2c815ee41ace4fa7e) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c85.m1",   0x00000, 0x08000, CRC(bdbfa992) SHA1(7c5496c1daaea6a7ab95c0b25625d325ec3427cc) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c85bl.q1",   0x000000, 0x400000, CRC(abdd224e) SHA1(3316708e050204eeee91c642a2546fab80984c60) )
	ROM_LOAD16_WORD_SWAP( "c85bl.q2",   0x400000, 0x400000, CRC(dac53406) SHA1(53fd271495429a7cbdeecbbc62b322f019f854a5) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ssf2ud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48ud.p1", 0x000000, 0x80000, CRC(fad5daf8) SHA1(5bed5d5af8dffc54d3b3371274a3905f46384f79) )
	ROM_LOAD16_WORD_SWAP( "c48ud.p2", 0x080000, 0x80000, CRC(0d31af65) SHA1(7e903e18cb899627fedd8ad92594b75b5d8ee8fd) )
	ROM_LOAD16_WORD_SWAP( "c48ud.p3",  0x100000, 0x80000, CRC(75c651ef) SHA1(8c6f60d3cf10d802190438c403a719bc30ccbeaa) )
	ROM_LOAD16_WORD_SWAP( "c48ud.p4",  0x180000, 0x80000, CRC(85c3ec00) SHA1(29ab3eccc5f9c0dd6bd19eaa83c72e1a1d44b320) )
	ROM_LOAD16_WORD_SWAP( "c48ud.p5",  0x200000, 0x80000, CRC(247e2504) SHA1(b2f13524fe7f75e69d2d8f2dd212340d75030149) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ssf2tbd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c48tbd.p1", 0x000000, 0x80000, CRC(5d86caf8) SHA1(6209caad6613f0b35002fb13350ae3f81c8c9c5f) )
	ROM_LOAD16_WORD_SWAP( "c48tbd.p2", 0x080000, 0x80000, CRC(f6e1f98d) SHA1(2d7506fdf12c8ca8766da1458420e0764f159a9b) )
	ROM_LOAD16_WORD_SWAP( "c48ud.p3",  0x100000, 0x80000, CRC(75c651ef) SHA1(8c6f60d3cf10d802190438c403a719bc30ccbeaa) )
	ROM_LOAD16_WORD_SWAP( "c48tbd.p4", 0x180000, 0x80000, CRC(9adac7d7) SHA1(8677f882f84e6db9ed39ae89ffc3bf9b6b53e4e5) )
	ROM_LOAD16_WORD_SWAP( "c48tbd.p5", 0x200000, 0x80000, CRC(84f54db3) SHA1(7c2016f9c1839f096c9a39bad882eed95bf552b4) )

	ROM_REGION( 0xc00000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c48r1.m1",   0x00000, 0x08000, CRC(eb247e8c) SHA1(24296c18d9b1136d69712bf1c9d9d15463041e83) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD( "c48.q1",  0x000000, 0x080000, CRC(a6f9da5c) SHA1(6d19f83a01bd25b838d5c2871f7964529d926c98) )
	ROM_LOAD( "c48.q2",  0x080000, 0x080000, CRC(8c66ae26) SHA1(32a82aee6ed4480e5a990f9af161734c7c0a1403) )
	ROM_LOAD( "c48.q3",  0x100000, 0x080000, CRC(695cc2ca) SHA1(c2675f0233608b76de528d2a6ef19846d1348060) )
	ROM_LOAD( "c48.q4",  0x180000, 0x080000, CRC(9d9ebe32) SHA1(9b26329370041374f1a90b479a172d2bc2801c4d) )
	ROM_LOAD( "c48.q5",  0x200000, 0x080000, CRC(4770e7b7) SHA1(0e764f0befb9227b0b36508d8ca8ec9be31bcb05) )
	ROM_LOAD( "c48.q6",  0x280000, 0x080000, CRC(4e79c951) SHA1(1144781d7dc57ef8a6458d982f5c91548ff59e27) )
	ROM_LOAD( "c48.q7",  0x300000, 0x080000, CRC(cdd14313) SHA1(ebe767a9d4b51dba4282fa0a36a546a88620dd59) )
	ROM_LOAD( "c48.q8",  0x380000, 0x080000, CRC(6f5a088c) SHA1(6c0b4690479647a99d76335f174be8455b4ff118) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ssf2xjr1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51jr1d.p1", 0x000000, 0x80000, CRC(316de996) SHA1(4036539a554a9ccd8b5fc364dfc4c97f3d5efa96) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p2",   0x080000, 0x80000, CRC(9bf3bb2e) SHA1(4bdc6fa585cc67d3b6695f390c95c518cba2bea6) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p3",   0x100000, 0x80000, CRC(c63358d0) SHA1(dde4d9e9adce4dee02322c0fd71615eed0af62e6) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p4",   0x180000, 0x80000, CRC(ccb29808) SHA1(10e7b135a936409fe7c4d7959ea375634a8c68e4) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p5",   0x200000, 0x80000, CRC(61f94982) SHA1(d2f22f50c21393deda7d5838dbd2b265722acd38) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p6",   0x280000, 0x80000, CRC(d399c36c) SHA1(b4c4217843e5c3ef00ab04d58ca9368d2d734065) )
	ROM_LOAD16_WORD_SWAP( "c51jr1d.p7", 0x300000, 0x80000, CRC(0b3a6196) SHA1(a0480ac878e82ef6ec0f64dbbd621f10bc7906ea) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( ssf2tad )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c51ad.p1",  0x000000, 0x80000, CRC(e3c92ece) SHA1(8b0d6f632f51719c3e13add5d27701578b6ce0d6) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p2",  0x080000, 0x80000, CRC(9bf3bb2e) SHA1(4bdc6fa585cc67d3b6695f390c95c518cba2bea6) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p3",  0x100000, 0x80000, CRC(c63358d0) SHA1(dde4d9e9adce4dee02322c0fd71615eed0af62e6) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p4",  0x180000, 0x80000, CRC(ccb29808) SHA1(10e7b135a936409fe7c4d7959ea375634a8c68e4) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p5",  0x200000, 0x80000, CRC(61f94982) SHA1(d2f22f50c21393deda7d5838dbd2b265722acd38) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p6",  0x280000, 0x80000, CRC(d399c36c) SHA1(b4c4217843e5c3ef00ab04d58ca9368d2d734065) )
	ROM_LOAD16_WORD_SWAP( "c51ad.p7",  0x300000, 0x80000, CRC(436784ae) SHA1(4f0c8a46842128acf4ef15e3ece5abad859d181f) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c48.c1",   0x000000, 0x200000, CRC(cf94d275) SHA1(bf2a6d98a656d1cb5734da7836686242d3211137) )
	ROM_LOAD64_WORD( "c48.c2",   0x000002, 0x200000, CRC(5eb703af) SHA1(4b302dbb66e8a5c2ad92798699391e981bada427) )
	ROM_LOAD64_WORD( "c48.c3",   0x000004, 0x200000, CRC(ffa60e0f) SHA1(b21b1c749a8241440879bf8e7cb33968ccef97e5) )
	ROM_LOAD64_WORD( "c48.c4",   0x000006, 0x200000, CRC(34e825c5) SHA1(4d320fc96d1ef0b9928a8ce801734245a4c097a5) )
	ROM_LOAD64_WORD( "c48.c5",   0x800000, 0x100000, CRC(b7cc32e7) SHA1(0f4d26af338dab5dce5b7b34d32ad0c573434ace) )
	ROM_LOAD64_WORD( "c48.c6",   0x800002, 0x100000, CRC(8376ad18) SHA1(f4456833fb396e6501f4174c0fe5fd63ea40a188) )
	ROM_LOAD64_WORD( "c48.c7",   0x800004, 0x100000, CRC(f5b1b336) SHA1(4b060501e56b9d61294748da5387cdae5280ec4d) )
	ROM_LOAD64_WORD( "c48.c8",   0x800006, 0x100000, CRC(459d5c6b) SHA1(32b11ba7a12004aff810d719bff7508204c7b7c0) )
	ROM_LOAD64_WORD( "c51.c9",   0xc00000, 0x100000, CRC(e32854af) SHA1(1a5e11e9caa2b96108d89ae660ef1f6bcb469a74) )
	ROM_LOAD64_WORD( "c51.c10",  0xc00002, 0x100000, CRC(760f2927) SHA1(491e28e14ee06821fc9e709efa7b91313bc0c2db) )
	ROM_LOAD64_WORD( "c51.c11",  0xc00004, 0x100000, CRC(1ee90208) SHA1(83df1d9953560edddc2951ea426d29fb014e6a8a) )
	ROM_LOAD64_WORD( "c51.c12",  0xc00006, 0x100000, CRC(f814400f) SHA1(ad6921af36d0bd5dfb89b1fb53c3ca3fd92d7204) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c51.m1",   0x00000, 0x08000, CRC(b47b8835) SHA1(c8b2d50fe3a329bd0592ea160d505155d873dab1) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c51.m2",   0x28000, 0x20000, CRC(0022633f) SHA1(cab3afc79da53e3887eb1ccd1f4d19790728e6cd) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c51.q1",   0x000000, 0x200000, CRC(9bdbd476) SHA1(a8520f77f30b97aae36408e0c4ca1ebbde1808a5) )
	ROM_LOAD16_WORD_SWAP( "c51.q2",   0x200000, 0x200000, CRC(a05e3aab) SHA1(d4eb9cae66c74e956569fea8b815156fbd420f83) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sgemfd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c74d.p1", 0x000000, 0x80000, CRC(8b83674a) SHA1(053a2cb6aab9aa43f4c0d4ba9f0eb5a964133b28) )
	ROM_LOAD16_WORD_SWAP( "c74d.p2", 0x080000, 0x80000, CRC(b58f1d03) SHA1(592649956471363967b9920f47bde23da3e9cc2b) )
	ROM_LOAD16_WORD_SWAP( "c74.p3",  0x100000, 0x80000, CRC(215655f6) SHA1(242c0f4401520f2a3b0deafc3a807b18b987e496) )
	ROM_LOAD16_WORD_SWAP( "c74.p4",  0x180000, 0x80000, CRC(ea6f13ea) SHA1(1bc924a8a9da1d2ad7667685cdb92fe317a39aba) )
	ROM_LOAD16_WORD_SWAP( "c74.p5",  0x200000, 0x80000, CRC(5ac6d5ea) SHA1(9ce8e4668b565658597a868830545fb75a5eeaa6) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c74.c1",   0x0000000, 0x400000, CRC(22d72ab9) SHA1(653efd95c34b4b9d2ab0d219f41a99ca84e12214) )
	ROM_LOAD64_WORD( "c74.c2",   0x0000002, 0x400000, CRC(16a4813c) SHA1(bf5fce6008214f353414d1b64bea4ed0c7673670) )
	ROM_LOAD64_WORD( "c74.c3",   0x0000004, 0x400000, CRC(1097e035) SHA1(4bd51e4e9447af27d2cac1f6d2201e37c949912b) )
	ROM_LOAD64_WORD( "c74.c4",   0x0000006, 0x400000, CRC(d362d874) SHA1(30c42af18440496cc05e4418e4efa41172ae4ced) )
	ROM_LOAD64_WORD( "c74.c5",   0x1000000, 0x100000, CRC(0383897c) SHA1(aba14afa1d0c971afcee4317f480e88117d77b5e) )
	ROM_LOAD64_WORD( "c74.c6",   0x1000002, 0x100000, CRC(76f91084) SHA1(3d1e32467f2aa5dd6fb96bd5c866ecc9691660fc) )
	ROM_LOAD64_WORD( "c74.c7",   0x1000004, 0x100000, CRC(756c3754) SHA1(be2f709b90222a567f198f851cf07ffb0ad433d7) )
	ROM_LOAD64_WORD( "c74.c8",   0x1000006, 0x100000, CRC(9ec9277d) SHA1(b7ceeaca30dfcdf498b61a6961f0aa1a068b8ec4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c74.m1",   0x00000, 0x08000, CRC(254e5f33) SHA1(c413ec0630b9bdd15e64f42893eba8958a09b573) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c74.m2",   0x28000, 0x20000, CRC(6902f4f9) SHA1(9bfe4ddade3c666076d26a2b545120f6d059fd7c) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c74.q1",   0x000000, 0x400000, CRC(a5dea005) SHA1(3ae79baf6ff5bd527f82b26f164c7e3c65423ae2) )
	ROM_LOAD16_WORD_SWAP( "c74.q2",   0x400000, 0x400000, CRC(4ce235fe) SHA1(795b94557e954cc0e45fd3778b609064d57a34a2) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( armwar1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c55r1d.p1", 0x000000, 0x80000, CRC(496bd483) SHA1(c3a116ea019843aba666a0d7e7bd2975622306e3) )
	ROM_LOAD16_WORD_SWAP( "c55r1d.p2", 0x080000, 0x80000, CRC(9bd6a38f) SHA1(da90162b7bff223df59ac362a5f61c580a86d967) )
	ROM_LOAD16_WORD_SWAP( "c55r1d.p3", 0x100000, 0x80000, CRC(4c11d30f) SHA1(463ba1845b1239c839e9419cbc6762d52b7918db) )
	ROM_LOAD16_WORD_SWAP( "c55.p4",   0x180000, 0x80000, CRC(87a60ce8) SHA1(e2085c7c8c6792d055dbbb023c7f4e4aa38ae924) )
	ROM_LOAD16_WORD_SWAP( "c55.p5",   0x200000, 0x80000, CRC(f7b148df) SHA1(f369669713cf647222094c570a2eacd48a8637cf) )
	ROM_LOAD16_WORD_SWAP( "c55.p6",   0x280000, 0x80000, CRC(cc62823e) SHA1(edaf9bebdfc65ae5414090abd6844176eec39a00) )
	ROM_LOAD16_WORD_SWAP( "c55u1.p7",   0x300000, 0x80000, CRC(ddc85ca6) SHA1(e794c679531632e2142c6a5e3b858494389ce65e) )
	ROM_LOAD16_WORD_SWAP( "c55.p8",   0x380000, 0x80000, CRC(07c4fb28) SHA1(58a1ff3d105be7df833dd4f32973766649efcbcf) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c55.c1",   0x0000000, 0x400000, CRC(ae8fe08e) SHA1(b6f09663dcda69b5d7ac13e4afaf1efd692fb61e) )
	ROM_LOAD64_WORD( "c55.c2",   0x0000002, 0x400000, CRC(db560f58) SHA1(0c3716b32eb24544ff5d16b5dcadce195cd10d00) )
	ROM_LOAD64_WORD( "c55.c3",   0x0000004, 0x400000, CRC(bc475b94) SHA1(a157664450895a146a532581dd6f4b63dff21c86) )
	ROM_LOAD64_WORD( "c55.c4",   0x0000006, 0x400000, CRC(07439ff7) SHA1(f71e07c6d77c32828f5e319268b24b13a1a4b0c2) )
	ROM_LOAD64_WORD( "c55.c5",   0x1000000, 0x100000, CRC(c3f9ba63) SHA1(66191a52c39daa89b17ede5804ee41c028036f14) )
	ROM_LOAD64_WORD( "c55.c6",   0x1000002, 0x100000, CRC(815b0e7b) SHA1(549785daac3122253fb94f6541bc7016147f5306) )
	ROM_LOAD64_WORD( "c55.c7",   0x1000004, 0x100000, CRC(0109c71b) SHA1(eb51284ee0c85ff8f605fe1d166b7aa202be1344) )
	ROM_LOAD64_WORD( "c55.c8",   0x1000006, 0x100000, CRC(eb75ffbe) SHA1(e9d1deca60be696ac5bff2017fb5de3525e5239a) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c55.m1",   0x00000, 0x08000, CRC(18a5c0e4) SHA1(bb1353dd74884aaeec9b5f1d0b284d9cad53c0ff) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c55.m2",   0x28000, 0x20000, CRC(c9dfffa6) SHA1(64e71028befe9a2514074be765dd020e1d2ea70b) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c55.q1",   0x000000, 0x200000, CRC(a78f7433) SHA1(e47ffba7b9dac9d0dda985c5d966194be18260f7) )
	ROM_LOAD16_WORD_SWAP( "c55.q2",   0x200000, 0x200000, CRC(77438ed0) SHA1(733ca6c6a792e66e2aa12c5fc06dd459527afe4b) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( dimahoud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c81ud.p1", 0x000000, 0x80000, CRC(12888435) SHA1(38204359c505ae861341936cd0fd2517411b8995) )
	ROM_LOAD16_WORD_SWAP( "c81.p2",   0x080000, 0x80000, CRC(37485567) SHA1(643c41fce6057bcaef0e0bedc62914c33d97eeaf) )
	ROM_LOAD16_WORD_SWAP( "c81.p3",   0x100000, 0x80000, CRC(da269ffb) SHA1(e99b04192030b6006cf67b563f40cea29c1b2e78) )
	ROM_LOAD16_WORD_SWAP( "c81ud.p4", 0x180000, 0x80000, CRC(d825efda) SHA1(7299f3629d5136f567f5cf373754ea13b2190533) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c81.c1",   0x000000, 0x400000, CRC(80dd19f0) SHA1(0fd8b1e8d73cc83e6c34f0d94487938da2344f76) )
	ROM_LOAD64_WORD( "c81.c2",   0x000002, 0x400000, CRC(dfd93a78) SHA1(c343d5ddcc25bd0739491e7439d7c0d0a8881a04) )
	ROM_LOAD64_WORD( "c81.c3",   0x000004, 0x400000, CRC(16356520) SHA1(058713bef30c1b1d8b7dd0ceaaa57a3ab9751a70) )
	ROM_LOAD64_WORD( "c81.c4",   0x000006, 0x400000, CRC(dfc33031) SHA1(a1ceaeddc2a79d5b79f1b107cac2ef6a5e621e77) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c81.m1",   0x00000, 0x08000, CRC(3f9bc985) SHA1(1616bbee82877b1052a07531066f5009a80706be) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c81.m2",   0x28000, 0x20000, CRC(3fd39dde) SHA1(6a6e3ef9baa430ee83ab2312aa0221bae4d73dbd) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c81.q1",   0x000000, 0x400000, CRC(06a65542) SHA1(a1b3df70c90055a3cd59d0149fd18a74eff5bcc9) )
	ROM_LOAD16_WORD_SWAP( "c81.q2",   0x400000, 0x400000, CRC(50bc7a31) SHA1(7283569fc646c39f4c693f14e0ce7ff2ee49111a) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( mmatrixd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c82d.p1", 0x000000, 0x80000, CRC(36711e60) SHA1(a192e786798f69320761017437ee4d2d47dc80c5) )
	ROM_LOAD16_WORD_SWAP( "c82d.p2", 0x080000, 0x80000, CRC(4687226f) SHA1(b831582f578eb1e40bce1d1cbf231e4c27f510cd) )
	ROM_LOAD16_WORD_SWAP( "c82d.p3", 0x100000, 0x80000, CRC(52124398) SHA1(0a18b9d2a7e3335ddf7ff9ac5c5f8298951f8c67) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c82.c1",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "c82.c2",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "c82.c3",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "c82.c4",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "c82.c5",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "c82.c6",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "c82.c7",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "c82.c8",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c82.m1",   0x00000, 0x08000, CRC(c57e8171) SHA1(dedb92af1910d38727f816e6f507d25148f31b74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c82.q1",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "c82.q2",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( mmatrixuf )
	ROM_REGION( CODE_SIZE, "maincpu", 0 ) // 68000 code
	ROM_LOAD16_WORD_SWAP( "mmxuf.03", 0x000000, 0x80000, CRC(a578097b) SHA1(7d7153d58930d07aca61b9cddab650b687db51ff) )
	ROM_LOAD16_WORD_SWAP( "mmxuf.04", 0x080000, 0x80000, CRC(0135fc6c) SHA1(e40c8fa51dcb300b3ee72dc7de137e0b39dea490) )
	ROM_LOAD16_WORD_SWAP( "mmxuf.05", 0x100000, 0x80000, CRC(a1fbd3d8) SHA1(87a2494c88de302b10bb70a218d74ccf6b3b59a8) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "mmx.13m",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "mmx.15m",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "mmx.17m",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "mmx.19m",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "mmx.14m",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "mmx.16m",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "mmx.18m",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "mmx.20m",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 ) // 64k for the audio CPU (+banks)
	ROM_LOAD( "mmxuf.01",   0x00000, 0x08000, CRC(45a29ce5) SHA1(f06f4479188c21e8be08797e4d86769c85aaaa1b) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 ) // QSound samples
	ROM_LOAD16_WORD_SWAP( "mmx.11m",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "mmx.12m",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmatrixuf.key", 0x000000, 0x000014, CRC(8ed66bc4) SHA1(324d7bf9d95220ecab140b1385eba7087f1a9669) )
ROM_END

ROM_START( mmatrixufj )
	ROM_REGION( CODE_SIZE, "maincpu", 0 ) // 68000 code
	ROM_LOAD16_WORD_SWAP( "mmxufj.03", 0x000000, 0x80000, CRC(dec3c9da) SHA1(91cfecd18ac3181387355e7b43ae8466b44ccb53) )
	ROM_LOAD16_WORD_SWAP( "mmxufj.04", 0x080000, 0x80000, CRC(d943a339) SHA1(ae3d217b35f92fc727bda3b14f13f3658dab3dd8) )
	ROM_LOAD16_WORD_SWAP( "mmxufj.05", 0x100000, 0x80000, CRC(c1a306fa) SHA1(3c7f2b4c86b8e96b13802bb942c6a7fbff041c86) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "mmx.13m",   0x0000000, 0x400000, CRC(04748718) SHA1(d2e84d9dcc779c08469d815ccd709f30705954b8) )
	ROM_LOAD64_WORD( "mmx.15m",   0x0000002, 0x400000, CRC(38074f44) SHA1(2002c4862c156b314bc4f3372b713c48e0667ec3) )
	ROM_LOAD64_WORD( "mmx.17m",   0x0000004, 0x400000, CRC(e4635e35) SHA1(48ef7a82df83b981ddd6138c241ca129ab770e8e) )
	ROM_LOAD64_WORD( "mmx.19m",   0x0000006, 0x400000, CRC(4400a3f2) SHA1(d0aa805ccbb153896e5983da1c398d1df4f40371) )
	ROM_LOAD64_WORD( "mmx.14m",   0x1000000, 0x400000, CRC(d52bf491) SHA1(2398895cfdcf86fc485472e33df2cc446539e977) )
	ROM_LOAD64_WORD( "mmx.16m",   0x1000002, 0x400000, CRC(23f70780) SHA1(691ee8964815b0ce54704e7feb59ca79b634f26d) )
	ROM_LOAD64_WORD( "mmx.18m",   0x1000004, 0x400000, CRC(2562c9d5) SHA1(e7defc3d33db632c4035ae069f2f2332c58afaf5) )
	ROM_LOAD64_WORD( "mmx.20m",   0x1000006, 0x400000, CRC(583a9687) SHA1(1d0b08b1e88509245db3c2090f0201938fd750b4) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 ) // 64k for the audio CPU (+banks)
	ROM_LOAD( "mmxufj.01",   0x00000, 0x08000, CRC(45a29ce5) SHA1(f06f4479188c21e8be08797e4d86769c85aaaa1b) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x800000, "qsound", 0 ) // QSound samples
	ROM_LOAD16_WORD_SWAP( "mmx.11m",   0x000000, 0x400000, CRC(4180b39f) SHA1(cabb1c358eae1bb6cfed07f5b92e4acd38650667) )
	ROM_LOAD16_WORD_SWAP( "mmx.12m",   0x400000, 0x400000, CRC(95e22a59) SHA1(b3431d170c0a1a0d826ad0af21300b9180e3f114) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "mmatrixufj.key", 0x000000, 0x000014, CRC(3b50d889) SHA1(fb749a5da0a799322ec131a15a017971e586c5fc) )
ROM_END


ROM_START( vhunt2d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c75d.p1", 0x000000, 0x80000, CRC(696e0157) SHA1(a5c8c7e389f76952ce40ce5792eddb185dd76c41) )
	ROM_LOAD16_WORD_SWAP( "c75d.p2", 0x080000, 0x80000, CRC(ced9bba3) SHA1(ef7be9d39ae93ffde7d940736de9711a24585ef4) )
	ROM_LOAD16_WORD_SWAP( "c75.p3",  0x100000, 0x80000, CRC(de34f624) SHA1(60bbbd1765e76839b01c38765da2368c5188ec61) )
	ROM_LOAD16_WORD_SWAP( "c75.p4",  0x180000, 0x80000, CRC(6a3b9897) SHA1(4f3b37004db8a3d3dde709b51c94c392615134b5) )
	ROM_LOAD16_WORD_SWAP( "c75.p5",  0x200000, 0x80000, CRC(b021c029) SHA1(de4299197600608e83fe50775e3f352f5add844d) )
	ROM_LOAD16_WORD_SWAP( "c75.p6",  0x280000, 0x80000, CRC(ac873dff) SHA1(ad9a085b8403801035683b6f63eee33daf4e97ae) )
	ROM_LOAD16_WORD_SWAP( "c75.p7",  0x300000, 0x80000, CRC(eaefce9c) SHA1(d842a824f0d0adc13a86f780084164c1273c45a4) )
	ROM_LOAD16_WORD_SWAP( "c75.p8",  0x380000, 0x80000, CRC(11730952) SHA1(2966b80b99ab065614a6ddb546110f482b998e32) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c75.c1",   0x0000000, 0x400000, CRC(3b02ddaa) SHA1(a73b0554afbfc7ace41bdf8e6cafd4c1ef0b0a08) )
	ROM_LOAD64_WORD( "c75.c2",   0x0000002, 0x400000, CRC(4e40de66) SHA1(e8b80eadffad6070aa04c8ab426311c44e7c5507) )
	ROM_LOAD64_WORD( "c75.c3",   0x0000004, 0x400000, CRC(b31d00c9) SHA1(7e7be64690663f52d10c8946aabec4250c8a8740) )
	ROM_LOAD64_WORD( "c75.c4",   0x0000006, 0x400000, CRC(149be3ab) SHA1(afc8e96e6aa3cf1db6dfd8075030a6c50b4419a9) )
	ROM_LOAD64_WORD( "c75.c5",   0x1000000, 0x400000, CRC(cd09bd63) SHA1(e582b20a948ae54f52590496051688dbfae2bc9c) )
	ROM_LOAD64_WORD( "c75.c6",   0x1000002, 0x400000, CRC(e0182c15) SHA1(a924d53ab39f4d85173bdb92a197dde2db0dc3f7) )
	ROM_LOAD64_WORD( "c75.c7",   0x1000004, 0x400000, CRC(778dc4f6) SHA1(8d0cd1c387b4b6ac7f92bb2e5a25983856328cdc) )
	ROM_LOAD64_WORD( "c75.c8",   0x1000006, 0x400000, CRC(605d9d1d) SHA1(99bc27557741527ca678d7b6307164bc04ebedc6) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c75.m1",  0x00000, 0x08000, CRC(67b9f779) SHA1(3994c65f888004b56ea9f478b1feaa81e306347e) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c75.m2",  0x28000, 0x20000, CRC(aaf15fcb) SHA1(6f61daa162c835165a8aabaf1d0ea8816fbfbd40) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c75.q1",  0x000000, 0x400000, CRC(38922efd) SHA1(8cfb36bdce3a524d0a81fec12ca0cba82222fa30) )
	ROM_LOAD16_WORD_SWAP( "c75.q2",  0x400000, 0x400000, CRC(6e2430af) SHA1(b475faf943bec4171ba0130f287e1948743ca273) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( vsav2d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c76d.p1", 0x000000, 0x80000, CRC(50865f7b) SHA1(30914bde1caec55847f28167bfb48bc4473a376c) )
	ROM_LOAD16_WORD_SWAP( "c76d.p2", 0x080000, 0x80000, CRC(c3bff0e3) SHA1(5b81dfdc3bf4e78a9d023cd8c753ce0839c0e175) )
	ROM_LOAD16_WORD_SWAP( "c76.p3",   0x100000, 0x80000, CRC(61979638) SHA1(4d5625a9a06926c1a42c8f6e3a4c943f17750ec2) )
	ROM_LOAD16_WORD_SWAP( "c76.p4",   0x180000, 0x80000, CRC(f37c5bc2) SHA1(d8c1040a6ee6b9fc677a6a32b99bf02b6a707812) )
	ROM_LOAD16_WORD_SWAP( "c76.p5",   0x200000, 0x80000, CRC(8f885809) SHA1(69dac07e1f483b6478f792d20a137d6a081fbea3) )
	ROM_LOAD16_WORD_SWAP( "c76.p6",   0x280000, 0x80000, CRC(2018c120) SHA1(de1184ab771c6f075cdefa744a28b09f78d91643) )
	ROM_LOAD16_WORD_SWAP( "c76.p7",   0x300000, 0x80000, CRC(fac3c217) SHA1(0e9dd54e401e6d7c4fe81107ffd27e42ca810fcb) )
	ROM_LOAD16_WORD_SWAP( "c76.p8",   0x380000, 0x80000, CRC(eb490213) SHA1(bf0416df66a33c7a4678ab4a047de334dfd3b31e) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c76.c1",   0x0000000, 0x400000, CRC(5c852f52) SHA1(528ce7fc9a0451e2e2d221dbf5e4a5796584e053) )
	ROM_LOAD64_WORD( "c76.c2",   0x0000002, 0x400000, CRC(a20f58af) SHA1(e873ad3e0fc8a06a5029113faf991f5c1b765316) )
	ROM_LOAD64_WORD( "c76.c3",   0x0000004, 0x400000, CRC(39db59ad) SHA1(da94f1529da82a6bf2129f51548412e1ab2b001a) )
	ROM_LOAD64_WORD( "c76.c4",   0x0000006, 0x400000, CRC(00c763a7) SHA1(0ff528e12e255ebf699101ac71f05b1f6bef7165) )
	ROM_LOAD64_WORD( "c76.c5",   0x1000000, 0x400000, CRC(cd09bd63) SHA1(e582b20a948ae54f52590496051688dbfae2bc9c) )
	ROM_LOAD64_WORD( "c76.c6",   0x1000002, 0x400000, CRC(e0182c15) SHA1(a924d53ab39f4d85173bdb92a197dde2db0dc3f7) )
	ROM_LOAD64_WORD( "c76.c7",   0x1000004, 0x400000, CRC(778dc4f6) SHA1(8d0cd1c387b4b6ac7f92bb2e5a25983856328cdc) )
	ROM_LOAD64_WORD( "c76.c8",   0x1000006, 0x400000, CRC(605d9d1d) SHA1(99bc27557741527ca678d7b6307164bc04ebedc6) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c76.m1",   0x00000, 0x08000, CRC(35190139) SHA1(07f8e53ea398461de5dcda9814dde7c09faf9f65) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c76.m2",   0x28000, 0x20000, CRC(c32dba09) SHA1(1fe337ff334fab79847f9677ba0e168e93daa1c8) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c76.q1",   0x000000, 0x400000, CRC(d67e47b7) SHA1(15a3f6779eccb10551ed94edf7e6e406a79b3de7) )
	ROM_LOAD16_WORD_SWAP( "c76.q2",   0x400000, 0x400000, CRC(6d020a14) SHA1(e98f862fac1e357c90949768bb2646263d9981a0) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( csclub1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c73ed.p1", 0x000000, 0x80000, CRC(5aedc6e6) SHA1(1330959ac96b0c267e81237974ec44ed7189495a) )
	ROM_LOAD16_WORD_SWAP( "c73ed.p2", 0x080000, 0x80000, CRC(a3d9aa25) SHA1(d7a98cf4506c30b321d8c5a0687fa6938740f951) )
	ROM_LOAD16_WORD_SWAP( "c73ed.p3", 0x100000, 0x80000, CRC(0915c9d1) SHA1(071c6bcfb0ed1047f06c2fbe43a0340aef73dc1d) )
	ROM_LOAD16_WORD_SWAP( "c73ed.p4", 0x180000, 0x80000, CRC(09c77d99) SHA1(16b21f975ad07dd3156291e8c274a0c52d783938) )
	ROM_LOAD16_WORD_SWAP( "c73d.p5",  0x200000, 0x80000, CRC(77478e25) SHA1(d2648a8d57a156c5a0416007dedb4c65af8403d5) )

	ROM_REGION( 0x1000000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c73.c01",  0x800000, 0x080000, CRC(335f07c3) SHA1(44e0385120e2c81fd1072e19b7e3ff05d42be226) )
	ROM_LOAD64_WORD( "c73.c02",  0x800002, 0x080000, CRC(3711b8ca) SHA1(ff7ba4f73d227212377327f4fcbe8f555ac4b9c9) )
	ROM_LOAD64_WORD( "c73.c03",  0x800004, 0x080000, CRC(0750d12a) SHA1(4c36cba88c58bdbfed923f56a6d489f42a4d0f6e) )
	ROM_LOAD64_WORD( "c73.c04",  0x800006, 0x080000, CRC(a756c7f7) SHA1(d0f44f8ded12291d5c79282eac45d088cb365b09) )
	ROM_LOAD64_WORD( "c73.c05",  0xa00000, 0x080000, CRC(ab215357) SHA1(c2600b5ba62a570f8a32de9c06f93ea8ae6cd854) )
	ROM_LOAD64_WORD( "c73.c06",  0xa00002, 0x080000, CRC(828a06d8) SHA1(ce1d147f1d747fcd90d93f7dcca0cdd24c85a971) )
	ROM_LOAD64_WORD( "c73.c07",  0xa00004, 0x080000, CRC(90a92f39) SHA1(75bef2440147bac417b48d61ff64b71ed5b9eb67) )
	ROM_LOAD64_WORD( "c73.c08",  0xa00006, 0x080000, CRC(fb7ccc73) SHA1(763e6fe1baf73341747c74d89df9443561fca4fa) )
	ROM_LOAD64_WORD( "c73.c09",  0xc00000, 0x080000, CRC(a2367381) SHA1(49d562be42c10cdf4e55e3b76c388f0b0121a967) )
	ROM_LOAD64_WORD( "c73.c10",  0xc00002, 0x080000, CRC(86ee4569) SHA1(725d82a1669f6f78fb4ef8756ceac8b444dcbd43) )
	ROM_LOAD64_WORD( "c73.c11",  0xc00004, 0x080000, CRC(d08ab012) SHA1(7f2e71b6bd85c6a3efbd417977261c21f3d6cb7c) )
	ROM_LOAD64_WORD( "c73.c12",  0xc00006, 0x080000, CRC(4d014297) SHA1(df172a3723793b9955ff1f65e76fe5c20fc37b89) )
	ROM_LOAD64_WORD( "c73.c13",  0xe00000, 0x080000, CRC(728aac1f) SHA1(2d04e8803f41af2372fc342fcd8a076c14338198) )
	ROM_LOAD64_WORD( "c73.c14",  0xe00002, 0x080000, CRC(c24f577f) SHA1(d3b97091e1f0171e087feb9d497c94a48872cd21) )
	ROM_LOAD64_WORD( "c73.c15",  0xe00004, 0x080000, CRC(41652583) SHA1(7132647dc9a29ce98866f489140703c5ed3d6051) )
	ROM_LOAD64_WORD( "c73.c16",  0xe00006, 0x080000, CRC(6754b1ef) SHA1(ab5d62056b19999ccecd0eeec7b7c5869ca8fea8) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c73.m1",   0x00000, 0x08000, CRC(ee162111) SHA1(ce8d4bd32bb10ee8b0274ba6fcef05a583b39d48) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c73e.q1",   0x000000, 0x080000, CRC(5a52afd5) SHA1(ab873ec556933b75be82ba249357cceb01a7a0bd) )
	ROM_LOAD16_WORD_SWAP( "c73e.q2",   0x080000, 0x080000, CRC(1408a811) SHA1(1d31e3cd505245b5ff4d3c55d5fa7017c0f1e168) )
	ROM_LOAD16_WORD_SWAP( "c73e.q3",   0x100000, 0x080000, CRC(4fb9f57c) SHA1(093e8e3a03b62783a84fe4ae239e9eb46cbfd71e) )
	ROM_LOAD16_WORD_SWAP( "c73e.q4",   0x180000, 0x080000, CRC(9a8f40ec) SHA1(c8db1ecfd6b08e9c83ae53a1d25c1387ab95535c) )
	ROM_LOAD16_WORD_SWAP( "c73e.q5",   0x200000, 0x080000, CRC(91529a91) SHA1(183569100ae98e17688e0e25932850e73a41eb88) )
	ROM_LOAD16_WORD_SWAP( "c73e.q6",   0x280000, 0x080000, CRC(9a345334) SHA1(330291400a73215c9797457507a86c90ba415247) )
	ROM_LOAD16_WORD_SWAP( "c73e.q7",   0x300000, 0x080000, CRC(aedc27f2) SHA1(55137f0f22c4823558e6a8ba76011695579a4f1f) )
	ROM_LOAD16_WORD_SWAP( "c73e.q8",   0x380000, 0x080000, CRC(2300b7b3) SHA1(f5ecbb45c24f7de1c1aa435870695551d4e343ca) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( cybotsud )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c58ud.p1", 0x000000, 0x80000, CRC(ee7560fb) SHA1(632e057a7b3a61127beee4ed833a39dcec4ccbd5) )
	ROM_LOAD16_WORD_SWAP( "c58ud.p2", 0x080000, 0x80000, CRC(7e7425a0) SHA1(6368cf5b04271e68b27abecd67d107bbf58976b4) )
	ROM_LOAD16_WORD_SWAP( "c58.p3",  0x100000, 0x80000, CRC(ec40408e) SHA1(dd611c1708e7ef86e4f7cac4b7b0dff7baaee5ed) )
	ROM_LOAD16_WORD_SWAP( "c58.p4",  0x180000, 0x80000, CRC(1ad0bed2) SHA1(2ea005f3e73b05f8f0ec006cd9e95f7731a73897) )
	ROM_LOAD16_WORD_SWAP( "c58.p5",  0x200000, 0x80000, CRC(6245a39a) SHA1(4f607e733e2dea80211497522be6d0f09571928d) )
	ROM_LOAD16_WORD_SWAP( "c58.p6",  0x280000, 0x80000, CRC(4b48e223) SHA1(9714579a7a78b9716e44bca6c18bf1a93aa4e482) )
	ROM_LOAD16_WORD_SWAP( "c58.p7",  0x300000, 0x80000, CRC(e15238f6) SHA1(16abd92ebed921a6a7e8eac4b098dc61f7e5485c) )
	ROM_LOAD16_WORD_SWAP( "c58.p8",  0x380000, 0x80000, CRC(75f4003b) SHA1(8a65026ae35247cda016ce85a34034c62b3aa1a6) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c58.c1",   0x0000000, 0x400000, CRC(f0dce192) SHA1(b743938dc8e772dc3f63ed88a4a54c34fffdba21) )
	ROM_LOAD64_WORD( "c58.c2",   0x0000002, 0x400000, CRC(187aa39c) SHA1(80e3cf5c69f13343de667e1476bb716d45d3ff63) )
	ROM_LOAD64_WORD( "c58.c3",   0x0000004, 0x400000, CRC(8a0e4b12) SHA1(40132f3cc79b0a74460ebd4e0d4ddbe240efc06f) )
	ROM_LOAD64_WORD( "c58.c4",   0x0000006, 0x400000, CRC(34b62612) SHA1(154bbceb7d303a208abb1b2f3d507d5afacc71ed) )
	ROM_LOAD64_WORD( "c58.c5",   0x1000000, 0x400000, CRC(c1537957) SHA1(bfb1cc6786277b94ce28bfd464e2bbb6f6d3486e) )
	ROM_LOAD64_WORD( "c58.c6",   0x1000002, 0x400000, CRC(15349e86) SHA1(b0cde577d29a9f4e718b673c8645529ef0ababc9) )
	ROM_LOAD64_WORD( "c58.c7",   0x1000004, 0x400000, CRC(d83e977d) SHA1(e03f4a120c95a2f476ffc8492bca85e0c5cea068) )
	ROM_LOAD64_WORD( "c58.c8",   0x1000006, 0x400000, CRC(77cdad5c) SHA1(94d0cc5f05de4bc2d43977d91f887005dc10310c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c58.m1",   0x00000, 0x08000, CRC(9c0fb079) SHA1(06d260875a76da08d56ea2b2ae277e8c2dbae6e3) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c58.m2",   0x28000, 0x20000, CRC(51cb0c4e) SHA1(c322957558d8d3e9dad090aebbe485978cbce8f5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.q1",   0x000000, 0x200000, CRC(362ccab2) SHA1(28e537067d4846f22657ee37166d18b8f05f4da1) )
	ROM_LOAD16_WORD_SWAP( "c58.q2",   0x200000, 0x200000, CRC(7066e9cc) SHA1(eb6a9d4998b3311344d73bae88d661d81609c492) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( cybotsjd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c58jd.p1", 0x000000, 0x80000, CRC(9eb34071) SHA1(b9ef1fa79f79bbb821862764c65a65357a79aa91) )
	ROM_LOAD16_WORD_SWAP( "c58jd.p2", 0x080000, 0x80000, CRC(cf223cd7) SHA1(834833ae68c3f14f1e06c92273fadfe6918cf3c5) )
	ROM_LOAD16_WORD_SWAP( "c58.p3",  0x100000, 0x80000, CRC(ec40408e) SHA1(dd611c1708e7ef86e4f7cac4b7b0dff7baaee5ed) )
	ROM_LOAD16_WORD_SWAP( "c58.p4",  0x180000, 0x80000, CRC(1ad0bed2) SHA1(2ea005f3e73b05f8f0ec006cd9e95f7731a73897) )
	ROM_LOAD16_WORD_SWAP( "c58.p5",  0x200000, 0x80000, CRC(6245a39a) SHA1(4f607e733e2dea80211497522be6d0f09571928d) )
	ROM_LOAD16_WORD_SWAP( "c58.p6",  0x280000, 0x80000, CRC(4b48e223) SHA1(9714579a7a78b9716e44bca6c18bf1a93aa4e482) )
	ROM_LOAD16_WORD_SWAP( "c58.p7",  0x300000, 0x80000, CRC(e15238f6) SHA1(16abd92ebed921a6a7e8eac4b098dc61f7e5485c) )
	ROM_LOAD16_WORD_SWAP( "c58.p8",  0x380000, 0x80000, CRC(75f4003b) SHA1(8a65026ae35247cda016ce85a34034c62b3aa1a6) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c58.c1",   0x0000000, 0x400000, CRC(f0dce192) SHA1(b743938dc8e772dc3f63ed88a4a54c34fffdba21) )
	ROM_LOAD64_WORD( "c58.c2",   0x0000002, 0x400000, CRC(187aa39c) SHA1(80e3cf5c69f13343de667e1476bb716d45d3ff63) )
	ROM_LOAD64_WORD( "c58.c3",   0x0000004, 0x400000, CRC(8a0e4b12) SHA1(40132f3cc79b0a74460ebd4e0d4ddbe240efc06f) )
	ROM_LOAD64_WORD( "c58.c4",   0x0000006, 0x400000, CRC(34b62612) SHA1(154bbceb7d303a208abb1b2f3d507d5afacc71ed) )
	ROM_LOAD64_WORD( "c58.c5",   0x1000000, 0x400000, CRC(c1537957) SHA1(bfb1cc6786277b94ce28bfd464e2bbb6f6d3486e) )
	ROM_LOAD64_WORD( "c58.c6",   0x1000002, 0x400000, CRC(15349e86) SHA1(b0cde577d29a9f4e718b673c8645529ef0ababc9) )
	ROM_LOAD64_WORD( "c58.c7",   0x1000004, 0x400000, CRC(d83e977d) SHA1(e03f4a120c95a2f476ffc8492bca85e0c5cea068) )
	ROM_LOAD64_WORD( "c58.c8",   0x1000006, 0x400000, CRC(77cdad5c) SHA1(94d0cc5f05de4bc2d43977d91f887005dc10310c) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c58.m1",   0x00000, 0x08000, CRC(9c0fb079) SHA1(06d260875a76da08d56ea2b2ae277e8c2dbae6e3) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c58.m2",   0x28000, 0x20000, CRC(51cb0c4e) SHA1(c322957558d8d3e9dad090aebbe485978cbce8f5) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c58.q1",   0x000000, 0x200000, CRC(362ccab2) SHA1(28e537067d4846f22657ee37166d18b8f05f4da1) )
	ROM_LOAD16_WORD_SWAP( "c58.q2",   0x200000, 0x200000, CRC(7066e9cc) SHA1(eb6a9d4998b3311344d73bae88d661d81609c492) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( mshvsfu1d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c72u1d.p1", 0x000000, 0x80000, CRC(1c88bee3) SHA1(53ab6df85a900b221cabd62948e68d91cf6146e4) )
	ROM_LOAD16_WORD_SWAP( "c72u1d.p2", 0x080000, 0x80000, CRC(1e8b2535) SHA1(51ebede487f955e3f838b7906b619830e67fea52) )
	ROM_LOAD16_WORD_SWAP( "c72u1d.p3", 0x100000, 0x80000, CRC(373856fb) SHA1(777bf7c74110c029c44a363ce79e50c1863950c2) )
	ROM_LOAD16_WORD_SWAP( "c72.p4",    0x180000, 0x80000, CRC(959f3030) SHA1(fbbaa915324815246738f3426232e623f039ce26) )
	ROM_LOAD16_WORD_SWAP( "c72.p5",    0x200000, 0x80000, CRC(7f915bdb) SHA1(683da09c5ba55e31b59aa95a8e13c45dc574ab3c) )
	ROM_LOAD16_WORD_SWAP( "c72.p6",    0x280000, 0x80000, CRC(c2813884) SHA1(49e5d4bc48f90c8146cb6aafb9240aff0119f1a7) )
	ROM_LOAD16_WORD_SWAP( "c72.p7",    0x300000, 0x80000, CRC(3ba08818) SHA1(9ab132a3cac55fcccebe6c99b6fb0ba1305f8f6e) )
	ROM_LOAD16_WORD_SWAP( "c72.p8",    0x380000, 0x80000, CRC(cf0dba98) SHA1(f4c1f8a6e7a79ecc6241d5268b3039f8a09ea516) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c72.c1",   0x0000000, 0x400000, CRC(29b05fd9) SHA1(e8fdb1ee5515a560eb4256ae4fd99bb1192e1a87) )
	ROM_LOAD64_WORD( "c72.c2",   0x0000002, 0x400000, CRC(faddccf1) SHA1(4ed03ea91883a0413325f57edcc1614120b5922c) )
	ROM_LOAD64_WORD( "c72.c3",   0x0000004, 0x400000, CRC(97aaf4c7) SHA1(6a054921cc14fe080cb3f62c391f8ae3cc7e8ba9) )
	ROM_LOAD64_WORD( "c72.c4",   0x0000006, 0x400000, CRC(cb70e915) SHA1(da4d2480d348ac6dfd01256a88f4f3db8357ae46) )
	ROM_LOAD64_WORD( "c72.c5",   0x1000000, 0x400000, CRC(b3b1972d) SHA1(0f2c3fb7de014181ee481ec35d0578b2c116c2dc) )
	ROM_LOAD64_WORD( "c72.c6",   0x1000002, 0x400000, CRC(08aadb5d) SHA1(3a2c222eca3e7df80ce69951b3db6442312751a4) )
	ROM_LOAD64_WORD( "c72.c7",   0x1000004, 0x400000, CRC(c1228b35) SHA1(7afdfb552888c79d0fbb30242b3d917b87fad57a) )
	ROM_LOAD64_WORD( "c72.c8",   0x1000006, 0x400000, CRC(366cc6c2) SHA1(6f2a789087c8e404c5227b927fa8328c03593243) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c72.m1",   0x00000, 0x08000, CRC(68252324) SHA1(138ef320ef27956b2ab5591d49a1315b7b0a194c) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c72.m2",   0x28000, 0x20000, CRC(b34e773d) SHA1(3bcf44bf06c35814cff29d244142db7abe05bd39) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c72.q1",   0x000000, 0x400000, CRC(86219770) SHA1(4e5b68d382a5aa37f8b0b6434c53a2b95f5f9a4d) )
	ROM_LOAD16_WORD_SWAP( "c72.q2",   0x400000, 0x400000, CRC(f2fd7f68) SHA1(28a30d55d3eaf963006c7cbe7c288099cd3ba536) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( spf2td )
	ROM_REGION(CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c65d.p1", 0x000000, 0x80000, CRC(7836b903) SHA1(9a96a9080716a0cc4d13f51490fd850afe4479a7) )
	ROM_LOAD16_WORD_SWAP( "c65.p2",  0x080000, 0x80000, CRC(b80649e2) SHA1(5bfccd656aea7ff82e9a20bb5856f4ab99b5a007) )

	ROM_REGION( 0xC00000, "gfx", ROMREGION_ERASE00 )
	ROM_LOAD64_WORD( "c65.c1", 0x800000, 0x100000, CRC(2d4881cb) SHA1(fd3baa183c25bed153b19c251980e2fb761600e2) )
	ROM_LOAD64_WORD( "c65.c2", 0x800002, 0x100000, CRC(4b0fd1be) SHA1(377aafdcdb7a866b1c8487670e3598d8197976e4) )
	ROM_LOAD64_WORD( "c65.c3", 0x800004, 0x100000, CRC(e43aac33) SHA1(d041e0688c3807d3363861a7f216de43b34d846c) )
	ROM_LOAD64_WORD( "c65.c4", 0x800006, 0x100000, CRC(7f536ff1) SHA1(905b9d62ef7bef47297c7f4a4dd697aed6df38a5) )

	ROM_REGION(QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c65.m1",   0x00000, 0x08000, CRC(600fb2a3) SHA1(1fab1c2a23bf6ad8309d29ddbbc29435a8aeea13) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c65.m2",   0x28000, 0x20000, CRC(496076e0) SHA1(1ee4e135140afd0e8e03231e570cd77d140f6367) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c65.q1",   0x000000, 0x200000, CRC(78442743) SHA1(b61190bb586871de6d54af580e3e1d9cc0de0acb) )
	ROM_LOAD16_WORD_SWAP( "c65.q2",   0x200000, 0x200000, CRC(399d2c7b) SHA1(e849dea97b8d16540415c0d9bbc4f9f4eb755ec4) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz3jr2d )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c78jr2d.p1", 0x000000, 0x80000, CRC(b0436151) SHA1(5170390d5d38bb003602d5714d557005bd7218be) )
	ROM_LOAD16_WORD_SWAP( "c78jr2d.p2", 0x080000, 0x80000, CRC(642d8170) SHA1(1a73f6090a384e7399e49e3a13fe02aa284d4775) )
	ROM_LOAD16_WORD_SWAP( "c78b.p3",  0x100000, 0x80000, CRC(9b21518a) SHA1(5a928307cb90a98a62e7598cb101fb66d62b85f9) )
	ROM_LOAD16_WORD_SWAP( "c78b.p4",  0x180000, 0x80000, CRC(e7a6c3a7) SHA1(63441eb19efcbf9149f4b723d3e9191fa972de2a) )
	ROM_LOAD16_WORD_SWAP( "c78b.p5",  0x200000, 0x80000, CRC(ec4c0cfd) SHA1(1a5148e77bf633c728a8179dacb59c776f981bc4) )
	ROM_LOAD16_WORD_SWAP( "c78b.p6",  0x280000, 0x80000, CRC(5c7e7240) SHA1(33bdcdd1889f8fa77916373ed33b0854410d0263) )
	ROM_LOAD16_WORD_SWAP( "c78b.p7",  0x300000, 0x80000, CRC(c5589553) SHA1(cda1fdc2ab2f390a2358defd9923a2796093926d) )
	ROM_LOAD16_WORD_SWAP( "c78b.p8",  0x380000, 0x80000, CRC(a9717252) SHA1(7ee94ace2a49e4e5d30474e49c0da04a488010fe) )

	ROM_REGION( 0x2000000, "gfx", 0 )
	ROM_LOAD64_WORD( "c78.c1",   0x0000000, 0x400000, CRC(0f7a60d9) SHA1(c69e0ee22537312909dacc86d2e4be319d54e426) )
	ROM_LOAD64_WORD( "c78.c2",   0x0000002, 0x400000, CRC(8e933741) SHA1(f4ac4bfe830dc7df9fe4f680e4e0c053e7cbd8fe) )
	ROM_LOAD64_WORD( "c78.c3",   0x0000004, 0x400000, CRC(d6e98147) SHA1(37f331fbb1284db446faecade6f484f58c0e1b2a) )
	ROM_LOAD64_WORD( "c78.c4",   0x0000006, 0x400000, CRC(f31a728a) SHA1(f14136564648f006c1b74afda78349f260524b5f) )
	ROM_LOAD64_WORD( "c78.c5",   0x1000000, 0x400000, CRC(5ff98297) SHA1(9e0ce43380b776c7a03872bafd4856f6fa60bda7) )
	ROM_LOAD64_WORD( "c78.c6",   0x1000002, 0x400000, CRC(52b5bdee) SHA1(7918204dc457f7a146d8fb8cf7242dfed3109fd8) )
	ROM_LOAD64_WORD( "c78.c7",   0x1000004, 0x400000, CRC(40631ed5) SHA1(c18c56822b90a71ca5fbdf3440eb2671011f3d8f) )
	ROM_LOAD64_WORD( "c78.c8",   0x1000006, 0x400000, CRC(763409b4) SHA1(af60a5116c1ca9050366a35ea29128921867f3cc) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c78.m1",   0x00000, 0x08000, CRC(de810084) SHA1(fd0b969b732921ed8b40c16fbfa30ee09c7a7cbd) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c78.m2",   0x28000, 0x20000, CRC(72445dc4) SHA1(14fca7596ac45ba655016eef5b6120f9f9671c23) )

	ROM_REGION( 0x800000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c78.q1",   0x000000, 0x400000, CRC(1c89eed1) SHA1(649a0b0a3eb72e2e69e9fb1ac51a58b70daa39f3) )
	ROM_LOAD16_WORD_SWAP( "c78.q2",   0x400000, 0x400000, CRC(f392b13a) SHA1(fa04ce0370144a49bd1d5acd873eef87b0dc9d15) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( sfz2jd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c64jd.p1", 0x000000, 0x80000, CRC(b7325df8) SHA1(cd6e242a5681017463102dcb9133d8dcb262f923) )
	ROM_LOAD16_WORD_SWAP( "c64jd.p2", 0x080000, 0x80000, CRC(a1022a3e) SHA1(4806637ddf22169f12d31f959982cb2f11693066) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p3",  0x100000, 0x80000, CRC(98e8e992) SHA1(41745b63e6b3888081d189b8315ed3b7526b3d20) )
	ROM_LOAD16_WORD_SWAP( "c64.p4",   0x180000, 0x80000, CRC(5b1d49c0) SHA1(f0a0c894c9cbe2b18e7f59058665949ee0025732) )
	ROM_LOAD16_WORD_SWAP( "c64jr1.p5", 0x200000, 0x80000, CRC(d910b2a2) SHA1(aa201660caa9cef993c147a1077c9e7767b34a78) )
	ROM_LOAD16_WORD_SWAP( "c64.p6",   0x280000, 0x80000, CRC(0fe8585d) SHA1(0cd5369a5aa90c98d8dc1ff3342cd4d990631cff) )

	ROM_REGION( 0x1400000, "gfx", 0 )
	ROM_LOAD64_WORD( "c64.c1",   0x0000000, 0x400000, CRC(4d1f1f22) SHA1(659fb4305bcf0cbbbbec97ede6e68a8323b13308) )
	ROM_LOAD64_WORD( "c64.c2",   0x0000002, 0x400000, CRC(19cea680) SHA1(4cb88963a0fbcef191c8419b6379387c01b4c81e) )
	ROM_LOAD64_WORD( "c64.c3",   0x0000004, 0x400000, CRC(e01b4588) SHA1(c2936608fd75ff6cd5fa94c6d6d6f0c77c44a450) )
	ROM_LOAD64_WORD( "c64.c4",   0x0000006, 0x400000, CRC(0feeda64) SHA1(f5b350601437bd94b70d97feb23d791df19da6b3) )
	ROM_LOAD64_WORD( "c64.c5",   0x1000000, 0x100000, CRC(0560c6aa) SHA1(f2bed3a8efef18052b51a7f0f6a888a18db813a1) )
	ROM_LOAD64_WORD( "c64.c6",   0x1000002, 0x100000, CRC(ae940f87) SHA1(39ee26333abbe302ba76dced0196a2e6b3b1d02a) )
	ROM_LOAD64_WORD( "c64.c7",   0x1000004, 0x100000, CRC(4bc3c8bc) SHA1(6256963c515bf56f39b6e559afefd653ead56c54) )
	ROM_LOAD64_WORD( "c64.c8",   0x1000006, 0x100000, CRC(39e674c0) SHA1(8e771a2d8c2accad0463bccd21d7b23af0c895a1) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c64.m1",   0x00000, 0x08000, CRC(1bc323cf) SHA1(83fbd6e9b327700dc9f1c59700b7385bc3705749) )
	ROM_CONTINUE(         0x10000, 0x18000 )
	ROM_LOAD( "c64.m2",   0x28000, 0x20000, CRC(ba6a5013) SHA1(7814f3e56b69529b9860dd61c3b1e8d700244b03) )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c64.q1",   0x000000, 0x200000, CRC(aa47a601) SHA1(a4d1ee89c84a3b9db06469bb66e85293b5aa9ac9) )
	ROM_LOAD16_WORD_SWAP( "c64.q2",   0x200000, 0x200000, CRC(2237bc53) SHA1(96d5693047e4cf1ed10a8ee1905cea267a278e92) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END

ROM_START( pzloop2jd )
	ROM_REGION( CODE_SIZE, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "c86jd.p1", 0x000000, 0x80000, CRC(1f5d41c9) SHA1(a205f8c73136d22c127ee547df724e300647c45a) )
	ROM_LOAD16_WORD_SWAP( "c86jd.p2", 0x080000, 0x80000, CRC(60721b73) SHA1(48918d94b8a21c1bb2bd2fc6166cde6bb427d862) )
	ROM_LOAD16_WORD_SWAP( "c86jd.p3", 0x100000, 0x80000, CRC(0c39799f) SHA1(3c16a814a5bbb2b00bd82a01f17e0ca797192c98) )
	ROM_LOAD16_WORD_SWAP( "c86jd.p4", 0x180000, 0x80000, CRC(f3974d76) SHA1(01f8caf5a85d27b24a18a313e459f508ba98a123) )

	ROM_REGION( 0x1000000, "gfx", 0 )
	ROM_LOAD64_BYTE( "c86.c1",   0x0000000, 0x200000, CRC(137b13a7) SHA1(a1ca1bc8699ddfc54d5de1b39a9db9a5ac8b12e5) )
	ROM_LOAD64_BYTE( "c86.c2",   0x0000001, 0x200000, CRC(a2db1507) SHA1(61c84c8d698a846d54a571b5f7b4824e22136db7) )
	ROM_LOAD64_BYTE( "c86.c3",   0x0000002, 0x200000, CRC(7e80ff8e) SHA1(afcebfa995ace8b8973e75f1589980c5c4535bca) )
	ROM_LOAD64_BYTE( "c86.c4",   0x0000003, 0x200000, CRC(cd93e6ed) SHA1(e4afce48fe481d8291ed2475d5de446afad65351) )
	ROM_LOAD64_BYTE( "c86.c5",   0x0000004, 0x200000, CRC(0f52bbca) SHA1(e76c29d445062f5e16d06bdc4ab44640ba35aaac) )
	ROM_LOAD64_BYTE( "c86.c6",   0x0000005, 0x200000, CRC(a62712c3) SHA1(2abfe0209e188010a0ae969f0d9eb7d28820b3f2) )
	ROM_LOAD64_BYTE( "c86.c7",   0x0000006, 0x200000, CRC(b60c9f8e) SHA1(40c7985e04463fb2bd59b3bb6aa5897328d37ff3) )
	ROM_LOAD64_BYTE( "c86.c8",   0x0000007, 0x200000, CRC(83fef284) SHA1(ef4429f54c456d6485a7d642d49dffafef4435fe) )

	ROM_REGION( QSOUND_SIZE, "audiocpu", 0 )
	ROM_LOAD( "c86.m1",   0x00000, 0x08000, CRC(35697569) SHA1(13718923cffb9ec53cef9e22d8875370b5f3dd74) )
	ROM_CONTINUE(         0x10000, 0x18000 )

	ROM_REGION( 0x400000, "qsound", 0 )
	ROM_LOAD16_WORD_SWAP( "c86.q1",   0x000000, 0x200000, CRC(85d8fbe8) SHA1(c19d5e9084d07e610379b6e1b6be7bdf0b9b7f7f) )
	ROM_LOAD16_WORD_SWAP( "c86.q2",   0x200000, 0x200000, CRC(1ed62584) SHA1(28411f610f48cca6424a2d53e2a4ac691e826317) )

	ROM_REGION( 0x20, "key", 0 )
	ROM_LOAD( "phoenix.key",  0x00, 0x14, CRC(2cf772b0) SHA1(eff33c65a4f3862c231f9e4d6fefa7b34398dbf2) )
ROM_END




// Phoenix bootleg sets

GAME( 1993, ddtodd,     ddtod,    dead_cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "bootleg", "Dungeons & Dragons: Tower of Doom (Euro 940412 Phoenix Edition) (bootleg)",                  MACHINE_SUPPORTS_SAVE )
GAME( 1993, ecofghtrd,  ecofghtr, dead_cps2,     ecofghtr,  cps2_state, init_ecofghtr, ROT0,   "bootleg", "Eco Fighters (World 931203 Phoenix Edition) (bootleg)",                                      MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2ud,     ssf2,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Street Fighter II: The New Challengers (USA 930911 Phoenix Edition) (bootleg)",        MACHINE_SUPPORTS_SAVE )
GAME( 1993, ssf2tbd,    ssf2,     dead_cps2,     cps2_2p6b, cps2_state, init_ssf2tb,   ROT0,   "bootleg", "Super Street Fighter II: The Tournament Battle (World 931119 Phoenix Edition) (bootleg)",    MACHINE_SUPPORTS_SAVE )
GAME( 1994, armwar1d,   armwar,   dead_cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Armored Warriors (Euro 941011 Phoenix Edition) (bootleg)",                                   MACHINE_SUPPORTS_SAVE )
GAME( 1994, avspd,      avsp,     dead_cps2,     cps2_3p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Alien vs. Predator (Euro 940520 Phoenix Edition) (bootleg)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1994, dstlku1d,   dstlk,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Darkstalkers: The Night Warriors (USA 940705 Phoenix Edition) (bootleg)",                    MACHINE_SUPPORTS_SAVE )
GAME( 1994, ringdstd,   ringdest, dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Ring of Destruction: Slammasters II (Euro 940902 Phoenix Edition) (bootleg)",                MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2tad,    ssf2t,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Street Fighter II Turbo (Asia 940223 Phoenix Edition) (bootleg)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1994, ssf2xjr1d,  ssf2t,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Street Fighter II X: Grand Master Challenge (Japan 940223 Phoenix Edition) (bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, xmcotar1d,  xmcota,   dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "X-Men: Children of the Atom (Euro 950105 Phoenix Edition) (bootleg)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1995, mshud,      msh,      dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Marvel Super Heroes (US 951024 Phoenix Edition) (bootleg)",                                  MACHINE_SUPPORTS_SAVE )
GAME( 1995, cybotsud,   cybots,   dead_cps2,     cybots,    cps2_state, init_cps2,     ROT0,   "bootleg", "Cyberbots: Fullmetal Madness (USA 950424 Phoenix Edition) (bootleg)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1995, cybotsjd,   cybots,   dead_cps2,     cybots,    cps2_state, init_cps2,     ROT0,   "bootleg", "Cyberbots: Fullmetal Madness (Japan 950424) (decrypted bootleg)",                            MACHINE_SUPPORTS_SAVE )
GAME( 1995, nwarrud,    nwarr,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Night Warriors: Darkstalkers' Revenge (USA 950406 Phoenix Edition) (bootleg)",               MACHINE_SUPPORTS_SAVE )
GAME( 1995, sfad,       sfa,      dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Alpha: Warriors' Dreams (Euro 950727 Phoenix Edition) (bootleg)",             MACHINE_SUPPORTS_SAVE )
GAME( 1996, 19xxd,      19xx,     dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT270, "bootleg", "19XX: The War Against Destiny (USA 951207 Phoenix Edition) (bootleg)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, ddsomud,    ddsom,    dead_cps2,     cps2_4p4b, cps2_state, init_cps2,     ROT0,   "bootleg", "Dungeons & Dragons: Shadow over Mystara (USA 960619 Phoenix Edition) (bootleg)",             MACHINE_SUPPORTS_SAVE )
GAME( 1996, gigaman2,   megaman2, gigaman2,      cps2_2p3b, cps2_state, init_gigaman2, ROT0,   "bootleg", "Giga Man 2: The Power Fighters (bootleg of Mega Man 2: The Power Fighters)",                 MACHINE_IMPERFECT_GRAPHICS | MACHINE_NO_SOUND | MACHINE_SUPPORTS_SAVE ) // Different layer offsets and sound system
GAME( 1996, megamn2d,   megaman2, dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Mega Man 2: The Power Fighters (USA 960708 Phoenix Edition) (bootleg)",                      MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2ad,     sfa2,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Zero 2 (Asia 960227 Phoenix Edition) (bootleg)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2jd,     sfa2,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Zero 2 (Japan 960227 Phoenix Edition) (bootleg)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2td,     spf2t,    dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Puzzle Fighter II Turbo (USA 960620 Phoenix Edition) (bootleg)",                       MACHINE_SUPPORTS_SAVE )
GAME( 1996, spf2xjd,    spf2t,    dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Puzzle Fighter II X (Japan 960531 Phoenix Edition) (bootleg)",                         MACHINE_SUPPORTS_SAVE )
GAME( 1996, sfz2ald,    sfz2al,   dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Zero 2 Alpha (Asia 960826 Phoenix Edition) (bootleg)",                        MACHINE_SUPPORTS_SAVE )
GAME( 1996, xmvsfu1d,   xmvsf,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "X-Men Vs. Street Fighter (USA 961004 Phoenix Edition) (bootleg)",                            MACHINE_SUPPORTS_SAVE )
GAME( 1997, batcird,    batcir,   dead_cps2,     cps2_4p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "Battle Circuit (Euro 970319 Phoenix Edition) (bootleg)",                                     MACHINE_SUPPORTS_SAVE )
GAME( 1997, csclub1d,   csclub,   dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Capcom Sports Club (Euro 970722 Phoenix Edition) (bootleg)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 1997, mshvsfu1d,  mshvsf,   dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Marvel Super Heroes Vs. Street Fighter (USA 970625 Phoenix Edition) (bootleg)",              MACHINE_SUPPORTS_SAVE )
GAME( 1997, sgemfd,     sgemf,    dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Super Gem Fighter Mini Mix (USA 970904 Phoenix Edition) (bootleg)",                          MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsavd,      vsav,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Vampire Savior: The Lord of Vampire (Euro 970519 Phoenix Edition) (bootleg)",                MACHINE_SUPPORTS_SAVE )
GAME( 1997, vhunt2d,    vhunt2,   dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Vampire Hunter 2: Darkstalkers Revenge (Japan 970913 Phoenix Edition) (bootleg)",            MACHINE_SUPPORTS_SAVE )
GAME( 1997, vsav2d,     vsav2,    dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Vampire Savior 2: The Lord of Vampire (Japan 970913 Phoenix Edition) (bootleg)",             MACHINE_SUPPORTS_SAVE )
GAME( 1998, mvscud,     mvsc,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Marvel Vs. Capcom: Clash of Super Heroes (USA 980123 Phoenix Edition) (bootleg)",            MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfa3ud,     sfa3,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Alpha 3 (USA 980904 Phoenix Edition) (bootleg)",                              MACHINE_SUPPORTS_SAVE )
GAME( 1998, sfz3jr2d,   sfa3,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Street Fighter Zero 3 (Japan 980629 Phoenix Edition) (bootleg)",                             MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawingd,  gigawing, dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "Giga Wing (USA 990222 Phoenix Edition) (bootleg)",                                           MACHINE_SUPPORTS_SAVE )
GAME( 1999, gigawingjd, gigawing, dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "Giga Wing (Japan 990223 Phoenix Edition) (bootleg)",                                         MACHINE_SUPPORTS_SAVE )
GAME( 2000, 1944d,      1944,     dead_cps2,     cps2_2p2b, cps2_state, init_cps2,     ROT0,   "bootleg", "1944: The Loop Master (USA 000620 Phoenix Edition) (bootleg)",                               MACHINE_SUPPORTS_SAVE )
GAME( 2000, dimahoud,   dimahoo,  dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT270, "bootleg", "Dimahoo (USA 000121 Phoenix Edition) (bootleg)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrixd,   mmatrix,  dead_cps2,     cps2_2p1b, cps2_state, init_cps2,     ROT0,   "bootleg", "Mars Matrix: Hyper Solid Shooting (USA 000412 Phoenix Edition) (bootleg)",                   MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrixuf,   mmatrix,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Mars Matrix: Hyper Solid Shooting (trap15 Improvement USA 20250412)(hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mmatrixufj,   mmatrix,  cps2, cps2_2p3b, cps2_state, init_cps2,     ROT0,   "Takumi (Capcom license)", "Mars Matrix: Hyper Solid Shooting (trap15 Improvement Japan 20250412)(hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, progearud,  progear,  dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Progear (USA 010117 Phoenix Edition) (bootleg)",                                             MACHINE_SUPPORTS_SAVE )
GAME( 2001, progearjd,  progear,  dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Progear no Arashi (Japan 010117 Phoenix Edition) (bootleg)",                                 MACHINE_SUPPORTS_SAVE )
GAME( 2001, progearjbl, progear,  dead_cps2,     cps2_2p3b, cps2_state, init_cps2,     ROT0,   "bootleg", "Progear no Arashi (Japan 010117) (decrypted bootleg)",                                       MACHINE_SUPPORTS_SAVE ) // Not an actual phoenix set, but works as one
GAME( 2001, pzloop2jd,  pzloop2,  dead_cps2,     pzloop2,   cps2_state, init_pzloop2,  ROT0,   "bootleg", "Puzz Loop 2 (Japan 010226 Phoenix Edition) (bootleg)",                                       MACHINE_SUPPORTS_SAVE )
GAME( 2004, hsf2d,      hsf2,     dead_cps2,     cps2_2p6b, cps2_state, init_cps2,     ROT0,   "bootleg", "Hyper Street Fighter II: The Anniversary Edition (Asia 040202 Phoenix Edition) (bootleg)",   MACHINE_SUPPORTS_SAVE )
// HBMAME
#include "cps2mis.cpp"
#include "cps2t.cpp"

