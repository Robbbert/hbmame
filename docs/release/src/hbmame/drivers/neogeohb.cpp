// license:CC BY-NC-ND 3.0 (copying this code into another project is not permitted)
// copyright-holders:Robbbert
/*********************************************************************************************************

neogeohb.cpp - started 2014-06-02

HBMAME list of protos, unreleased, and homebrews. This number system is to make the categorisation
of non-official NeoGeo sets easier. 300-399 is for unreleased commercial sets, while 400-999 is for
homebrews. Protos will generally have an equivalent 'official' number that can be used.

300-399 is mostly speculation, it is not really known what may or may not have actually existed.
It is possible that a game is already known under another title, or perhaps it was a
prototype of another title. Corrected info is welcome.

400-599 playable homebrew games.
600-799 test roms, non-playable demos and the like.

We want the MVS or AES roms for any game marked with * after the number.


******* CD-only titles wanted as roms *************

NUM YEAR COMPANY          TITLE
---.----.----------------.-------------------------------------------
085*1997 SNK              Samurai Showdown R.P.G.
091*1995 ADK              ADK World
205*1995 SNK              NeoGeo CD Special
229*1996 SNK              King of Fighters 96 CD Collection



******* Normal games that have alternate titles (or other) *************

NUM TITLES
---.-------------------------------------------------------------------
001 NAM-1975 (The MVS misses the demo sequence)
004 Mahjong Kyo Retsuden / Real Mah-Jong Game
007 Alpha Mission II / ASD Last Guardian II (MGD2 dump has extra sounds & roms)
021 Puzzled / JoyJoyKid  (The MVS misses the story mode, AES misses demo)
022 Blue's Journey / Raguy
023 Quiz Daisousa Den / The Last Count Down
029 Legend of Success Joe / Ashita no Joe Densetsu
033 Fatal Fury / Garou Densetsu
038 Thrash Rally / Rally Chase
043 3 Count Bout / Fire Suplex
044 Art of Fighting / Ryuuko no Ken
045 Samurai Shodown / Samurai Spirits
047 Fatal Fury 2 / Garou Densetsu 2
048 Mahjong Janshin / Quest of Jongmaster
052 Super Sidekicks / Tokuten Ou
056 Art of Fighting 2 / Ryuuko no Ken 2
058 Fatal Fury Special / Garou Densetsu Special
059 Savage Reign / Fu'un Mokujiroku
061 Super Sidekicks 2: The World Championship / Tokuten Ou 2 : Real Fight Football
062 Spin Master / Miracle Adventure
063 Samurai Shodown 2 / Salubi Spirits / Samurai Spirits 2: Haohmaru Jigokuhen
065 Windjammers / Flying Power Disc
066 Karnov's Revenge / Fighter's History Dynamite
069 Fatal Fury 3 / Garoudensetsu 3; The Battle of Destiny; Road to the Final Victory
074 Aggressors of Dark Combat / Tsukai Gan Gan Koshinkyoku
075 Aero Fighters 2 / Sonic Wings 2
076 Zed Blade / Operation Ragnarok
079 Street Hoop / Dunk Dream / Street Slam
081 Super Sidekicks 3: The Next Glory / Tokuten Ou 3: Eiko e no Chousen
083 Bust-a-move / Puzzle Bobble
085 Samurai Shodown RPG / Shinsetsu Samurai Spirits Bushidouretsuden
087 Samurai Shodown III: Blades of Blood / Fighters Swords / Samurai Spirits: Zankurou MusouKen
091 ADK World / ADK Special
092 Far East of Eden: Kabuki Klash / Tengai Makyo Shinden
094 Voltage Fighter Gowcaizer / Chojin Gakuen Gowcaizer
095 Real Bout Fatal Fury / Real Bout Garou Densetsu
096 Art of Fighting 3 / Ryuuko no Ken Gaiden
097 Aero Fighters 3 / Sonic Wings 3
200 Neo Turf Masters / Big Tournament Golf
203 Master of Syougi / Shogi no Tatsujin
208 Super Dodgeball / Kunio no Nekketsu Toukyuu Densetsu
211 ZinTrick / Oshidashi Zentrix
215 Super Sidekicks 4: The Ultimate 11 / SNK Football Championship / Tokuten Ou: Honou no Libero
216 Kizuna Encounter / Fu'un Super Tag Battle
218 Ragnagard / Shinoken
219 Pleasure Goal: 5 on 5 Mini Soccer / Futsal
220 Ironclad / Chotetsu Brikinger
222 Samurai Shodown 4 / Legend of a Warrior / Samurai Spirits: Amakusa Kourin
231 Money Puzzle Exchanger / Money Idol Exchanger
234 The Last Blade / Last Soldier / Bakumatsu Roman: Gekka no Kenshi
236 The Irritating Maze / Ultra Denryuu Iararabou
237 Pop 'n' Bounce / Gapporin
240 Real Bout Fatal Fury 2: The Newcomers / Real Bout Garou Densetsu 2: The Newcomers
242 King of Fighters '98: { The Slugfest / Dream Match Never Ends }
243 The Last Blade 2 / Last Soldier 2 / Bakumatsu Roman Dai Ni Maku: Gekka no Kenshi
248 Bust-a-move Again / Puzzle Bobble 2
252 Ganryu / Musashi Ganryuki
253 Garou: Mark of the Wolves
261 Sengoku 3 / Sengoku Legends 2001 / Sengoku Denshou 2001
266 Matrimelee / Shin Goketsuji Ichizoku Matrimelee
269 SVC Chaos: SNK vs Capcom / SNK vs Capcom SVC Chaos
270 Samurai Shodown 5 / Samurai Spirits Zero
272 Samurai Shodown 5 Special / Samurai Spirits Zero Special
??? Dragon's Heaven / Dark Seed


********************** Official Titles (including non-free indie) **********************************
(from info gathered from various sites, hopefully correct)
(The notation GAME#nnnn? refers to unverified info from wiki.neogeodev.org/index.php?title=NGH_number)

NUM GAME YEAR COMPANY                 TITLE
---.----.----.-----------------------.-------------------------------------------
008*     1990 Alpha Denshi            Sun Shine (video on youtube)
012*     1990 Alpha Denshi            Block Paradise (video on youtube)
026*     1991 Alpha Denshi            Fun Fun Brothers (video on youtube)
028*     1991 Sammy                   Dunk Star (video on youtube)
035*     1991 Alpha Denshi            Mystic Wand (video on youtube)
   *     1995 Yumekobo                The Karate Ninjya Sho (info from www.neo-geo.com)
071*     1994 Visco                   Bang Bang Busters (1994 on title screen)
072*     1995 Monolith                Last Odyssey Pinball Fantasia (video on youtube)
   *     1994 Face                    Treasure of the Caribbean (may also be known as 'Carib No Zaihou')
077*     1995 Astec21                 The Warlocks of the Fates / Shinryu Senki (video on nicovideo.jp and youtube)
099*     1996 Sunsoft                 Hebereke's Pair Pair Wars
   *     1996 Video System            Danger Droppers Caution (Droppers)
204*     1997 Success                 Q.P. (Quality People)
210*     1996 Viccom                  The Eye of Typhoon
226*     1997 Saurus/Takumi           Magic Master / Mahou Juku
258*     2002 ADK                     Dance RhythMIX
300 0300 2006 NG:DEV.Team             Last Hope
301*     2010 NG:DEV.Team             Fast Striker (GAME#0301?)
302*     2011 NG:DEV.Team             Last Hope Pink Bullets (could be year2008) (GAME#0302?)
303*     2012 NG:DEV.Team             Gunlord (GAME#0303?)
304*     2013 NG:DEV.Team             NEO XYX (GAME#0304?)
305*     2014 NG:DEV.Team             Razion (GAME#0305?)
306*     2015 NG:DEV.Team             Kraut Buster (could be year2016) (GAME#0306?)
307 DAD1 2023 LudoScience             Yo-Yo Shuriken (released 2023-04-11)
316 0048 2011 Le Cortex               Treasure of the Caribbean (remake) (also GAME#0F48?)
323 1234 2022 OzzyOuzo                The Eye of Typhoon (remake)
331 0008 2001 Brezzasoft              Jockey Grand Prix
332 03E7 2001 Brezzasoft              V-Liner
334 1919 2022 ADK                     19YY (extracted from ADK World CD (year 1995))
335 00CF 2022 ADK                     Cake Fighter (extracted from Twinkle Star Sprites rom (year1996))
336 0094      Face                    Dragon's Heaven
338 FEDC 2004 Vektorlogic             Super Bubble Pop
   *                                  161in1 Multigame (GAME#9237?)
340 0502 2014 Neobitz                 Knight's Chance (also GAME#0501?)
341 0400 2013 Le Cortex               Crouching Pony Hidden Dragon demo
342 BB01 2019 Bitmap Bureau           Xeno Crisis
343 14A1 2024 Neo Byte Force          Captain Barrel
344*     2023 PixelHeart              Punky Circus
345*     2025 PixelHeart              Daemon Claw
360 5003 2003 PhenixSoft              Crouching Tiger Hidden Dragon (hack of kof2001)
361 5232 2003 SNK?                    King of Gladiator (hack of kof97)
362 5246 2004 Saurus/SNK              Lansquenet 2004 (hack of Shock Troopers 2nd squad)
364 0271 2004 SNK?                    King of Fighters 2004 EX (hack of kof2003)
365 0265 2004 Dragon Co, Ltd          King of Fighters Special Edition 2004 (hack of kof2002)
365 2265 2004 Dragon Co, Ltd          King of Fighters Special Edition Plus 2004 (hack of kof2002)
366 0275 2004 SNK?                    King of Fighters 10th Anniversary (hack of kof2002) (original number 5004,5008)
367 0275 2005 SNK?                    King of Fighters 10th Anniversary 2005 (hack of kof10th) (original number 5006,5007)
368 0275 2008 SNK?                    King of Fighters 10th Anniversary 2008 (hack of kof10th) (original number 5009)
369 0275 2019 ?                       King of Fighters 10th Anniversary 2019 (hack of kof10th)
370 0275 2020 ?                       King of Fighters 10th Anniversary 2020 (hack of kof10th)



********************** Homebrew Games (400 to 599) *************************

GAME = Game number inside the rom (personally verified)

NUM GAME YEAR COMPANY                 TITLE
---.----.----.-----------------------.-------------------------------------------
400 0066 1999 Kyle Hodgetts           Digger Man
403 2003 2003 Neobitz                 Poker Night
404 2000 2004 Neobitz                 Columns
407 2000 2002 Blastar                 Neo no Panepon
408 1234 2004 Blastar                 Neo Puzzle League
409 FFFF 2005 Blastar                 Jonas Indiana and the Lost Temple of Ra
410 FFFF 2006 Blastar                 Codename: Blut Engel
410 FFFF 2018 Blastar                 Codename: Blut Engel 2018
411 FFFF 2006 Blastar                 NGEM2K
412 07D6 2020 Blastar                 Jump n Run Tech Demo
413 2019 2021 Blastar                 Looptris
414 2019 2022 Blastar                 Looptris Plus
415 9999 2023 Blastar                 Flappy Chicken (2023-04-30)
416 2023 2025 Blastar                 Block Panic DX 250101
417 2026 2025 Blastar                 World Racer (2025-11-14)
418 1234 2008 Crim                    Neogeo 2-player Tetris
419 1234 2011 Sebastian Mihai         Neo Thunder
420 0420 2023 tcdev                   Xevious
422 0422 2023 tcdev                   Galaxians
430 0275 2012 NeoGeoFreak             Time's Up! demo
430 0276 2012 NeoGeoFreak             Time's Up!
431 0202 2005 Rastersoft              Frog Feast demo
431 0273 2006 Rastersoft              Frog Feast
436 0275 2010 CeL                     Neo Pang
443 1234 2012 M.Priewe                Santaball
444 1234 2018 kl3mousse               Sea Fighter
446 7777 2014 M.Priewe                Monitor Test Tool v1.0
446 0602 2022 M.Priewe                Monitor Test Tool v1.3
447 0600 2021 M.Priewe                Hypernoid
461 0280 2020 Elrayzeur               Puzzle Crazy
462 0083 2009 Elrayzeur               Puzzle Bobble Crazy beta
469 9999 2002 Neodev                  Neo Pong
469 0202 2002 Neodev                  Neo Pong (neoponga)
491 0539 2019 Totologic               Neotris
492 7777 2019 Fullset                 Project Neon Caravan Edition
494 0539 2020 Totologic               Bonus Stage
495 1234 2020 OzzyOuzo                New! Super Mario Bros Demo
495 1234 2020 OzzyOuzo                New! Super Mario Bros
496 1234 2020 OzzyOuzo                Neo Black Tiger
497 0993 2021 iq132                   Cabal (Neo-Geo port)
498 0052 2021 Kako Eimon              Abyssal Infants
499 2048 2021 Nicole Branagan         Neo 2048
500 0993 2022 iq_132                  Kid no Hore Hore Daisakusen (Neo-Geo port)
501 0282 2023 iq_132                  Soldier Girl Amazon (Neo-Geo port)
502 14A0 2023 Neo Byte Force          Cyborg Force (https://ozzyouzo.itch.io/cyborg-force)
503 0722 2024 Pixelheart              Gladmort Demo
503 0723 2024 Pixelheart              Gladmort Demo 2, Gladmort (full)
504 1337 2025 Hoffman                 Shinobi (Neo-Geo port)
505 14A3 2025 La Casa De Ruivo        Double Dragon One demo/beta
506 0283 2025 iq_132                  Karnov (Neo-Geo port)
507 1338 2025 Hoffman                 Golden Axe (Neo-Geo port)
508 1324 2025 iq_132                  P.O.W (Neo-Geo port)
509 0539 2025 Kakoeimon               Super Power Kick
510 1320 2024 iq_132                  Atomic Runner Chelnov (Neo-Geo port)
511 14A5 2025 OzzyOuzo                Bad Dudes vs Dragonninja (Neo-Geo port)
512 14A4 2025 OzzyOuzo                In the Hunt (Neo-Geo port)
513 0539 2025 Tonsomo Entertainment   Jurl
514 F00D 2025 iq_132                  Food Fight (Neo-Geo port)
515 0070 2025 Pixelheart              Bang Bang Busters 2 (demo)
516 14A6 2025 Balek Corp              Violent Vengeance: The Universal Hero
539 0539 2025 Shadow Gangs            Shadow Gangs demo



********************** Non-playable demos (600-799) ************************
(includes input tests and the like)
GAME = Game number inside the rom (personally verified)

NUM GAME YEAR COMPANY                 TITLE
---.----.----.-----------------------.-------------------------------------------
600 2501 2023 Artemio Urbina          240p Test Suite
601 2002 2002 Jeff Kurtz              Shadow of the Beast demo
602*          Neobitz                 Neobitz demo (Jeff M. Kurtz is Neobitz)
603 0962 2022 Ichikyu Wai Wai/EKORZ   MVS 4P Key System Check
606 FFFF 2000 Blastar                 System Check
607 2500 2004 Blastar                 Neo 2500 demo
608 09C5 2015 Blastar                 Twister in a mirror intro
609 09C8 2016 Blastar                 Christmas/Happy New Year 2017 demo
610 09CC 2018 Blastar                 NGYM2610
611 2009 2021 Blastar                 4-player multitab test
612 07E1 2021 Blastar                 Neo Driftout Tech Demo
613 0202 2001 Rastersoft              WW2 demo
616 0202 2003 Charles Doty            Neogeo Demo
620 1234 2004 BarfHappy               Neo Castlevania demo
621 0052 2012 Oxygene                 Neo 3D demo
624 0275 2012 NeoGeoFreak             Transparency demo
625 0269 2009 Raregame                Chip n Dale intro
626 0269 2009 Raregame                Darkwing Duck intro
627 0245 2009 Raregame                Ghostbusters intro
628 0269 2009 Raregame                Robocop intro
629 0245 2009 Raregame                Spiderman intro
630 0269 2009 Raregame                Teenage Mutant Ninja Turtles intro
631 0269 2009 Raregame                Duck Tales Intro
634*     2010 CeL                     Hello World
635 1234 2010 CeL                     Neorom Jukebox
639 1234 2013 Cristiano Bei           Primo demo
640 1234 2013 Cristiano Bei           NeoGeo Galaga demo
641 0017 2013 Cristiano Bei           NeoGeo Sound Test
642 7777 2017 Cristiano Bei           Bad Apple demo
642 BADA 2017 Cristiano Bei           Bad Apple demo (badappleb)
643 0539 2023 Dekadence               68k Inside
644 2025 2025 TTE                     Vaporous
645 0202 2019 Peter Lemon             Neogeo Chaos Demo (identical to neodemo)
646 0202 2019 Peter Lemon             Green Space Demo (useless)
647 0202 2019 Peter Lemon             Hello World (useless)
653 0052 2011 Furrtek                 Nyan Cat demo
657 0052      Furrtek                 MVSTest01
658 0052      Furrtek                 KnackiBalls
667 0052 2011 Furrtek                 Sprite Experimenter
669 3CFB 2015 freem                   ADPCM-A Playback Demo
670 3CFB 2015 freem                   ADPCM-B Playback Demo
674 0FAB 2015 freem                   Sound-Loop Demo
675 0052 2016 Kannagi                 Ennemi Sprite Demo
676 0052 2015 Kannagi                 Neo Fight
677 7777 2016 Mega Shocked            DatImage demo
678 7777 2016 Mega Shocked            Demo
679*     2016 Luis Miguel Mayor       Tech Demo #1 (Street Fighter 2)
680 7777 2016 Luis Miguel Mayor       Tech Demo #2
681 7777 2018 Vasily Familiya         Operation I.G.L.A. demo
682 7777 2018 Vasily Familiya         Team Lernit
683 7777 2018 Vasily Familiya         Everlasting Summer: Samantha
684 7777 2018 Vasily Familiya         Vlad2000
685 7777 2018 Vasily Familiya         Raroggame
687 7777 2018 Vasily Familiya         Double Dragon SpritePool Demo
688 7777 2018 Vasily Familiya         Shaman King demo
689 7777 2019 Vasily Familiya         Venus Wars demo
690 7777 2020 Vasily Familiya         Girl X Girl demo



********************** CD-exclusive that exist (800-899) ************************
(Will be removed/renumbered if MVS/AES version is found)
GAME = Game number inside the PRG file (personally verified)

NUM GAME YEAR COMPANY                 TITLE
---.----.----.-----------------------.-------------------------------------------
800 1234 2000 Fabrice Martinez        VIP2 [nebula]
802 0666 2007 NG:Dev:Group            Last Hope (NGCD V0.52 beta)
803 1234 2009 freem                   Maths Test
804 0000 2011 Furrtek                 VU Meter [nebula]
805 1234 2012 NG:Dev:Group            NGD::ARK
806 1234 2012 RKGAMES                 Neo-Sprite Test/Demo
807 1234 2014 freem                   Input Test
808 1234 2014 freem                   Process Test
809 7777 2014 freem                   Color Test
810 1234 2014 freem                   Fade Test
811 1234 2014 freem                   Hello
812 0202 2014 freem                   HelloFix
813 0202 2014 freem                   HelloSpr
814 0202 2014 freem                   Palette Basics
815 1234 2014 Christiano Bei          Phoenix
816 7777 2014 M.Priewe                NeoSubmarine demo (early)
817 1234 2015 Resistance              Eira
818 1234 2016 Christiano Bei (BEY)    Nata Demo
819 09C6 2016 Blastar                 We Love Alice
820 09C6 2016 Citavia                 Diff
821 7777 2016 Trilobit                Visual Novel
822 7777 2017 Visy                    Neo Fukkireta
823 2019 2019 Blastar                 No Neo No Party
824 2007 2020 Blastar                 Subspecies Unfinished
825 2018 2021 Blastar                 N*Tris3D
826 7777 2021 David Vandensteen       Flamble
827 7777 2023 David Vandensteen       CD Pong



********************** Vapourware and Rumours **********************************
(Every unnumbered title starts here, until it actually exists)
(Generally, these should be considered as abandoned)

YEAR COMPANY                          TITLE
----.--------------------------------.-------------------------------------------
     SNK                              Baseball Stars 3 (confirmed never existed)
     SNK                              Magician Lord 2 (confirmed never released)
                                      Death Match
                                      Fire Suplex 2
                                      Heavy Glove Boxing
                                      Ninja Gaiden (This may be Ninja Gaiden II, 1994, Tecmo)
                                      Reactor
                                      World Heroes Link-up
1994 Visco                            Crystal Legacy  (this may be an early name for Breakers)
1994 Video System Co                  Super Volley 94 (this may be an early name for Power Spikes 2)
1994 Mahou                            All Stars Volley
1994 Visco                            Puzzlekko Club (Puzzle KO Club)
1995 Face                             Ultimate 4 / King of Athlete
1995 MAX                              Maseki Taisen Stoon / Magic Stone War
1995 Nintendo/SNK                     Crystalis (CD) (neogeocdworld.info)
1996 Kigyo                            Action Pac
1996 SNK                              Kizuna Encounter Special Edition (could be "Kizuna Encounter 4way Battle") (could be "Fu'un Super Tag Battle Link-up")
1996 Saurus                           Mahou Juku Magic Master
1997 Saurus                           Neo Pool Master
2000 Jeff Ferrier for Vektorlogic     Neo Sparks (cancelled) (video on youtube)
2004 Jeff Ferrier for Vektorlogic     TommyK (cancelled) (video on youtube)
2013 Jeff Ferrier for Vektorlogic     Manic Panic (video on youtube)
2000 SNK                              Garou 2 / Mark of the Wolves Special
2002 Sun Amusement                    Poker Kingdom / Bingo Island
2002 ADK                              World Heroes 3
                                      Neo Dev Cart
                                      MVS Test Cart
2014 M.Priewe                         Doom in the Deep (Submarine Shooter Demo - NeoSubmarine) (www.neohomebrew.com) (video on youtube)
     M.Priewe                         Witch Adventure Demo
     M.Priewe                         3D Racing Game Demo
     M.Priewe                         Fighting Game Demo
     M.Priewe                         NeoGeo User Subroutine
2013 Le Cortex                        Crouching Pony Hidden Dragon (full game)(Game never released because LeCortex died)
2014 RiKo(aka CosmicR/Phoenix Risen)  Cosmic Tower (demo on youtube)
2013 Cristiano Bei                    NGPhoenix (CD) (playable CD demos exist)(www.iocera.com) [site dead]
     Furrtek                          Unleashed (furrtek.free.fr)
2012 Furrtek                          Astrosmash (GAME#0400?)
2012 NGD                              NGD::ARK (Arkanoid port) (playable CD demos exist)(demo on youtube)
     NGD                              NGD::AIR (ngdevgroup.com [site dead])
     NGD                              NGD::FIGHT
2005 Neobitz                          Neo-Pac (pacman game)
     Neobitz                          Submarine Shooter (maybe same as Submarine Shooter demo)
     Neobitz                          Stadium Stars (neobitz.com) [site dead]
     Neobitz                          untitled (setting in a castle) (neobitz.com) [site dead]
2016 Neobitz                          Flight of the Dragon (video at www.facebook.com/Neobitz/videos/1239136276111256/)
     tcdev                            Asteroids
2023 tcdev                            Pengo (GAME#0423?)
     tcdev                            Scramble
2016 tcdev                            Knight Lore
2014 tcdev                            Lode Runner (video at retroports.blogspot.com/2014/07/ill-c-your-lode-runner-and-raise-you.html)
2014 tcdev                            Donkey Kong (GAME#0421?) (CD) (video at retroports.blogspot.com/2015/08/dusting-off-neo-kong.html)
2016 Luis Miguel Mayor                Tech Demo #1 (Street Fighter 2)
2017 HPMAN                            Beats of Rage (some alpha videos on youtube)
2017 Blastar                          Blut Engel II (some alpha videos on youtube)
2018 Blastar                          Neo No Panepon 2 Sunnyland Edition (CD) (video on youtube)
2018 NeoHomeBrew (aka M.Priewe)       Pseudo 3D Racing Game Engine Demo (video on youtube)
2019 Hine62/SAOH (aka Steven Hine)    NeoGeo Space Invaders (video on youtube)
2020 Chips on Steroids                Galaxy Guns (demo on youtube)
2022 Tigerskunk                       Inviyya II / Hyperblaster (video on youtube)
2022 Frogbull                         Neo Mania (video on youtube)
2022 Geezer Games / Raster Wizards    Alien Warhead (demos on youtube) (https://www.facebook.com/RasterWizards)
2023 Kimera Gamesoft                  Operation Killmeister (www.kimera.gs) (cancelled due to bankruptcy)
2023 Bitmap Bureau                    Final Vendetta (there's talk of a NeoGeo release, but...?)
2023 NeoHomeBrew                      Flippers Demo (on youtube)
2023 NeoHomeBrew                      Pinball Game Playfield Demo (on youtube)
2023 NeoHomeBrew                      Bouncing Ball Demo (on youtube)
2023 Nalua Studio                     Vengeance Hunters (https://shop.naluastudio.com/shop/products)
2023 Shadow Gangs                     Shadow Gangs (https://www.shadowgangs.net)
2023 PixelHeart                       Daemon Claw: Origins of Nnar (https://twitter.com/DAsteborg/status/1696175973253734420)
2025 Neofid Studios                   Demons of Asteborg DX (https://neofidstudios.itch.io/demons-of-asteborg-dx)
2025 Neofid Studios                   Copper Storm
2025 Rosenthal Castle                 Petal Crash (video on youtube) (https://rosenthalcastle.itch.io/petal-crash-neo)
2025 Retro Sumus                      SovietBorgs (video on youtube)
2025 Bit Beam Cannon                  Metal Mack


*********************************************************************************************************/

#include "includes/neogeo.h"


void neogeo_state::init_fr2ch()
{
//// Fix rebooting at start

	uint16_t *mem16 = (uint16_t*)cpuregion;

	// change jsr to C004DA
	mem16[0x01AF8 /2] = 0x04DA; // C00552 (Not used?)
	mem16[0x01BF6 /2] = 0x04DA; // C0056A (fixes crash)
	mem16[0x01ED8 /2] = 0x04DA; // C00570 (Not used?)
	mem16[0x1C384 /2] = 0x04DA; // C00552 (fixes crash)

	// 0x001C06 - this routine can cause a loop/freeze
	mem16[0x01C06 /2] = 0x4E75;

//// Fix text on bottom line

	uint8_t *dst = fix_region;

	// Move text for credit + coin info (Thanks to Kanyero), overwrites "MA" in neogeo logo
	memcpy(dst, dst + 0x600, 0x140);

	// Patch out neogeo intro (because of above line)
//  mem16[0x114 /2]=0x200;


//// Optional stuff


	// Hack in the proper identification (see setup menu [F2])
	mem16[0x3a6 / 2] = 0x4649;
	mem16[0x3a8 / 2] = 0x4e41;
	mem16[0x3aa / 2] = 0x4c20;
	mem16[0x3ac / 2] = 0x524f;
	mem16[0x3ae / 2] = 0x4d41;
	mem16[0x3b0 / 2] = 0x4e43;
	mem16[0x3b2 / 2] = 0x4520;
	mem16[0x3b4 / 2] = 0x3220;
	uint8_t  *mem8 = cpuregion;
	memcpy(mem8+0x61e, mem8+0x3a6, 16);
	memcpy(mem8+0x896, mem8+0x3a6, 16);

	// Album Fix
	mem16[0x1C382 /2] = 0x0008; // C00552
	mem16[0x1C384 /2] = 0x0000;
	mem16[0x80000 /2] = 0x33FC;
	mem16[0x80002 /2] = 0x0001;
	mem16[0x80004 /2] = 0x0020;
	mem16[0x80006 /2] = 0x0002;
	mem16[0x80008 /2] = 0x4E75;

	init_neogeo();

	/* old fr2cd code:
	uint16_t *mem16 = (uint16_t*)memory_region(machine, "maincpu");
	mem16[0x1BF2/2] = 0x4E71;
	mem16[0x1BF4/2] = 0x4E71;
	mem16[0x1BF6/2] = 0x4E71;
	init_neogeo();  */
}



// 054 : Crossed Swords 2
ROM_START( crswd2bl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_BE|ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "054bl.p1", 0x100000, 0x100000, CRC(64836147) SHA1(083cb1626885893e736fc9998036c952cd4d503b) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "054bl.s1", CRC(22e02ddd) SHA1(ebd834affc763cc5854abf1c6c42f43f3f3755fd) )

	NEO_BIOS_AUDIO_128K( "054bl.m1", CRC(63e28343) SHA1(f46dbc2f1d6033b11047cca31a9a7d715dc69cb2) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "054bl.v1", 0x000000, 0x200000, CRC(22d4b93b) SHA1(0515f2ee5d9a8ce424c80721e06f746ac6a543a8) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "054.c1", 0x000000, 0x400000, CRC(8221b712) SHA1(7e68871f1bfc402ef27c8fa088c680cbd133f71a) )
	ROM_LOAD16_BYTE( "054.c2", 0x000001, 0x400000, CRC(d6c6183d) SHA1(cc546ff063fae2c01c109fabcd5b2d29ec3299db) )
ROM_END

// Roms missing; not working at all
ROM_START( csw2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "054.p1", 0x000000, 0x100000, CRC(b6e8f765) SHA1(fac63109664b834b43240a6e93353173e129e6af) )

	NEO_SFIX_128K( "054.s1", CRC(7ddb9c7e) SHA1(c7ca63f1ff9edb2f71d13cc14d6a426b85182228) )

	NEO_BIOS_AUDIO_64K( "054.m1", CRC(c45e902b) SHA1(fa17a16077393f986b72f412ce91bded34558af0) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "054.v1", 0x000000, 0x80000, CRC(a12842f2) SHA1(6f6377ff54465c4c13e34f8cb4f333f4f15aed5c) )
	ROM_LOAD( "054.v2", 0x000001, 0x80000, CRC(3b7feef0) SHA1(748db17fbd3bf8fc427ff57e0952f21dd2782e6b) )

	ROM_REGION( 0x800000, "sprites", 0 ) // c-roms not found yet, using crswd2bl roms for now
	ROM_LOAD16_BYTE( "054.c1", 0x000000, 0x400000, CRC(8221b712) SHA1(7e68871f1bfc402ef27c8fa088c680cbd133f71a) )
	ROM_LOAD16_BYTE( "054.c2", 0x000001, 0x400000, CRC(d6c6183d) SHA1(cc546ff063fae2c01c109fabcd5b2d29ec3299db) )
ROM_END


// 098 : Idol Mahjong Final Romance 2
ROM_START( fr2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "098.p1", 0x000000, 0x80000, CRC(09675541) SHA1(6afb89d43e67f93e40f3877cbedfec9566e3ff0f) )

	NEO_SFIX_128K( "098.s1", CRC(0e6a7c73) SHA1(31b1194524dcc80ec4d63bac088b6fb4909f496c) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "098.c1", 0x0000000, 0x400000, CRC(29148bf7) SHA1(75097fbe8877720afbcbe4dbe30bc600466d759f) )
	ROM_LOAD16_BYTE( "098.c2", 0x0000001, 0x400000, CRC(226b1263) SHA1(dee6a4a0a727c1d8a6d298cb38ed1b9901992d5b) )
ROM_END

ROM_START( fr2cd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "098cd.p1", 0x000000, 0x100000, CRC(a2527a5f) SHA1(1f6e3c7de9154c0026360ee6fa0bd211ed541af1) )

	NEO_SFIX_128K( "098cd.s1", CRC(764ac7aa) SHA1(cc40ad276e63084ebf3c3ee224083762a47c3cf8) )

	NEO_BIOS_AUDIO_64K( "098cd.m1", CRC(a455fa31) SHA1(82ccc5e2e5869a46da7533ed10ac5eab549a0063) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098cd.v1", 0x000000, 0x100000, CRC(92e175f0) SHA1(788a9cce9028f16d6734bff163beb1a19305b8be) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "098cd.c1", 0x0000000, 0x300000, CRC(29bf9af0) SHA1(6c7e9c2b7062bf361a2290010c4e1828eb7019cc) )
	ROM_LOAD16_BYTE( "098cd.c2", 0x0000001, 0x300000, CRC(3cf46f63) SHA1(c5316e83586e7b1c902746f8f2049baf026b52bf) )
ROM_END

ROM_START( fr2ch ) // CD to MVS Conversion
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "098ch.p1", 0x000000, 0x080000, CRC(9AA8CEE0) SHA1(b2201f16d8fde4eaae9dbc754d019e7b381ecc06) )

	NEO_SFIX_128K( "098cd.s1", CRC(764ac7aa) SHA1(cc40ad276e63084ebf3c3ee224083762a47c3cf8) )

	NEO_BIOS_AUDIO_64K( "098cd.m1", CRC(a455fa31) SHA1(82ccc5e2e5869a46da7533ed10ac5eab549a0063) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098cd.v1", 0x000000, 0x100000, CRC(92e175f0) SHA1(788a9cce9028f16d6734bff163beb1a19305b8be) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "098ch.c1", 0x000000, 0x100000, CRC(6158CF4A) SHA1(8e34b65f2cdfccaa6eeb40fef1ab3453eab92f46) )
	ROM_LOAD16_BYTE( "098ch.c2", 0x000001, 0x100000, CRC(93A809A3) SHA1(9b680b8b758cfc77f01d7211b67343a90462b3f3) )
	ROM_LOAD16_BYTE( "098ch.c3", 0x200000, 0x100000, CRC(FAFA3381) SHA1(0c4f20c4d5f57c1dad50b914f9d1299a9d882b42) )
	ROM_LOAD16_BYTE( "098ch.c4", 0x200001, 0x100000, CRC(9895E23F) SHA1(565b5fbe07f65fb48d0bde769d3245a6ac747774) )
	ROM_LOAD16_BYTE( "098ch.c5", 0x400000, 0x100000, CRC(EEAAA818) SHA1(434c13852153d7bebe138611a836d9bf07d1d4cf) )
	ROM_LOAD16_BYTE( "098ch.c6", 0x400001, 0x100000, CRC(F3D9A190) SHA1(44932122e044d23234235cb3899f6e1e63970983) )
ROM_END


// 211 : Zintrick
ROM_START( zintrkcd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211cd.p1", 0x000000, 0x100000, CRC(9a0bfe0a) SHA1(94299c51572b66fa37e3e496436299573b1faaa8) )

	NEO_SFIX_128K( "211cd.s1", CRC(56d16afa) SHA1(6e1f960a781f5ef1f858c51507fe573bead8ea66) )

	NEO_BIOS_AUDIO_64K( "211cd.m1", CRC(fcae1407) SHA1(5b4bff97a8c5930852eff6aee553eadc18e8f3d9) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "211cd.v1", 0x000000, 0x100000, CRC(781439da) SHA1(a80cdf3be55b5fc2ba1d167f69e222463d06ad88) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "211.c1", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "211.c2", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END

ROM_START( zintrkcd1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211m1.p1", 0x000000, 0x100000, CRC(032df2ba) SHA1(dfd35d77da203ebad5319b355fd65e6ed6a553da) )

	NEO_SFIX_128K( "211cd.s1", CRC(56d16afa) SHA1(6e1f960a781f5ef1f858c51507fe573bead8ea66) )

	NEO_BIOS_AUDIO_64K( "211m1.m1", CRC(8e539e59) SHA1(fba718ca565a6d5f122baa6d695108632672d1b0) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "211m1.v1", 0x000000, 0x800000, CRC(00e392ed) SHA1(8a46673c02fc25e65821d45749aa6fe697fa81c1) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "211.c1", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "211.c2", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END

ROM_START( zintrkm )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211m.p1", 0x000000, 0x100000, CRC(b3fb325c) SHA1(27a34ff0dfea02b1aac3ddbc7c965684694390f1) )

	NEO_SFIX_128K( "211m.s1", CRC(b67cb1fd) SHA1(3abf201bcc28ec5bd145f0848dc44c239a6ec651) )

	NEO_BIOS_AUDIO_128K( "211m.m1", CRC(a602c2c2) SHA1(19fd5d0379244c528b58343f6cbf78b4766fb23d) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "211m.v1", 0x000000, 0x200000, CRC(baa2b9a5) SHA1(914782b6c81d9a76ce02251575592b0648434ba3) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "211.c1", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "211.c2", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END

ROM_START( zintrckbh )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211h.p1", 0x000000, 0x100000, CRC(328af665) SHA1(ece74116955ecb9ec40ac90913d496836ecc82cf) )

	NEO_SFIX_128K( "211.s1", CRC(a7ab0e81) SHA1(f0649819b96cea79b05411e0b15c8edc677d79ba) )

	NEO_BIOS_AUDIO_128K( "211.m1", CRC(fd9627ca) SHA1(b640c1f1ff466f734bb1cb5d7b589cb7e8a55346) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "211.v1", 0x000000, 0x200000, CRC(c09f74f1) SHA1(d0b56a780a6eba85ff092240b1f1cc6718f17c21) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "211.c1", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "211.c2", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END

ROM_START( zintricks01 ) // all confirmed : same as zintrckb except s1
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211.p1", 0x000000, 0x100000, CRC(06c8fca7) SHA1(b7bf38965c3d0db4d7a9684d14cac94a45b4a45b))

	NEO_SFIX_128K( "211s01.s1", CRC(07948446) SHA1(10df47a2bb515f06c75d0077fee89f7193626d7b) )

	NEO_BIOS_AUDIO_128K( "211.m1", CRC(fd9627ca) SHA1(b640c1f1ff466f734bb1cb5d7b589cb7e8a55346) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "211.v1", 0x000000, 0x200000, CRC(c09f74f1) SHA1(d0b56a780a6eba85ff092240b1f1cc6718f17c21) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "211.c1", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "211.c2", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END


// 300 : Last Hope CD Beta by NG:DEV.Team
// Bugs: Insert Coin causes reboot; purple boxes instead of graphics; some corrupt graphics
ROM_START( lhcdb )  // AES conv from CD
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "300cd.p1", 0x000000, 0x100000, CRC(83e7b181) SHA1(eb13909c0b062d56d20cbdfe75abf6139eae9ee0) )

	NEO_SFIX_128K( "300cd.s1", CRC(298495d6) SHA1(c217aba6c1916d2e5337bb5c17d979b11a53582e) )

	NEO_BIOS_AUDIO_128K( "300cd.m1", CRC(d9f6c153) SHA1(c08f7ca288be1c34f4b33ed13abd805b5cd66d4e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "300cd.v1", 0x000000, 0x400000, CRC(de563ec3) SHA1(cd0f1a436cdac679792fc78906e718c78369b15a) )
	ROM_LOAD( "300cd.v2", 0x400000, 0x400000, CRC(93478033) SHA1(a03e0fcb7f51c66ec0bc5d164744db00b96973c2) )
	ROM_LOAD( "300cd.v3", 0x800000, 0x3c3500, CRC(f0ad87b8) SHA1(f992e84c6cbbc3cab79747a2ca4d646d18bebcbd) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "300cd.c1", 0x000000, 0x400000, CRC(554e6b73) SHA1(3637a8a4474e33eb75ac4a4ce0815f6a514fe337) )
	ROM_LOAD16_BYTE( "300cd.c2", 0x000001, 0x400000, CRC(7c84b0fc) SHA1(cbd7a7865113e4f21f2b1f990a077fa5c2eae894) )
	ROM_LOAD16_BYTE( "300cd.c3", 0x800000, 0x400000, CRC(28ec7555) SHA1(f094739272e6017a7193fa96cde3c1ed573a66b6) )
	ROM_LOAD16_BYTE( "300cd.c4", 0x800001, 0x400000, CRC(8b7c236b) SHA1(1cb3fe81f433a2180c85be935e340da3c55aafdb) )
ROM_END

ROM_START( lhcdba ) // AES conv from CD (alt), v3,p1 rom are different, but same bugs as above
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "300cda.p1", 0x000000, 0x100000, CRC(0f7405d7) SHA1(3858564413be0bbbcb1c970de8f66bedc7d1ac6a) )

	NEO_SFIX_128K( "300cd.s1", CRC(298495d6) SHA1(c217aba6c1916d2e5337bb5c17d979b11a53582e) )

	NEO_BIOS_AUDIO_128K( "300cd.m1", CRC(d9f6c153) SHA1(c08f7ca288be1c34f4b33ed13abd805b5cd66d4e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "300cd.v1", 0x000000, 0x400000, CRC(de563ec3) SHA1(cd0f1a436cdac679792fc78906e718c78369b15a) )
	ROM_LOAD( "300cd.v2", 0x400000, 0x400000, CRC(93478033) SHA1(a03e0fcb7f51c66ec0bc5d164744db00b96973c2) )
	ROM_LOAD( "300cda.v3", 0x800000, 0x400000, CRC(e0fc99ca) SHA1(6de935b54bf5adc4394fe824b001b38eca0291ad) ) // This is 300cd.v3 padded out with extra FFs

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "300cd.c1", 0x000000, 0x400000, CRC(554e6b73) SHA1(3637a8a4474e33eb75ac4a4ce0815f6a514fe337) )
	ROM_LOAD16_BYTE( "300cd.c2", 0x000001, 0x400000, CRC(7c84b0fc) SHA1(cbd7a7865113e4f21f2b1f990a077fa5c2eae894) )
	ROM_LOAD16_BYTE( "300cd.c3", 0x800000, 0x400000, CRC(28ec7555) SHA1(f094739272e6017a7193fa96cde3c1ed573a66b6) )
	ROM_LOAD16_BYTE( "300cd.c4", 0x800001, 0x400000, CRC(8b7c236b) SHA1(1cb3fe81f433a2180c85be935e340da3c55aafdb) )
ROM_END

ROM_START( lhopecd ) // NGCD V0.52 beta (GAME#0666) - These roms were extracted from the Japanese ISO, not working at all.
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "300cdb.p1", 0x000000, 0x100000, CRC(cd108282) SHA1(bb9b3c8bb94c05b46ea24173e62245611dbc29ce) )
	//ROM_LOAD( "300cdc.p1", 0x000000, 0x100000, CRC(a957c41c) SHA1(63fef6c72619f0e8ec7d711f18043a5f1d8f17c2) )   // HACK OF THE ABOVE

	NEO_SFIX_128K( "300cd.s1", CRC(298495d6) SHA1(c217aba6c1916d2e5337bb5c17d979b11a53582e) )

	NEO_BIOS_AUDIO_64K( "300cdb.m1", CRC(698372df) SHA1(1eae816223c3d0c5b501e6fdc5a62388450991a5) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "300cdb.v1", 0x000000, 0x80000, CRC(bc65aee1) SHA1(2090133dcbb4f5b73696aab71ed5ea5b25b09b55) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD( "lhl1spr0.spr", 0x000000, 0x178000, CRC(23448ddf) SHA1(c429c90f58125a5182255567d458883ac9ef3e85) ) // Level 1 (demo 1)
	//ROM_LOAD( "lhl2spr0.spr", 0x000000, 0x1c0000, CRC(b3010e19) SHA1(dc9a238b6b945eacd523ef0b6bb8df6432bd3da0) ) // Level 2
	//ROM_LOAD( "lhl3spr0.spr", 0x000000, 0x170000, CRC(db699a50) SHA1(56a94e34abfc0b45c3f33a3ac3435b107ccf9148) ) // Level 3 (demo 2)
	//ROM_LOAD( "lhl4spr0.spr", 0x000000, 0x200000, CRC(c5741cc8) SHA1(8593b320c140456ede6a7e355fcde978e74a0ffd) ) // Level 4
	//ROM_LOAD( "lhl5spr0.spr", 0x000000, 0x200000, CRC(a93eba1f) SHA1(da313b0ac97480cc451ba4678a819a4fab0bd5b3) ) // Level 5
	//ROM_LOAD( "lhl6spr0.spr", 0x000000, 0x0e8000, CRC(e6a92f3c) SHA1(2e0af35e5bd65a5045410a87540c8ab1d2ef3018) ) // Level 6
	//ROM_LOAD( "lhoutro0.spr", 0x000000, 0x02e000, CRC(698944df) SHA1(cfd6897fbf4b1be597be940418f3dfb7ba88116a) ) // ?
	//ROM_LOAD( "lhtitel0.spr", 0x000000, 0x003400, CRC(bbff9511) SHA1(6471c68ec368d41082c34a831452bbda969b7562) ) // ?
ROM_END

ROM_START( lhbb )  // AES V1.1 - Last Hope with Blue Bullets (hack)
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "300bb.p1", 0x000000, 0x100000, CRC(a5e6f24b) SHA1(d5a39d146982b4847e9008600f4ea9d38a1226e0) )

	NEO_SFIX_64K( "300.s1", CRC(0c0ff9e6) SHA1(c87d1ea8731ac1e63ab960b8182dd1043bcc10bb) )

	NEO_BIOS_AUDIO_128K( "300.m1", CRC(113c870f) SHA1(854425eb4be0d7fa088a6c3bf6078fdd011707f5) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "300.v1", 0x000000, 0x200000, CRC(b765bafe) SHA1(b2048c44089bf250c8dcfabb27c7981e9ee5002a) )
	ROM_LOAD( "300.v2", 0x200000, 0x200000, CRC(9fd0d559) SHA1(09e70d5e1c6e172a33f48feb3e442515c34a8f3d) )
	ROM_LOAD( "300.v3", 0x400000, 0x200000, CRC(6d5107e2) SHA1(4ba74836e3d0421a28af47d3d8341ac16af1d7d7) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "300.c1", 0x000000, 0x400000, CRC(53ef41b5) SHA1(a8f1fe546403b609e12f0df211c05d7ac479d98d) )
	ROM_LOAD16_BYTE( "300.c2", 0x000001, 0x400000, CRC(f9b15ab3) SHA1(d8ff2f43686bfc8c2f7ead3ef445e51c15dfbf16) )
	ROM_LOAD16_BYTE( "300.c3", 0x800000, 0x400000, CRC(50cc21cf) SHA1(0350aaef480c5fa12e68e540a4c974dbf5870add) )
	ROM_LOAD16_BYTE( "300.c4", 0x800001, 0x400000, CRC(8486ad9e) SHA1(19a2a73c825687e0cb9fd62bde00db91b5409529) )
ROM_END


// 307 : Yo-Yo Shuriken
ROM_START( yoyoshkn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "307.p1", 0x000000, 0x100000, CRC(8a36bd7d) SHA1(5a1af3f1de96cadd9680996d3b59471dfe6768e6) )

	NEO_SFIX_128K( "307.s1", CRC(59ce2b22) SHA1(1b5633bd9e707f035c8a3c9811c47e1ea3c74481) )

	NEO_BIOS_AUDIO_128K( "307.m1", CRC(079ac27e) SHA1(d5c8429a80845be46de0833d3fa64f1864d2fe15) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "307.v1", 0x000000, 0x200000, CRC(79e1b73f) SHA1(a905b24f973874199803d95beff786a81cb581ba) )
	ROM_LOAD( "307.v2", 0x200000, 0x200000, CRC(4c464d41) SHA1(15ae20f3e26e7458ccfc3ad11ea99af754bf696d) )
	ROM_LOAD( "307.v3", 0x400000, 0x200000, CRC(0fbe1f09) SHA1(952045f146aad3e593cc568fbebf1c4f4cdaddf3) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "307.c1", 0x000000, 0x200000, CRC(fde9b178) SHA1(21060a2695e3f1cd046322ebaacde9dbb3fb5436) )
	ROM_LOAD16_BYTE( "307.c2", 0x000001, 0x200000, CRC(780c2045) SHA1(5551c7f3010da658ac31c3478820a59eab71f5f4) )
ROM_END


// 316 : Treasure of the Caribbean
ROM_START( totc )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "316.p1", 0x000000, 0x100000, CRC(99604539) SHA1(88d5f4fe56516aa36496cafd2508f6864118f1e2) )

	NEO_SFIX_128K( "248.s1", CRC(0a3fee41) SHA1(0ab2120e462086be942efcf6ffb37f58ea966ca3) )

	NEO_BIOS_AUDIO_128K( "316.m1", CRC(18b23ace) SHA1(d55495f3d8bb5568c8f2322763278a86ba5297b3) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", ROMREGION_ERASE00 )
	ROM_LOAD( "316.v1", 0x000000, 0x200000, CRC(15c7f9e6) SHA1(c51328a92dc7c612fd6c2f3841caee4fbd120f36) )
	ROM_LOAD( "316.v2", 0x200000, 0x200000, CRC(1b264559) SHA1(50c28858304e51cdc60f425483a1d1d2bbfb2f8d) )
	ROM_LOAD( "316.v3", 0x400000, 0x100000, CRC(84b62c5d) SHA1(30d7f2e16c6d602cebce5e9b4b8a5b2bde20af58) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "316.c1", 0x000000, 0x200000, CRC(cdd6600f) SHA1(85649c199b32db7553012cb5a058a92bbbb7355a) )
	ROM_LOAD16_BYTE( "316.c2", 0x000001, 0x200000, CRC(f362c271) SHA1(4a2efa043cd2553ff7039cb18a1dba8b41493be7) )
ROM_END


// 323 : The Eye of Typhoon (demo)
ROM_START( teot_1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323a.p1", 0x000000, 0x100000, CRC(759b68d3) SHA1(2ccec3f12c1e35f47e5f5419c9770c72d783d27f) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "323a.m1", CRC(2b5738dc) SHA1(53fb556a3a12030d8e4abecafc5823037ba88c1b) )

	ROM_REGION( 0x900000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323a.v1",   0x000000, 0x100000, CRC(ac261416) SHA1(c9092127362fb07bc969d655dd2806f0e6c43e28) )
	ROM_LOAD( "323a.v2",   0x100000, 0x100000, CRC(95ea979a) SHA1(54007defb7c833b0e15d08d2be2ac21f6830d625) )
	ROM_LOAD( "323a.v3",   0x200000, 0x100000, CRC(493223d3) SHA1(eb8cc3967fd1e1228807ec79c2584b8cc0cfbbe8) )
	ROM_LOAD( "323a.v4",   0x300000, 0x100000, CRC(fbf00c96) SHA1(4083f8efbcf748dba31f0030b82487e0ea56980b) )
	ROM_LOAD( "323a.v5",   0x400000, 0x100000, CRC(9b2031d4) SHA1(15c8f5cb26af29b8dca53f1c1c49384a1b73b820) )
	ROM_LOAD( "323a.v6",   0x500000, 0x100000, CRC(107cfc89) SHA1(c4ced7dfab24ff42c6fc2424b18cf199ba30cbfa) )
	ROM_LOAD( "323a.v7",   0x600000, 0x100000, CRC(0703b761) SHA1(7d586b69bb9578d550871eda884cd6b32d86a01f) )
	ROM_LOAD( "323a.v8",   0x700000, 0x100000, CRC(8d525588) SHA1(297871c1e8888d9adab3781fb6c2c27ecf50ca45) )
	ROM_LOAD( "323a.v9",   0x800000, 0x100000, CRC(97f073b6) SHA1(02a4cce3f0cdb9421350ee5af49af13d7dcd16c2) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323a.c1", 0x000000, 0x800000, CRC(76b8e9ae) SHA1(3129ab5283c1cde389c8b311fb6c469688492fdf) )
	ROM_LOAD16_BYTE( "323a.c2", 0x000001, 0x800000, CRC(b0c6b4d0) SHA1(38fbff87722b3ae2f3f005369dbdbdea60a3be12) )
ROM_END

ROM_START( teot_2 ) // 2nd demo, 2021-02-20
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323b.p1", 0x000000, 0x100000, CRC(3ab0b686) SHA1(c950bc58044a31fe3575fb5c32ba222014b65677) )
	ROM_LOAD16_WORD_SWAP( "323b.p2", 0x100000, 0x100000, CRC(258909d5) SHA1(a4707264d39d2a06b5f9417f5d55d92cc4c328dd) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "323b.m1", CRC(62bd5336) SHA1(dd496daca2c662f6671d3c820f0cafac1bffe0b2) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323b.v1",   0x000000, 0x100000, CRC(a0906304) SHA1(681b24a72c5c082433d04feae76f9fc20af589af) )
	ROM_LOAD( "323b.v2",   0x100000, 0x100000, CRC(c5e10c1a) SHA1(e4f48144405d8de3dde86b460f4102114e8b9525) )
	ROM_LOAD( "323b.v3",   0x200000, 0x100000, CRC(cb78034b) SHA1(ead8368002c7858e4d14f14e1e5a1756f6a381b0) )
	ROM_LOAD( "323b.v4",   0x300000, 0x100000, CRC(4a2d0d6d) SHA1(70703d0952146619b1dfa5fd86cce94de2aca9d6) )
	ROM_LOAD( "323b.v5",   0x400000, 0x100000, CRC(51285019) SHA1(d279fbab0cd7c179c6c45f2604ae67c83b3bcce3) )
	ROM_LOAD( "323b.v6",   0x500000, 0x100000, CRC(03641f40) SHA1(cc979d28b548de35e53a4c52cb1cf9508a38660d) )
	ROM_LOAD( "323b.v7",   0x600000, 0x100000, CRC(e425eff3) SHA1(3a924e544da43daab4f782e344565cc81dade183) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323b.c1", 0x000000, 0x800000, CRC(94080cf2) SHA1(f2464da2076466f1387d4e4b812ea4ebec80e72c) )
	ROM_LOAD16_BYTE( "323b.c2", 0x000001, 0x800000, CRC(fb5116b6) SHA1(d70b13b24a3b85a0881bd0fe998fc98f6a99e99b) )
ROM_END

ROM_START( teot_3 ) // Alpha5 demo, 2021-05-16  (can crash sometimes)
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323c.p1", 0x000000, 0x100000, CRC(9038ff78) SHA1(33f0ce3874e24c366177c89824c4b0cd1d30dfce) )
	ROM_LOAD16_WORD_SWAP( "323c.p2", 0x100000, 0x800000, CRC(fe363160) SHA1(e503dd9cefa2b7d69ea2aaff60c63c00b83c158d) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "323c.m1", CRC(31b05f06) SHA1(da1f984af58bcc7d4d496382bcb938fa7aff5ab1) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323c.v1",   0x000000, 0x800000, CRC(299d84cf) SHA1(da0bb20b8faeaf0d78f987afc775abaadcc59dfa) )
	ROM_LOAD( "323c.v2",   0x800000, 0x800000, CRC(aced6c72) SHA1(8dc481445dbcc717c6dccb27dff446c5f05080f0) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323c.c1",   0x0000000, 0x1000000, CRC(28872e1f) SHA1(420d68ed2d417e1edfbd351619ff144fb7eacd9b) )
	ROM_LOAD16_BYTE( "323c.c2",   0x0000001, 0x1000000, CRC(fe31d1fc) SHA1(947d4cc47905308d03423640d863e60007b2309a) )
ROM_END

ROM_START( teot_4 ) // Beta, 2022-01-05
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323d.p1", 0x000000, 0x100000, CRC(603fac98) SHA1(b5d4caf9162cac28d153fe33169cfc8acb679f84) )
	ROM_LOAD16_WORD_SWAP( "323d.p2", 0x100000, 0x800000, CRC(5f557ad5) SHA1(9983d3b913f0d3bb315b696cd77f2ad7d04fbcd3) )

	NEO_SFIX_128K( "323d.s1", CRC(aba8f74c) SHA1(c8d12f535099ce0232bb637e04ac7b79569e15fe) )

	NEO_BIOS_AUDIO_64K( "323d.m1", CRC(dc29f33b) SHA1(cb1f45e93b19b00b901c5992a2646db652755d6e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323d.v1",   0x000000, 0x800000, CRC(91b3deed) SHA1(6408ad38bcbe2712a11e30efecb47b027cabd49a) )
	ROM_LOAD( "323d.v2",   0x800000, 0x800000, CRC(985e6f6b) SHA1(b3e92d6ab08ad42347b622dbb72a7589a7dbe885) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323d.c1",   0x0000000, 0x1000000, CRC(7919ea09) SHA1(bb8e058346ebc02b7e20f324411c6455271a837b) )
	ROM_LOAD16_BYTE( "323d.c2",   0x0000001, 0x1000000, CRC(62aa59a8) SHA1(dc4b7dc87d43b2c61bea799d2de5344d1c1999b9) )
ROM_END

ROM_START( teot_5 ) // Beta 2, 2022-01-29
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323e.p1", 0x000000, 0x100000, CRC(0d5763ae) SHA1(53dc53f9945cd7daeb7ab1e121f3aed132c75d0a) )
	ROM_LOAD16_WORD_SWAP( "323e.p2", 0x100000, 0x800000, CRC(bee21724) SHA1(0c37bb950568932493a1806cd1038e5935a5bddf) )

	NEO_SFIX_128K( "323e.s1", CRC(52895190) SHA1(d11385c51591b685c533a9f27c4fd3aa62927bef) )

	NEO_BIOS_AUDIO_64K( "323e.m1", CRC(583db6a5) SHA1(4f6e15a50b2074aa1d2eb5d1b91bead0639be963) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323e.v1",   0x000000, 0x800000, CRC(d88114d4) SHA1(981f151c2b7e63e876f350169426759e0e73cdd1) )
	ROM_LOAD( "323e.v2",   0x800000, 0x800000, CRC(6fccb0d0) SHA1(386f7f054d1d988cca904c03ca52023236b22813) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323e.c1",   0x0000000, 0x800000, CRC(b087909a) SHA1(e4dc29edb0f5d8ea53d8c4c5934ae5e1176466fa) )
	ROM_LOAD16_BYTE( "323e.c2",   0x0000001, 0x800000, CRC(a4d6cd16) SHA1(780e0b3eae94536951731fea1bb4145bfa5eec77) )
ROM_END

ROM_START( teot_6 ) // Beta 3, 2022-03-12
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323f.p1", 0x000000, 0x100000, CRC(2af068c0) SHA1(82f5885fbfdef47c0e9416cf9bd9accf8e5a739d) )
	ROM_LOAD16_WORD_SWAP( "323f.p2", 0x100000, 0x800000, CRC(3ddc321d) SHA1(496573202a417d40894f69c2044ef408d3a0f01b) )

	NEO_SFIX_128K( "323e.s1", CRC(52895190) SHA1(d11385c51591b685c533a9f27c4fd3aa62927bef) )

	NEO_BIOS_AUDIO_64K( "323f.m1", CRC(5edaa24d) SHA1(90209de31a87da4118600be64869b36067d76c4b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323f.v1",   0x000000, 0x800000, CRC(70042574) SHA1(368e85d38a94238b61969ee564f94bef143b808d) )
	ROM_LOAD( "323f.v2",   0x800000, 0x800000, CRC(d84d05b2) SHA1(7ad3914b5e95572d3fc9dadc6ad0e23463983118) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323f.c1",   0x0000000, 0x1000000, CRC(360e5362) SHA1(a86a5eea49218669c5e00a6b286a7d2d6f02ab3c) )
	ROM_LOAD16_BYTE( "323f.c2",   0x0000001, 0x1000000, CRC(1fdff0ce) SHA1(4ef77f3e01d58612e9b730426adb81d87ccb5572) )
ROM_END

ROM_START( teot_7 ) // Beta 4, 2022-05-07
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323g.p1", 0x000000, 0x100000, CRC(fdb9a8d4) SHA1(7e0d43ebe7b3841948a0de311b521b3eaa1c91d7) )
	ROM_LOAD16_WORD_SWAP( "323g.p2", 0x100000, 0x800000, CRC(02dd52ed) SHA1(71a310cf48fe719726f60aa11e87e11bb217ac5c) )

	NEO_SFIX_128K( "323g.s1", CRC(6d05f74b) SHA1(1643bcf32249d3d90c230ccac09c8026dbd62960) )

	NEO_BIOS_AUDIO_64K( "323g.m1", CRC(16f81e41) SHA1(25a8f098254e3addc2040111c7c388e2285da445) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323g.v1",   0x000000, 0x800000, CRC(d2911e9c) SHA1(ca53ebda1ae2a339dfceda509c334bc4be24d7a2) )
	ROM_LOAD( "323g.v2",   0x800000, 0x800000, CRC(49e3afe6) SHA1(0c37f171035baa4392b28df5de3261f084727a47) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323g.c1",   0x0000000, 0x1000000, CRC(66f0afa6) SHA1(3ee249703bf4710d4ae7ac6b9928f81dcb679654) )
	ROM_LOAD16_BYTE( "323g.c2",   0x0000001, 0x1000000, CRC(e773037f) SHA1(ac3d07091fc8f25f01b9f21f5a0abff750af8208) )
ROM_END

ROM_START( teot_8 ) // Beta 5, 2022-05-29
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323h.p1", 0x000000, 0x100000, CRC(1cd69227) SHA1(dd2669a5f0db942b4dfddaa6448179038d1852b6) )
	ROM_LOAD16_WORD_SWAP( "323h.p2", 0x100000, 0x800000, CRC(68dc7463) SHA1(e304187c3343d6a3f4dcad05ab68a1e73434079c) )

	NEO_SFIX_128K( "323g.s1", CRC(6d05f74b) SHA1(1643bcf32249d3d90c230ccac09c8026dbd62960) )

	NEO_BIOS_AUDIO_64K( "323g.m1", CRC(16f81e41) SHA1(25a8f098254e3addc2040111c7c388e2285da445) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323g.v1",   0x000000, 0x800000, CRC(d2911e9c) SHA1(ca53ebda1ae2a339dfceda509c334bc4be24d7a2) )
	ROM_LOAD( "323g.v2",   0x800000, 0x800000, CRC(49e3afe6) SHA1(0c37f171035baa4392b28df5de3261f084727a47) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323h.c1",   0x0000000, 0x1000000, CRC(2fdbfbef) SHA1(3bc2f207538893f71a7f14800dc9f11f1788c5bf) )
	ROM_LOAD16_BYTE( "323h.c2",   0x0000001, 0x1000000, CRC(4b953a79) SHA1(4851352e7f499f3cef20415072a0df7dc0c53bed) )
ROM_END

ROM_START( teot_9 ) // Beta 6, 2022-07-06
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323i.p1", 0x000000, 0x100000, CRC(49b25c64) SHA1(91d2d27952818c40ad288af90df606a75ca7f824) )
	ROM_LOAD16_WORD_SWAP( "323h.p2", 0x100000, 0x800000, CRC(68dc7463) SHA1(e304187c3343d6a3f4dcad05ab68a1e73434079c) )

	NEO_SFIX_128K( "323g.s1", CRC(6d05f74b) SHA1(1643bcf32249d3d90c230ccac09c8026dbd62960) )

	NEO_BIOS_AUDIO_64K( "323g.m1", CRC(16f81e41) SHA1(25a8f098254e3addc2040111c7c388e2285da445) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323g.v1",   0x000000, 0x800000, CRC(d2911e9c) SHA1(ca53ebda1ae2a339dfceda509c334bc4be24d7a2) )
	ROM_LOAD( "323g.v2",   0x800000, 0x800000, CRC(49e3afe6) SHA1(0c37f171035baa4392b28df5de3261f084727a47) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323h.c1",   0x0000000, 0x1000000, CRC(2fdbfbef) SHA1(3bc2f207538893f71a7f14800dc9f11f1788c5bf) )
	ROM_LOAD16_BYTE( "323h.c2",   0x0000001, 0x1000000, CRC(4b953a79) SHA1(4851352e7f499f3cef20415072a0df7dc0c53bed) )
ROM_END

ROM_START( teot ) // Beta 7, 2022-07-31
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "323j.p1", 0x000000, 0x100000, CRC(c0ae0a56) SHA1(d5817755641c5b105c1191b3a1f0dbca64060878) )
	ROM_LOAD16_WORD_SWAP( "323h.p2", 0x100000, 0x800000, CRC(68dc7463) SHA1(e304187c3343d6a3f4dcad05ab68a1e73434079c) )

	NEO_SFIX_128K( "323g.s1", CRC(6d05f74b) SHA1(1643bcf32249d3d90c230ccac09c8026dbd62960) )

	NEO_BIOS_AUDIO_64K( "323j.m1", CRC(0c17ccac) SHA1(39b33f6e6dc96012161cdb452a162495355dd73c) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "323g.v1",   0x000000, 0x800000, CRC(d2911e9c) SHA1(ca53ebda1ae2a339dfceda509c334bc4be24d7a2) )
	ROM_LOAD( "323g.v2",   0x800000, 0x800000, CRC(49e3afe6) SHA1(0c37f171035baa4392b28df5de3261f084727a47) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "323h.c1",   0x0000000, 0x1000000, CRC(2fdbfbef) SHA1(3bc2f207538893f71a7f14800dc9f11f1788c5bf) )
	ROM_LOAD16_BYTE( "323h.c2",   0x0000001, 0x1000000, CRC(4b953a79) SHA1(4851352e7f499f3cef20415072a0df7dc0c53bed) )
ROM_END


// 334 : 19YY
ROM_START( 19yy01 ) // v1.0, first release
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "334.p1", 0x100000, 0x100000, CRC(59374c47) SHA1(dfd8f5b8edba3116998c12b77dd2583d0363b76a) )
	ROM_CONTINUE(0x000000, 0x100000)

	NEO_SFIX_128K( "334.s1", CRC(219b6f40) SHA1(a771872489c8b6d4011e412362366d30ed363e22) )

	NEO_BIOS_AUDIO_128K( "334a.m1", CRC(636d8ac8) SHA1(a9155bf34f5046bb3d6fa1d740ab328054f754d5) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "334a.v1",  0x000000, 0x400000, CRC(7bb79a6a) SHA1(4eb92dcadf8742632ba632e16cf22ead90e3a0c9) )
	ROM_LOAD( "090.v3",  0x400000, 0x200000, CRC(1908a7ce) SHA1(78f31bcfea33eb94752bbf5226c481baec1af5ac) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "334.c1",  0x000000, 0x400000, CRC(622719d5) SHA1(38c51619992bf9b6de07d3739d28e0098295cec0) )
	ROM_LOAD16_BYTE( "334.c2",  0x000001, 0x400000, CRC(41b07be5) SHA1(3f0bb6ebf2e4b396fbc72e949cff415603c0ae96) )
ROM_END

ROM_START( 19yy ) // sound fixed by elrayzeur, 2022-10-20
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "334.p1", 0x100000, 0x100000, CRC(59374c47) SHA1(dfd8f5b8edba3116998c12b77dd2583d0363b76a) )
	ROM_CONTINUE(0x000000, 0x100000)

	NEO_SFIX_128K( "334.s1", CRC(219b6f40) SHA1(a771872489c8b6d4011e412362366d30ed363e22) )

	NEO_BIOS_AUDIO_128K( "334.m1", CRC(8e05762a) SHA1(da28724c6a50ed3e9a46eed35bd16ae8d3a232db) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "334.v1",  0x000000, 0x800000, CRC(944146c2) SHA1(e81fa519cddcc9489d013937eb366ad090fb6b74) )
	ROM_LOAD( "334.v2",  0x800000, 0x800000, CRC(a4bafe45) SHA1(6936f842103897fed35fc0c325c71f3c024e9174) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "334.c1",  0x000000, 0x400000, CRC(622719d5) SHA1(38c51619992bf9b6de07d3739d28e0098295cec0) )
	ROM_LOAD16_BYTE( "334.c2",  0x000001, 0x400000, CRC(41b07be5) SHA1(3f0bb6ebf2e4b396fbc72e949cff415603c0ae96) )
ROM_END

// 335 : Cake Fighter
ROM_START( cakefght ) // v1.0
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "335.p1", 0x000000, 0x200000, CRC(717afa0f) SHA1(b0e0556b9c996e29c16fcb3416d763d454b8e8ab) )

	NEO_SFIX_128K( "335.s1", CRC(10037303) SHA1(7a31d747a3f3828f5b41c793a2bbbbefcd24d276) )

	NEO_BIOS_AUDIO_128K( "224.m1", CRC(364d6f96) SHA1(779b95a6476089b71f48c8368d9043ee1dba9032) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "335.v1",  0x000000, 0x400000, CRC(3eb19769) SHA1(5fe4e4de25e354406ebb30d5a964dd3b80f161f0) )
	ROM_LOAD( "224.v2",  0x400000, 0x200000, CRC(7ad26599) SHA1(822030037b7664795bf3d64e1452d0aecc22497e) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD( "335.c1",  0x000000, 0xa00000, CRC(8cd7c899) SHA1(c471466634e42c8df8dd343bbe9058e12c88e785) )
ROM_END

// You need to press Fire / Button A to get attract mode for a little while
// There's a strange noise after about 15 seconds of the cakes sitting there.
ROM_START( cakefght1 ) // v1.1
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "335a.p1", 0x100000, 0x100000, CRC(e9f38bee) SHA1(5ab86835a13e563f09f7533c2e60224329fd803b) )
	ROM_CONTINUE(0x000000, 0x100000)

	NEO_SFIX_128K( "335.s1", CRC(10037303) SHA1(7a31d747a3f3828f5b41c793a2bbbbefcd24d276) )

	NEO_BIOS_AUDIO_128K( "224.m1", CRC(364d6f96) SHA1(779b95a6476089b71f48c8368d9043ee1dba9032) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "335.v1",  0x000000, 0x400000, CRC(3eb19769) SHA1(5fe4e4de25e354406ebb30d5a964dd3b80f161f0) )
	ROM_LOAD( "224.v2",  0x400000, 0x200000, CRC(7ad26599) SHA1(822030037b7664795bf3d64e1452d0aecc22497e) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "335a.c1",  0x000000, 0x400000, CRC(5c56060d) SHA1(c1c42c50cbaff5f3adf5b2e471ae1c9275fa8eb8) )
	ROM_LOAD16_BYTE( "335a.c2",  0x000001, 0x400000, CRC(b51249d0) SHA1(f62b1bf432ce24a28630ce1c82bcbbeaffc15b5c) )
	ROM_LOAD16_BYTE( "335a.c3",  0x800000, 0x100000, CRC(9f634b11) SHA1(7081a0864919db38d5f34818dcac73f1e5a5c2a4) )
	ROM_LOAD16_BYTE( "335a.c4",  0x800001, 0x100000, CRC(1a120e61) SHA1(8a0233e1f61ab3195d9aaec98ccc7d39634795b0) )
ROM_END


// 340: Knights Chance by Neobitz
ROM_START( knightsch )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "340.p1", 0x000000, 0x100000, CRC(d7ac5077) SHA1(bbb72cb8eba04b7a93c1e611c50cec8cbc52eb9a) )

	NEO_SFIX_128K( "340.s1", CRC(d007e769) SHA1(c09936ace59a25c15ba9436f8514956538ed4c2d) )

	NEO_BIOS_AUDIO_128K( "340.m1", CRC(aaf76ef5) SHA1(ebfa2d30539932133dc8c9fe0255890626e145ab) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "340.v1", 0x000000, 0x800000, CRC(b8e55619) SHA1(d1e03486b9f33954e0b3f411414ff4cf764aa55f) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "340.c1", 0x000000, 0x400000, CRC(206ca233) SHA1(05490faba5b4003c1f073c94f9335e89ff14b464) )
	ROM_LOAD16_BYTE( "340.c2", 0x000001, 0x400000, CRC(782437cb) SHA1(30bf0362663a42c1069f67ee65f49d19d2c695ec) )
ROM_END


// 341 : Crouching Pony Hidden Dragon demo by Le Cortex
// If the game doesn't work, read the FAQ.
ROM_START( cphd )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "341d.p1", 0x000000, 0x100000, CRC(de032a95) SHA1(5566ddbb17c8cc4016e25de1afb0ea5c281844ae) )
	ROM_LOAD16_WORD_SWAP( "341d.p2", 0x100000, 0x800000, CRC(7a3a2e41) SHA1(a1bd9e20f29f283f892ed3d937b3a79a63d9a04b) )

	NEO_SFIX_128K( "341.s1", CRC(1736099a) SHA1(486bb9b5abd43d1739c717f0dd3aff0d6ed37ec0) )

	NEO_BIOS_AUDIO_64K( "341.m1", CRC(535a7397) SHA1(3752017175030b05a91078bdf0709af91dee5fc3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "341.v1",     0x0000000, 0x1000000, CRC(04eb9c07) SHA1(226f6d6e0365613fb66671775a1f8e5ca62d0549) )
	ROM_IGNORE(0x1000000) // empty

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "341.v2",     0x0000000, 0x1000000, CRC(9c9aec7f) SHA1(dc54b4ea34724da5dc9e5b498a2b16d7f1ffc1b4) )
	ROM_IGNORE(0x1000000) // empty

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "341.c1", 0x0000000, 0x2000000, CRC(f3a4b475) SHA1(07a5e8c3a0a8e29f952e448d8eac772ccef6586e) )
	ROM_LOAD16_BYTE( "341.c2", 0x0000001, 0x2000000, CRC(05ce93a9) SHA1(a29f7ca6d0b3c53a0b2889e716b34fe921da432c) )
	// not used (blank)
	//ROM_LOAD16_BYTE( "341.c3", 0x4000000, 0x2000000, CRC(59450445) SHA1(57b587e1bf2d09335bdac6db18902d43dfe76449) )
	//ROM_LOAD16_BYTE( "341.c4", 0x4000001, 0x2000000, CRC(59450445) SHA1(57b587e1bf2d09335bdac6db18902d43dfe76449) )
ROM_END


// 342: Xeno Crisis by Bitmap Bureau
ROM_START( xeno )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "342.p1", 0x000000, 0x100000, CRC(637605a6) SHA1(179ebcdeaac3e561fd7acb72022eda8c3c74cb8a) )
	ROM_LOAD16_WORD_SWAP( "342.p2", 0x100000, 0x100000, CRC(84838145) SHA1(c1f48d333dfc85b9371f811dd449a42d8cdecf3f) )

	NEO_SFIX_128K( "342.s1", CRC(7537ea79) SHA1(b7242a6dd7b2ad8ccf7a223c08d626abf013f366) )

	NEO_BIOS_AUDIO_64K( "342.m1", CRC(28c13ed9) SHA1(a3c8cf36906293a24f1ed49376c9d561560d2730) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "342.v1", 0x000000, 0x1000000, CRC(60d57867) SHA1(e1f3f759b4af4404f19dd5b75135e6968b6be3c5) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "342.c1", 0x000000, 0x200000, CRC(ae51ef89) SHA1(da82214263a99520364a2b7ab8140bdc68940f6d) )
	ROM_LOAD16_BYTE( "342.c2", 0x000001, 0x200000, CRC(a8610100) SHA1(aded6eaa17a518a8f4af9c3779c41ef8dd32a316) )
ROM_END


// 343: Captain Barrel by Neo Byte Force
ROM_START( cbarrel )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "343.p1", 0x000000, 0x100000, CRC(6da1737d) SHA1(49c8dafb1448bcbf706fcb5b3e713119d421d964) )
	ROM_LOAD16_WORD_SWAP( "343.p2", 0x100000, 0x100000, CRC(f6f90237) SHA1(c92b2ab39a2d01cd7c83ececa72dda0a50f429d0) )

	NEO_SFIX_128K( "343.s1", CRC(9785df9c) SHA1(e07bfa3f729a0b9ce8daff3c6bc0367fd7ce1a2c) )

	NEO_BIOS_AUDIO_64K( "343.m1", CRC(ed6260d2) SHA1(4aea38615117ea96b5e744ce49dd0578ca39a501) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "343.v1", 0x000000, 0x800000, CRC(8cfcceb7) SHA1(6657b89d0b0cf6b27ec6cd051eb73d83efa025a2) )
	ROM_LOAD( "343.v2", 0x800000, 0x800000, CRC(d1080962) SHA1(e017e5211c2701ee454ea28bba87b05ccc385c11) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "343.c1", 0x000000, 0x200000, CRC(bce671cd) SHA1(755e4091871d6db28fc3cbb43f7662b48dd2bb57) )
	ROM_LOAD16_BYTE( "343.c2", 0x000001, 0x200000, CRC(843e16ac) SHA1(4288905ef92f813b540e38702477c76dc5146312) )
ROM_END


// 403 : Poker Night by Neobitz
// No sound
ROM_START( poknight )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "403.p1", 0x000000, 0x080000, CRC(750421ee) SHA1(0a86823456a82506bc96989c361626be4b3ceeaf) )

	NEO_SFIX_128K( "403.s1", CRC(a26d2f09) SHA1(e7da6005a66e52f5479f9fe9f0cc67527dcb5d04) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "403.c1", 0x000000, 0x80000, CRC(5fb2b761) SHA1(12ec2abcd199da866519aca767818a080d9b40bf) )
	ROM_LOAD16_BYTE( "403.c2", 0x000001, 0x80000, CRC(f2b570e3) SHA1(1c8d8dc75651ec788729bfc7198c926285ad6080) )
ROM_END

ROM_START( poknightcd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "403.p1", 0x000000, 0x080000, CRC(750421ee) SHA1(0a86823456a82506bc96989c361626be4b3ceeaf) )

	NEO_SFIX_64K( "403a.s1", CRC(d6919a39) SHA1(46c858249292c1077c1e4ba5a6be469eb39ed483) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "403a.c1", 0x000000, 0x80000, CRC(0ab5143b) SHA1(73d81c09deec02174aa5528aafad28c1294e8b8e) )
	ROM_LOAD16_BYTE( "403a.c2", 0x000001, 0x80000, CRC(a7b2d3b9) SHA1(9723d53d10efdf9d1d402d4b9487ef5697262491) )
ROM_END

ROM_START( poknightfr )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "403b.p1", 0x000000, 0x020000, CRC(e0b3e776) SHA1(4b5aa783d59d37a04553671830e932a35561d447) )

	NEO_SFIX_MT(0x8000)
	ROM_LOAD( "403b.s1", 0x00000, 0x08000, CRC(3824214d) SHA1(446bf7ec6afc80a758d7d5dc3156a12cdee3b38d) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "403b.c1", 0x000000, 0x18000, CRC(0ee80e14) SHA1(4dbe8b0750ca8e479bc6936ecf15572ef5ff7462) )
	ROM_LOAD16_BYTE( "403b.c2", 0x000001, 0x18000, CRC(84f17a6d) SHA1(3a2232469274c5418e58dd48b78106d540f2c6ad) )
ROM_END


// 404 : Columns by Neobitz
// No sound
ROM_START( columnsn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "404.p1", 0x000000, 0x080000, CRC(c6c9888d) SHA1(5a2ec1071872c817bf82053ca7b7d5d63fab6755) )

	NEO_SFIX_128K( "404.s1", CRC(089a28a8) SHA1(b38b5c42eb909c57c16b016addffcd4e930806ce) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "404.c1", 0x000000, 0x80000, CRC(9a579cab) SHA1(9efd285c1bdc49a0d1daf2a062d0f906a0f5b542) )
	ROM_LOAD16_BYTE( "404.c2", 0x000001, 0x80000, CRC(91d14043) SHA1(de6a1178fbfa3d75285e4338967f0ce4ed1aff23) )
ROM_END

ROM_START( columnsncd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "404.p1", 0x000000, 0x080000, CRC(c6c9888d) SHA1(5a2ec1071872c817bf82053ca7b7d5d63fab6755) )

	NEO_SFIX_64K( "404a.s1", CRC(16cdb953) SHA1(a699f05fe9fb02a9bb0f03778578a5adcae00b91) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "404a.c1", 0x000000, 0x80000, CRC(cf503ff1) SHA1(3fb3131fa55d6137381b817dbb07b740096749b7) )
	ROM_LOAD16_BYTE( "404a.c2", 0x000001, 0x80000, CRC(c4d6e319) SHA1(19a5bb2622c01bea2c0eb2206724c7751caca318) )
ROM_END


// 407 : Neo No Panepon by Blastar, 2002
// No sound
/* There are 4 versions.
   Release dates: v1a and v1b = 2002-07-07, v1c = 2002-08-22, v2 = 2002-10-05

   MAME has a bug where the first 2 high scores do not show. (Change pixel clock to /6 to fix this)
*/
// Keeps rebooting; use Universe Bios to see opening screen, then press Space to reboot
ROM_START( neonopon_d1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "407a.p1", 0x000000, 0x080000, CRC(43b7c759) SHA1(bbb4bf7957a336be6e5daa4030a5ebb1c72d92ee) )

	NEO_SFIX_64K( "407a.s1", CRC(e28ea706) SHA1(bd113a91a1303e2ec670250225a1cd35d34c7f60) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "407a.c1", 0x000000, 0x100000, CRC(24dcba77) SHA1(da7ce6797bf4ac9903dbb00587815c51c835c879) )
	ROM_LOAD16_BYTE( "407a.c2", 0x000001, 0x100000, CRC(57c41e26) SHA1(39d810547f3f8a44b45e7807f1e6b3a0672a1688) )
ROM_END
// Keeps rebooting; use Universe Bios to see opening screen, then press Space to reboot
ROM_START( neonopon_d2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "407a.p1", 0x000000, 0x080000, CRC(43b7c759) SHA1(bbb4bf7957a336be6e5daa4030a5ebb1c72d92ee) )

	NEO_SFIX_64K( "407a.s1", CRC(e28ea706) SHA1(bd113a91a1303e2ec670250225a1cd35d34c7f60) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "407b.c1", 0x000000, 0x100000, CRC(0833c9e7) SHA1(62f4e093bd5b521ee3f64aa1744e324a3a50eb12) )
	ROM_LOAD16_BYTE( "407b.c2", 0x000001, 0x100000, CRC(fbf5f67f) SHA1(52a6a800d4afaa9675e1f6eab5ed4be772f17338) )
ROM_END
// Keeps rebooting; use Universe Bios to see opening screen, then press Space to reboot
ROM_START( neonopon_d3 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "407a.p1", 0x000000, 0x080000, CRC(43b7c759) SHA1(bbb4bf7957a336be6e5daa4030a5ebb1c72d92ee) )

	NEO_SFIX_64K( "407.s1", CRC(93e6fe7d) SHA1(791789c33eeeaa23fe6581db2f0ee3c35c73c052) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "407c.c1", 0x000000, 0x100000, CRC(e0573498) SHA1(80ec6ef8a6133f1f66704920332e92daa3daee14) )
	ROM_LOAD16_BYTE( "407b.c2", 0x000001, 0x100000, CRC(fbf5f67f) SHA1(52a6a800d4afaa9675e1f6eab5ed4be772f17338) )
ROM_END
// Works
ROM_START( neonopon ) // v.0.2
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "407.p1", 0x000000, 0x080000, CRC(8a792271) SHA1(6f8db18f9ced0706cb2e4b5545da97be871352dd) )

	NEO_SFIX_64K( "407.s1", CRC(93e6fe7d) SHA1(791789c33eeeaa23fe6581db2f0ee3c35c73c052) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "407.c1", 0x000000, 0x100000, CRC(4a718ae3) SHA1(e59515bacd9065c4b2712710a6a9a647e42c31e6) )
	ROM_LOAD16_BYTE( "407.c2", 0x000001, 0x100000, CRC(0e2cbc25) SHA1(3c45eedb2efc6a3a7ec65487f980e117485abc9f) )
ROM_END


// 408: Neo Puzzle League by Blastar, converted from CD by Robbbert, 2025-03-03.
// Game is incomplete. No sound. Some graphics not implemented.
// No audio files on the CD, so dummy ones used.
ROM_START( neopl )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "408.p1", 0x000000, 0x00e7a8, CRC(2306d129) SHA1(6cdd535b6356196f42fb6d49e2cbad0c387b63ae) )
	ROM_FILL(0x485,1,0x30) // make it run slower

	NEO_SFIX_MT(0x2000)
	ROM_LOAD( "408.s1", 0x000000, 0x002000, CRC(18fc6ad9) SHA1(e9f0dee058c9a06f2db5fac8bf24485705ae4185) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) ) // dummy m1

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASE00 )
	ROM_LOAD( "408.c1",  0x000000, 0x00c900, CRC(1d3129b4) SHA1(54ac06c4c71b1500c39d7a823b328d569e6a7c43) )
ROM_END


// 409 : Jonas Indiana and the Lost Temple of Ra by Blastar (c) 2005
// Bugs: Insert Coin causes reboot; You can climb into the ceiling and get stuck there; can't jump from one rope to another
ROM_START( ltorb )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "409.p1", 0x000000, 0x040000, CRC(353f6b05) SHA1(ae2ebb9a08e936a86cd09aaafda9e90826461bbf) )

	NEO_SFIX_64K( "409.s1", CRC(8224d8b7) SHA1(2e7b176e5dffd8a44fa813b928155801d80b1ee6) )

	NEO_BIOS_AUDIO_64K( "409.m1", CRC(6847fd52) SHA1(6e6d9045299966d41e3f193b3aff032d5b5e135d) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "409.v1", 0x000000, 0x080000, CRC(0329c2e7) SHA1(44c7f04d4e37fb5a06b0366d291f80c3a67a46f5) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "409.c1", 0x000000, 0x080000, CRC(8b607198) SHA1(23dc0b285518f3e80a8feab4d674d625e76e8896) )
	ROM_LOAD16_BYTE( "409.c2", 0x000001, 0x080000, CRC(6b96b59c) SHA1(a62600c337d7f6821ce56bdfd5c484f05c88187f) )
ROM_END


// 410 : Codename: Blut Engel by Blastar (c) 2005
// Bugs: Insert Coin causes reboot
ROM_START( cnbe )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "410.p1", 0x000000, 0x080000, CRC(7ece4186) SHA1(fc4ed3b5ed551744542c2ff1e45a6e97c68eff88) )

	NEO_SFIX_64K( "410.s1", CRC(40a4698f) SHA1(be631408b261906f36c41c58c775c1a4c418a256) )

	NEO_BIOS_AUDIO_128K( "410.m1", CRC(a5821c9c) SHA1(81779f12bbb012bf910c484725779e03b07e44ec) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "410.v1", 0x000000, 0x080000, CRC(70191764) SHA1(7dff2122c32886a3626c0d7befc7e7c3961c6cee) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "410.c1", 0x000000, 0x100000, CRC(0f4fea6f) SHA1(2eeaa661272817dc205072cd63d8bffbe7d6fbec) )
	ROM_LOAD16_BYTE( "410.c2", 0x000001, 0x100000, CRC(3d5fc36f) SHA1(59c045bc5999ccd6c1413364a6cd337a858f599e) )
ROM_END

// 410 : Codename: Blut Engel by Blastar (c) 2018
// Bugs: Insert Coin causes reboot
// Seems original didn't work correctly on MVS, this fixes it. Makes no difference in emulation.
ROM_START( cnbe2018 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "410a.p1", 0x000000, 0x40000, CRC(189fecd5) SHA1(d78ef1b3c6af9e75618dabd52fec9403f5007c36) )

	NEO_SFIX_64K( "410a.s1", CRC(ec227c62) SHA1(07a2b5daf8a6369d4b604293ad7b0368455c386b) )

	NEO_BIOS_AUDIO_64K( "410a.m1", CRC(b94f41b4) SHA1(2d6d8d6e6fdae35efedc0732e790448df4e07e9a) )

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "410a.v1", 0x000000, 0x40000, CRC(54947995) SHA1(30616f95020a06d95a77a69b07fd195c86575307) )

	ROM_REGION( 0x80000, "sprites", 0 )
	ROM_LOAD16_BYTE( "410a.c1", 0x000000, 0x40000, CRC(db0e9083) SHA1(e0120e19fe0c04a4452fdc7bd3a159f116cbc412) )
	ROM_LOAD16_BYTE( "410a.c2", 0x000001, 0x40000, CRC(6114b6fb) SHA1(d62bc4a49c6ba2a03d637a5d06a4140e7bdc78b2) )
ROM_END

#if 0
// This uses a .neo file: 0x1000 bytes for header, then p rom (word_swap), then remainder is normal
ROM_START( cnbe2018 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "410.neo", 0x000000, 0x001000, CRC(d2c40244) SHA1(a3888dd1ff9168339afa58f7823c4944043678e0) ) // skip header
	ROM_CONTINUE(0x000000, 0x040000)
	ROM_IGNORE(0x0e0000)

	ROM_REGION(0x120000, "asis", 0)
	ROM_LOAD("410.neo", 0x000000, 0x001000, CRC(d2c40244) SHA1(a3888dd1ff9168339afa58f7823c4944043678e0) )
	ROM_CONTINUE(0x000000, 0x120000)

	NEO_SFIX_MT(0x10000)
	ROM_COPY("asis", 0x40000, 0x00000, 0x10000) // srctag, srcoffs, dstoffs, length

	NEO_BIOS_AUDIO_128K( "410.m1", CRC(a5821c9c) SHA1(81779f12bbb012bf910c484725779e03b07e44ec) ) // dummy, gets overwritten
	ROM_COPY("asis", 0x50000, 0x00000, 0x10000)

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_COPY("asis", 0x60000, 0x00000, 0x40000)

	ROM_REGION( 0x80000, "sprites", 0 )
	ROM_COPY("asis", 0xa0000, 0x00000, 0x80000)
ROM_END
#endif


// 411 : NGEM2K by Blastar (c) 2006
// No sound
// Bugs: Insert Coin causes reboot
ROM_START( ngem2k )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "411.p1", 0x000000, 0x080000, CRC(f2c42fe3) SHA1(fcd7453019ad21ecf77999a60b6792fc8d72d409) )

	NEO_SFIX_64K( "411.s1", CRC(fb3c02d2) SHA1(961cf3cfd56e9ace059265f0ff4c283289972938) )

	NEO_BIOS_AUDIO_128K( "411.m1", CRC(b1f44a15) SHA1(a1d7b498430b713b15ce886bac2ea473adadfb0c) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "411.v1", 0x000000, 0x080000, CRC(c32199f0) SHA1(2e17a6736c27d703c64d58fdc1b43edd8bb36d35) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "411.c1", 0x000000, 0x100000, CRC(a8e2ab47) SHA1(69a6ee8f659a63600a48e31b10782fba263e32e2) )
	ROM_LOAD16_BYTE( "411.c2", 0x000001, 0x100000, CRC(a738ea1c) SHA1(3b71f43ff30f4b15b5cd85dd9e95ebc7e84eb5a3) ) // all zeroes
ROM_END


// 412 : Jump n Run Tech Demo by Blastar 2020-11
ROM_START( jumpnrund )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "412.p1", 0x000000, 0x080000, CRC(924f0e82) SHA1(6934e1f1ed5e54b25bff139fa57679958b0a0e4f) )

	NEO_SFIX_128K( "412.s1", CRC(409a9f52) SHA1(23874ebee8698b240c18041eacad875237a5c585) )

	NEO_BIOS_AUDIO_64K( "412.m1", CRC(30094ef9) SHA1(1f2b2caa7155e8b4bc9c90641d664c9d1fe2061b) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "412.v11",  0x000000, 0x080000, CRC(01a1a847) SHA1(6f82532101e19d9a60a495d562b4ef0836b10124) )

	ROM_REGION( 0x100000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "412.v21",  0x000000, 0x080000, CRC(398c3d56) SHA1(f9724345908abf2abdfd1e92e29ac7307ccc46e9) )
	ROM_LOAD( "412.v22",  0x080000, 0x080000, CRC(47aa9572) SHA1(de02801e619537ad336361eda29ee40818de6a5c) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "412.c1", 0x000000, 0x80000, CRC(0164735f) SHA1(e5049f7bc39c6dbe886b29dac59850a333a4b58b) )
	ROM_LOAD16_BYTE( "412.c2", 0x000001, 0x80000, CRC(38f8b27e) SHA1(d621220bacb0d8da78acb83f10bc56bf6b988db6) )
ROM_END

ROM_START( jumpnrund01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "412a.p1", 0x000000, 0x080000, CRC(e2d0dcf3) SHA1(51864e7506f8364ec52da92cd18aef292f8aba94) )

	NEO_SFIX_128K( "412a.s1", CRC(acc6a2a3) SHA1(77fc4397b838a7d49e9fce8e94151bc62256c9fe) )

	NEO_BIOS_AUDIO_64K( "412.m1", CRC(30094ef9) SHA1(1f2b2caa7155e8b4bc9c90641d664c9d1fe2061b) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "412.v11",  0x000000, 0x080000, CRC(01a1a847) SHA1(6f82532101e19d9a60a495d562b4ef0836b10124) )

	ROM_REGION( 0x100000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "412.v21",  0x000000, 0x080000, CRC(398c3d56) SHA1(f9724345908abf2abdfd1e92e29ac7307ccc46e9) )
	ROM_LOAD( "412.v22",  0x080000, 0x080000, CRC(47aa9572) SHA1(de02801e619537ad336361eda29ee40818de6a5c) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "412.c1", 0x000000, 0x80000, CRC(0164735f) SHA1(e5049f7bc39c6dbe886b29dac59850a333a4b58b) )
	ROM_LOAD16_BYTE( "412.c2", 0x000001, 0x80000, CRC(38f8b27e) SHA1(d621220bacb0d8da78acb83f10bc56bf6b988db6) )
ROM_END


// 413 : Looptris by Blastar 2021-12-26
ROM_START( looptris )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "413.p1", 0x000000, 0x080000, CRC(8fcb5104) SHA1(6a0da5c4e72ef57600ecd15cba64e3871f48391e) )

	NEO_SFIX_128K( "413.s1", CRC(70e70448) SHA1(40461af534afcba341d2f14b755d6de353fe7afd) )

	NEO_BIOS_AUDIO_128K( "413.m1", CRC(e7105df8) SHA1(9f4efad6dcc8b9617d59576415019820e973f45c) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "413.v1", 0x000000, 0x080000, CRC(dfa63cd2) SHA1(bbf4a74a660f12cc037cfa0ac5c664988016ee45) )

	ROM_REGION( 0x100000, "sprites", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "413.c1", 0x000000, 0x080000, CRC(b9413f13) SHA1(5e7175dff9510dad55f8f05e9b307c3c05d92209) )
	ROM_LOAD16_BYTE( "413.c2", 0x000001, 0x080000, CRC(9409dbe8) SHA1(1e9562aa34f6202e8012104ab79916453fec072e) )
ROM_END


// 414 : Looptris Plus by Blastar 2022-12-24
ROM_START( looptrsp )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "414.p1", 0x000000, 0x080000, CRC(894bb290) SHA1(f33ed9fe75ca7b5a6da00fe5bff76e26cba85e08) )

	NEO_SFIX_128K( "413.s1", CRC(70e70448) SHA1(40461af534afcba341d2f14b755d6de353fe7afd) )

	NEO_BIOS_AUDIO_128K( "414.m1", CRC(249bba11) SHA1(e8248ff9bc391b1163746c256b137928a9d81fe0) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "414.v1", 0x000000, 0x080000, CRC(c9f86637) SHA1(edb712d5cce180e1d5b6e338e9abfab1125c5581) )
	ROM_LOAD( "414.v2", 0x080000, 0x080000, CRC(41b3e17a) SHA1(ef216379dbdbbea04cfffa47123436213a8cf26e) )
	ROM_LOAD( "414.v3", 0x100000, 0x080000, CRC(1ed4e538) SHA1(921913be499194236535052ed3140fb7169d16e9) )
	ROM_LOAD( "414.v4", 0x180000, 0x080000, CRC(705e7065) SHA1(a5eea38afa58db69c0f17e0379da3d819639971f) )

	ROM_REGION( 0x100000, "sprites", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "413.c1", 0x000000, 0x080000, CRC(b9413f13) SHA1(5e7175dff9510dad55f8f05e9b307c3c05d92209) )
	ROM_LOAD16_BYTE( "413.c2", 0x000001, 0x080000, CRC(9409dbe8) SHA1(1e9562aa34f6202e8012104ab79916453fec072e) )
ROM_END


// 415 : Flappy Chicken by Blastar 2023-04-30
ROM_START( flapchck )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "415.p1", 0x000000, 0x080000, CRC(2a7454a1) SHA1(6351645c6afdc8b1392e05c6f508217dd7af5bb2) )

	NEO_SFIX_128K( "415.s1", CRC(3fd2b4d4) SHA1(98e6ae2c5cb6ca6b0022b7335cddc83496c4b69d) )

	NEO_BIOS_AUDIO_128K( "415.m1", CRC(5abc1bf6) SHA1(150fe7580e6f9e974a1c9004862c2c50c0c67d7a) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "415.v1",  0x000000, 0x080000, CRC(c5d09e58) SHA1(6536a18a2df3eec251478ca0dd622d64accb9e40) )
	ROM_LOAD( "415.v2",  0x080000, 0x080000, CRC(b9afe241) SHA1(f6224822730b667a38643627160278bb84eb695f) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "415.c1", 0x000000, 0x80000, CRC(4ecdb8ca) SHA1(fb0ae76f1343f92a111763be267cec56c013972e) )
	ROM_LOAD16_BYTE( "415.c2", 0x000001, 0x80000, CRC(47d06927) SHA1(bf9ada131768c23f816a9c367596782e5bedc739) )
ROM_END


// 416 : Block Panic DX 250101 by Blastar
ROM_START( bpanicdx )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "416.p1", 0x000000, 0x080000, CRC(8f59134f) SHA1(ddee11eec6f53b8b6375185c341bc11d39706921) )

	NEO_SFIX_128K( "416.s1", CRC(387a8f43) SHA1(4d9cb4f2ba9d716a5d21c9644eaaf5d158ebf090) )

	NEO_BIOS_AUDIO_128K( "416.m1", CRC(d74159c0) SHA1(00730329032267e60006710f948067d4c744912c) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "416.v1",  0x000000, 0x080000, CRC(55185687) SHA1(437e042400fee20770db1975b800784c267920f2) )
	ROM_LOAD( "416.v2",  0x080000, 0x080000, CRC(7211595d) SHA1(f4c8d0bfe16587740bd389ee98cfdfa8d5e65be4) )
	ROM_LOAD( "416.v3",  0x100000, 0x080000, CRC(e08afa9f) SHA1(8aa9749d1a6bc592cca5c697043af9597abd59f4) )
	ROM_LOAD( "416.v4",  0x180000, 0x080000, CRC(5a9d8d69) SHA1(f7e280e68c7eb32bf472892154466955f3f5d389) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "416.c1", 0x000000, 0x80000, CRC(9463af81) SHA1(50962cc75bb86283186a6cf5e8cb7115ed805468) )
	ROM_LOAD16_BYTE( "416.c2", 0x000001, 0x80000, CRC(a290a403) SHA1(0b56485b00b10b94e21b9c08f93f01042ffc5e1d) )
ROM_END


// 417 : World Racer by Blastar
ROM_START( wrldracr ) // 2025-11-20
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "417.p1", 0x000000, 0x100000, CRC(cc59276f) SHA1(3e12619b00996816025e0cba5c1d6e141fe95d1f) )

	NEO_SFIX_128K( "417.s1", CRC(312508af) SHA1(4370a16fc089dd01ff00d3096a9a97cb1ad6359a) )

	NEO_BIOS_AUDIO_64K( "417.m1", CRC(6178075b) SHA1(1cfb82931b74868460f864fc2756dfded68d36b7) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "417.v1", 0x000000, 0x80000, CRC(a4763f5d) SHA1(bf518f70c4248b4934163bc44cc1c94ff445dcf0) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "417.c1", 0x000000, 0x80000, CRC(44423f83) SHA1(cd6d7a9bb15033741c570d59e4788a3917f2a1ca) )
	ROM_LOAD16_BYTE( "417.c2", 0x000001, 0x80000, CRC(57008242) SHA1(24616e9a40063eb45749127cc58e6f1e46760939) )
ROM_END

ROM_START( wrldracr01 ) // 2025-12-14
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "417s01.p1", 0x000000, 0x100000, CRC(d9f8921c) SHA1(52d6057a3a550bbec533b44db0228de93d1304e3) )

	NEO_SFIX_128K( "417.s1", CRC(312508af) SHA1(4370a16fc089dd01ff00d3096a9a97cb1ad6359a) )

	NEO_BIOS_AUDIO_64K( "417.m1", CRC(6178075b) SHA1(1cfb82931b74868460f864fc2756dfded68d36b7) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "417.v1", 0x000000, 0x80000, CRC(a4763f5d) SHA1(bf518f70c4248b4934163bc44cc1c94ff445dcf0) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "417s01.c1", 0x000000, 0x80000, CRC(ec05e878) SHA1(2874add101c3131694d657a5c905466b01acac3c) )
	ROM_LOAD16_BYTE( "417s01.c2", 0x000001, 0x80000, CRC(ff4755b9) SHA1(3199e67ab5cd69fd71ec4926f0da83eddff65f7a) )
ROM_END

ROM_START( wrldracr02 ) // 2025-12-26
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "417s02.p1", 0x000000, 0x100000, CRC(3350fa1b) SHA1(1e5f4f3713797e0aeb4daaf0b337f9c61ef5e2e8) )

	NEO_SFIX_128K( "417s02.s1", CRC(2290a5b3) SHA1(2684864f48be7772a80a3e34d00aa85f52f428f3) )

	NEO_BIOS_AUDIO_64K( "417s02.m1", CRC(f10683ac) SHA1(bf607cbc6eaaab9924b035b7f64fe5cd4e8aa97d) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "417.v1", 0x000000, 0x80000, CRC(a4763f5d) SHA1(bf518f70c4248b4934163bc44cc1c94ff445dcf0) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "417s02.c1", 0x000000, 0x80000, CRC(51858e96) SHA1(2404e3369f5fabe480b55bf86d117feda779ec21) )
	ROM_LOAD16_BYTE( "417s02.c2", 0x000001, 0x80000, CRC(43dc168f) SHA1(5cd34c935a36b8c544f0337ea998ca5612299d6e) )
ROM_END


// 418 : Tetris by Crim
ROM_START( ngtetris )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "418.p1", 0x000000, 0x80000, CRC(3465569a) SHA1(51506d549fa048581321fc39403c0a3bf8f77467) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "418.m1", CRC(6b3703c6) SHA1(6fa41b713f03e0957f410a387aa896e7d924acb2) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "418.v1", 0x000000, 0x80000, CRC(2be8e290) SHA1(198cff1b1d70f7d05344d6ab38a4354f993dc4b8) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "418.c1", 0x000000, 0x80000, CRC(acc6d1d4) SHA1(1f6ad8578d8edaa22a35ad58dc76de8543a96db9) )
	ROM_LOAD16_BYTE( "418.c2", 0x000001, 0x80000, CRC(7ec06ab5) SHA1(3a8503be7f14d451f5ae964888b385334c12e8c6) )
ROM_END


// 419 : Neo Thunder demo by Sebastian Mihai
// No sound
ROM_START( neothund )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "419.p1", 0x000000, 0x100000, CRC(f7050757) SHA1(b73009050905ad2b1f06690e0106e59fb5ede86b) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "419.c1", 0x000000, 0x100000, CRC(48b9126e) SHA1(38bf1296c3d8762372d4eab785f34507a4f8776c) )
	ROM_LOAD16_BYTE( "419.c2", 0x000001, 0x100000, CRC(214df62e) SHA1(21e4ce2a0ac636081c9e07a294cb44987692039e) )
ROM_END


// 420: Xevious by tcdev (beta 1)
ROM_START( xeviousn )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "420.p1", 0x000000, 0x100000, CRC(11dff034) SHA1(ac8ebbdc96f4d2f74d1c820c1536ca1ccdc936b6) )

	NEO_SFIX_128K( "420.s1", CRC(f41d121a) SHA1(ad30e7fde4c7e7ad7e4c32d14bf1df3b3089eaa8) )

	NEO_BIOS_AUDIO_64K( "420.m1", CRC(6f31c601) SHA1(9b89e7d205ee497f7b495e93328cf35eab6e56b2) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "420.v1", 0x000000, 0x100000, CRC(37ef6dc5) SHA1(3d50197e22d8727a8395564b8645ed1e0e2a5d26) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "420.c1", 0x000000, 0x100000, CRC(f963e5b6) SHA1(e80dd61476028de3ff8369e14b6fb1bb7561e370) )
	ROM_LOAD16_BYTE( "420.c2", 0x000001, 0x100000, CRC(c97c4e64) SHA1(d33f26dbc569c69bdbf95cdde908d7700843bde7) )
ROM_END


// 420: Zetsu Xevious by Zeroco (hack of xeviousn)
// Year on title screen says 2035, wrong obviously.
ROM_START( xeviousn01 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "420s01.p1", 0x000000, 0x100000, CRC(31092abe) SHA1(4615e13cc560b25716792ca9434c39207f67e10a) )

	NEO_SFIX_128K( "420.s1", CRC(f41d121a) SHA1(ad30e7fde4c7e7ad7e4c32d14bf1df3b3089eaa8) )

	NEO_BIOS_AUDIO_64K( "420.m1", CRC(6f31c601) SHA1(9b89e7d205ee497f7b495e93328cf35eab6e56b2) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "420.v1", 0x000000, 0x100000, CRC(37ef6dc5) SHA1(3d50197e22d8727a8395564b8645ed1e0e2a5d26) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "420s01.c1", 0x000000, 0x100000, CRC(52ff5807) SHA1(510ec3184ccec0f8d749d1c20918fddaefb00d2e) )
	ROM_LOAD16_BYTE( "420s01.c2", 0x000001, 0x100000, CRC(160d4ca2) SHA1(ac893c1094a2db993e08f846f915fb8993452ab4) )
ROM_END

ROM_START( xeviousn02 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "420s02.p1", 0x000000, 0x100000, CRC(19025936) SHA1(765a41c5385efb2b3ef1a24bb376b4dd066fc2f6) )

	NEO_SFIX_128K( "420.s1", CRC(f41d121a) SHA1(ad30e7fde4c7e7ad7e4c32d14bf1df3b3089eaa8) )

	NEO_BIOS_AUDIO_64K( "420.m1", CRC(6f31c601) SHA1(9b89e7d205ee497f7b495e93328cf35eab6e56b2) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "420.v1", 0x000000, 0x100000, CRC(37ef6dc5) SHA1(3d50197e22d8727a8395564b8645ed1e0e2a5d26) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "420s02.c1", 0x000000, 0x100000, CRC(4ce910ee) SHA1(363f054a913561bca5eb2058446bd614153c3eca) )
	ROM_LOAD16_BYTE( "420s02.c2", 0x000001, 0x100000, CRC(d8f980cf) SHA1(c445b905f2a469ed30943588df9d2f627b0a2110) )
ROM_END

ROM_START( xeviousn03 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "420s03.p1", 0x000000, 0x100000, CRC(8a1e9214) SHA1(6785934e08679245b40f9a88bee1eb038cfa25de) )

	NEO_SFIX_128K( "420.s1", CRC(f41d121a) SHA1(ad30e7fde4c7e7ad7e4c32d14bf1df3b3089eaa8) )

	NEO_BIOS_AUDIO_64K( "420s03.m1", CRC(06773aff) SHA1(79f810790366edb38a177ff2df27402bce8058c5) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "420s03.v1", 0x000000, 0x100000, CRC(929b34ab) SHA1(f3bb51036e51621d40c2d7c3f763dcf6b5d0fea3) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "420s03.c1", 0x000000, 0x100000, CRC(8759341e) SHA1(9f557ffed34cc6832f726261cf32ba71cb9938d1) )
	ROM_LOAD16_BYTE( "420s03.c2", 0x000001, 0x100000, CRC(f1c31214) SHA1(b31beea8380029b95b9339489f471f4e56d97d28) )
ROM_END

ROM_START( xeviousn04 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "420s04.p1", 0x000000, 0x100000, CRC(dfa5538b) SHA1(547eb0ccbfa0eb660437372491387f135f18f7e3) )

	NEO_SFIX_128K( "420.s1", CRC(f41d121a) SHA1(ad30e7fde4c7e7ad7e4c32d14bf1df3b3089eaa8) )

	NEO_BIOS_AUDIO_64K( "420s04.m1", CRC(06773aff) SHA1(79f810790366edb38a177ff2df27402bce8058c5) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "420s04.v1", 0x000000, 0x100000, CRC(929b34ab) SHA1(f3bb51036e51621d40c2d7c3f763dcf6b5d0fea3) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "420s04.c1", 0x000000, 0x100000, CRC(8759341e) SHA1(9f557ffed34cc6832f726261cf32ba71cb9938d1) )
	ROM_LOAD16_BYTE( "420s04.c2", 0x000001, 0x100000, CRC(f1c31214) SHA1(b31beea8380029b95b9339489f471f4e56d97d28) )
ROM_END


// 422: Galaxians by tcdev (beta 1)
// Bugs: Invisible aliens and bullets just come from nowhere; background sound missing
ROM_START( galaxiann )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "422.p1", 0x000000, 0x100000, CRC(e1d6f03b) SHA1(e1def1fdf659fcb0d8ce7bd4e939161d6dee4cc2) )

	NEO_SFIX_128K( "422.s1", CRC(05192811) SHA1(609ac057b3fcdf69207870842d26321013d47878) )

	NEO_BIOS_AUDIO_64K( "422.m1", CRC(43f557ca) SHA1(5385714c04bb4030587a1d660eab4800ce722d9d) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "422.v1", 0x000000, 0x80000, CRC(67d1fe70) SHA1(d38f2abc92854b3cbad3fd2d0990a87ceff1d826) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "422.c1", 0x000000, 0x100000, CRC(34283c88) SHA1(cce783772e1fd33bf773bf180a48f58f107f9700) )
	ROM_LOAD16_BYTE( "422.c2", 0x000001, 0x100000, CRC(31f090aa) SHA1(7e8173740b886787b735277e21306bb99d5adbe0) )
ROM_END


// 422: Galaxians by tcdev (beta 2)
// Bugs: Invisible aliens and bullets just come from nowhere; background sound missing
ROM_START( galaxiann2 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "422a.p1", 0x000000, 0x100000, CRC(68f8097c) SHA1(7648cf481634743bd97f5e983790ceb68c45c96b) )

	NEO_SFIX_128K( "422.s1", CRC(05192811) SHA1(609ac057b3fcdf69207870842d26321013d47878) )

	NEO_BIOS_AUDIO_64K( "422.m1", CRC(43f557ca) SHA1(5385714c04bb4030587a1d660eab4800ce722d9d) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "422.v1", 0x000000, 0x80000, CRC(67d1fe70) SHA1(d38f2abc92854b3cbad3fd2d0990a87ceff1d826) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "422a.c1", 0x000000, 0x100000, CRC(f2f2187a) SHA1(09c3f7c54bb9ef17ffb9d19c1638d8d63622e477) )
	ROM_LOAD16_BYTE( "422a.c2", 0x000001, 0x100000, CRC(70c8f4d2) SHA1(e77c8eca95d390f2866f24fbe411bbdca3458902) )
ROM_END


// 430 : Time's UP! by NGF
ROM_START( timesup )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "430.p1", 0x000000, 0x100000, CRC(b4be3ede) SHA1(95a9b87e674cc3af0932f5dab51d1e8e3d84385b) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "430.m1", CRC(8b83308b) SHA1(b6b48689a0138af35375f741620e2569125a93e8) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "430.v1", 0x000000, 0x200000, CRC(1b48708b) SHA1(ac4e363a0b5988496272092ea4dc7c2259f9f1e3) )
	ROM_LOAD( "430.v2", 0x200000, 0x200000, CRC(6b17df84) SHA1(b8233110f91ef408f68beda641da01d2896109da) )
	ROM_LOAD( "430.v3", 0x400000, 0x200000, CRC(6c798d46) SHA1(7f91f5dbe58a2bceb8a9ed6394eb6b11a9ae7d97) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "430.c1", 0x000000, 0x200000, CRC(1c83fc38) SHA1(74a6c9116443a673ff49b2fe4d7b8bc79eadec0c) )
	ROM_LOAD16_BYTE( "430.c2", 0x000001, 0x200000, CRC(2b3f48b4) SHA1(5d05d312d06e387fa532f71f5144347a0dd77bb3) )
ROM_END

// 430 : Time's UP! demo by NGF
ROM_START( timesupd )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "430d.p1", 0x000000, 0x200000, CRC(be86adb1) SHA1(4b363bab51dfc43b2302956f79a0b1d5202339ac) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "430d.m1", CRC(fe795d11) SHA1(53ba44f2197aa700229f18dee6513bc0c2619904) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "430d.v1", 0x000000, 0x400000, CRC(13b8f47b) SHA1(4f0384101f9f6fce8c86a27a11a2b9d43a84a154) )
	ROM_LOAD( "430d.v2", 0x400000, 0x400000, CRC(6fdd663d) SHA1(9667f11b4350285d0722c67052ff4e9a63a3409f) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "430d.c1", 0x000000, 0x200000, CRC(c19a300a) SHA1(3ab4ec4b10583257b92f5a989434db89f8130626) )
	ROM_LOAD16_BYTE( "430d.c2", 0x000001, 0x200000, CRC(fdb3f7ed) SHA1(dd1d69515e58e9fba8cefb1c4cd808dfea0475d9) )
ROM_END


// 431 : Frog Feast demo by Rastersoft
// No sound
ROM_START( ffeastd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "431d.p1", 0x000000, 0x80000, CRC(db0417ec) SHA1(dff85b47749142b3e965b2a9b5b8857096e58464) )

	NEO_SFIX_64K( "431d.s1", CRC(29814a2c) SHA1(a7313fa1280a2a2340b1c8a0d522354d82496a68) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "431d.c1", 0x000000, 0x100000, CRC(22a65cea) SHA1(3fb8cb92cd8d40c2b24e2b54833fde4c7dc255a4) )
	ROM_LOAD16_BYTE( "431d.c2", 0x000001, 0x100000, CRC(63ddd657) SHA1(41fb6fdd66fcdf5fce42eb01c8a3e00e632f6347) )
ROM_END


// 431 : Frog Feast by Rastersoft
ROM_START( ffeast ) /* Frog Feast - Full Version */
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "431.p1", 0x000000, 0x080000, CRC(c26848da) SHA1(33737f8d2366b3a554ceb9ea2fcd85aaed75f344) )

	NEO_SFIX_128K( "431.s1", CRC(88f4cd13) SHA1(7f3b28c903ecf90478f2dcddf790714d7363aa2b) )

	NEO_BIOS_AUDIO_128K( "431.m1", CRC(c825b484) SHA1(7dea872ae39ef83981436aaf3c9b602ac8bec676) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "431.v1", 0x000000, 0x100000, CRC(2639da11) SHA1(4309e46014d623066d427ce3473542c99f678f86) )
	ROM_LOAD( "431.v2", 0x100000, 0x100000, CRC(e5eda58b) SHA1(ae203cfedf4b563da914939e0a4d7f4697070f82) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "431.c1", 0x000000, 0x100000, CRC(7b2f9fb3) SHA1(62deacf16c3b259422f962c9604b4efc2f32d91b) )
	ROM_LOAD16_BYTE( "431.c2", 0x000001, 0x100000, CRC(a7f18a6f) SHA1(1a277c79eaacf09d12a4b7c88421e7a6d6153503) )
ROM_END


// 436 : Neo Pang by CeL
ROM_START( neopang )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "436.p1", 0x000000, 0x080000, CRC(dc5922ee) SHA1(08804fa9894b7e84375300acbdec8ea6829575d0) )

	NEO_SFIX_64K( "436.s1", CRC(d78bd9a0) SHA1(17a237166aede98bee27d5b52654414c8a1b071a) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "202.v1", 0x000000, 0x80000, CRC(debeb8fb) SHA1(49a3d3578c087f1a0050168571ef8d1b08c5dc05) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "436.c1", 0x000000, 0x100000, CRC(62f5405c) SHA1(f9eb8d1838da9953aecf40c3314c608ab655cb8f) )
	ROM_LOAD16_BYTE( "436.c2", 0x000001, 0x100000, CRC(e1183030) SHA1(60262d53ab12314db5e5c00a94463500abe3e8b0) )
ROM_END


// 443 : Santaball by M.Priewe.
// Press 1 to play a game of Pong.
// If the game misbehaves, read the FAQ.
ROM_START( santabll )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "443.p1", 0x000000, 0x100000, CRC(2af7a77c) SHA1(ff79d4695e8060f5a7b981524303336a0ad99c0c) )

	NEO_SFIX_128K( "443.s1", CRC(ac031928) SHA1(7516593efc70e6effed8f5423e9736d6555b3baf) )

	NEO_BIOS_AUDIO_64K( "443.m1", CRC(37a8d679) SHA1(957de2aa1775502a51b4e2d5cb6268cee00f6c2a) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "443.v1", 0x000000, 0x100000, CRC(d0150804) SHA1(b3d4cdb6d0f426de3801eb04d56f867bec8fe611) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "443.c1", 0x000000, 0x100000, CRC(1e271a9b) SHA1(5cae43aa094d5ee34983bc70aeab06652a6b0e7f) )
	ROM_LOAD16_BYTE( "443.c2", 0x000001, 0x100000, CRC(1c7fd396) SHA1(aa2e2fff1ad1804631f3058b9666b970a368a847) )
ROM_END


// 444 : Sea Fighter by kl3mousse.
ROM_START( seafight )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "444.p1", 0x000000, 0x100000, CRC(598d14ef) SHA1(da6dda92ec8cf3a5f46fae2dd4578f945f7a8ec7) )

	NEO_SFIX_128K( "444.s1", CRC(c46cf6b1) SHA1(6785e9dad6dbe339f7d8ab0d3852fb6df036c737) )

	NEO_BIOS_AUDIO_64K( "444.m1", CRC(16adbe82) SHA1(88c8fbe61735c41adffb0598e6311547a739f4d5) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "444.v1", 0x000000, 0x100000, CRC(becc781e) SHA1(a5a3ed13de539ce55c4f35bb052ee35ffe29fb5d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "444.c1", 0x000000, 0x100000, CRC(383a9dde) SHA1(6d54c6389f9f992c1654643a537c01fda9b83259) )
	ROM_LOAD16_BYTE( "444.c2", 0x000001, 0x100000, CRC(1f43c1f4) SHA1(aa0cd4eebb02225b939f57cee4347c0a43654d31) )
ROM_END


// 446 : Monitor Test by M.Priewe.
// No sound.
ROM_START( ngmontst ) // v1.0
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "446.p1", 0x000000, 0x100000, CRC(17a4ad61) SHA1(38c0f2022947043f30a27ccda06cc709fe980fed) )

	NEO_SFIX_128K( "sfix.sfix", CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) )

	NEO_BIOS_AUDIO_128K( "052.m1", CRC(49f17d2d) SHA1(70971fcf71ae3a6b2e26e7ade8063941fb178ae5) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "052.v1", 0x000000, 0x200000, CRC(22c097a5) SHA1(328c4e6db0a026f54a633cff1443a3f964a8daea) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "446.c1", 0x000000, 0x100000, CRC(715df715) SHA1(74534eb9bb48a04f25d0044b1f2b57e59f49b06f) )
	ROM_LOAD16_BYTE( "446.c2", 0x000001, 0x100000, CRC(d8ee3c9b) SHA1(c731eaffae34b4394e290e68ca8c3700e1c6dc2b) )
ROM_END
ROM_START( ngmontst3 ) // v1.3
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "446c.p1", 0x000000, 0x100000, CRC(8a140722) SHA1(a7e4ac8b7bf531a10b68460e37e7e6da59ecccfe) )

	NEO_SFIX_128K( "447.s1", CRC(bb82ab71) SHA1(307f420446c2d411a65b59543b61c21d0360536b) )

	NEO_BIOS_AUDIO_64K( "446c.m1", CRC(6f41ac95) SHA1(92dd52bd6717e116301f965a45aa1762e797154c) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "446c.v1", 0x000000, 0x200000, CRC(0f88bd1a) SHA1(75a288744533e2546fb94dddf50da0148f158d19) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "446c.c1", 0x000000, 0x100000, CRC(5c63f88a) SHA1(418e3345c40fc1d8d964ee55ca68a56be32f4df2) )
	ROM_LOAD16_BYTE( "446c.c2", 0x000001, 0x100000, CRC(3c7e0729) SHA1(695d36002cc428dfaf6174bdcf5fed351df39b35) )
ROM_END

// 447 : Hypernoid by M.Priewe.
ROM_START( hypernoid )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "447.p1", 0x000000, 0x100000, CRC(e024fa76) SHA1(6ef393ad80ec80e30929f07c95305d97fca3ad22) )

	NEO_SFIX_128K( "447.s1", CRC(bb82ab71) SHA1(307f420446c2d411a65b59543b61c21d0360536b) )

	NEO_BIOS_AUDIO_512K( "447.m1", CRC(6c8eaacc) SHA1(6bc065be18db6830a4c94b424f9380d38da6f8b7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "447.v1", 0x000000, 0x400000, CRC(dafa1bdd) SHA1(743a5ad24a3dba04ce8f3e2d95ebd5ca83fd9f98) )
	ROM_LOAD( "447.v2", 0x400000, 0x400000, CRC(85ad8283) SHA1(8abcf48a353dcd4777608b13471608f7290d89a5) )
	ROM_LOAD( "447.v3", 0x800000, 0x400000, CRC(86c27f0c) SHA1(85b740e0224d5ea09b600683dec6fe532a723355) )
	ROM_LOAD( "447.v4", 0xc00000, 0x400000, CRC(a3982244) SHA1(7191909d7264df3dc417dc76cee53291986d84e9) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "447.c1", 0x000000, 0x200000, CRC(41d6140a) SHA1(862b95ca1fe2b75d7ead0de8ac50c1b8a049c774) )
	ROM_LOAD16_BYTE( "447.c2", 0x000001, 0x200000, CRC(36f35df2) SHA1(22715561d5383263e41563fcb060f83a0c544531) )
ROM_END

// 447 : Hypernoid by M.Priewe.
ROM_START( hypernoid01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "447a.p1", 0x000000, 0x100000, CRC(43515e22) SHA1(14168cb0f51489ffbf6f4cd93ba62c4567378527) )

	NEO_SFIX_128K( "447.s1", CRC(bb82ab71) SHA1(307f420446c2d411a65b59543b61c21d0360536b) )

	NEO_BIOS_AUDIO_512K( "447.m1", CRC(6c8eaacc) SHA1(6bc065be18db6830a4c94b424f9380d38da6f8b7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "447.v1", 0x000000, 0x400000, CRC(dafa1bdd) SHA1(743a5ad24a3dba04ce8f3e2d95ebd5ca83fd9f98) )
	ROM_LOAD( "447.v2", 0x400000, 0x400000, CRC(85ad8283) SHA1(8abcf48a353dcd4777608b13471608f7290d89a5) )
	ROM_LOAD( "447.v3", 0x800000, 0x400000, CRC(86c27f0c) SHA1(85b740e0224d5ea09b600683dec6fe532a723355) )
	ROM_LOAD( "447.v4", 0xc00000, 0x400000, CRC(a3982244) SHA1(7191909d7264df3dc417dc76cee53291986d84e9) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "447.c1", 0x000000, 0x200000, CRC(41d6140a) SHA1(862b95ca1fe2b75d7ead0de8ac50c1b8a049c774) )
	ROM_LOAD16_BYTE( "447.c2", 0x000001, 0x200000, CRC(36f35df2) SHA1(22715561d5383263e41563fcb060f83a0c544531) )
ROM_END


// 469 : Neo Pong by Neodev (c) 2002
// MAME bug : On the title screen, there's a vertical blank area that slowly moves from left to right
ROM_START( neopong ) // v1.1
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "469.p1", 0x000000, 0x020000, CRC(9f35e29d) SHA1(261ce9305950debbc2fe088a45ab0fa83d6cf10d) )

	NEO_SFIX_128K( "469.s1", CRC(cd19264f) SHA1(531be2305cd56d332fb7a53ab924214ade34a9e8) )

	NEO_BIOS_AUDIO_128K( "469.m1", CRC(78c851cb) SHA1(a9923c002e4e2171a564af45cff0958c5d57b275) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "469.v1", 0x000000, 0x200000, CRC(6c31223c) SHA1(ede3a2806d7d872a0f737626a23ecce200b534e6) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "469.c1", 0x000000, 0x100000, CRC(f5a57d1a) SHA1(781170ae439f48fcbcee6af4cb3a1367dbb70498) )
	ROM_LOAD16_BYTE( "469.c2", 0x000001, 0x100000, CRC(affde64e) SHA1(25ba8b4f6f961027d97d35f13cebeaee2dbb6f5a) )
ROM_END

ROM_START( neoponga ) // v1.0, has no sound, does not satisfy the watchdog
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "469a.p1", 0x000000, 0x020000, CRC(31b724d7) SHA1(e374613609aeda64b547d8ce38d2078b6ed55ad5) )

	NEO_SFIX_128K( "469.s1", CRC(cd19264f) SHA1(531be2305cd56d332fb7a53ab924214ade34a9e8) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "202.v1", 0x000000, 0x80000, CRC(debeb8fb) SHA1(49a3d3578c087f1a0050168571ef8d1b08c5dc05) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "469a.c1", 0x000000, 0x100000, CRC(d7587282) SHA1(6fbc317dede78265dc4cfd2a6d79968320b8fd50) )
	ROM_LOAD16_BYTE( "469a.c2", 0x000001, 0x100000, CRC(fefc9d06) SHA1(f191bcd3683a184b5a7d6046156e0b2bca231e95) )
ROM_END


// 491 : Neotris by Totologic
// Bugs: Pressing 1 will reboot the game and can corrupt the right-hand side of the title
ROM_START( neotrisd1 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "491.p1", 0x000000, 0x080000, CRC(6a253ce9) SHA1(b9a3949dcd7e7dd3cd8b4e739ce2c913c16435b1) )

	NEO_SFIX_128K( "491.s1", CRC(7a30a592) SHA1(3d7fd159fafaf20b12388849e4c54192ce410fe8) )

	NEO_BIOS_AUDIO_64K( "491.m1", CRC(7a38ff47) SHA1(084cdf9dea053126783a4f570a56cc2444529f44) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "491.v1", 0x000000, 0x300000, CRC(95b3b726) SHA1(d0c361a78e509b083243e5f3a4b89bbaa658216e) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "491.c1", 0x000000, 0x100000, CRC(d53e8069) SHA1(55a9bb30fe1ce0abd9ebe54fa806992b2a790f28) )
	ROM_LOAD16_BYTE( "491.c2", 0x000001, 0x100000, CRC(61143d05) SHA1(093e0c819ee7884786c029e4a0da9e779f11db02) )
ROM_END

ROM_START( neotrisd2 )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "491a.p1", 0x000000, 0x080000, CRC(33a71f8d) SHA1(f4fe090b30193da3dce895fd9f75e94372d9b8c9) )

	NEO_SFIX_128K( "491a.s1", CRC(6809043a) SHA1(31efdd2bd0713d7a87f2c6dc92e43f312f8d8626) )

	NEO_BIOS_AUDIO_64K( "491a.m1", CRC(2f9f8265) SHA1(4bf718f428f8c0ce8b1d0005ea8b425a7729f622) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "491a.v1", 0x000000, 0x400000, CRC(9855b179) SHA1(e92b8373f4251a4a05da92483dc6c2d8b699f7f1) )
	ROM_LOAD( "491a.v2", 0x400000, 0x400000, CRC(b187368b) SHA1(9d0fa6ed14e87d9c38ed07ba8762c795dd3bb905) )
	ROM_LOAD( "491a.v3", 0x800000, 0x400000, CRC(5506b016) SHA1(181235f7b8792dd931ab00540d611221caaf0578) )
	ROM_LOAD( "491a.v4", 0xc00000, 0x400000, CRC(6bb8a938) SHA1(3d27cd2bdfc624b3ea4dfa79bc8c615eb34d612a) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "491a.c1", 0x000000, 0x400000, CRC(7f9c1a22) SHA1(633918addc8c2b22f55fb9c8b85edc4fc96c9753) )
	ROM_LOAD16_BYTE( "491a.c2", 0x000001, 0x400000, CRC(4896e921) SHA1(4abf2a36f1117a02bbf14859e3351a1ac5db9c53) )
	ROM_LOAD16_BYTE( "491a.c3", 0x800000, 0x400000, CRC(e8e2fcc3) SHA1(7fff344a476857460129a303ce680822d270a79a) )
	ROM_LOAD16_BYTE( "491a.c4", 0x800001, 0x400000, CRC(ec219a96) SHA1(f150d4956d32f204b51c0adf5db434d2ed258638) )
	ROM_LOAD16_BYTE( "491a.c5", 0x1000000, 0x400000, CRC(b8873b09) SHA1(4aed67ac2fac387dfaf7e0f78e127cdc98078c1d) )
	ROM_LOAD16_BYTE( "491a.c6", 0x1000001, 0x400000, CRC(fb994623) SHA1(198dddc5e7531d6fd2f7405243671d67e80fe26e) )
ROM_END

ROM_START( neotrisd3 ) // Beta 2
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "491b.p1", 0x000000, 0x080000, CRC(4cd619cf) SHA1(081a7819c71171a37f10d1b20f8887ca90df49a8) )

	NEO_SFIX_128K( "491a.s1", CRC(6809043a) SHA1(31efdd2bd0713d7a87f2c6dc92e43f312f8d8626) )

	NEO_BIOS_AUDIO_64K( "491b.m1", CRC(5a63bb9d) SHA1(de024be2535253d6846c380d05717002c0cc032d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "491b.v1", 0x000000, 0x400000, CRC(fef16eb4) SHA1(64021cf913c4594d27d0602afc28754f0b76091f) )
	ROM_LOAD( "491b.v2", 0x400000, 0x400000, CRC(f0d28e19) SHA1(e47318bf3b5ef1d9e08928eb7f7ca7dfc15ab2e5) )
	ROM_LOAD( "491b.v3", 0x800000, 0x400000, CRC(fc652c8b) SHA1(a76040eef8dd76da3657bc39b6b5c3dded539829) )
	ROM_LOAD( "491b.v4", 0xc00000, 0x400000, CRC(c25764ca) SHA1(ee0f2fa8db2da5c95b94c062bff8ee37ec2b2c7d) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "491b.c1", 0x000000, 0x400000, CRC(e1c8eca9) SHA1(be1babdc0ae0fd9916e8335c1635148e0498a961) )
	ROM_LOAD16_BYTE( "491b.c2", 0x000001, 0x400000, CRC(11ca6e64) SHA1(166c1f973f4fd7f472c3ddd37e0eb945c36d7454) )
	ROM_LOAD16_BYTE( "491b.c3", 0x800000, 0x400000, CRC(1d9e2046) SHA1(c2415775170455bd44e19e7c7c8e10817da56385) )
	ROM_LOAD16_BYTE( "491b.c4", 0x800001, 0x400000, CRC(9091e795) SHA1(59e7a751633ce8f3db138fc818e50003da4582dc) )
	ROM_LOAD16_BYTE( "491b.c5", 0x1000000, 0x400000, CRC(bf278afe) SHA1(e7f84cb325dd4535accd44d84f61c53cb4bab312) )
	ROM_LOAD16_BYTE( "491b.c6", 0x1000001, 0x400000, CRC(8eb17e24) SHA1(7977475bf19073b8bc38fcb88be9226453c7d6e4) )
ROM_END


// 492 : Project Neon Caravan Edition Prealpha 0.4.19 (c) 2019
// Bugs: Insert Coin (first time) causes reboot
// First time: Use video option to rotate 90CCW
ROM_START( caravan )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "492.p1", 0x000000, 0x80000, CRC(f45cac3a) SHA1(21e283f5fa41d10f0c2bf1f9848bb8f10ea51ad5) )

	NEO_SFIX_128K( "492.s1", CRC(f32de862) SHA1(07215b0a974ce0dff5065698a9ec00b3ba75eeee) )

	NEO_BIOS_AUDIO_64K( "492.m1", CRC(6d24cf93) SHA1(5d408af31ce949a8e75c8706498043790a518589) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "492.v1", 0x000000, 0x100000, CRC(2823ca49) SHA1(09a9c52ad3be25c0e3edf380a219efb6d4c327b7) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "492.c1", 0x000000, 0x80000, CRC(87df580b) SHA1(5c5dd1fcca16f4bb98156d2ed52a403930843f53) )
	ROM_LOAD16_BYTE( "492.c2", 0x000001, 0x80000, CRC(7b854cc1) SHA1(df7084e1049c7b156cd17c02000d99dd37d679bb) )
ROM_END


// 494 : Bonus Stage by Totologic
ROM_START( bonusstage )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "494.p1", 0x000000, 0x080000, CRC(de637e0b) SHA1(08f902bca19c815e7c69a0b9d3e5366eb82b944c) )

	NEO_SFIX_128K( "494.s1", CRC(a12d8990) SHA1(969db205d77cc80a1ebb88be6a265112142533ec) )

	NEO_BIOS_AUDIO_64K( "494.m1", CRC(dee563b2) SHA1(75e0fca36e8c23922ed9b54942ec11170b9b505a) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "494.v1", 0x000000, 0x400000, CRC(87bdd5fa) SHA1(e7ccc9a56aeaf1f4f79d2265fc5f19a0894dd96b) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "494.c1", 0x000000, 0x400000, CRC(bfe4eb8d) SHA1(e03ccfc50cf4813833a23b152bc0e47f453532a4) )
	ROM_LOAD16_BYTE( "494.c2", 0x000001, 0x400000, CRC(41fab784) SHA1(9d86d1362ed3981b9dedbd0bc9707a515260ce25) )
ROM_END


// 495: New! Super Mario Brothers (demo of the intro screen)
ROM_START( nsmbd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "495d.p1", 0x000000, 0x100000, CRC(6abacc0f) SHA1(83884b5fe816a132b1a59fea36b63fa700fd44b4) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "495d.m1", CRC(40d6f48d) SHA1(bad050e766d03f70008f7b5b3f1cf65ce7e85a63) )

	ROM_REGION( 0x900000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "495d.v1", 0x000000, 0x100000, CRC(d94bea55) SHA1(6e31add41ff8a30b3bc4eb06a81e6453156780aa) )
	ROM_LOAD( "495d.v2", 0x100000, 0x100000, CRC(b110df8b) SHA1(4b7a13c7de5843a669ec9bf914a48029233ba3c9) )
	ROM_LOAD( "495d.v3", 0x200000, 0x100000, CRC(3c623679) SHA1(5fa98eacb612be496aa7937a9678398ba16f2a13) )
	ROM_LOAD( "495d.v4", 0x300000, 0x100000, CRC(fbf00c96) SHA1(4083f8efbcf748dba31f0030b82487e0ea56980b) )
	ROM_LOAD( "495d.v5", 0x400000, 0x100000, CRC(9b2031d4) SHA1(15c8f5cb26af29b8dca53f1c1c49384a1b73b820) )
	ROM_LOAD( "495d.v6", 0x500000, 0x100000, CRC(107cfc89) SHA1(c4ced7dfab24ff42c6fc2424b18cf199ba30cbfa) )
	ROM_LOAD( "495d.v7", 0x600000, 0x100000, CRC(0703b761) SHA1(7d586b69bb9578d550871eda884cd6b32d86a01f) )
	ROM_LOAD( "495d.v8", 0x700000, 0x100000, CRC(8d525588) SHA1(297871c1e8888d9adab3781fb6c2c27ecf50ca45) )
	ROM_LOAD( "495d.v9", 0x800000, 0x100000, CRC(97f073b6) SHA1(02a4cce3f0cdb9421350ee5af49af13d7dcd16c2) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "495d.c1", 0x000000, 0x800000, CRC(81ac72e5) SHA1(e7b3a243ef826bc4833cc5d3465c03b923a72b0a) )
	ROM_LOAD16_BYTE( "495d.c2", 0x000001, 0x800000, CRC(a94a817b) SHA1(b41caf19c9eab62ce5a448970ca0afa6082dd717) )
ROM_END

// 495: New! Super Mario Brothers
// Bugs: Even though you can collect coins, there's no point score.
ROM_START( nsmb )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "495.p1", 0x000000, 0x100000, CRC(9e0fded1) SHA1(333da236947c440e10e69ee70b8e99a818488f1d) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "495.m1", CRC(3c99e181) SHA1(7f827b65f5c170d95cd3dd14d644ea92e104a268) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "495.v1", 0x000000, 0x100000, CRC(d1e4fc94) SHA1(03bf016533b08eac16f1e8a368ab6422d4163364) )
	ROM_LOAD( "495.v2", 0x100000, 0x100000, CRC(7d42fab3) SHA1(242288e91790818f130d7ddacbf68f5bf2d993be) )
	ROM_LOAD( "495.v3", 0x200000, 0x100000, CRC(806b797d) SHA1(24b4951991afea6d81a55901320cf6f532713b16) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "495.c1", 0x0000000, 0x800000, CRC(f187934c) SHA1(72c7f23e45febb94fc89c01ffb475ae9c4b8fff3) )
	ROM_LOAD16_BYTE( "495.c2", 0x0000001, 0x800000, CRC(7ecfc8bb) SHA1(2d61e86ccfe9e92dcc400efba63f91fd2e77e251) )
ROM_END


// 496: Neo Black Tiger
// Bugs: after playing for a while, graphics bugs appear, followed by getting stuck in areas, then total screen corruption, and finally it resets.
// Even though you can collect coins, there's no point score.
// The time bonus does nothing.
// You can only die by falling down a hole, whereupon the game restarts.
// Can't exit the stage at the end.
ROM_START( nblktiger )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "496.p1", 0x000000, 0x100000, CRC(18f34200) SHA1(78b3b9ef20ddf780a81e644d996d56dd5c0e6d10) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_64K( "496.m1", CRC(2037dc19) SHA1(c88588d07fc63aeed828a87f0f633c7badafeb3e) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	// incomplete sound
	//ROM_LOAD( "496.v1", 0x000000, 0x100000, CRC(3ac066e1) SHA1(cab7f04fbce0db685f5fa0d22923f7b7ca37a804) )
	//ROM_LOAD( "496.v2", 0x100000, 0x100000, CRC(c5a12987) SHA1(71236afffdb69aa3ba8ebc345e1e7b1830db437e) )
	// better sound
	ROM_LOAD( "496.v1",   0x000000, 0x400000, CRC(f44460dc) SHA1(9b31c5fae67fc40c4c6c4fe4a4c21fa92374c8c7) )
	ROM_LOAD( "496.v2",   0x400000, 0x400000, CRC(bf52d283) SHA1(59f2d26ab21d792228d6ee0bf840080fc1f4ba5d) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "496.c1", 0x0000000, 0x800000, CRC(aa469494) SHA1(fd98b7445c85174ccb745c74cb9e852b08353324) )
	ROM_LOAD16_BYTE( "496.c2", 0x0000001, 0x800000, CRC(fa07ba1e) SHA1(8c00be2c0c1402789011e2aa272e04e216fdac10) )
ROM_END


// 497: Cabal (port by iq_132)
// Bugs: A few minor graphics issues
ROM_START( cabalng )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "497.p1", 0x000000, 0x100000, CRC(49232965) SHA1(f13352d5b853ea832eb751da508c7878fb149c20) )

	NEO_SFIX_128K( "497.s1", CRC(45f0bc5e) SHA1(de2533e4981c7597a768b8839c737a37243d4bdb) )

	NEO_BIOS_AUDIO_64K( "497.m1", CRC(efd97334) SHA1(139ef9da1fba0adcd4b41fbe6a2af0e094f33b05) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "497.v1", 0x000000, 0x100000, CRC(1a5bbc12) SHA1(3b4c74d943bd41189052313d41cd37b81e18ba25) )
	ROM_LOAD( "497.v2", 0x100000, 0x100000, CRC(a2b9c011) SHA1(6019d3b7bf78c509394765f2e8bfd5c91645d5c4) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "497.c1", 0x0000000, 0x100000, CRC(9f5ea8bb) SHA1(dbe5ebe37d720bba20fc68a948298aa96a11dc85) )
	ROM_LOAD16_BYTE( "497.c2", 0x0000001, 0x100000, CRC(edf277f2) SHA1(aa287598a620f03794e143500ff842dfb4b64d9a) )
ROM_END

ROM_START( cabalng1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "497a.p1", 0x000000, 0x100000, CRC(bb193926) SHA1(e87473aaaa8c7af56148e86a410672870fa63d00) )

	NEO_SFIX_128K( "497.s1", CRC(45f0bc5e) SHA1(de2533e4981c7597a768b8839c737a37243d4bdb) )

	NEO_BIOS_AUDIO_64K( "497.m1", CRC(efd97334) SHA1(139ef9da1fba0adcd4b41fbe6a2af0e094f33b05) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "497.v1", 0x000000, 0x100000, CRC(1a5bbc12) SHA1(3b4c74d943bd41189052313d41cd37b81e18ba25) )
	ROM_LOAD( "497.v2", 0x100000, 0x100000, CRC(a2b9c011) SHA1(6019d3b7bf78c509394765f2e8bfd5c91645d5c4) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "497.c1", 0x0000000, 0x100000, CRC(9f5ea8bb) SHA1(dbe5ebe37d720bba20fc68a948298aa96a11dc85) )
	ROM_LOAD16_BYTE( "497.c2", 0x0000001, 0x100000, CRC(edf277f2) SHA1(aa287598a620f03794e143500ff842dfb4b64d9a) )
ROM_END


// 498: Abyssal Infants
// Bugs: Various minor control issues and spelling mistakes
ROM_START( abyssal )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "498.p1", 0x000000, 0x080000, CRC(ae12bd96) SHA1(299c66c7038edb9f8bac858172f8ce282cb89edf) )

	NEO_SFIX_128K( "498.s1", CRC(1ebbb83e) SHA1(4a52fe5463db19b0617fa990c9f2b8a006ac813c) )

	NEO_BIOS_AUDIO_128K( "498.m1", CRC(79b190e9) SHA1(e63232fba9fd1cb777059fac5aec41304b411212) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "498.v1", 0x000000, 0x200000, CRC(be46bf92) SHA1(4bf6b27dedba3321d9adf40543edf28e85a9f6ab) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "498.c1", 0x0000000, 0x200000, CRC(b8c84619) SHA1(1d634242281a7bf75979cd16ee5ae98b5b161b65) )
	ROM_LOAD16_BYTE( "498.c2", 0x0000001, 0x200000, CRC(376084b8) SHA1(b42e04c1168dd88a13d8690f0b5b5212457c558e) )
ROM_END

ROM_START( abyssal1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "498a.p1", 0x000000, 0x080000, CRC(a8a2a8a6) SHA1(da0ea757e17531be8c8e3b0a1f6cc71b0eb04b92) )

	NEO_SFIX_128K( "498.s1", CRC(1ebbb83e) SHA1(4a52fe5463db19b0617fa990c9f2b8a006ac813c) )

	NEO_BIOS_AUDIO_128K( "498a.m1", CRC(3ddf772b) SHA1(9ac9dba6feac48dcead44020cb17540bba6c0590) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "498a.v1", 0x000000, 0x200000, CRC(1fae3797) SHA1(2e3b410bc52366788b18f70ae5e912a8e2d5e93f) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "498a.c1", 0x0000000, 0x200000, CRC(24dabe27) SHA1(4b7519cbcf028b0be7b0acf9c8827a126a33007d) )
	ROM_LOAD16_BYTE( "498a.c2", 0x0000001, 0x200000, CRC(bbdacc24) SHA1(39025e7f593cc7f464e9f0403feab69ddbd371c8) )
ROM_END


// 499: Neo 2048 by Nicole Branagan (Nicole Express)
// Bugs: No in-game sound. Going by the original rom names there's supposed to be a logo, but it never shows.
ROM_START( neo2048 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "499.p1", 0x000000, 0x000bde, CRC(a1039bdb) SHA1(4cbeaaa1e7834d007e8cdbcb5e7bb2ac1cbdbf93) )

	NEO_SFIX_128K( "499.s1", CRC(d1e4c798) SHA1(b0ce4b6097558b837e9fe991b66ea0ee18f53ed5) )

	NEO_BIOS_AUDIO_64K( "499.m1", CRC(6e363bd2) SHA1(9005b6b34caa5405c290775b79c7cd644ce2881f) )

	ROM_REGION( 0x10000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "499.v1", 0x000000, 0x00e200, CRC(dba56bf0) SHA1(0bcee505e10300108b967c0475caa92ed36e6a2b) )

	ROM_REGION( 0x200000, "sprites", 0 )
	// Logo roms
	ROM_LOAD16_BYTE( "499.c1", 0x000000, 0x100000, CRC(479543cf) SHA1(772690c872632320133a799aa41f6e68a8d07a4c) )
	ROM_LOAD16_BYTE( "499.c2", 0x000001, 0x100000, CRC(1f6431d5) SHA1(7c90d6ec9df9e6223a066c338b7a7886071370cf) )
	// Sprites - yes, these overwrite part of the above roms
	ROM_LOAD16_BYTE( "499.c3", 0x100000, 0x000c00, CRC(ef3e848e) SHA1(7a85764ead7a1a48b52479d760e6013ba018e9b4) )
	ROM_LOAD16_BYTE( "499.c4", 0x100001, 0x000c00, CRC(933b4e8f) SHA1(8cad7038bfd696ba8288d2d1292a3271087e2ec7) )
ROM_END


// 500: Kid no Hore Hore Daisakusen (port by iq_132)
ROM_START( horekidb3 ) // beta 3, incomplete?
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "500.p1", 0x000000, 0x080000, CRC(f0b31d22) SHA1(3b942f5f4de04bcaf2a46c8bc16e38b557191a10) )

	NEO_SFIX_128K( "500.s1", CRC(b88013e2) SHA1(99ba753412d21ee6a6f1d63c645d74a027e5a143) )

	NEO_BIOS_AUDIO_64K( "500.m1", CRC(d82c2064) SHA1(ddec7b210c6395501797b650b9473693b52daab2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "500.v1", 0x000000, 0x800000, CRC(fccb11f4) SHA1(ac027581c1b23036aab74afa79d5fefc59a45e32) )
	ROM_LOAD( "500.v2", 0x100000, 0x800000, CRC(c65a5900) SHA1(a87f0bddb885082f1e8907a451640572f574c0b9) )

	ROM_REGION( 0x080000, "sprites", 0 )
	ROM_LOAD16_BYTE( "500.c1", 0x0000000, 0x040000, CRC(18b0b56b) SHA1(b3e4cdcb72bd1d8af6b505280774ec49de1f7c66) )
	ROM_LOAD16_BYTE( "500.c2", 0x0000001, 0x040000, CRC(9ccea1d5) SHA1(10c7c99d2a647e90e9b2b0d005733fea786ac61c) )
ROM_END


// 501: Soldier Girl Amazon (port by iq_132)
ROM_START( amazonn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "501.p1", 0x000000, 0x020000, CRC(f5f57b9d) SHA1(3c5c902d6db9342a0ad82568af48d34c61af46bb) )

	NEO_SFIX_128K( "501.s1", CRC(128166bb) SHA1(152677c61b3b33f26dad377a9e8c3329a58b8a3c) )

	NEO_BIOS_AUDIO_64K( "501.m1", CRC(681d2398) SHA1(3f5d3832fc8c3470dedc97f8418120895e198fcc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "501.v1", 0x000000, 0x200000, CRC(1a1a886c) SHA1(36a43396ab04e1d95f9231dc349cf43e59e65a3b) )
	ROM_LOAD( "501.v2", 0x100000, 0x200000, CRC(86cade60) SHA1(45d6b847675d8e7dadff83abf1a12db17e1da6f3) )

	ROM_REGION( 0x080000, "sprites", 0 )
	ROM_LOAD16_BYTE( "501.c1", 0x0000000, 0x020000, CRC(901c6ae1) SHA1(d4a98a0467198e2154c7838ebac3513216426310) )
	ROM_LOAD16_BYTE( "501.c2", 0x0000001, 0x020000, CRC(a02a8679) SHA1(18d785c994630043f822f4a71207f75b1ebe7d48) )
ROM_END


// 502: Cyborg Force by Neo Byte Force
ROM_START( cyborg )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "502.p1", 0x000000, 0x100000, CRC(89e1c728) SHA1(4b996f42373d77969c490534858aa0906d110685) )
	ROM_LOAD16_WORD_SWAP( "502.p2", 0x100000, 0x300000, CRC(e4a8b27a) SHA1(cbdf21414a5be59664dfd363b4d62ba0c8b05847) )

	NEO_SFIX_128K( "502.s1", CRC(5bd29810) SHA1(cfa054dfa027e3617ba1a1f25a043f03a25676fd) )

	NEO_BIOS_AUDIO_64K( "502.m1", CRC(06da3cec) SHA1(034c250229920891edc70fecb5844e12a7c1c1bd) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "502.v1",   0x000000, 0x800000, CRC(dc50718c) SHA1(e4150bc3625a60be197d629444906a8663a4aebe) )
	ROM_LOAD( "502.v2",   0x800000, 0x800000, CRC(8135d5a8) SHA1(186a516ed0fbc1d39f96385513de2ad662511559) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "502.c1",   0x0000000, 0x1000000, CRC(77078687) SHA1(9485bcc2af6f59ebce57a6b19f215db712f93278) )
	ROM_LOAD16_BYTE( "502.c2",   0x0000001, 0x1000000, CRC(a5abdb83) SHA1(cfc54c46ae6a593a9d2f4fdd1ee0d27877f55c20) )
ROM_END

ROM_START( cyborg1 ) // Retroarch/fbneo patched
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "502p.p1", 0x000000, 0x100000, CRC(bcd819ee) SHA1(35d65019d220776472750ef10dd3e59bf41d8501) )
	ROM_LOAD16_WORD_SWAP( "502p.p2", 0x100000, 0x800000, CRC(3e0b80a2) SHA1(b7604c692ad10b3b4c27f3ce7ce6a1f5a507e980) )

	NEO_SFIX_128K( "502.s1", CRC(5bd29810) SHA1(cfa054dfa027e3617ba1a1f25a043f03a25676fd) )

	NEO_BIOS_AUDIO_64K( "502.m1", CRC(06da3cec) SHA1(034c250229920891edc70fecb5844e12a7c1c1bd) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "502.v1",   0x000000, 0x800000, CRC(dc50718c) SHA1(e4150bc3625a60be197d629444906a8663a4aebe) )
	ROM_LOAD( "502.v2",   0x800000, 0x800000, CRC(8135d5a8) SHA1(186a516ed0fbc1d39f96385513de2ad662511559) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "502.c1",   0x0000000, 0x1000000, CRC(77078687) SHA1(9485bcc2af6f59ebce57a6b19f215db712f93278) )
	ROM_LOAD16_BYTE( "502.c2",   0x0000001, 0x1000000, CRC(a5abdb83) SHA1(cfc54c46ae6a593a9d2f4fdd1ee0d27877f55c20) )
ROM_END


// 503 : Gladmort by Pixelheart/ChipsOnSteroids
ROM_START( gladmort )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "503.p1", 0x000000, 0x100000, CRC(64d77ca4) SHA1(8cf0e12d462dd1f4f67c782c5c643a7da29bc718) )

	NEO_SFIX_128K( "503.s1", CRC(c96154b9) SHA1(c95251dc566d4649496b7785c8e4cddd0aa67d7f) )

	NEO_BIOS_AUDIO_64K( "503.m1", CRC(fbccedbd) SHA1(61e7f745d24c4ec3598cbf3cefc603f7deb21497) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "503.v1",   0x000000, 0x400000, CRC(4c0efb20) SHA1(80424564a8cd19bbe7e3f8003129585174cc9367) )
	ROM_LOAD( "503.v2",   0x400000, 0x400000, CRC(dbd083f5) SHA1(c76980473550fda0ece72fa87104ea79a6a8024e) )
	ROM_LOAD( "503.v3",   0x800000, 0x400000, CRC(2f5c4eeb) SHA1(4ed687bde1d3b549581270e90832d90a46101df0) )
	ROM_LOAD( "503.v4",   0xc00000, 0x400000, CRC(12e01947) SHA1(b8147e584712d77c47e97c7a4790807536a22a86) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "503.c1",   0x0000000, 0x1400000, CRC(c7d367cc) SHA1(91a5241ba793c367e06341c951e9fb43726d0944) )
	ROM_LOAD16_BYTE( "503.c2",   0x0000001, 0x1400000, CRC(2c39b98e) SHA1(da26d5073fe483650cbcacf595b4410e4f5f4787) )
ROM_END

// 503 : Gladmort Demo by Pixelheart
ROM_START( gladmortd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "503d1.p1", 0x000000, 0x080000, CRC(e0ceafec) SHA1(f147fdb7c0fee02e8ae1923bf0ef3aff954588ae) )

	NEO_SFIX_128K( "503d1.s1", CRC(8304be52) SHA1(fbeaee12553709e54e7f5134dfd2ff96df5fdee0) )

	NEO_BIOS_AUDIO_64K( "503d1.m1", CRC(fb945087) SHA1(a2bae6907bf02c93068f9ee23409506277c8a027) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "503d1.v1",   0x000000, 0x1000000, CRC(49d60c52) SHA1(56134745ce1fef3272aa5ddfcd24e3e894d5cca9) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "503d1.c1",   0x0000000, 0x1000000, CRC(2c4fe4c3) SHA1(7121a8713d58bf61a88fcf075cdc9529838af6e1) )
	ROM_LOAD16_BYTE( "503d1.c2",   0x0000001, 0x1000000, CRC(1bdd21bd) SHA1(13749d078525a63ac85ea2aa17b326409a4609e9) )
ROM_END

// 503 : Gladmort Demo 2 by Pixelheart
ROM_START( gladmortd2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "503d2.p1", 0x000000, 0x100000, CRC(d7712425) SHA1(eb9c32b584a600cc67784113cd87629c3eab4e1c) )

	NEO_SFIX_128K( "503d1.s1", CRC(8304be52) SHA1(fbeaee12553709e54e7f5134dfd2ff96df5fdee0) )

	NEO_BIOS_AUDIO_64K( "503d2.m1", CRC(0dfddae3) SHA1(4bfa6af6874510d1da95badee1bd23d3be853461) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "503d2.v1",   0x000000, 0x400000, CRC(3b5e1408) SHA1(90cc923273d002819228cde8bdc57d76915b52f1) )
	ROM_LOAD( "503d2.v2",   0x400000, 0x400000, CRC(d44f004d) SHA1(bfb652111b1d065d48833034ab0333e69af9c947) )
	ROM_LOAD( "503d2.v3",   0x800000, 0x400000, CRC(e9e8de2f) SHA1(2a480f925073776308c07b6d4cc2334115bc17f2) )
	ROM_LOAD( "503d2.v4",   0xc00000, 0x400000, CRC(aaf3f4e2) SHA1(9e126db19a98de4bd0c649f7a655e1e79cc236ed) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "503d2.c1",   0x0000000, 0x1400000, CRC(bcb081ba) SHA1(68cdd3c2920a0fc5c52a5ba1d6e4efd0e735e11d) )
	ROM_LOAD16_BYTE( "503d2.c2",   0x0000001, 0x1400000, CRC(1b927329) SHA1(4861da95ce6a9d0521bbeed3b4f06daa0750c15c) )
ROM_END


// Shinobi v1.0 (Neo-Geo port) by Hoffman
ROM_START( shinobin )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "504.p1", 0x000000, 0x080000, CRC(3bf8e303) SHA1(84ed6ef7a2cddbc49341c3f7545b63b7d7ee67a0) )

	NEO_SFIX_128K( "504.s1", CRC(6b8c8124) SHA1(c2fbc49234360925379024ee5c5edf7000d08127) )

	NEO_BIOS_AUDIO_64K( "504.m1", CRC(6b52f62d) SHA1(9ee0ad09ceede00a0bc068065978aceebb4a28ea) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "504.v1",   0x000000, 0x400000, CRC(67480d87) SHA1(4d2876922ae0fd70bd62a59fd357742bce59ce6a) )
	ROM_LOAD( "504.v2",   0x400000, 0x400000, CRC(b335aa46) SHA1(3aba53c22fb1ed0c3524d6fa7ab945946e1764eb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "504.c1",   0x000000, 0x080000, CRC(74227c08) SHA1(a3835be816ba6afb5533c6f68500d87996f49b28) )
	ROM_LOAD16_BYTE( "504.c2",   0x000001, 0x080000, CRC(df4104e6) SHA1(5aa9f75305107648f3065db7a4a1b570cf3d62ef) )
ROM_END


// Shinobi v1.1 (Neo-Geo port) by Hoffman
ROM_START( shinobin1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "504a.p1", 0x000000, 0x080000, CRC(05fd9434) SHA1(6169b376c5258723a52c08a35d2990dd1c2eb6aa) )

	NEO_SFIX_128K( "504.s1", CRC(6b8c8124) SHA1(c2fbc49234360925379024ee5c5edf7000d08127) )

	NEO_BIOS_AUDIO_64K( "504.m1", CRC(6b52f62d) SHA1(9ee0ad09ceede00a0bc068065978aceebb4a28ea) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "504.v1",   0x000000, 0x400000, CRC(67480d87) SHA1(4d2876922ae0fd70bd62a59fd357742bce59ce6a) )
	ROM_LOAD( "504.v2",   0x400000, 0x400000, CRC(b335aa46) SHA1(3aba53c22fb1ed0c3524d6fa7ab945946e1764eb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "504.c1",   0x000000, 0x080000, CRC(74227c08) SHA1(a3835be816ba6afb5533c6f68500d87996f49b28) )
	ROM_LOAD16_BYTE( "504.c2",   0x000001, 0x080000, CRC(df4104e6) SHA1(5aa9f75305107648f3065db7a4a1b570cf3d62ef) )
ROM_END


// Shinobi Test by Hoffman
ROM_START( shinobint )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "504t.p1", 0x000000, 0x080000, CRC(6ea08aef) SHA1(f1a9bbceb1caa56fe10ac046df07b7bf02b70900) )

	NEO_SFIX_128K( "504.s1", CRC(6b8c8124) SHA1(c2fbc49234360925379024ee5c5edf7000d08127) )

	NEO_BIOS_AUDIO_64K( "504.m1", CRC(6b52f62d) SHA1(9ee0ad09ceede00a0bc068065978aceebb4a28ea) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "504.v1",   0x000000, 0x400000, CRC(67480d87) SHA1(4d2876922ae0fd70bd62a59fd357742bce59ce6a) )
	ROM_LOAD( "504.v2",   0x400000, 0x400000, CRC(b335aa46) SHA1(3aba53c22fb1ed0c3524d6fa7ab945946e1764eb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "504.c1",   0x000000, 0x080000, CRC(74227c08) SHA1(a3835be816ba6afb5533c6f68500d87996f49b28) )
	ROM_LOAD16_BYTE( "504.c2",   0x000001, 0x080000, CRC(df4104e6) SHA1(5aa9f75305107648f3065db7a4a1b570cf3d62ef) )
ROM_END


// 505: Double Dragon One demo by La Casa De Ruivo, released by Neo Byte Force
ROM_START( doubled1 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "505.p1", 0x000000, 0x100000, CRC(5991da92) SHA1(c7387217e28c03483ea5d92fa062bb834f8d0534) )
	ROM_LOAD16_WORD_SWAP( "505.p2", 0x100000, 0x400000, CRC(7e5ed6b8) SHA1(cae16c7d27512f94a242bff167393a2c56c0220d) )

	NEO_SFIX_128K( "505.s1", CRC(714afb4b) SHA1(63b69781585e503e7fa6a62bcf1e6742f8cb3921) )

	NEO_BIOS_AUDIO_64K( "505.m1", CRC(43295479) SHA1(5f08b91444e89f918cd239019b6df540b5d158fc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "505.v1",   0x000000, 0x800000, CRC(f01e97dc) SHA1(3bfb92c2cfb937ddf02299b6ac14c6851b6c1f11) )
	ROM_LOAD( "505.v2",   0x800000, 0x800000, CRC(79156c41) SHA1(8f8835139390c021b201384c304d235300b47af7) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "505.c1",   0x0000000, 0x1000000, CRC(fd883db8) SHA1(9b5c2e916a38438290d9b82eb7b5f34e8b27f660) )
	ROM_LOAD16_BYTE( "505.c2",   0x0000001, 0x1000000, CRC(718050d3) SHA1(4bab6c856a8e91d4d8dab12257211e6c6e82b306) )
ROM_END


// 506: Karnov port by iq_132
ROM_START( karnovn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "506.p1", 0x000000, 0x80000, CRC(79fd92aa) SHA1(4a56a325c32be69a27443776055152a0a436c1eb) )

	NEO_SFIX_128K( "506.s1", CRC(f6a144cf) SHA1(cd8d438431d54f1451b796410864caf299787278) )

	NEO_BIOS_AUDIO_64K( "506.m1", CRC(089a15b5) SHA1(bdccbf1042e0d611e30178652fdeec01e00a5b44) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "506.v1",   0x000000, 0x200000, CRC(51502fea) SHA1(cfb2bd316e2b05e8559d662569837c0ca72ff6bc) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "506.c1",   0x0000000, 0x80000, CRC(a27d94ff) SHA1(1497a1e2a8b471edca7942201bd3e891f8fdb0b9) )
	ROM_LOAD16_BYTE( "506.c2",   0x0000001, 0x80000, CRC(b9435703) SHA1(a0c309dc1d768c5182876883928588789eb1b372) )
ROM_END


// 507: Golden Axe port by Hoffman
ROM_START( goldaxen )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "507.p1", 0x000000, 0x100000, CRC(3c3ed057) SHA1(4f3ba87fb85a6e79d9efd8da0cdf91515630e60a) )

	NEO_SFIX_128K( "507.s1", CRC(707d91c0) SHA1(aa9c9c458116d5ca4c40def1f71934a1a94fcf7b) )

	NEO_BIOS_AUDIO_64K( "507.m1", CRC(7805d21b) SHA1(15620425344ca4a4bea8364618586f84b3241321) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "507.v1",   0x000000, 0x400000, CRC(5ed99267) SHA1(a720ed8942e88eb883b95f65f9fde10939b7b19a) )
	ROM_LOAD( "507.v2",   0x400000, 0x400000, CRC(d30632dd) SHA1(8e3d17cc3054e842ba36cd4b872929c5163960ec) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "507.c1",   0x0000000, 0x200000, CRC(7540f3d6) SHA1(a505201798f121d02278ca1761fc2dd451a16336) )
	ROM_LOAD16_BYTE( "507.c2",   0x0000001, 0x200000, CRC(a78a6647) SHA1(19a9b65701935165bd972542f868b71b3d70cb47) )
ROM_END


// 508: POW port by iq_132, converted from CD by Robbbert.
// bugs: no music, because it's in separate wav files on the CD
ROM_START( pown )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "508.p1", 0x000000, 0x60000, CRC(792d54be) SHA1(17a10c180860e99036e48873849a642c8c41f894) )

	NEO_SFIX_128K( "508.s1", CRC(89043389) SHA1(01ea488de2c401d3ee836cc7d5c696ad8cb4d8f9) )

	NEO_BIOS_AUDIO_64K( "508.m1", CRC(b50f61bb) SHA1(46d5017e40ca79e280fa247de3eb74ca5092117c) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "508.v1",   0x000000, 0x4c900, CRC(fba165f8) SHA1(948b19b340fe2a1d6123746d537714adaa07aa1d) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD( "508.c1",   0x0000000, 0x200000, CRC(887ca97e) SHA1(b4a1176ba6862554d8f2dbea9864afb865f6f296) )
ROM_END


// 509: Super Power Kick beta 1 by Kakoeimon
ROM_START( spkick )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "509.p1", 0x000000, 0x100000, CRC(c61a5d8d) SHA1(309642eb4fd466ce8bf243d9f0ac292ac25b90d2) )

	NEO_SFIX_MT(0x40000)
	ROM_LOAD( "509.s1", 0x00000, 0x40000, CRC(8856e60b) SHA1(d695fcc77770782c753fa07a35c70963fb3f2cdb) )

	NEO_BIOS_AUDIO_256K( "509.m1", CRC(52939569) SHA1(e612e7d1563b0ef9d1b607e6f487d70a476870f1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "509.v1",   0x000000, 0x100000, CRC(b1f6bcc0) SHA1(a134db583c6aa5f8f365381deed94634f7f745ad) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "509.c1",   0x0000000, 0x400000, CRC(0a6c2c38) SHA1(6a737433e3d52d4bc85af68b0b6b428113df83a9) )
ROM_END

// 509: Super Power Kick beta 2 by Kakoeimon
ROM_START( spkick2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "509a.p1", 0x000000, 0x100000, CRC(5de08fed) SHA1(cc0a4da3cec73a8e6372b78b65a9805c28403e75) )

	NEO_SFIX_MT(0x40000)
	ROM_LOAD( "509.s1", 0x00000, 0x40000, CRC(8856e60b) SHA1(d695fcc77770782c753fa07a35c70963fb3f2cdb) )

	NEO_BIOS_AUDIO_256K( "509a.m1", CRC(32e878ae) SHA1(cee0e6c5173484f23f7a89df088a11c5e7bc3b70) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "509a.v1",   0x000000, 0x100000, CRC(3159d577) SHA1(06c9c1ee9a67faa8cc8b9a7a6812b042c4b94f87) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "509a.c1",   0x0000000, 0x400000, CRC(ef914101) SHA1(9f5f37a930982a476ef42ad6da9a25c7a7fdd0e2) )
ROM_END


// 510: Atomic Runner Chelnov port by iq_132, converted from CD by Robbbert.
// bugs: no music, because it's in separate wav files on the CD
ROM_START( chelnovn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "510.p1", 0x000000, 0x2c000, CRC(cfc064f2) SHA1(61c221dfd3a75a89257a1227fa0c079637218156) )
	ROM_LOAD( "510.p2", 0x040000, 0x11400, CRC(624ceaaa) SHA1(759d099a316f4a4c39ec5753ffbb4c4e639ca7f8) )

	NEO_SFIX_128K( "510.s1", CRC(3f86c046) SHA1(52262fda22c44d93a0b367f86df1111bff63949c) )

	NEO_BIOS_AUDIO_64K( "510.m1", CRC(f675a58e) SHA1(71d78ace6e0bbe25094a15d953b8660be8a81328) )

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "510.v1",   0x000000, 0x35e00, CRC(ae367d92) SHA1(3af5d584820835aee3a1c6ab21f9c76670befdb0) )

	ROM_REGION( 0x80000, "sprites", 0 )
	ROM_LOAD( "510.c1",   0x000000, 0x80000, CRC(d8eed0b2) SHA1(ff3c81737e7045c9a39f104bf7f48406bf8895d5) )
ROM_END


// 511: Bad Dudes vs Dragonninja port by OzzyOuzo.
ROM_START( baddudesn )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "511.p1", 0x000000, 0x100000, CRC(77a74315) SHA1(90a63f02ec8e242af776b55aaca7998fa493cc39) )
	ROM_LOAD16_WORD_SWAP( "511.p2", 0x100000, 0x700000, CRC(f827dd6e) SHA1(36ac12cbde4869c266c1159a58c40d851d113fdf) )

	NEO_SFIX_128K( "511.s1", CRC(83b0f6c4) SHA1(4e3fd4369216fc9f4c699aea6bdf699b577c53e1) )

	NEO_BIOS_AUDIO_64K( "511.m1", CRC(2854b516) SHA1(9417756c419e374853d56d5025f78e2645956d85) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "511.v1",   0x000000, 0x800000, CRC(d8ac5857) SHA1(afea061d7122398938899a42f2c4141d4f125c69) )
	ROM_LOAD( "511.v2",   0x800000, 0x800000, CRC(f37bd666) SHA1(66d1cf293eea4cfe1cc36fe7ee9f6b46b0d731ee) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "511.c1",   0x0000000, 0x1000000, CRC(fe7bb928) SHA1(9afa02773e1fcd845ecb3c7ec3c5d5232ac8be2c) )
	ROM_LOAD16_BYTE( "511.c2",   0x0000001, 0x1000000, CRC(68deb392) SHA1(c01f17582b54332c50086f4161e13d6433d16c85) )
ROM_END


// 512: In the Hunt port by OzzyOuzo.
ROM_START( inthuntn )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "512.p1", 0x000000, 0x100000, CRC(58e3317a) SHA1(437dd2321f1bc8f5013daebe6399d25b02bd0fe9) )
	ROM_LOAD16_WORD_SWAP( "512.p2", 0x100000, 0x100000, CRC(c6494f12) SHA1(3842c99dd0ad5443228f9db39c6f1e5a063e595b) )

	NEO_SFIX_128K( "512.s1", CRC(35c8a6e0) SHA1(710af5ad77e5be09cbdaaa94624646cec54bca24) )

	NEO_BIOS_AUDIO_64K( "512.m1", CRC(dd055711) SHA1(2fcae3ba868213d8efc0d17c1290d08aceb6cb4a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "512.v1",  0x000000, 0x800000, CRC(eb947c63) SHA1(acb360fed600fb8143c41ca5389c8f7263d3915b) )
	ROM_LOAD( "505.v2",  0x800000, 0x800000, CRC(79156c41) SHA1(8f8835139390c021b201384c304d235300b47af7) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "512.c1",   0x0000000, 0x1000000, CRC(1b32eaf5) SHA1(9d17ae76d8d67dbd34c209cd1c2a0036786b0924) )
	ROM_LOAD16_BYTE( "512.c2",   0x0000001, 0x1000000, CRC(87b8def8) SHA1(d27685fc28d5745a75ac7337c5765325c56f5d70) )
ROM_END


// 513: Jurl by Tonsomo Entertainment
// bugs: no sound
ROM_START( jurl )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "513.p1", 0x000000, 0x080000, CRC(601b540e) SHA1(d2ec144bde329a2f23c08c425a47e4661c950034) )

	NEO_SFIX_128K( "513.s1", CRC(d6c058c9) SHA1(b5f3faef8e87a517584e85e42e4af9dd90003047) )

	NEO_BIOS_AUDIO_128K( "513.m1", CRC(b758c5d2) SHA1(939ab3d66cb1138cf4170432fab213beb4cef0d1) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "513.v1",   0x000000, 0x400000, CRC(16db022e) SHA1(dab8973b95e813cc215011d6d00c6f96f651c636) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "513.c1",   0x0000000, 0x100000, CRC(02811fe9) SHA1(0257251d7ce31202f7604d1eb48f2dadd09e7819) )
	ROM_LOAD16_BYTE( "513.c2",   0x0000001, 0x100000, CRC(953bb9a9) SHA1(cd75c64b559ed92c7c02da4f74653aaed314cae6) )
ROM_END


// 514: Food Fight port by iq_132
// bugs: priority issues
ROM_START( foodfn )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "514.p1", 0x000000, 0x080000, CRC(904bb32f) SHA1(b6e29634485af5e88997971f0dfb7661e93e8724) )

	NEO_SFIX_128K( "021.s1", CRC(6956d778) SHA1(e3757776d60dc07d8e07c9ca61b223b14732f860) )

	NEO_BIOS_AUDIO_256K( "514.m1", CRC(35cb565b) SHA1(887b91c47e99c5b3beb92425d49e240425507afa) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "514.v1", 0x000000, 0x080000, CRC(16d20386) SHA1(79787d1a6f23937f3da6cb3d7bfea798a3ccaf1f) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "514.c1", 0x000000, 0x080000, CRC(7b4ca9e3) SHA1(ed5aa0f673e6489f608b4fe3cd16ef7f6fdcff31) )
	ROM_LOAD16_BYTE( "514.c2", 0x000001, 0x080000, CRC(d9f831bb) SHA1(f2bf06591506c8d142873800a55713de831c49eb) )
ROM_END


// 515: Bang Bang Busters 2 demo
ROM_START( bbb2d1 ) // demo 1
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "515.p1", 0x000000, 0x100000, CRC(3659f253) SHA1(78474a0dbf812e987cae7b7ca0f8d00c35060939) )

	NEO_SFIX_128K( "515.s1", CRC(e53ef3d0) SHA1(ef3ade7aadc1b57a20c1830bf1d3444dfc9ce1fb) )

	NEO_BIOS_AUDIO_64K( "515.m1", CRC(bff0ad49) SHA1(e75296afefe011a2b96245065b3d97739e3c338e) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "515.v1", 0x000000, 0x400000, CRC(f1dda8dd) SHA1(2cc21436eca6eaa08f8de82fc5252e1f4a335333) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "515.c1", 0x000000, 0x800000, CRC(739173e2) SHA1(5703a97bba12c63411700341abe4e2981650d59e) )
	ROM_LOAD16_BYTE( "515.c2", 0x000001, 0x800000, CRC(9a439c98) SHA1(faa43e7c892d1ec76da6e88ccf85b5262faf0f93) )
ROM_END

ROM_START( bbb2d2 ) // demo 2
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "515s02.p1", 0x000000, 0x100000, CRC(b6f02669) SHA1(e35e612fbe7aa829d3f2dab9f5975affe31687de) )

	NEO_SFIX_128K( "515.s1", CRC(e53ef3d0) SHA1(ef3ade7aadc1b57a20c1830bf1d3444dfc9ce1fb) )

	NEO_BIOS_AUDIO_64K( "515s02.m1", CRC(4c134828) SHA1(110848755185eac24b552e949ce22daf37d7085f) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "515s02.v1", 0x000000, 0x400000, CRC(da7cb2b8) SHA1(daccb8bb9c445b5b4a2ce70f5fb008685e063421) )
	ROM_LOAD( "515s02.v2", 0x400000, 0x400000, CRC(0043d4f2) SHA1(22535283d3501feca3459746e4438a677da79432) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "515s02.c1", 0x000000, 0x800000, CRC(c887be9b) SHA1(ed36d80a3dbb1f17c3bf9e8072380a15c8a16857) )
	ROM_LOAD16_BYTE( "515s02.c2", 0x000001, 0x800000, CRC(13c6dd38) SHA1(a6fcc704106f3f64eba60eed9e88325cfda68acd) )
ROM_END

// 516: Violent Vengeance: the Universe Hero by Balek Corp
ROM_START( violentv ) // beta 3.28
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "516b.p1", 0x000000, 0x100000, CRC(59b78bc5) SHA1(12dbf637041a9b0b16a24b830a035407e76798bd) )
	ROM_LOAD16_WORD_SWAP( "516b.p2", 0x100000, 0x800000, CRC(fdad36d7) SHA1(8d8af3bb540185fc25d459608b0c67fd587f49fb) )

	NEO_SFIX_128K( "516.s1", CRC(f7302142) SHA1(5812ff4a2fedaf33ad085d09673ce2a42fda34fb) )

	NEO_BIOS_AUDIO_64K( "516b.m1", CRC(2cf78121) SHA1(e29f9787a98513a5ae3fd4c1ffb374ec1c0b7634) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "516b.v1", 0x000000, 0x800000, CRC(dd128a57) SHA1(d3431e9aa946c3aa294477b46a01e4454ccaf855) )
	ROM_LOAD( "516b.v2", 0x800000, 0x800000, CRC(b6ff9217) SHA1(da84a7ee79cfad282f8ed7b8763604a814edb2fc) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "516b.c1", 0x000000, 0x1000000, CRC(5b3ef4e1) SHA1(7c9a8b22c994d25880e36bb493a16a6b321f950d) )
	ROM_LOAD16_BYTE( "516b.c2", 0x000001, 0x1000000, CRC(970ba8c2) SHA1(c75444236de9aba99f9415f8ab0748376435046e) )
ROM_END


// 539: Shadow Gangs demo by Shadow Gangs
// bugs: screen goes black, sound stops after a few seconds
ROM_START( sgz ) // demo
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "539.p1", 0x000000, 0x100000, CRC(825d0989) SHA1(0b58b061af47df7bfe402fd8207aa7d9f76aecda) )
	ROM_LOAD16_WORD_SWAP( "539.p2", 0x100000, 0x100000, CRC(3d1f0314) SHA1(f3699d294145910fa75a2deee822f9ecd9a53150) )

	NEO_SFIX_128K( "539.s1", CRC(2e4238d1) SHA1(f556cd9afcf53c3a1128cdaa639d49fb3abfa3ed) )

	NEO_BIOS_AUDIO_128K( "539.m1", CRC(11c56b34) SHA1(5c5efc07c0dea8d2d36ba449e00839379b69d889) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "539.v1",   0x000000, 0x200000, CRC(8928bcf2) SHA1(aa139c5bfa79f12188794c373fdc91d6cae972de) )
	// v2 included in the set is pgmdemo.m1, so entirely useless

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "539.c1",   0x0000000, 0x200000, CRC(7679fe24) SHA1(7ad065393103869c80caaac8b90ac324ef165128) )
	ROM_LOAD16_BYTE( "539.c2",   0x0000001, 0x200000, CRC(5374cdad) SHA1(177f5a2b1e9c37b5997e7cd5175e80373a6ccdc9) )
ROM_END


// 600 : 240p Test Suite by Artemio Urbina
ROM_START( 240ptest ) // 0.95
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "600.p1", 0x000000, 0x100000, CRC(5ce36ff3) SHA1(c59beaed2e82955dc2954f844ce9be93b0024bdf) )

	NEO_SFIX_128K( "600.s1", CRC(5023f1ee) SHA1(7bbf9fa1a93daf1d4e3a28b0bc12d228efbdea36) )

	NEO_BIOS_AUDIO_64K( "600.m1", CRC(92065500) SHA1(040bf5a293c4140dd9a5c2a9592cf3fb5835a119) )

	ROM_REGION( 0x040000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "600.v1", 0x000000, 0x040000, CRC(727a6292) SHA1(57666210b311c9f366203cfe88eaf10507eca143) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "600.c1", 0x000000, 0x100000, CRC(7f0e11b9) SHA1(302d82521c90017b84782bc4e50676b38be96dbf) )
	ROM_LOAD16_BYTE( "600.c2", 0x000001, 0x100000, CRC(223c113c) SHA1(8e43ce378aaf0a445fe6f68a71d32f8ca9746082) )
ROM_END

ROM_START( 240ptest1 ) // 1.0
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "600a.p1", 0x000000, 0x100000, CRC(69196380) SHA1(2fb81767ee78eff58cdbe0513ceb3d0b2fdb75f9) )

	NEO_SFIX_128K( "600.s1", CRC(5023f1ee) SHA1(7bbf9fa1a93daf1d4e3a28b0bc12d228efbdea36) )

	NEO_BIOS_AUDIO_64K( "600a.m1", CRC(25058131) SHA1(705241b9c1a6b1d3b031e863910bd70263637857) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "600a.v1", 0x000000, 0x080000, CRC(74730639) SHA1(27a59eaf5ea9b50d0c028b44dc680bf6d6a59a95) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "600a.c1", 0x000000, 0x100000, CRC(1df22c5b) SHA1(e85b7b86fb3d5f50093eb5959f3c53af4da1672b) )
	ROM_LOAD16_BYTE( "600a.c2", 0x000001, 0x100000, CRC(68fbb87d) SHA1(bd22f074f2b8184d98ebe0612999ad146ae6808d) )
ROM_END


// 601 : Shadow of the Beast demo by Neobitz
// No sound
ROM_START( beast )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "601.p1", 0x000000, 0x080000, CRC(1f9e62a4) SHA1(f245ce9802c5deb257a11fa714b2d1d4d72fc786) )

	NEO_SFIX_128K( "601.s1", CRC(05a417f5) SHA1(932eeab5ba06f654ed39112c71a4e5a1f78c70bd) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x080000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "601.c1", 0x000000, 0x80000, CRC(e1974741) SHA1(175b09778875a18f12e7b34c53862ac8c93141c1) )
	ROM_LOAD16_BYTE( "601.c2", 0x000001, 0x80000, CRC(9f5b9cf6) SHA1(857595fc93bfec069c8b1cf647ab9feedce56f1f) )
ROM_END


// 603: MVS 4P Key System Check
// No Sound.
// On first run there's an address error. Press F3.
ROM_START( ng4psyschk )
	ROM_REGION( 0x100000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_WORD_SWAP( "603.p1", 0x000000, 0x100000, CRC(3c3d741b) SHA1(1788eecb02f1ff2a19c1ce04571b08c55203fa04) )

	NEO_SFIX_128K( "262n.s1", CRC(de828076) SHA1(7f79d0245b02e64bd4b50c103722f7e115d1e711) )

	NEO_BIOS_AUDIO_128K( "603.m1", CRC(5132580e) SHA1(1fc374b35f596feaca7d5c6ea416556fa73a378e) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 606 : NeoSystemCheck v1.0b by Blastar
// No sound
ROM_START( syscheck )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "606.p1", 0x000000, 0x80000, CRC(a4adbe1e) SHA1(eee4a8ebe62a2c97282f0350b3c28c615ea44956) )

	NEO_SFIX_64K( "606.s1", CRC(4774f28e) SHA1(f381dac250fca3dfe1eb8c8f1b9b3bf0d521000e) )

	NEO_BIOS_AUDIO_64K( "407.m1", CRC(7669de64) SHA1(caa170b561df4f68000beaad41d942c66a4a10ee) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "407.v1", 0x000000, 0x80000, CRC(504bf849) SHA1(13a184ec9e176371808938015111f8918cb4df7d) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 607 : Neo 2500 demo by Blaster (c) 2004
// No sound
ROM_START( neo2500 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "607.p1", 0x000000, 0x080000, CRC(2321c0d8) SHA1(e0d535af20f271794dc14c83ff3ffbc1f6bf98d9) )

	NEO_SFIX_64K( "607.s1", CRC(1a37925c) SHA1(2060196c3a4d5dcc75aa62e5f02b0d397ec54b1b) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "607.c1", 0x000000, 0x80000, CRC(8c304b56) SHA1(f99298b7942c0bafb6b8038b2ac3b80b75034e34) )
	ROM_LOAD16_BYTE( "607.c2", 0x000001, 0x80000, CRC(9a0758d6) SHA1(ac1c147fe55c80082c9a68a86c30b516ff086cc4) )
ROM_END


// 608 : Twister in a mirror intro by Blastar.
ROM_START( twister )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "608.p1", 0x000000, 0x080000, CRC(514bf0fd) SHA1(d7d084b83ae5ae776d6cc7b7d183bd79b2ab114a) )

	NEO_SFIX_128K( "608.s1", CRC(111ea86c) SHA1(1251d7ef30493b76b15ddc8975a0278ad4a48434) )

	NEO_BIOS_AUDIO_128K( "608.m1", CRC(c1c27b33) SHA1(655c1b01deacacbdbfe3a89196d4b0d9d3aae1eb) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "608.v1", 0x000000, 0x80000, CRC(0c5e044c) SHA1(e5717ea0e6daaf7c529f1e8cf5e4e73ee4a3f26f) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "608.c1",    0x000000, 0x100000, CRC(72ceed51) SHA1(aa5ead993db739cf34917287988aeb43afa7f7d5) )
	ROM_LOAD16_BYTE( "608.c2",    0x000001, 0x100000, CRC(27da57ee) SHA1(323fcd0893aec82cc3c78687834920e2eeedb67b) )
ROM_END


// 609 : Xmas/NewYear2017 demo by Blastar (c) 2016
ROM_START( xmas2017 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "609.p1", 0x000000, 0x004000, CRC(fde7cb36) SHA1(d85ef72dc33e18a7bfebf61ce5bd3d315a1399e6) )

	NEO_SFIX_MT(0x8000)
	ROM_LOAD( "609.s1", 0x000000, 0x008000, CRC(750b643c) SHA1(fb4cac813ba5ab9eb93b1b6391b73b6e3ed0e0a6) )

	NEO_BIOS_AUDIO_64K( "609.m1", CRC(7f4c51e6) SHA1(9d3e4e2a57b7bd38c05398c569baa2b8f7ae5296) )

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "609.v1", 0x00000, 0x40000, CRC(aa5dc85c) SHA1(b07372624110833c292fd2e66332be23b535d8b7) )

	ROM_REGION( 0x20000, "sprites", 0 )
	ROM_LOAD( "609.c1", 0x00000, 0x20000, CRC(18cde781) SHA1(2b28f3ee7a332532a20694436167cf19f33a2753) )
ROM_END


// 610 : NGYM2610 by Blastar (c) 2018
// Bugs: ADPCMA no longer works after the YM emulation was rewritten
ROM_START( ngym2610 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "610.p1", 0x000000, 0x100000, CRC(61c31db6) SHA1(7f43848b5a5ad321849e3c80f640449ba73c3716) )

	NEO_SFIX_128K( "610.s1", CRC(64030dbd) SHA1(9ff13be4b37e0c0985163bb58e5d3d9f0aa392be) )

	NEO_BIOS_AUDIO_128K( "610.m1", CRC(494bf458) SHA1(f6e2bc2c57f9c8bd9233f98663ba6707f2c0a4d9) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "610.v1", 0x000000, 0x1000000, CRC(cb87831f) SHA1(22572f4dbe3a85fe05d0818efeb90611b1078245) )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "610.v21", 0x000000, 0x1000000, CRC(23f83969) SHA1(c29fd0a8ab2b2f8511f8724221c2614dea026503) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "610.c1", 0x000000, 0x100000, CRC(c8050090) SHA1(7168328582c5ce336b58d662fbb59c2d382bf30e) )
	ROM_LOAD16_BYTE( "610.c2", 0x000001, 0x100000, CRC(908d8b09) SHA1(7abd2ab313cbe1b859151b606b3e8facaa2fca25) )
ROM_END


// 611: Neogeo 4 Player Multitab test (from CD by Robbbert)
// No Sound.
ROM_START( ng4ptest )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "611.p1", 0x000000, 0x0028b6, CRC(2326f2f4) SHA1(4c982e750d9ea2e29748f02d5b916b38548daaa5) )

	NEO_SFIX_128K( "611.s1", CRC(4d15e0ca) SHA1(7ae00a68ad479b6a3a7988844649c95e0394b65a) )

	NEO_BIOS_AUDIO_64K( "611.m1", CRC(432dc48d) SHA1(3852feb672ea158cc855cab95c8e92ee9fe11639) )

	ROM_REGION( 0x10000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "611.v1", 0x000000, 0x010000, CRC(62ea940e) SHA1(65e22c4435d3950b1fc3d2949494b5fe3370e855) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 612: Neo Driftout Tech Demo
// No sound.
// Bugs: Can get stuck in position 00
ROM_START( ndo_a_td )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "612.p1", 0x000000, 0x080000, CRC(0b42acff) SHA1(3a52fdedf99acb9a49c2a952465766d1e2cf644d) )

	NEO_SFIX_128K( "612.s1", CRC(00bef8e5) SHA1(e5523d896f04d92981bca4fe270b5f20ad699457) )

	NEO_BIOS_AUDIO_64K( "612.m1", CRC(0125c35b) SHA1(e4d35447e18c2e398afb4d883fd02b620b4e299a) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "612.v1", 0x000000, 0x080000, CRC(23ce128d) SHA1(d6423525ffbb3a59c5fc2fc0f14dcddcfd8c5687) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "612.c1", 0x000000, 0x080000, CRC(3913d0d5) SHA1(c1bb4d336676828c5754f763bc75d310aed6d73f) )
	ROM_LOAD16_BYTE( "612.c2", 0x000001, 0x080000, CRC(0a70ac8e) SHA1(b7371609001ff73907bee49f1e9944fdd269eaab) )
ROM_END


// 613 : WW2 demo by Rastersoft
// No sound
// The supplied m1 gives a Z80 ERROR, so it has been replaced.
// If the game doesn't work, read the FAQ.
// Bugs: Insert Coin freezes HBMAME.
ROM_START( ww2demo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "613.p1", 0x000000, 0x80000, CRC(d53762ff) SHA1(231174363266ec1d5a028295f36bddfbf0a54235) )

	NEO_SFIX_64K( "613.s1", CRC(3fda5d1a) SHA1(ec0a9e008f56d694184e6b553c271763529eb711) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )
	// Z80 error NEO_BIOS_AUDIO_128K( "613.m1", CRC(99c2354e) SHA1(fa2ea40ae3d01bda909c82b6c4baa01335c6fe8f) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "613.c1", 0x000000, 0x100000, CRC(955efd3e) SHA1(b1c3ebff7158f1653b3b9bc5a52b06d8c62da1bb) )
	ROM_LOAD16_BYTE( "613.c2", 0x000001, 0x100000, CRC(c213940f) SHA1(430c017f93ee87127a146bcfba684c840bc69f5b) )
ROM_END


// 616 : Neogeo chaos demo by Charles Doty
// Left-right will move the turning logo
// No sound
ROM_START( neodemo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "616.p1", 0x000000, 0x080000, CRC(fbfce2a4) SHA1(53472533456e131bee9fa792996b4110999b5c21) )

	NEO_SFIX_128K( "469.s1", CRC(cd19264f) SHA1(531be2305cd56d332fb7a53ab924214ade34a9e8) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "616.c1", 0x000000, 0x100000, CRC(870b28ea) SHA1(255f8e394534afc929bad31f253a2e8b03253e07) )
	ROM_LOAD16_BYTE( "616.c2", 0x000001, 0x100000, CRC(1ecf8128) SHA1(66d200ceb023a1ff3cbe4ef7c5b1ef0068ab47f0) )
ROM_END


// 620 : Castlevania demo by BarfHappy
// No sound.
ROM_START( neocstlv )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "620.p1", 0x000000, 0x080000, CRC(8d6cd9f2) SHA1(23258690b0268367420cb25ef951cfb15f79b88e) )

	NEO_SFIX( 0x40000, "620.s1", CRC(be950195) SHA1(883a99b79b495f66f81025bf4c1f5fb23cc0b4c7) )

	NEO_BIOS_AUDIO_128K( "620.m1", CRC(89c71460) SHA1(3c675b9b0e46be2cafd2aa8858a40efd07034fcd) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "620.c1", 0x000000, 0x100000, CRC(960a7414) SHA1(753a8af8bfe9a6e9322744b9a31c8d8ef3615cae) )
	ROM_LOAD16_BYTE( "620.c2", 0x000001, 0x100000, CRC(24a961d0) SHA1(c11dd77c9df0f829461bb3aebb3c6287b7e703f9) )
ROM_END


// 621 : 3D demo by Oxygene
ROM_START( neo3d )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "621.p1", 0x000000, 0x080000, CRC(791f6042) SHA1(d44848322d08daad91c67c7dc7e9ee985d582b2b) )

	NEO_SFIX_128K( "469.s1", CRC(cd19264f) SHA1(531be2305cd56d332fb7a53ab924214ade34a9e8) )

	NEO_BIOS_AUDIO_128K( "621.m1", CRC(7e74cc1f) SHA1(2855577a9785baa4076567de0b37fc0d0117abe5) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "202.v1", 0x000000, 0x80000, CRC(debeb8fb) SHA1(49a3d3578c087f1a0050168571ef8d1b08c5dc05) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "621.c1", 0x000000, 0x100000, CRC(a7eaca76) SHA1(7b046932466092e250dc879d76656227102037c5) )
	ROM_LOAD16_BYTE( "621.c2", 0x000001, 0x100000, CRC(042f2cde) SHA1(225b03541ac3d5258de2c33d93e46e068798731f) )
ROM_END


// 624 : Transparency demo by NGF
ROM_START( ngftdemo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "624.p1", 0x000000, 0x080000, CRC(84d87190) SHA1(7b944603a47a062c050faf9d7e2f9ed754c325cf) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "624.m1", CRC(5ea216be) SHA1(5ae7e878a08dcd5a1a3b5d25abfe98eea0dfc7d3) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "624.v1", 0x000000, 0x200000, CRC(d03c87eb) SHA1(033815e3cf8fa3732c481ef308aacea595e3e008) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "624.c1", 0x000000, 0x200000, CRC(b2fba994) SHA1(278bc9b2f969cec8160dbea879426bbf2c7ea2d4) )
	ROM_LOAD16_BYTE( "624.c2", 0x000001, 0x200000, CRC(37495ab2) SHA1(b6caba50aaf7cf73cfd5c23302fac4785baafe43) )
ROM_END


// 625 : Chip n Dale intro from Raregame
ROM_START( cndi )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "625.p1", 0x000000, 0x800000, CRC(2df9a41d) SHA1(f08d81f529f17d22218b4bb52840f4a13f5821a2) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "625.m1", CRC(591adc4f) SHA1(02886106b5bbcd1c63a7ccd5c91456a785b0621a) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "625.v1", 0x000000, 0x091000, CRC(15d318a6) SHA1(10f99dbded4093bac104ce954e93dba7ce8c4ade) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "625.c1", 0x0000000, 0x400000, CRC(55bd32fc) SHA1(9ca14628b50182257e812a64ae7779b677f49b3a) )
	ROM_LOAD16_BYTE( "625.c2", 0x0000001, 0x400000, CRC(bbb43733) SHA1(cbb698354f34045a08f3f30ccd54796374c47ab9) )
	ROM_LOAD16_BYTE( "625.c3", 0x0800000, 0x400000, CRC(715e0318) SHA1(ceab1079b9e8d53f32f0aafce5d7cda037a91e02) )
	ROM_LOAD16_BYTE( "625.c4", 0x0800001, 0x400000, CRC(016f3c7e) SHA1(05fc02c571008dfa83eea11dc596248cee53153a) )
	ROM_LOAD16_BYTE( "625.c5", 0x1000000, 0x400000, CRC(28c6cb8c) SHA1(05acb12720a12918debed333243b4bc42555a0b3) )
	ROM_LOAD16_BYTE( "625.c6", 0x1000001, 0x400000, CRC(9c89503f) SHA1(8c3fc78f7bb903684367d663ede13b87c72c8cca) )
	ROM_LOAD16_BYTE( "625.c7", 0x1800000, 0x400000, CRC(074dd407) SHA1(83f4c80ab88d506152b375480b2fabb0e4ed2eaa) )
	ROM_LOAD16_BYTE( "625.c8", 0x1800001, 0x400000, CRC(5ddb7db5) SHA1(29cf08e39d7454fdeb7ab4a13afff1d422c7c859) )
ROM_END

// 625 : Chip n Dale intro from Raregame
ROM_START( cndia )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "625.p1", 0x000000, 0x800000, CRC(2df9a41d) SHA1(f08d81f529f17d22218b4bb52840f4a13f5821a2) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "625a.m1", CRC(ae120a59) SHA1(11a49a82007810890ac01757af217ce76c78335b) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "625a.v1", 0x000000, 0x092000, CRC(91d45537) SHA1(2e3fe9733a49a7d5597fea40f59274133c331795) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "625.c1", 0x0000000, 0x400000, CRC(55bd32fc) SHA1(9ca14628b50182257e812a64ae7779b677f49b3a) )
	ROM_LOAD16_BYTE( "625.c2", 0x0000001, 0x400000, CRC(bbb43733) SHA1(cbb698354f34045a08f3f30ccd54796374c47ab9) )
	ROM_LOAD16_BYTE( "625.c3", 0x0800000, 0x400000, CRC(715e0318) SHA1(ceab1079b9e8d53f32f0aafce5d7cda037a91e02) )
	ROM_LOAD16_BYTE( "625.c4", 0x0800001, 0x400000, CRC(016f3c7e) SHA1(05fc02c571008dfa83eea11dc596248cee53153a) )
	ROM_LOAD16_BYTE( "625.c5", 0x1000000, 0x400000, CRC(28c6cb8c) SHA1(05acb12720a12918debed333243b4bc42555a0b3) )
	ROM_LOAD16_BYTE( "625.c6", 0x1000001, 0x400000, CRC(9c89503f) SHA1(8c3fc78f7bb903684367d663ede13b87c72c8cca) )
	ROM_LOAD16_BYTE( "625.c7", 0x1800000, 0x400000, CRC(074dd407) SHA1(83f4c80ab88d506152b375480b2fabb0e4ed2eaa) )
	ROM_LOAD16_BYTE( "625.c8", 0x1800001, 0x400000, CRC(5ddb7db5) SHA1(29cf08e39d7454fdeb7ab4a13afff1d422c7c859) )
ROM_END


// 626 : Darkwing Duck intro from Raregame
ROM_START( dwi )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "626.p1", 0x000000, 0x800000, CRC(8e3f2ea8) SHA1(80efd45770938b2dc3ac8c67d823bae4369a68aa) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "626.s1", CRC(3dc5c1ef) SHA1(fa194481524cd95ec7188fef0857ef4d2435d7eb) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "626.m1", CRC(2249e065) SHA1(3dc737888846acfbe693452b03ee2b5c09ff9408) )

	ROM_RELOAD( 0x10000, 0x2000 )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "626.v1", 0x000000, 0x09b000, CRC(13bb4ce4) SHA1(0762db7991a5958ae7d8fd5cad2a850f1803a372) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "626.c1", 0x0000000, 0x400000, CRC(2b8bda94) SHA1(68a8239a620dba52762fec548b44c7d0e703fca3) )
	ROM_LOAD16_BYTE( "626.c2", 0x0000001, 0x400000, CRC(5345e4de) SHA1(eee16fc63c901184b1b59dc3edcea260e82b5bf5) )
	ROM_LOAD16_BYTE( "626.c3", 0x0800000, 0x400000, CRC(c6185f14) SHA1(46539c90a2c3e59b7736925cdd59700ca99ac1e1) )
	ROM_LOAD16_BYTE( "626.c4", 0x0800001, 0x400000, CRC(c9e59701) SHA1(67b5c396591e00ca6d58d35b7d95465d6a65d3eb) )
	ROM_LOAD16_BYTE( "626.c5", 0x1000000, 0x400000, CRC(42dd1c90) SHA1(1ca19d11a7d0f9f8dbedf5c8fe2eadfc28337ad9) )
	ROM_LOAD16_BYTE( "626.c6", 0x1000001, 0x400000, CRC(9fa6d651) SHA1(653592013a44b804b713302f17b4109b316662ce) )
	ROM_LOAD16_BYTE( "626.c7", 0x1800000, 0x400000, CRC(5cfa6886) SHA1(e550e898f64a2ad9992aaa4ff7a64d6881294ce8) )
	ROM_LOAD16_BYTE( "626.c8", 0x1800001, 0x400000, CRC(59c37871) SHA1(7413282800d4461ea151d10b91fa24fef1df94a0) )
ROM_END

// 626 : Darkwing Duck intro from Raregame
ROM_START( dwia )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "626.p1", 0x000000, 0x800000, CRC(8e3f2ea8) SHA1(80efd45770938b2dc3ac8c67d823bae4369a68aa) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "626a.m1", CRC(b9f8df96) SHA1(8472b7308df7b7d3f1fc8824b17d81f670e6d68e) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "626.v1", 0x000000, 0x09b000, CRC(13bb4ce4) SHA1(0762db7991a5958ae7d8fd5cad2a850f1803a372) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "626.c1", 0x0000000, 0x400000, CRC(2b8bda94) SHA1(68a8239a620dba52762fec548b44c7d0e703fca3) )
	ROM_LOAD16_BYTE( "626.c2", 0x0000001, 0x400000, CRC(5345e4de) SHA1(eee16fc63c901184b1b59dc3edcea260e82b5bf5) )
	ROM_LOAD16_BYTE( "626.c3", 0x0800000, 0x400000, CRC(c6185f14) SHA1(46539c90a2c3e59b7736925cdd59700ca99ac1e1) )
	ROM_LOAD16_BYTE( "626.c4", 0x0800001, 0x400000, CRC(c9e59701) SHA1(67b5c396591e00ca6d58d35b7d95465d6a65d3eb) )
	ROM_LOAD16_BYTE( "626.c5", 0x1000000, 0x400000, CRC(42dd1c90) SHA1(1ca19d11a7d0f9f8dbedf5c8fe2eadfc28337ad9) )
	ROM_LOAD16_BYTE( "626.c6", 0x1000001, 0x400000, CRC(9fa6d651) SHA1(653592013a44b804b713302f17b4109b316662ce) )
	ROM_LOAD16_BYTE( "626.c7", 0x1800000, 0x400000, CRC(5cfa6886) SHA1(e550e898f64a2ad9992aaa4ff7a64d6881294ce8) )
	ROM_LOAD16_BYTE( "626.c8", 0x1800001, 0x400000, CRC(59c37871) SHA1(7413282800d4461ea151d10b91fa24fef1df94a0) )
ROM_END


// 627 : Ghostbusters intro from Raregame
ROM_START( gbi )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "627.p1", 0x000000, 0x100000, CRC(af69ef5f) SHA1(e218d1c99e491afcf1887ef9f5bed3e7a5e53fe4) )
	ROM_LOAD16_WORD_SWAP( "627.p2", 0x100000, 0x400000, CRC(f57e393f) SHA1(4e4ac08db6cc28bcc47808854b1ceb407a486fe5) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "627.m1", CRC(6624e642) SHA1(03a723c477905425ced965a63fb6d006abd045e9) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "627.v1", 0x000000, 0x094a00, CRC(44864761) SHA1(35334b93c65b3e27338290c2ba1f73aa902aa893) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "627.c1", 0x0000000, 0x400000, CRC(4281fecd) SHA1(a4f4c278b28740cde042f81769c8c10e5d274fcf) )
	ROM_LOAD16_BYTE( "627.c2", 0x0000001, 0x400000, CRC(0f07018c) SHA1(eccd998c8fe4c4e3039f104dbdc89713cd9e10cb) )
	ROM_LOAD16_BYTE( "627.c3", 0x0800000, 0x400000, CRC(a4589b2a) SHA1(ff3784921b093f61c3f620c26f296b8f4f798028) )
	ROM_LOAD16_BYTE( "627.c4", 0x0800001, 0x400000, CRC(2a72bd04) SHA1(8d30b1851ca706b669d8d02b1d17a2b40a424b8d) )
	ROM_LOAD16_BYTE( "627.c5", 0x1000000, 0x400000, CRC(ad3b72f9) SHA1(0274f7b85cb1a53a5f98399ac1952418803ab765) )
	ROM_LOAD16_BYTE( "627.c6", 0x1000001, 0x400000, CRC(963e0941) SHA1(04ae20247e74f27721a2a87f0acd41a733c6f753) )
ROM_END


// 628 : Robocop intro from Raregame
ROM_START( rci )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "628.p1", 0x000000, 0x800000, CRC(adcb4fe0) SHA1(596242e344d1e171eed7ef242c10aec1a0090acf) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "628.m1", CRC(c4d35e83) SHA1(0f2bb58e6e545090a11f7fbf471dcd46768a313b) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "628.v1", 0x000000, 0x096000, CRC(ecb284a9) SHA1(1ff34e6f3091ddf0da7b8afb06b903ab39d0b629) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "628.c1", 0x0000000, 0x400000, CRC(8cf565a0) SHA1(c8d383d96b03be9e96fcea94b355c5c33717b4e1) )
	ROM_LOAD16_BYTE( "628.c2", 0x0000001, 0x400000, CRC(76ef561d) SHA1(3dfd4d923b7381e8f1ec4512915a05efa299e255) )
	ROM_LOAD16_BYTE( "628.c3", 0x0800000, 0x400000, CRC(7166790c) SHA1(778db868373c4fa00899f224339c0d171519002a) )
	ROM_LOAD16_BYTE( "628.c4", 0x0800001, 0x400000, CRC(c0cdcbfc) SHA1(d2bf8c0560e593820f2a556779602cabb3272b8d) )
	ROM_LOAD16_BYTE( "628.c5", 0x1000000, 0x400000, CRC(a5ddc363) SHA1(d2bee74072c2e1b5cac0fc422472aa5d53bc7358) )
	ROM_LOAD16_BYTE( "628.c6", 0x1000001, 0x400000, CRC(1fcc8fce) SHA1(3a7ecfc3a37b315a0a5c7a0127c9412f8214fd81) )
	ROM_LOAD16_BYTE( "628.c7", 0x1800000, 0x400000, CRC(be9f9da2) SHA1(9d619074b368db463182be999c2350b815ee2779) )
	ROM_LOAD16_BYTE( "628.c8", 0x1800001, 0x400000, CRC(7cff633a) SHA1(4d02abcb21dd20ffe7dfc94579fc6cbfaf1bf14e) )
ROM_END


// 629 : Spiderman intro from Raregame
ROM_START( smi )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "629.p1", 0x000000, 0x100000, CRC(66b5cfe8) SHA1(65754292a085fce07a04fbe8b91a62c42a9695b1) )
	ROM_LOAD16_WORD_SWAP( "629.p2", 0x100000, 0x400000, CRC(72082aea) SHA1(a56a8d9196c7d6cc5d66204623cce964e1feda05) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "629.m1", CRC(6a02cb1d) SHA1(fffd39708bfb121496c78d01e53f07aa9a539c47) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "629.v1", 0x000000, 0x096100, CRC(95c138da) SHA1(7bfcd525ecf01737ec85db88355323d676f1f03b) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "629.c1", 0x0000000, 0x400000, CRC(8e5ccfdd) SHA1(a26a3cadf67f864ded64daca3af13a8db8a73b62) )
	ROM_LOAD16_BYTE( "629.c2", 0x0000001, 0x400000, CRC(a6406f5b) SHA1(fe63d5aba68bc51861d44ad3ec385e05fd3198c5) )
	ROM_LOAD16_BYTE( "629.c3", 0x0800000, 0x400000, CRC(adfff926) SHA1(39a1c634168803181b43843dbd562405ed7a8fe6) )
	ROM_LOAD16_BYTE( "629.c4", 0x0800001, 0x400000, CRC(068c307f) SHA1(47d52fa960a2451f20d6b45c87aef7f5b78e00a5) )
	ROM_LOAD16_BYTE( "629.c5", 0x1000000, 0x400000, CRC(9a5cf7a6) SHA1(2c6a0fd9139152a85f936323f9e8a26b13819fc2) )
	ROM_LOAD16_BYTE( "629.c6", 0x1000001, 0x400000, CRC(934f03c0) SHA1(2a54f8fb1be63dcfef2756b9fa5904af3a2ca874) )
ROM_END


// 630 : Teenage Mutant Ninja Turtles intro from Raregame
ROM_START( tmnti )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "630.p1", 0x000000, 0x800000, CRC(2d5066ff) SHA1(e1109c1e7532fa167ba5c9c90648d019740efd8e) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "626.s1", CRC(3dc5c1ef) SHA1(fa194481524cd95ec7188fef0857ef4d2435d7eb) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "630.m1", CRC(953542ef) SHA1(95e93833b938b8d374d29f925777cf296468ae9e) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "630.v1", 0x000000, 0x094900, CRC(273640c3) SHA1(6d445e728fa9c4af00de4f454a73651de7a69ec9) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "630.c1", 0x0000000, 0x400000, CRC(ae1735c1) SHA1(a7a943fb088bccbd5e2c0fa34766bab8169000fa) )
	ROM_LOAD16_BYTE( "630.c2", 0x0000001, 0x400000, CRC(344281b5) SHA1(a7de77ce07bcd87ba6348278e9c2a61e0f96a499) )
	ROM_LOAD16_BYTE( "630.c3", 0x0800000, 0x400000, CRC(1058a648) SHA1(878384e1bb8e7666723935a6270d61d3015fe475) )
	ROM_LOAD16_BYTE( "630.c4", 0x0800001, 0x400000, CRC(1c6ba1b0) SHA1(065f059ef412c69c1a6e7d96e0d41d4151802c95) )
	ROM_LOAD16_BYTE( "630.c5", 0x1000000, 0x400000, CRC(73a89fb0) SHA1(6b17c878cf6e33f98f3d0779e908264a95a186ba) )
	ROM_LOAD16_BYTE( "630.c6", 0x1000001, 0x400000, CRC(a139246a) SHA1(facc1caa4f5e4f8476df75c3f68802184072a8ee) )
	ROM_LOAD16_BYTE( "630.c7", 0x1800000, 0x400000, CRC(ed957842) SHA1(0066baf544b87f7d99334401a93c239ce151df50) )
	ROM_LOAD16_BYTE( "630.c8", 0x1800001, 0x400000, CRC(8c15d91b) SHA1(4b4d4fd2302c43a5c82bb849803d13d43947a695) )
ROM_END

ROM_START( tmntia )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "630a.p1", 0x000000, 0x800000, CRC(ed4f5fec) SHA1(6f3d00137b44ab2734d84bc6cea4ea58b9b2350d) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "626.s1", CRC(3dc5c1ef) SHA1(fa194481524cd95ec7188fef0857ef4d2435d7eb) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "630.m1", CRC(953542ef) SHA1(95e93833b938b8d374d29f925777cf296468ae9e) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "630.v1", 0x000000, 0x094900, CRC(273640c3) SHA1(6d445e728fa9c4af00de4f454a73651de7a69ec9) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "630.c1", 0x0000000, 0x400000, CRC(ae1735c1) SHA1(a7a943fb088bccbd5e2c0fa34766bab8169000fa) )
	ROM_LOAD16_BYTE( "630.c2", 0x0000001, 0x400000, CRC(344281b5) SHA1(a7de77ce07bcd87ba6348278e9c2a61e0f96a499) )
	ROM_LOAD16_BYTE( "630.c3", 0x0800000, 0x400000, CRC(1058a648) SHA1(878384e1bb8e7666723935a6270d61d3015fe475) )
	ROM_LOAD16_BYTE( "630.c4", 0x0800001, 0x400000, CRC(1c6ba1b0) SHA1(065f059ef412c69c1a6e7d96e0d41d4151802c95) )
	ROM_LOAD16_BYTE( "630.c5", 0x1000000, 0x400000, CRC(73a89fb0) SHA1(6b17c878cf6e33f98f3d0779e908264a95a186ba) )
	ROM_LOAD16_BYTE( "630.c6", 0x1000001, 0x400000, CRC(a139246a) SHA1(facc1caa4f5e4f8476df75c3f68802184072a8ee) )
	ROM_LOAD16_BYTE( "630.c7", 0x1800000, 0x400000, CRC(ed957842) SHA1(0066baf544b87f7d99334401a93c239ce151df50) )
	ROM_LOAD16_BYTE( "630.c8", 0x1800001, 0x400000, CRC(8c15d91b) SHA1(4b4d4fd2302c43a5c82bb849803d13d43947a695) )
ROM_END


// 631 : Duck Tales intro from Raregame
ROM_START( dti )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "631.p1", 0x000000, 0x800000, CRC(f975711c) SHA1(0bce8bf92536dd18920cdc1ab66a9c42b0a96df3) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "631.m1", CRC(7761d993) SHA1(b269e9878ca28faa1d36c7fa3dd8e9939714a5f4) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "631.v1", 0x000000, 0x090200, CRC(b40c3a5a) SHA1(fda5e4bf3af75aa70df32b21f132c41670212c52) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "631.c1", 0x0000000, 0x400000, CRC(02adabba) SHA1(7b1eafebed09dbfc1b8d9612f0ed477e25ab152d) )
	ROM_LOAD16_BYTE( "631.c2", 0x0000001, 0x400000, CRC(00bd87ac) SHA1(cb3c02c799109fa7083110825dacf19c89372c55) )
	ROM_LOAD16_BYTE( "631.c3", 0x0800000, 0x400000, CRC(263307c8) SHA1(5dabc76b18c02c6955abfbc5010b8fbac7d15753) )
	ROM_LOAD16_BYTE( "631.c4", 0x0800001, 0x400000, CRC(b8330c1e) SHA1(e3ab645eb1101e561bc581e6b2cf696a0ca1e673) )
	ROM_LOAD16_BYTE( "631.c5", 0x1000000, 0x400000, CRC(ce99fcb8) SHA1(f6222f1b0dc31e5ae5e6c7e3223aa7129b38b772) )
	ROM_LOAD16_BYTE( "631.c6", 0x1000001, 0x400000, CRC(a6d30781) SHA1(a29e6b39169f9354a64cf64b302b9e91e0671c82) )
	ROM_LOAD16_BYTE( "631.c7", 0x1800000, 0x400000, CRC(eda15323) SHA1(ef7291870fd30e7b04cb63e59ff45a3c0acab714) )
	ROM_LOAD16_BYTE( "631.c8", 0x1800001, 0x400000, CRC(400fd4cd) SHA1(20cc401de5d429deb7d076535a0b42fc62c48c52) )
ROM_END

// 631 : Duck Tales intro from Raregame
ROM_START( dtia )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "631.p1", 0x000000, 0x800000, CRC(f975711c) SHA1(0bce8bf92536dd18920cdc1ab66a9c42b0a96df3) )
	ROM_FILL(0x30A,1,0x70)
	ROM_FILL(0x30B,1,0x04)

	NEO_SFIX_64K( "627.s1", CRC(63b8b25e) SHA1(1dacfaebfa68d1b2518324c2bc000f310ed8fc3f) )

	NEO_JAPAN_BIOS_AUDIO(0x2000, "631.m1", CRC(7761d993) SHA1(b269e9878ca28faa1d36c7fa3dd8e9939714a5f4) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "631a.v1", 0x000000, 0x090200, CRC(24b716ee) SHA1(ec153b7f4054644a8dbeacfc1c03a464a7c47c3b) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "631.c1", 0x0000000, 0x400000, CRC(02adabba) SHA1(7b1eafebed09dbfc1b8d9612f0ed477e25ab152d) )
	ROM_LOAD16_BYTE( "631.c2", 0x0000001, 0x400000, CRC(00bd87ac) SHA1(cb3c02c799109fa7083110825dacf19c89372c55) )
	ROM_LOAD16_BYTE( "631.c3", 0x0800000, 0x400000, CRC(263307c8) SHA1(5dabc76b18c02c6955abfbc5010b8fbac7d15753) )
	ROM_LOAD16_BYTE( "631.c4", 0x0800001, 0x400000, CRC(b8330c1e) SHA1(e3ab645eb1101e561bc581e6b2cf696a0ca1e673) )
	ROM_LOAD16_BYTE( "631.c5", 0x1000000, 0x400000, CRC(ce99fcb8) SHA1(f6222f1b0dc31e5ae5e6c7e3223aa7129b38b772) )
	ROM_LOAD16_BYTE( "631.c6", 0x1000001, 0x400000, CRC(a6d30781) SHA1(a29e6b39169f9354a64cf64b302b9e91e0671c82) )
	ROM_LOAD16_BYTE( "631.c7", 0x1800000, 0x400000, CRC(eda15323) SHA1(ef7291870fd30e7b04cb63e59ff45a3c0acab714) )
	ROM_LOAD16_BYTE( "631.c8", 0x1800001, 0x400000, CRC(400fd4cd) SHA1(20cc401de5d429deb7d076535a0b42fc62c48c52) )
ROM_END


// 635 : Neo Rom Jukebox by CeL
ROM_START( neoromjb )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "635.p1", 0x000000, 0x080000, CRC(fc301f3d) SHA1(e7e02963167a612e29e14cfaa53a8eb441961885) )

	NEO_SFIX_64K( "436.s1", CRC(d78bd9a0) SHA1(17a237166aede98bee27d5b52654414c8a1b071a) ) // same rom is in Neo Pang

	NEO_BIOS_AUDIO_128K( "635.m1", CRC(60bf52f5) SHA1(86fcb7693ce0c3ee97f3fd2715d3bcb575511cd7) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "635.v1", 0x000000, 0x080000, CRC(68a2a556) SHA1(8343465cca82db43c0decea74c314f641e4bbb7a) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 639 : Primo demo by BEY
// No sound.
ROM_START( iocero )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "639.p1", 0x000000, 0x100000, CRC(a315fb81) SHA1(c958d78de5d83ff31c9545d54c6429c85cdbdb64) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "639.c1", 0x000000, 0x100000, CRC(d68941e1) SHA1(20b70416b9b284bae576b385ddc63fe9a47cf976) )
	ROM_LOAD16_BYTE( "639.c2", 0x000001, 0x100000, CRC(df7e359f) SHA1(08d45350047ad9bcad17739d8b00b911b47417ee) )
ROM_END


// 640 : Galaga demo by BEY
// No sound.
ROM_START( neogal1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "640_1.p1", 0x000000, 0x100000, CRC(7226db5c) SHA1(d598096265286b5d32a3c6abbea8c281ee94458c) )

	NEO_SFIX( 0x2000, "640_1.s1", CRC(90557449) SHA1(a6aac83715a5d2c7d0436c3d0b80d61459316ee7) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "640_1.c1", 0x000000, 0x100000, CRC(6de8c6f6) SHA1(51d29bab7b482a5a850af08f9939a42e7127df3b) )
	ROM_LOAD16_BYTE( "640_1.c2", 0x000001, 0x100000, CRC(fa5f0c1d) SHA1(8e8fb1f0314450ab9cdbd9619a9cdccdd78fea99) )
ROM_END

// No sound.
ROM_START( neogal1t )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "640_1t.p1", 0x000000, 0x100000, CRC(01dc85fd) SHA1(7fed5c859901066e27bac777d3846e69185648e6) )

	NEO_SFIX( 0x2000, "640_1.s1", CRC(90557449) SHA1(a6aac83715a5d2c7d0436c3d0b80d61459316ee7) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "640_1t.c1", 0x000000, 0x100000, CRC(f8221e34) SHA1(c8bb90c03213dc493bdaa36d94c713508303a275) )
	ROM_LOAD16_BYTE( "640_1t.c2", 0x000001, 0x100000, CRC(c30cf1fc) SHA1(774a1896fc15cdf9fddf1e1a874ea43fe4136bd6) )
ROM_END

// No sound.
ROM_START( neogal1d )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "640_1d.p1", 0x000000, 0x100000, CRC(2c49e6ec) SHA1(69602bce2a119f644385b1cfda5ee38dbc081679) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "640_1.c1", 0x000000, 0x100000, CRC(6de8c6f6) SHA1(51d29bab7b482a5a850af08f9939a42e7127df3b) )
	ROM_LOAD16_BYTE( "640_1.c2", 0x000001, 0x100000, CRC(fa5f0c1d) SHA1(8e8fb1f0314450ab9cdbd9619a9cdccdd78fea99) )
ROM_END

ROM_START( neogal2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "640_2.p1", 0x000000, 0x100000, CRC(a6321942) SHA1(e2085c677e124708770846c2b9f1d3ba9e6a4323) )

	NEO_SFIX_128K( "419.s1", CRC(a545b593) SHA1(09077b63595eebb7dddd55e041e7786164df0ead) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "202.v1", 0x000000, 0x80000, CRC(debeb8fb) SHA1(49a3d3578c087f1a0050168571ef8d1b08c5dc05) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "640_1.c1", 0x000000, 0x100000, CRC(6de8c6f6) SHA1(51d29bab7b482a5a850af08f9939a42e7127df3b) )
	ROM_LOAD16_BYTE( "640_1.c2", 0x000001, 0x100000, CRC(fa5f0c1d) SHA1(8e8fb1f0314450ab9cdbd9619a9cdccdd78fea99) )
ROM_END


// 641 : NeoGeo Sound Test by BEY
// Bugs: Insert Coin causes reboot
ROM_START( neotest )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "641.p1", 0x000000, 0x80000, CRC(2ee900b2) SHA1(4b083f4e1e502ac17e3e396d8655870dbac7fba7) )

	NEO_SFIX( 0x2000, "640_1.s1", CRC(90557449) SHA1(a6aac83715a5d2c7d0436c3d0b80d61459316ee7) )

	NEO_BIOS_AUDIO_128K( "641.m1", CRC(ad14cc39) SHA1(852f71ebb54c2c26795e8a2771d1638e42e0c634) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "641.v1", 0x000000, 0x100000, CRC(c1c07bd5) SHA1(79a25211590dc1192913a7655d7f2d1682acdc97) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "640_1.c1", 0x000000, 0x100000, CRC(6de8c6f6) SHA1(51d29bab7b482a5a850af08f9939a42e7127df3b) )
	ROM_LOAD16_BYTE( "640_1.c2", 0x000001, 0x100000, CRC(fa5f0c1d) SHA1(8e8fb1f0314450ab9cdbd9619a9cdccdd78fea99) )
ROM_END


// 642 : Bad Apple demo by BEY
// No colours
ROM_START( badapple )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD( "642.p1", 0x000000, 0x09e0f, CRC(c8068de3) SHA1(f240b23f5f2aed998ebcbb95680225e3e35a8051) )
	ROM_LOAD16_WORD( "642.p2", 0x100000, 0xeeb5c, CRC(f5fe8e16) SHA1(68398a9719eb9f4e42668991384683965c5787bf) )
	ROM_LOAD16_WORD( "642.p3", 0x200000, 0xf4764, CRC(a70f49bd) SHA1(1b8a70e0f5467ea750586e3c0cb7a6188dc482d2) )
	ROM_LOAD16_WORD( "642.p4", 0x300000, 0xf4764, CRC(dbf6bc37) SHA1(ecfdfa79811d586ace3577ccb7bc1c535482fe84) )
	ROM_LOAD16_WORD( "642.p5", 0x400000, 0x8122c, CRC(55ca8ba8) SHA1(899d974765d28b342d2fa0ce5e66734a09278ed0) )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "642.m1", CRC(ad7f4d47) SHA1(8700ed799c013db270c5b80002c23733dfaaa534) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "642.v1", 0x000000, 0x300000, CRC(ce9bf279) SHA1(d14133414570d30ac477c226771175fa4b54073b) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_WORD( "642.c1",  0x000000, 0x5a3b80, CRC(5fd2d36d) SHA1(13a045627029ade7a1e0f5cdcce3bf2b92de9559) )
	ROM_LOAD16_WORD( "642.c2",  0x600000, 0x5bb580, CRC(5cbbc6e0) SHA1(aad04b9b7cdf7c3951006b39676ee4ee989338b0) )
	ROM_LOAD16_WORD( "642.c3",  0xc00000, 0x5b1200, CRC(befd5392) SHA1(49f0610b9a4e1bdbb881c2156f01631cc45915c1) )
	ROM_LOAD16_WORD( "642.c4",  0x1200000, 0x2c7480, CRC(e4b70cf5) SHA1(af717a650dbde0c4a852f8ee87a1d55ac9349172) )
ROM_END

ROM_START( badapplea )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD( "642a.p1",    0x000000, 0x00c5b0, CRC(f916d0bf) SHA1(21b7bdb47893e00ef77abca9aba831059e0d48dc) )
	ROM_LOAD( "642a.p2",    0x100000, 0x0c628c, CRC(6e0e218c) SHA1(f15ceae5c9fb4b268d8b1c6513a3954746ace550) )
	ROM_LOAD( "642a.p3",    0x200000, 0x0872cc, CRC(475877e0) SHA1(f6fa2aeab36264a544895bb49c3858286a3edde7) )
	ROM_LOAD( "642a.p4",    0x300000, 0x08ced4, CRC(9529aad1) SHA1(54ce5fa06e915383430b4fc2eb4b2bd8d5d0ca62) )
	ROM_LOAD( "642a.p5",    0x400000, 0x08ca3c, CRC(1fa70a3b) SHA1(d4f5560a38d057081fc16d7c3e673171bd194b72) )
	ROM_LOAD( "642a.p6",    0x500000, 0x0c99c4, CRC(7b7bc0e2) SHA1(6bd31d2e4e424ac27117b4c94273ae8cb779f17d) )
	ROM_LOAD( "642a.p7",    0x600000, 0x09824c, CRC(9a331574) SHA1(38454cca5e9dc003aa575b2463dd644a1f46a206) )
	ROM_LOAD( "642a.p8",    0x700000, 0x067d3c, CRC(473889f4) SHA1(5120a1dd1ff8fa30836ac934f08cae73616983e7) )
	ROM_LOAD( "642a.p9",    0x800000, 0x04b114, CRC(73424cd1) SHA1(fe48a88c10925e17e25e02d7728c80a7f72dd24f) )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "642a.m1", CRC(6432e6e8) SHA1(5cc0547320cf6ccb4c5641497d5469f867d33eee) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "642a.v1",    0x000000, 0x400000, CRC(77656df1) SHA1(c95a1b09ee77020c19d5ab58de4ab8dc8b94a3b4) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD( "642a.c0",    0x0000000, 0x413b00, CRC(d5a0e4dc) SHA1(a3a34ba916c7df9ffdafdc25579d627784cfbdf1) )
	ROM_LOAD( "642a.c1",    0x0500000, 0x464d80, CRC(35da6dbc) SHA1(f5cc58620074a11363f0937f16c0eec6b6c0438e) )
	ROM_LOAD( "642a.c2",    0x0a00000, 0x398e80, CRC(34482b96) SHA1(a6a88e99d55a56a9e84e65e9edc68099214cdbd6) )
	ROM_LOAD( "642a.c3",    0x0f00000, 0x439780, CRC(95aa9237) SHA1(55a085727e3e30e40aa13e7444611226ab114b00) )
	ROM_LOAD( "642a.c4",    0x1400000, 0x4f0d00, CRC(4489296d) SHA1(bd32f6647179080413d196b33a72280d2a1b6694) )
	ROM_LOAD( "642a.c5",    0x1900000, 0x3eac00, CRC(744c9151) SHA1(503c2bdb07fe1677213368c4db1cc6acf236ca54) )
	ROM_LOAD( "642a.c6",    0x1e00000, 0x2a2200, CRC(1320af55) SHA1(b6ea612bd689799a36cab1f6ab6ba213ce07b3ea) )
	ROM_LOAD( "642a.c7",    0x2300000, 0x1cbf80, CRC(95d6d202) SHA1(2f10a548420b9d52ddd6b6698ba1fbeee6fcdfa1) )
ROM_END

ROM_START( badappleb )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "642b.p1", 0x000000, 0x400000, CRC(f68ade5a) SHA1(c2aa87783bc04cccb9d5df0ed74524787be1860f) )

	NEO_SFIX_128K( "642b.s1", CRC(bc7ebee7) SHA1(401d98fc4dbecf0281ee04718cf549cd35a80fed) )

	NEO_BIOS_AUDIO_64K( "642b.m1", CRC(c11e8874) SHA1(5b3316757b77e43ac5d18206bdeb90f7e8bc882a) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "642b.v1", 0x000000, 0x500000, CRC(3b741894) SHA1(c3e509830b53e36211823d6b912d5a2acb983ed9) )

	ROM_REGION( 0x6200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "642b.c1",  0x000000, 0x3100000, CRC(88ddc443) SHA1(b682f86647903c10bd7b525bb92dfd0a13499b36) )
	ROM_LOAD16_BYTE( "642b.c2",  0x000001, 0x3100000, CRC(b64face4) SHA1(f7f36370c6d6f56a6b924bedbdf87a3cd572b61d) )
ROM_END


// 643: 68k Inside intro by Dekadence
// Bugs: At end, black screen. Pressing 5 will get it to run again, about halfway, before it internally crashes.
ROM_START( 68kng )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "643.p1",       0x000000, 0x080000, CRC(b10d2cb6) SHA1(6a63f5424e3b5ae3e97fbf0e1841b8cdfd5740f2) )

	NEO_SFIX_128K( "539.s1", CRC(2e4238d1) SHA1(f556cd9afcf53c3a1128cdaa639d49fb3abfa3ed) )

	NEO_BIOS_AUDIO_64K( "643.m1", CRC(de1963da) SHA1(f133785dfe437a2a1235f39521c6a64949cf4823) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "643.v1",       0x000000, 0x100000, CRC(d1b3294e) SHA1(1aa1cf1951b8848a5e5a58b155656101dc51298e) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "643.c1",  0x000000, 0x100000, CRC(d1d755d8) SHA1(7d892886db756cae59474f619a814c97d4041154) )
	ROM_LOAD16_BYTE( "643.c2",  0x000001, 0x100000, CRC(4d751816) SHA1(276d13a5a91e5f86f07d2cf644f898386846c435) )
ROM_END


// 644: Vaporous intro by TTE
ROM_START( vaporous )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "644.p1",  0x000000, 0x100000, CRC(536517ca) SHA1(448cc860cb4e91e8a9ce9fcd24fe7a8b9e45c760) )

	NEO_SFIX_128K( "644.s1", CRC(11ac8ba5) SHA1(c58496d0a2d7674b5eb6ec4809ac7c3ac8814825) )

	NEO_BIOS_AUDIO_64K( "644.m1", CRC(1df9cbdc) SHA1(8d48cb63b7a05270e56b463539d1720f219e9003) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "644.v1",    0x000000, 0x400000, CRC(90f21f51) SHA1(302a8edbefa5a697d641cf39ce2932fa90219ed1) )
	ROM_LOAD( "644.v2",    0x400000, 0x400000, CRC(3354e822) SHA1(0973b579f0951a905df176706157062ece512342) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "644.c1",  0x000000, 0x800000, CRC(cd5b5b97) SHA1(6e2451343f159728869fa16879075ad741866866) )
	ROM_LOAD16_BYTE( "644.c2",  0x000001, 0x800000, CRC(ecbcf96f) SHA1(bc875ab87cae1146d12d77f8dd17ce9b574ed080) )
ROM_END


// 645: Chaos Demo that has been modified by Peter Lemon
// The changes are internal; there's no difference to the user
// No Sound
ROM_START( neodemo02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "645.p1", 0x000000, 0x080000, CRC(39e10ecc) SHA1(0493260f3140c612573014188e3d2165b0c214be) )

	NEO_SFIX_128K( "465.s1", CRC(10f12a9e) SHA1(750ce7ccdddf93c579b7cfb86b501cc15630dab2) )

	NEO_BIOS_AUDIO_128K( "645.m1", CRC(90990515) SHA1(bf3c60b43be464ef7fee85f69f73378ae2999d9c) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "616.c1", 0x000000, 0x100000, CRC(870b28ea) SHA1(255f8e394534afc929bad31f253a2e8b03253e07) )
	ROM_LOAD16_BYTE( "616.c2", 0x000001, 0x100000, CRC(1ecf8128) SHA1(66d200ceb023a1ff3cbe4ef7c5b1ef0068ab47f0) )
ROM_END


// 646: Greenspace by Peter Lemon
// Useless; no sound
ROM_START( greenspace )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "646.p1", 0x000000, 0x080000, CRC(194065e1) SHA1(42363196eb774fe11acbb2e9264296f03a660a57) )

	NEO_SFIX_128K( "646.s1", CRC(ac119f59) SHA1(fdb51f7c4f3b2130b70d615814cb265e463f9afa) )

	NEO_BIOS_AUDIO_128K( "646.m1", CRC(90990515) SHA1(bf3c60b43be464ef7fee85f69f73378ae2999d9c) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "646.c1", 0x000000, 0x100000, CRC(2e28134d) SHA1(fad8d76e9b398d87d31206b2018e7cdc60394ceb) )
	ROM_LOAD16_BYTE( "646.c2", 0x000001, 0x100000, CRC(666f11f3) SHA1(2a7417b0a0716ca38e031b4fd29a763f49ac46bc) )
ROM_END


// 647: Hello World demo by Peter Lemon
// Untested; no sound
ROM_START( neohw )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "647.p1", 0x000000, 0x080000, CRC(fd1a2387) SHA1(f25e14a612d9594c855be21133d8bea5a1c6aae3) )

	NEO_SFIX_128K( "467.s1", CRC(967f9046) SHA1(2e0a716d64bbd44001e7e8671478b4a14f69c34c) )

	NEO_BIOS_AUDIO_128K( "202.m1", CRC(9c0291ea) SHA1(3fa67c62acba79be6b3a98cc1601e45569fa11ae) )

	ROM_REGION( 0x100000, "sprites", ROMREGION_ERASEFF )
ROM_END


// 653 : Nyan Cat demo by Furrtek
// Movement keys will move nyancat around
// Bugs: Insert Coin freezes HBMAME.
ROM_START( nyan )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "653.p1", 0x000000, 0x20000, CRC(1c9e076f) SHA1(95361da308bf8f946fa218424c991ace872fd374) )

	NEO_SFIX_128K( "653.s1", CRC(2e7f15ec) SHA1(b41ac46ca2a81cf240dc6df4a80694ecebf65202) )

	NEO_BIOS_AUDIO_128K( "653.m1", CRC(97b9a8e0) SHA1(161a06ac0a6d33a6b337a5c6ec65345cd10472e4) )

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "653.v1", 0x000000, 0x3d400, CRC(8870e0bb) SHA1(5f6f33c07f04bb00337cca6ee4db1cfa7beaa63e) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "653.c1", 0x000000, 0x400000, CRC(71128701) SHA1(76ae8ef8f032621706cb39161f01ef7e747ceb70) )
	ROM_LOAD16_BYTE( "653.c2", 0x000001, 0x400000, CRC(85dcccef) SHA1(4270b11c9f9400b3b818124c96db352d94512807) )
ROM_END

ROM_START( nyana )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "653a.p1", 0x000000, 0x80000, CRC(91bbcb56) SHA1(e35906e7ecf8735a4c16d9c1463674c4f29a8b9d) )

	NEO_SFIX_128K( "653.s1", CRC(2e7f15ec) SHA1(b41ac46ca2a81cf240dc6df4a80694ecebf65202) )

	NEO_BIOS_AUDIO_128K( "653.m1", CRC(97b9a8e0) SHA1(161a06ac0a6d33a6b337a5c6ec65345cd10472e4) )

	ROM_REGION( 0x40000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "653.v1", 0x000000, 0x3d400, CRC(8870e0bb) SHA1(5f6f33c07f04bb00337cca6ee4db1cfa7beaa63e) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "653.c1", 0x000000, 0x400000, CRC(71128701) SHA1(76ae8ef8f032621706cb39161f01ef7e747ceb70) )
	ROM_LOAD16_BYTE( "653.c2", 0x000001, 0x400000, CRC(85dcccef) SHA1(4270b11c9f9400b3b818124c96db352d94512807) )
ROM_END


// 657 : MVS Test 01 by Furrtek.
ROM_START( test01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "657.p1", 0x000000, 0x080000, CRC(dd059433) SHA1(17cc7c48464d2d2a3d104f47e70a5f7cf3e19a5e) )

	NEO_SFIX_128K( "052.s1", CRC(97689804) SHA1(fa8dab3b3353d7115a0368f3fc749950c0186fbc) )

	NEO_BIOS_AUDIO_128K( "052.m1", CRC(49f17d2d) SHA1(70971fcf71ae3a6b2e26e7ade8063941fb178ae5) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "052.v1", 0x000000, 0x200000, CRC(22c097a5) SHA1(328c4e6db0a026f54a633cff1443a3f964a8daea) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "052-c1.c1", 0x000000, 0x100000, CRC(53e1c002) SHA1(2125b1be379ea7933893ffb1cd65d6c4bf8b03bd) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "052-c2.c2", 0x000001, 0x100000, CRC(776a2d1f) SHA1(bca0bac87443e9e78c623d284f6cc96cc9c9098f) )
	ROM_CONTINUE( 0x400001, 0x100000 )
ROM_END


// 658 : KnackiBalls by Furrtek. Patched by Robbbert.
// Bad address alignment causes it to continually reboot, so I have patched it.
// Instruction 5019 inserted to align A1 to the next word. So, some instructions are pushed upward.
// Not sure if the result is totally correct, but at least it runs now.
// No sound.
ROM_START( knacki )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "658.p1", 0x000000, 0x080000, CRC(56c0d270) SHA1(aebb3223e519f76e2abfa59f48626aa5e970ce4c) )
	ROM_COPY("maincpu",0x3e2, 0x3e4, 16) // move code so we can insert 5019
	ROM_FILL(0x12d, 1, 0xf4) // jump to another rts as we are overwriting this one's destination
	ROM_FILL(0x3e2, 1, 0x50) // insert 50
	ROM_FILL(0x3e3, 1, 0x19) // insert 19
	ROM_FILL(0x3e7, 1, 0xc4) // fix jump

	NEO_SFIX_128K( "052.s1", CRC(97689804) SHA1(fa8dab3b3353d7115a0368f3fc749950c0186fbc) )

	NEO_BIOS_AUDIO_128K( "658.m1", CRC(183c234a) SHA1(366c875703ced09c460d1810aadb3cd2f2fcd067) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "658.v1", 0x000000, 0x200000, CRC(ad1d1925) SHA1(4fa973bd8d2793ef553ea4e536193d3b89027828) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "658.c1", 0x000000, 0x100000, CRC(71dd4d25) SHA1(682671b55375dea8f86b25031320611a8c60326e) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "658.c2", 0x000001, 0x100000, CRC(0deccb3d) SHA1(ee9d1e5d3efcea0545a6dae8cd28b913e6f106b6) )
	ROM_CONTINUE( 0x400001, 0x100000 )
ROM_END


// 667 : Sprite Experimenter by Furrtek.
// Bugs: Insert Coin freezes HBMAME
ROM_START( spriteex )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "667.p1", 0x000000, 0x080000, CRC(9bce0611) SHA1(5fcb0fa6a434baf20972819484430faf44ffa21d) )

	NEO_SFIX_128K( "sfix.sfix", CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) )

	NEO_BIOS_AUDIO_128K( "667.m1", CRC(78bd1a39) SHA1(11963ee5d01a54e5f7e1b8973adb10960308a091) )

	ROM_REGION( 0x10000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "667.v1", 0x000000, 0x10000, CRC(48077f32) SHA1(97784ee122aec09f72b62387796726c3f3e9be5e) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "667.c1", 0x000000, 0x200000, CRC(dcda2ca8) SHA1(b37714532d36608eb7b819396a6796d0d49dbf92) )
	ROM_CONTINUE( 0x400000, 0x200000 )
	ROM_LOAD16_BYTE( "667.c2", 0x000001, 0x200000, CRC(c8a4d037) SHA1(6541074be8493bf2c7b869e8dbdb39f3309dcaf7) )
	ROM_CONTINUE( 0x400001, 0x200000 )
ROM_END


// 669 : ADPCM-A Playback Demo by freem.
ROM_START( pcmademo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "669.p1", 0x000000, 0x080000, CRC(a198ce30) SHA1(17b9ad5fe5136c0425823f49a65d428c544170c5) )

	NEO_SFIX_128K( "669.s1", CRC(c3dddf6e) SHA1(cce290e869ecbd52b7a7ed1e35c2c825450cc24c) )

	NEO_BIOS_AUDIO_128K( "669.m1", CRC(1c667c87) SHA1(3bc2cb708aa9c4618d901da1f627ae719dee445d) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "669.v1", 0x000000, 0x80000, CRC(e7ef9bd8) SHA1(d5de1f63189b3a18382da81f02b20af68996ad7e) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 670 : ADPCM-B Playback Demo by freem.
// Note that these 2 old roms are buggy and should be discarded: (use only the 'v3' set)
// CRC(6d719183) SHA1(e1e064bb785823a1f20f4a931bd7961515de2a98) )
// CRC(7b8abba4) SHA1(c492c468b34f0a7d57e2abd54d6057f3e470ee27) )
ROM_START( pcmbdemo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "670.p1", 0x000000, 0x080000, CRC(904e90ad) SHA1(1e13a81a2e0b1926d1745288e81dc51cd1ade5ac) )

	NEO_SFIX_128K( "670.s1", CRC(9647ff8a) SHA1(ecfaa4649d490fae1697e34cc5bbbc386ffa5ef6) )

	NEO_BIOS_AUDIO_128K( "670.m1", CRC(df4acc0c) SHA1(9c2999312ede7a1a415fa151a9acc6c98e510bf0) )

	ROM_REGION( 0x80000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "670.v1", 0x000000, 0x80000, CRC(baffec3e) SHA1(58ec4b684220ea558ed7dea99dfed1d24c57e662) )

	ROM_REGION( 0x10000, "sprites", ROMREGION_ERASEFF )
	// no sprites
ROM_END


// 674 : Sound-Loop Demo by freem.
// Bugs: The first time 5 is pressed, the game resets. After that it has no effect.
ROM_START( snddemo )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "674.p1", 0x000000, 0x080000, CRC(fbb26284) SHA1(80837fb870e36360f2c8c4576d1b43f3feef3ff5) )

	NEO_SFIX_128K( "674.s1", CRC(a7a76e2a) SHA1(894194f43b0bb884cb6f392bb0c9d1284639e577) )

	NEO_BIOS_AUDIO_64K( "674.m1", CRC(ba37ca7d) SHA1(4bb0897b74eadd7eb2782fcf2ae0a9897b9e8c8f) )

	ROM_REGION( 0x10000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "674.v1", 0x000000, 0x10000, CRC(95101457) SHA1(81ca771f441edc01019b8c629709eccc746059ba) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "674.c1",    0x000000, 0x080000, CRC(cad47785) SHA1(538c072417d849f33d1da3f61f98be4674c10762) )
	ROM_LOAD16_BYTE( "674.c2",    0x000001, 0x080000, CRC(c43d0953) SHA1(1ceb394b495f8a082452d50f78d2d7d8d1ec9a56) )
ROM_END


// 675 : Sprite demo by Kannagi (2016-03-26)
// Arrows to move fighter around. A = kick; B = punch; C & D slide background left & right. No sound.
ROM_START( neofightb )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "675.p1", 0x000000, 0x080000, CRC(caaf0da5) SHA1(089a89c787d4955fcb11141b50d587caa785c30a) )

	NEO_SFIX_128K( "052.s1", CRC(97689804) SHA1(fa8dab3b3353d7115a0368f3fc749950c0186fbc) )

	NEO_BIOS_AUDIO_128K( "675.m1", CRC(ffcbeb27) SHA1(101fee624083f33aac88e73afc69907df0b41de3) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "675.v1", 0x000000, 0x1e3300, CRC(da4f84c0) SHA1(7e9c039a47445ef805adf68c65e20063d93ba601) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "675.c1",    0x000000, 0x200000, CRC(4c2ec7e3) SHA1(572f087d2adb4900dbab39fa9bc517237ddba54e) )
	ROM_LOAD16_BYTE( "675.c2",    0x000001, 0x200000, CRC(319b0d51) SHA1(f57b4b3c96d94a6887f9e02e53af8eaef59d617a) )
ROM_END


// 676 : Neo Fight by Kannagi (2015-12-14)
// Arrows to move fighter around. A = kick; B = punch; C & D slide background left & right. No sound.
ROM_START( neofighto )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "676o.p1", 0x000000, 0x080000, CRC(b0eb16a0) SHA1(136b1cb9b9daf1b8563c66002d07bb5128d41061) )

	NEO_SFIX_128K( "052.s1", CRC(97689804) SHA1(fa8dab3b3353d7115a0368f3fc749950c0186fbc) )

	NEO_BIOS_AUDIO_128K( "052.m1", CRC(49f17d2d) SHA1(70971fcf71ae3a6b2e26e7ade8063941fb178ae5) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "052.v1", 0x000000, 0x200000, CRC(22c097a5) SHA1(328c4e6db0a026f54a633cff1443a3f964a8daea) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "676.c1",    0x000000, 0x200000, CRC(99fda4d5) SHA1(c5ff1b724a8cbc209c47eb4fb1eca4f0b76aad5c) )
	ROM_LOAD16_BYTE( "676.c2",    0x000001, 0x200000, CRC(2e48378a) SHA1(80389cfc420b2e7c5f50c809995a3298a57c697f) )
ROM_END
ROM_START( neofight )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "676.p1", 0x000000, 0x080000, CRC(c44e5685) SHA1(59bc5814454e74f069dcdc8d3fb32da5b5b65bec) )

	NEO_SFIX_128K( "052.s1", CRC(97689804) SHA1(fa8dab3b3353d7115a0368f3fc749950c0186fbc) )

	NEO_BIOS_AUDIO_128K( "052.m1", CRC(49f17d2d) SHA1(70971fcf71ae3a6b2e26e7ade8063941fb178ae5) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "052.v1", 0x000000, 0x200000, CRC(22c097a5) SHA1(328c4e6db0a026f54a633cff1443a3f964a8daea) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "676.c1",    0x000000, 0x200000, CRC(99fda4d5) SHA1(c5ff1b724a8cbc209c47eb4fb1eca4f0b76aad5c) )
	ROM_LOAD16_BYTE( "676.c2",    0x000001, 0x200000, CRC(2e48378a) SHA1(80389cfc420b2e7c5f50c809995a3298a57c697f) )
ROM_END


// 677 : Demo that comes with DatImage, by Mega Shocked (2016-07-26)
// No sound.
ROM_START( didemo )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "677.p1", 0x000000, 0x200000, CRC(732691f4) SHA1(40bddb4e636799f77b85a5b46fd9c90f9e125dce) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "063.m1", CRC(56675098) SHA1(90429fc40d056d480d0e2bbefbc691d9fa260fc4) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "677.c1",    0x000000, 0x200000, CRC(af99af71) SHA1(81b723a159dc80f2b784eb6027ccece94dc8324b) )
	ROM_LOAD16_BYTE( "677.c2",    0x000001, 0x200000, CRC(7b17b0f2) SHA1(b11071fd4782592a375f260352a0ca0d0df2c6e5) )
ROM_END


// 678 : Demo by Mega Shocked (2016-08-17)
ROM_START(akiradmo)
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD( "678.p1", 0x000000, 0x003188, CRC(a87fa008) SHA1(89f41671f185fd5e92237fcf82e9ed60387fafad) )
	ROM_LOAD( "678.p2", 0x100000, 0x09edf8, CRC(16d8b6b4) SHA1(b246b380b3cca17e11ab223bf4d1aeb605663a03) )
	ROM_LOAD( "678.p3", 0x200000, 0x074da4, CRC(94a22666) SHA1(e9cb0f159404ddcce10554753e56084fb79f2e60) )
	ROM_LOAD( "678.p4", 0x300000, 0x076b0c, CRC(d170285c) SHA1(b19bd0de148a68de9dbd18d89417c3dd32518780) )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "678.m1", CRC(37d8cb43) SHA1(b89baa6f41aefad57fb546fd1135506538ab9099) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", ROMREGION_ERASEFF )
	ROM_LOAD( "678.v1", 0x000000, 0x100000, CRC(148c5203) SHA1(8ddbfeeac4493f6e6ed7df834fcf1f0cede42ac3) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD( "678.c1", 0x000000, 0x34c700, CRC(84eba506) SHA1(a771cbc5efe7b61114db6b7f776b5ab93f09b2bc) )
	ROM_LOAD( "678.c2", 0x400000, 0x2e0400, CRC(a731451c) SHA1(aae521c38430a2797c7a60d970a0179673f05215) )
	ROM_LOAD( "678.c3", 0x800000, 0x249a80, CRC(87b2a616) SHA1(915d5c7bf832b725a3678b3735790f503dd34f48) )
ROM_END


// 680 : Tech Demo 2 by Luis Miguel Mayor
// No sound. No text. This is a beta, much earlier than the one shown on youtube.
ROM_START( ngtd2 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "680.p1", 0x000000, 0x100000, CRC(35131b60) SHA1(74877b4f1bdbba712b04afdf65bede1d30f846b0) )

	NEO_SFIX_64K( "680.s1", CRC(9a246f50) SHA1(1f1358dd9c2f88d2197d6c8272ec899b7c9a524b) )

	NEO_BIOS_AUDIO_128K( "680.m1", CRC(5ef0d5eb) SHA1(a327d6a88f3c7ccd242fceaffb3ef33a0171c215) ) // bad code

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "680.v1", 0x000000, 0x100000, CRC(5ea51b63) SHA1(62126f407175cd7bf44fb5acc1df12fbc0e23df6) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "680.c1", 0x000000, 0x100000, CRC(0ecfa2ba) SHA1(a5a7cbd3b79260f8cc99992067df315b20ae6741) )
	ROM_LOAD16_BYTE( "680.c2", 0x000001, 0x100000, CRC(f16b9ae8) SHA1(cb43089370a77bdd177db7eb6819ed21c994a914) )
ROM_END


// 681 : Operation I.G.L.A. demo by Vasily Familiya
// No sound.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
ROM_START( igla )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "681.p1", 0x000000, 0x007c82, CRC(1ce5f0ee) SHA1(7ee85f3e1ee5dc4878dbbb26e979f35b4cd5b15c) )

	NEO_SFIX_128K( "681.s1", CRC(e3ab9cde) SHA1(c438deaef29a705f5aa62116ad206ecec4bec282) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "681.c1", 0x000000, 0x024a80, CRC(4635f284) SHA1(99e185eb6f8d054c84d4124943419361e4bef342) )
ROM_END


// 682 : Team Lernit by Vasily Familiya
// No sound. "Press A to shoot" - doesn't work.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
ROM_START( lernit )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "682.p1", 0x000000, 0x00b6f1, CRC(63d5cf31) SHA1(331a65cec376b0c780ec506001b59f74cccbd5a3) )

	NEO_SFIX_128K( "681.s1", CRC(e3ab9cde) SHA1(c438deaef29a705f5aa62116ad206ecec4bec282) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "682.c1", 0x000000, 0x047e80, CRC(5e85884b) SHA1(7d7ba5718cddc582f7f2ec20ed8e46e5800589a6) )
ROM_END


// 683 : Everlasting Summer: Samantha by Vasily Familiya
// No sound. After the 13th screen there's bad gfx corruptions. All the text is in Russian.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
ROM_START( samantha )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "683.p1", 0x000000, 0x035651, CRC(709fc946) SHA1(b9190e74ab626a1cdae6c220767e87ad5ddaed53) )

	NEO_SFIX_128K( "683.s1", CRC(2f6c5cff) SHA1(5c429af7b1f1204f961472ba93eee55e68c8c739) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "683.c1", 0x000000, 0x0a7000, CRC(d302cab0) SHA1(fa558640fea8c85d7416d74a604a59169447a944) )
ROM_END


// 684 : Vladivostok 2000 by Vasily Familiya
// No sound. Some kind of test.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
ROM_START( vlad2000 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "684.p1", 0x000000, 0x00de95, CRC(a736301f) SHA1(88f1d19a822ab315c77ba960da13245e058ea23c) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "684.c1", 0x000000, 0x05d400, CRC(fe480580) SHA1(c97ca106789ec0640cf76a56cb1fe0ba7dc3114c) )
ROM_END


// 685 : Ryurik: Poteryannaya by Vasily Familiya
// No sound. Appears incomplete & not working.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
ROM_START( raroggame )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "685.p1", 0x000000, 0x00c269, CRC(7a7bc41c) SHA1(a154477f55380060b0c7b8cda83af250825fbee9) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD( "685.c1", 0x000000, 0x051f80, CRC(423d679f) SHA1(24adcf1d041af512efc3141a33515cb6bc4fe8a7) )
ROM_END


// 687 : Double Dragon SpritePool Demo by Vasily Familiya
// No sound.
ROM_START( ddsprdmo )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD( "687.p1", 0x000000, 0x00618f, CRC(c9379981) SHA1(17ca5d41a27daf47887df55596e7bf2955cf9cbf) )
	ROM_LOAD( "687.p2", 0x100000, 0x028424, CRC(1e717427) SHA1(f605f02f8f35313eeb58dc9858599e4ba8a0c632) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "687.m1", CRC(360c6863) SHA1(43d0b823859efe10e8d958dc0c32ea38e7a8096c) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "687.v1", 0x000000, 0x100000, CRC(8d9a4aa4) SHA1(52dfbcdb6015e692b5b030c3e44099d51c31d92d) )
	ROM_LOAD( "687.v2", 0x100000, 0x100000, CRC(5a326fc1) SHA1(18034072a32bd25ee3562ce56d7685ba6db2cd8e) )
	ROM_LOAD( "687.v3", 0x200000, 0x100000, CRC(c37bb043) SHA1(6fcfeccad763a91c07ce7e2ab8ef5efd4aa7bda9) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD( "687.c1", 0x000000, 0x127000, CRC(092721a3) SHA1(c5b8ce994baf1bbf50727dbab0759cba0d01a719) )
ROM_END
// This one has sound
ROM_START( ddsprdmo1 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD( "687a.p1", 0x000000, 0x0062bf, CRC(ad865bda) SHA1(10b83b3d90094fc0a1cdec3398c406d741871bac) )
	ROM_LOAD( "687a.p2", 0x100000, 0x01e190, CRC(2decf0cf) SHA1(73a3b36edaef65ea0936dba7d2556bd06b74cb2c) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "687.m1", CRC(360c6863) SHA1(43d0b823859efe10e8d958dc0c32ea38e7a8096c) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "687.v1", 0x000000, 0x100000, CRC(8d9a4aa4) SHA1(52dfbcdb6015e692b5b030c3e44099d51c31d92d) )
	ROM_LOAD( "687.v2", 0x100000, 0x100000, CRC(5a326fc1) SHA1(18034072a32bd25ee3562ce56d7685ba6db2cd8e) )
	ROM_LOAD( "687.v3", 0x200000, 0x100000, CRC(c37bb043) SHA1(6fcfeccad763a91c07ce7e2ab8ef5efd4aa7bda9) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD( "687a.c1", 0x000000, 0x0b8d00, CRC(1f508463) SHA1(d77c758729b7792d14ddec344a96dd4a07cba023) )
ROM_END


// 688 : Shaman King Demo by Vasily Familiya
// No sound.
// Supplied m1 is actually 071.s1 (yes, s1) from b2b. Used a real m1.
// Same background as igla.
ROM_START( shaman16 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688.p1", 0x000000, 0x0682f9, CRC(19223e4e) SHA1(944e1dc661a77bdc80c0b83bd72e988d498e5f23) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688.c1", 0x000000, 0x3ce380, CRC(1174dafe) SHA1(493fc5da846b1369529a819efe87d4a5438af02f) )
ROM_END

ROM_START( shaman17 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688a.p1", 0x000000, 0x06899c, CRC(d09f063e) SHA1(f44f6d778d064043ebf86ea81bc216e11ead23ca) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688a.c1", 0x000000, 0x3d2e80, CRC(3cabd968) SHA1(b0003037575fce9d1c83860d7db715acd6908630) )
ROM_END

ROM_START( shaman18 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688b.p1", 0x000000, 0x068a3e, CRC(fabb2b46) SHA1(41c4db93c20fe7302be5b2516bbfda5931f58cfd) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688b.c1", 0x000000, 0x3d1780, CRC(effb008f) SHA1(3c2fde77bbd09b57a5a27b01940796ec25c28234) )
ROM_END

ROM_START( shaman19 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688c.p1", 0x000000, 0x06a165, CRC(59703944) SHA1(7a5f531ecd593e8e8687b65031c19d9a747adbfd) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_128K( "098.m1", CRC(da4878cf) SHA1(ce13d18a4c5d01974df8542c67c4df00dbc6e7c1) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "098.v1", 0x000000, 0x100000, CRC(6f8ccddc) SHA1(696df2d0f416c2374b0eb7c858486054688c5bca) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END
// Has Sound
ROM_START( shaman20 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688d.p1", 0x000000, 0x06a199, CRC(7c5ef2e0) SHA1(e93e2438d27d854a1bd0aad62a40869371ddc06b) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "688.m1", CRC(feee7e20) SHA1(6e4c0e621773c2b86a36d006da646b85a8cb48e8) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "688.v1", 0x000000, 0x800000, CRC(cab45861) SHA1(ca4a1c20bb9aae84b9266279926d8fd9cf82016c) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END
// New background
ROM_START( shaman21 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688e.p1", 0x000000, 0x06a49f, CRC(e6d98567) SHA1(3b356903ea5e9b3fb3b7231fcc29b6f12f5d8d96) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "688.m1", CRC(feee7e20) SHA1(6e4c0e621773c2b86a36d006da646b85a8cb48e8) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "688.v1", 0x000000, 0x800000, CRC(cab45861) SHA1(ca4a1c20bb9aae84b9266279926d8fd9cf82016c) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END

ROM_START( shaman22 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688f.p1", 0x000000, 0x06a4d3, CRC(ccf28f80) SHA1(c4887f5f2aded6614971b529f91dc3e9961e5f2c) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "688f.m1", CRC(9cb5df77) SHA1(3b19082bdc9df67b585f3d1ca517b2237f3a2d73) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "688f.v1", 0x000000, 0x800000, CRC(0740252d) SHA1(068c3a6f53d0d406c5074c5095d52ecc6b1725ea) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END

ROM_START( shaman23 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688g.p1", 0x000000, 0x06a4f7, CRC(1d301b58) SHA1(9566cf01c1997b346829e869b108a41bea194959) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "688g.m1", CRC(6db119df) SHA1(51287e6a72b04c4f19841142281bdf77b94863dc) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "688g.v1", 0x000000, 0x800000, CRC(ba3f4043) SHA1(adc0aab8b1f5b0de71cded8cd474226af365312b) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END

ROM_START( shaman24 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "688h.p1", 0x000000, 0x06a54f, CRC(115d4bee) SHA1(eed58c87d90bed0a75cc15f1d0975e2192ba04ba) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "688h.m1", CRC(36d60aba) SHA1(01befcf0cb92b9d66c745d9aae6553ea4336ad02) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "688h.v1", 0x000000, 0x800000, CRC(664d4093) SHA1(bee80981066d876c0eeb0e069eee0273d8abdb65) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "688c.c1", 0x000000, 0x3da380, CRC(e7c36d00) SHA1(38b302e44298dec3cccb97ea466122310557f057) )
ROM_END


// 689 : Venus Wars Demo by Vasily Familiya
ROM_START( venuswars )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "689.p1", 0x000000, 0x05beaa, CRC(29950bbd) SHA1(8acb73f03924ba4fd2cbcb9f227641a268ac75f7) )

	NEO_SFIX_128K( "684.s1", CRC(b7070d6f) SHA1(546934c53d854347e4812bbfb014d3c58284da73) )

	NEO_BIOS_AUDIO_64K( "689.m1", CRC(980c52fd) SHA1(5a8a33b28d8223f8582c59dced2a61e1a93431cd) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "689.v1", 0x000000, 0x800000, CRC(5e44a6e6) SHA1(409330797213af87d9d86425ef056ae4d8ca79d1) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "689.c1", 0x000000, 0x37e880, CRC(6983fb91) SHA1(1f626f52544b6ddcdd1756bc529df999f3a47872) )
ROM_END


// 690 : Casanova She And She (Girl X Girl) Demo by Vasily Familiya
ROM_START( gxg )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD( "690.p1", 0x000000, 0x027a7e, CRC(caf04493) SHA1(6b97dfd861e05762e31c3496bbc18d014ff3d98f) )

	NEO_SFIX_128K( "690.s1", CRC(b386f0e0) SHA1(113dd36cb710403b58b9eb860b7abc9f04ef5348) )

	NEO_BIOS_AUDIO_64K( "690.m1", CRC(5bb91b91) SHA1(5a96d09808fe709a0bd07681c13a3be70e3fb685) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "690.v1", 0x000000, 0x800000, CRC(61f5a37f) SHA1(870d25530898c5de5f1f93e673fc9490f594d57c) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD( "690.c1", 0x000000, 0x086780, CRC(e0dbb8cc) SHA1(26963461716cc45cc2fab4fa820ae292ee82afb9) )
ROM_END


GAME( 2022, 19yy,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Ichikyu Wai Wai/EKORZ/Elrayzeur", "19YY", MACHINE_SUPPORTS_SAVE )
GAME( 2022, 19yy01,       19yy,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Ichikyu Wai Wai/EKORZ", "19YY (early release)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, cakefght,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "ADK/EKORZ", "Cake Fighter", MACHINE_SUPPORTS_SAVE )
GAME( 2022, cakefght1,    cakefght, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "ADK/EKORZ", "Cake Fighter v1.1", MACHINE_SUPPORTS_SAVE )
GAME( 2024, cbarrel,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Neo Byte Force", "Captain Barrel", MACHINE_SUPPORTS_SAVE )
GAME( 1996, crswd2bl,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Razoola", "Crossed Swords 2 (CD conversion)", MACHINE_SUPPORTS_SAVE )
GAME( 1995, csw2,         crswd2bl, no_watchdog,     neogeo,  neogeo_state, init_neogeo,   ROT0, "hack", "Crossed Swords 2", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1995, fr2,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Video Systems Co.", "Idol Mahjong Final Romance 2", MACHINE_SUPPORTS_SAVE )
GAME( 1995, fr2cd,        fr2,      neogeo_noslot,   neogeo,  neogeo_state, init_fr2ch,    ROT0, "Video Systems Co.", "Idol Mahjong Final Romance 2 (CD Bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 1995, fr2ch,        fr2,      neogeo_noslot,   neogeo,  neogeo_state, init_fr2ch,    ROT0, "Video Systems Co.", "Idol Mahjong Final Romance 2 (CD conversion)", MACHINE_SUPPORTS_SAVE )
GAME( 2011, totc,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "FACE Corporation / N.C.I - Le Cortex", "Treasure of the Caribbean", MACHINE_SUPPORTS_SAVE )
GAME( 2023, yoyoshkn,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Dr Ludos", "Yo-Yo Shuriken", MACHINE_SUPPORTS_SAVE )
GAME( 2009, zintrckbh,    zintrckb, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Yumeji", "ZinTricK (Enable hidden characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, zintrkcd,     zintrckb, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Razoola", "ZinTricK (CD conversion)", MACHINE_SUPPORTS_SAVE )
GAME( 2010, zintrkm,      zintrckb, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Elrayseur", "ZinTricK (Music test)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, zintrkcd1,    zintrckb, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Razoola", "ZinTricK (CD test)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, zintricks01,  zintrckb, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "hack", "ZinTricK (Unknown Hack)", MACHINE_SUPPORTS_SAVE )

GAME( 2023, 68kng,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Dekadence", "68k Inside intro", MACHINE_SUPPORTS_SAVE )
GAME( 2022, 240ptest,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Dasutin/Artemio", "240p Test Suite v0.95", MACHINE_SUPPORTS_SAVE )
GAME( 2023, 240ptest1,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Dasutin/Artemio", "240p Test Suite v1.0", MACHINE_SUPPORTS_SAVE )
GAME( 2021, abyssal,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kako Eimon", "Abyssal Infants", MACHINE_SUPPORTS_SAVE )
GAME( 2021, abyssal1,     abyssal,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kako Eimon", "Abyssal Infants (newer version)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, akiradmo,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "HP Man", "Akira Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2023, amazonn,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "iq132", "Soldier Girl Amazon (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, badapple,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "BEY", "Bad Apple demo (v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, badapplea,    badapple, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "BEY", "Bad Apple demo (v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, badappleb,    badapple, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "HP Man", "Bad Apple demo (Giga Power Edition)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, baddudesn,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "La Casa De Ruivo / Neo Byte Force", "Bad Dudes vs Dragonninja Burger Edition demo (2025-06-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, bbb2d1,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Pixelheart", "Bang Bang Busters 2 (demo 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, bbb2d2,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Pixelheart", "Bang Bang Busters 2 (demo 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, beast,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Shadow of the Beast (NeoGeo Demo)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2020, bonusstage,   neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Totologic", "Bonus Stage", MACHINE_SUPPORTS_SAVE )
GAME( 2025, bpanicdx,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Block Panic DX (2025-01-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, cabalng,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "iq132", "Cabal (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, cabalng1,     cabalng,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "iq132", "Cabal (Neo-Geo port, sprite fix)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, caravan,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Fullset", "Project Neon Caravan Edition (Prealpha 0.4.19)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, chelnovn,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_cdc,      ROT0, "iq132", "Atomic Runner Chelnov (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, cnbe,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Codename: Blut Engel (2006-01-19)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, cnbe2018,     cnbe,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Codename: Blut Engel (2018-09-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, cndi,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Chip n Dale (Intro demo v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, cndia,        cndi,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Chip n Dale (Intro demo v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, cphd,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Le Cortex", "Crouching Pony Hidden Dragon Demo", MACHINE_SUPPORTS_SAVE )
GAME( 1990, columnsn,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Columns (NeoGeo)(Homebrew)", MACHINE_SUPPORTS_SAVE )
GAME( 1990, columnsncd,   columnsn, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Columns (CD conversion)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cyborg,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Neo Byte Force", "Cyborg Force", MACHINE_SUPPORTS_SAVE )
GAME( 2023, cyborg1,      cyborg,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Neo Byte Force", "Cyborg Force (patched)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, ddsprdmo,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Double Dragon SpritePool demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2018, ddsprdmo1,    ddsprdmo, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Double Dragon SpritePool demo (newer)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2016, didemo,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Mega Shocked", "DatImage demo", MACHINE_SUPPORTS_SAVE )
GAME( 2025, doubled1,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "La Casa De Ruivo / Neo Byte Force", "Double Dragon One demo (2025-09-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, dti,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Duck Tales (Intro demo v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, dtia,         dti,      neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Duck Tales (Intro demo v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, dwi,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "DarkWing Duck (Intro demo v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, dwia,         dwi,      neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "DarkWing Duck (Intro demo v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, ffeast,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Rastersoft", "Frog Feast (NeoGeo)", MACHINE_SUPPORTS_SAVE )
GAME( 2005, ffeastd,      ffeast,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Rastersoft", "Frog Feast demo", MACHINE_SUPPORTS_SAVE )
GAME( 2023, flapchck,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Flappy Chicken (2023-04-30)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, foodfn,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "iq132", "Food Fight (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, galaxiann,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "tcdev", "Galaxians (beta 1, 2023-05-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2023, galaxiann2,   galaxiann,neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "tcdev", "Galaxians (beta 2, 2023-06-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, gbi,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "GhostBusters (Intro demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, gladmort,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Pixelheart / ChipsOnSteroids", "Gladmort", MACHINE_SUPPORTS_SAVE )
GAME( 2024, gladmortd,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Pixelheart", "Gladmort (Demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2024, gladmortd2,   gladmortd,neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Pixelheart", "Gladmort (Demo 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, goldaxen,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Hoffman", "Golden Axe (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, gxg,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Casanova She And She demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2022, horekidb3,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "iq132", "Kid no Hore Hore Daisakusen (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, hypernoid,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "M.Priewe", "Hypernoid (2021-11-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, hypernoid01,  neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "M.Priewe", "Hypernoid (2022-02-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, igla,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Operation I.G.L.A. demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2025, inthuntn,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Balek Corp / Neo Byte Force", "In the Hunt demo (2025-05-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, iocero,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "iocerom", "Primo Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2020, jumpnrund,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Jump n Run Tech Demo (Nov 2020)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, jumpnrund01,  neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Jump n Run Tech Demo (Dez 2020)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, jurl,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Tonsomo Entertainment", "Jurl (2025-06-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, karnovn,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "iq132", "Karnov (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, knightsch,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Neobitz", "Knight's Chance", MACHINE_SUPPORTS_SAVE )
GAME( 2018, lernit,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Team Lernit", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2020, lhbb,         lasthope, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Elrayzeur", "Last Hope (Blue Bullets)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, lhcdb,        lasthope, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "NG:DEV.TEAM", "Last Hope (CD conversion 1)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2007, lhcdba,       lasthope, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "NG:DEV.TEAM", "Last Hope (CD conversion 2)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2007, lhopecd,      lasthope, neogeo_noslot,   neogeo,  neogeo_state, init_cdc,      ROT0, "NG:DEV.TEAM", "Last Hope JP NGCD (Beta V0.52)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 2021, looptris,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Looptris (2021-12-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, looptrsp,     looptris, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Looptris Plus (2022-12-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2005, ltorb,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Jonas Indiana and The Lost Temple of RA (beta, 2005-07-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, knacki,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Furrtek", "KnackiBalls", MACHINE_SUPPORTS_SAVE )
GAME( 2021, ndo_a_td,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo Driftout Tech Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2004, neo2500,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo 2500 Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2021, neo2048,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Nicole Branagan", "Neo 2048", MACHINE_SUPPORTS_SAVE )
GAME( 2012, neo3d,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Oxygene", "Neo 3D Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2004, neocstlv,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "BarfHappy", "Neo Castlevania Demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2003, neodemo,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Charles Doty", "Demo - NeoGeo", MACHINE_SUPPORTS_SAVE )
GAME( 2015, neofightb,    neofight, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kannagi", "Neogeo Sprite Demo (Neo Fight beta)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2015, neofighto,    neofight, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kannagi", "Neo Fight (old)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2015, neofight,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kannagi", "Neo Fight", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2013, neogal1,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Cristiano Bei", "NeoGeo Galaga Demo R001", MACHINE_SUPPORTS_SAVE )
GAME( 2013, neogal1d,     neogal1,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Cristiano Bei", "NeoGeo Galaga Demo R001 (debug)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, neogal1t,     neogal1,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Cristiano Bei", "NeoGeo Galaga Demo R001 (text)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, neogal2,      neogal1,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Cristiano Bei", "NeoGeo Galaga Demo R002", MACHINE_SUPPORTS_SAVE )
GAME( 2002, neonopon,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo No Panepon (V.0.2, 2002-10-05)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2002, neonopon_d1,  neonopon, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo No Panepon (demo 1, 2002-07-07)", MACHINE_NOT_WORKING | MACHINE_IS_INCOMPLETE | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2002, neonopon_d2,  neonopon, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo No Panepon (demo 2, 2002-07-07)", MACHINE_NOT_WORKING | MACHINE_IS_INCOMPLETE | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2002, neonopon_d3,  neonopon, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo No Panepon (demo 3, 2002-08-22)", MACHINE_NOT_WORKING | MACHINE_IS_INCOMPLETE | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2010, neopang,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "CeL", "Neo Pang", MACHINE_SUPPORTS_SAVE )
GAME( 2004, neopl,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_cdc,      ROT0, "Blastar", "Neo Puzzle League (from CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, neopong,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Neodev", "Neo Pong (v1.1)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2002, neoponga,     neopong,  no_watchdog,     neogeo,  neogeo_state, init_neogeo,   ROT0, "Neodev", "Neo Pong (v1.0)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2010, neoromjb,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "CeL", "Neo Rom Jukebox", MACHINE_SUPPORTS_SAVE )
GAME( 2013, neotest,      neogal1,  neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "BEY", "NeoGeo Sound Test", MACHINE_SUPPORTS_SAVE )
GAME( 2012, neothund,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Sebastian Mihai", "Neo Thunder", MACHINE_SUPPORTS_SAVE )
GAME( 2019, neotrisd1,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Totologic", "Neotris Demo 1", MACHINE_SUPPORTS_SAVE )
GAME( 2020, neotrisd2,    neotrisd1,neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Totologic", "Neotris Demo 2", MACHINE_SUPPORTS_SAVE )
GAME( 2020, neotrisd3,    neotrisd1,neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Totologic", "Neotris Demo 3", MACHINE_SUPPORTS_SAVE )
GAME( 2020, nblktiger,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "Neo Black Tiger", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2021, ng4ptest,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "4 Player Multitab Test (from CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, ng4psyschk,   neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Ichikyu Wai Wai/EKORZ", "MVS 4P Key System Check", MACHINE_SUPPORTS_SAVE )
GAME( 2006, ngem2k,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "NGEM2K (Beta, 2006-01-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, ngftdemo,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "CeL", "NGF Transparency Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2014, ngmontst,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "M.Priewe", "NeoGeo Monitor Test v1.0", MACHINE_SUPPORTS_SAVE )
GAME( 2022, ngmontst3,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "M.Priewe", "NeoGeo Monitor Test v1.3", MACHINE_SUPPORTS_SAVE )
GAME( 2016, ngtd2,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Luis Miguel Mayor", "NeoGeo Tech Demo 2", MACHINE_SUPPORTS_SAVE )
GAME( 2008, ngtetris,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Crim/Stephen", "NeoGeo 2-Player Tetris", MACHINE_SUPPORTS_SAVE )
GAME( 2018, ngym2610,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "NeoGeo YM2610 Test", MACHINE_SUPPORTS_SAVE )
GAME( 2020, nsmb,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "New! Super Mario Bros.", MACHINE_SUPPORTS_SAVE )
GAME( 2020, nsmbd,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "New! Super Mario Bros Demo Screen", MACHINE_SUPPORTS_SAVE )
GAME( 2011, nyan,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Furrtek", "Nyan Cat demo (v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2011, nyana,        nyan,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Furrtek", "Nyan Cat demo (v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, pcmademo,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Freem", "ADPCM-A Playback Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2015, pcmbdemo,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Freem", "ADPCM-B Playback Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2003, poknight,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Poker Night", MACHINE_SUPPORTS_SAVE )
GAME( 2003, poknightcd,   poknight, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Poker Night (CD)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, poknightfr,   poknight, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Jeff Kurtz", "Poker Night (French)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, pown,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_cdc,      ROT0, "iq132", "P.O.W. (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, raroggame,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Ryurik: Poteryannaya demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2009, rci,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Robocop (Intro demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samantha,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Everlasting Summer: Samantha demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2012, santabll,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "M.Priewe", "Santaball", MACHINE_SUPPORTS_SAVE )
GAME( 2018, seafight,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "kl3mousse", "Sea Fighter 0.01.009", MACHINE_SUPPORTS_SAVE )
GAME( 2025, sgz,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Shadow Gangs", "Shadow Gangs demo", MACHINE_SUPPORTS_SAVE )
GAME( 2018, shaman16,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.16", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2018, shaman17,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.17", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2018, shaman18,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.18", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman19,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.19", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman20,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.20", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman21,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.21", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman22,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.22", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman23,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.23", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, shaman24,     shaman16, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Shaman King demo 0.24", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2025, shinobin,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Hoffman", "Shinobi v1.0 (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, shinobin1,    shinobin, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Hoffman", "Shinobi v1.1 (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, shinobint,    shinobin, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Hoffman", "Shinobi Test (Neo-Geo port)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, smi,          neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Spiderman (Intro demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, snddemo,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Freem", "Sound-Loop Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2025, spkick,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kakoeimon", "Super Power Kick (beta 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, spkick2,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Kakoeimon", "Super Power Kick (beta 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2011, spriteex,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Furrtek", "Sprite Experimenter", MACHINE_SUPPORTS_SAVE )
GAME( 2000, syscheck,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Neo System Check (ver 1.0b)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, teot_1,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, teot_2,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, demo 2, 2021-02-20)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, teot_3,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, alpha 5, 2021-05-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_4,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta, 2022-01-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_5,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 2, 2022-01-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_6,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 3, 2022-03-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_7,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 4, 2022-05-07)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_8,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 5, 2022-05-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot_9,       teot,     neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 6, 2022-07-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, teot,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "OzzyOuzo", "The Eye of Typhoon (Tsunami Edition, beta 7, 2022-07-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, test01,       neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Furrtek", "MVS Test 01", MACHINE_SUPPORTS_SAVE )
GAME( 2012, timesup,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "NGF Dev. Inc", "Time's Up!", MACHINE_SUPPORTS_SAVE )
GAME( 2012, timesupd,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "NGF Dev. Inc", "Time's Up! (Demo)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, tmnti,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Teenage Mutant Ninja Turtles (Intro demo v1)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, tmntia,       tmnti,    neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "[Raregame]", "Teenage Mutant Ninja Turtles (Intro demo v2)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, twister,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Twister in a mirror intro", MACHINE_SUPPORTS_SAVE )
GAME( 2025, vaporous,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "TTE", "Vaporous intro", MACHINE_SUPPORTS_SAVE )
GAME( 2019, venuswars,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Venus Wars demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2026, violentv,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Balek Corp", "Violent Vengeance: the universe hero (beta 3.28, 2026-02-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, vlad2000,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Vasily Familiya", "Vladivostok 2000 demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2025, wrldracr,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "World Racer (2025-11-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, wrldracr01,   wrldracr, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "World Racer (2025-12-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, wrldracr02,   wrldracr, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "World Racer (2025-12-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, ww2demo,      neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Rastersoft", "WW2demo", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2019, xeno,         neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Bitmap Bureau", "Xeno Crisis v1.0.0", MACHINE_SUPPORTS_SAVE )
GAME( 2023, xeviousn,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "tcdev", "Xevious (beta 1, 2023-03-07)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, xeviousn01,   xeviousn, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "Zeroco", "Zetsu Xevious (beta 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, xeviousn02,   xeviousn, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "Zeroco", "Zetsu Xevious (beta 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, xeviousn03,   xeviousn, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "Zeroco", "Zetsu Xevious (beta 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2025, xeviousn04,   xeviousn, neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT270, "Zeroco", "Zetsu Xevious (beta 4)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, xmas2017,     neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Blastar", "Xmas/Happy New Year 2017 demo", MACHINE_SUPPORTS_SAVE )
GAME( 2019, neohw,        neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Peter Lemon", "Hello World", MACHINE_SUPPORTS_SAVE )
GAME( 2019, neodemo02,    neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Peter Lemon", "Chaos Demo", MACHINE_SUPPORTS_SAVE )
GAME( 2019, greenspace,   neogeo,   neogeo_noslot,   neogeo,  neogeo_state, init_neogeo,   ROT0, "Peter Lemon", "Green Space", MACHINE_SUPPORTS_SAVE )

