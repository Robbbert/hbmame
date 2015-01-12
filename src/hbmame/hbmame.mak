###########################################################################
#
#   hbmame.mak
#
#   HBMAME target makefile
#
###########################################################################

ifeq ($(TARGET),hbmame)
# In order to keep dependencies reasonable, we exclude objects in the base of
# $(SRC)/emu, as well as all the OSD objects and anything in the $(OBJ) tree
depend: maketree $(MAKEDEP_TARGET)
	@echo Rebuilding depend_emu.mak...
	$(MAKEDEP) -I. $(INCPATH) -X$(SRC)/emu -X$(SRC)/osd/... -X$(OBJ)/... $(SRC)/emu > depend_emu.mak
	@echo Rebuilding depend_$(TARGET).mak...
	$(MAKEDEP) -I. $(INCPATH) -X$(SRC)/emu -X$(SRC)/osd/... -X$(OBJ)/... $(SRC)/$(TARGET) > depend_$(TARGET).mak
endif

INCPATH += \
	-I$(SRC)/mame \
	-I$(OBJ)/mame/layout \
	-I$(SRC)/mess/osd \
	-I$(SRC)/mess/osd/$(OSD) \
	-I$(SRC)/hbmame/includes


HBMAMESRC = $(SRC)/hbmame
HBMAMEOBJ = $(OBJ)/hbmame

MAMESRC = $(SRC)/mame
MAMEOBJ = $(OBJ)/mame

HBMAME_AUDIO = $(HBMAMEOBJ)/audio
HBMAME_DRIVERS = $(HBMAMEOBJ)/drivers
HBMAME_LAYOUT = $(HBMAMEOBJ)/layout
HBMAME_MACHINE = $(HBMAMEOBJ)/machine
HBMAME_VIDEO = $(HBMAMEOBJ)/video

MAME_AUDIO = $(MAMEOBJ)/audio
MAME_DRIVERS = $(MAMEOBJ)/drivers
MAME_LAYOUT = $(MAMEOBJ)/layout
MAME_MACHINE = $(MAMEOBJ)/machine
MAME_VIDEO = $(MAMEOBJ)/video

EMU_AUDIO = $(OBJ)/emu/sound
EMU_MACHINE = $(OBJ)/emu/machine

OBJDIRS += \
	$(HBMAME_AUDIO) \
	$(HBMAME_DRIVERS) \
	$(HBMAME_LAYOUT) \
	$(HBMAME_MACHINE) \
	$(HBMAME_VIDEO) \
	$(MAME_AUDIO) \
	$(MAME_DRIVERS) \
	$(MAME_LAYOUT) \
	$(MAME_MACHINE) \
	$(MAME_VIDEO) \

#-------------------------------------------------
# specify required CPU cores
#-------------------------------------------------

CPUS += Z80
#CPUS += Z180
CPUS += I8085
CPUS += M6502
CPUS += H6280
CPUS += I86
#CPUS += I386
CPUS += NEC
#CPUS += V30MZ
#CPUS += V60
CPUS += MCS48
CPUS += MCS51
CPUS += M6800
CPUS += M6805
CPUS += HD6309
CPUS += M6809
#CPUS += KONAMI
CPUS += M680X0
#CPUS += T11
CPUS += S2650
CPUS += TMS340X0
#CPUS += TMS9900
#CPUS += Z8000
#CPUS += Z8001
CPUS += TMS32010
#CPUS += TMS32025
CPUS += TMS32031
#CPUS += TMS32051
#CPUS += TMS57002
CPUS += CCPU
CPUS += ADSP21XX
#CPUS += ASAP
#CPUS += AM29000
#CPUS += UPD7810
CPUS += ARM
CPUS += ARM7
#CPUS += JAGUAR
#CPUS += CUBEQCPU
#CPUS += ESRIP
#CPUS += MIPS
#CPUS += PSX
CPUS += SH2
#CPUS += SH4
CPUS += DSP16A
#CPUS += DSP32C
CPUS += PIC16C5X
#CPUS += PIC16C62X
#CPUS += G65816
#CPUS += SPC700
#CPUS += E1
#CPUS += I860
#CPUS += I960
#CPUS += H83002
#CPUS += H83334
CPUS += V810
#CPUS += M37710
#CPUS += POWERPC
#CPUS += SE3208
#CPUS += MC68HC11
#CPUS += ADSP21062
#CPUS += DSP56156
#CPUS += RSP
#CPUS += ALPHA8201
#CPUS += COP400
#CPUS += TLCS90
#CPUS += TLCS900
CPUS += MB88XX
#CPUS += MB86233
CPUS += SSP1601
#CPUS += APEXC
#CPUS += CP1610
#CPUS += F8
#CPUS += LH5801
#CPUS += PDP1
#CPUS += SATURN
#CPUS += SC61860
#CPUS += LR35902
#CPUS += TMS7000
#CPUS += SM8500
#CPUS += MINX
#CPUS += SSEM
#CPUS += AVR8
#CPUS += TMS0980
#CPUS += I4004
#CPUS += SUPERFX
#CPUS += Z8
#CPUS += I8008
#CPUS += SCMP
#CPUS += MN10200
#CPUS += COSMAC
#CPUS += UNSP
#CPUS += HCD62121
#CPUS += PPS4
#CPUS += UPD7725
#CPUS += HD61700




#-------------------------------------------------
# specify available sound cores; some of these are
# only for MESS and so aren't included
#-------------------------------------------------

SOUNDS += SAMPLES
SOUNDS += DAC
SOUNDS += DMADAC
#SOUNDS += SPEAKER
SOUNDS += BEEP
SOUNDS += DISCRETE
SOUNDS += AY8910
SOUNDS += YM2151
SOUNDS += YM2203
SOUNDS += YM2413
SOUNDS += YM2608
SOUNDS += YM2610
#SOUNDS += YM2610B
SOUNDS += YM2612
#SOUNDS += YM3438
SOUNDS += YM3812
SOUNDS += YM3526
#SOUNDS += Y8950
#SOUNDS += YMF262
#SOUNDS += YMF271
SOUNDS += YMF278B
SOUNDS += YMZ280B
SOUNDS += SN76477
SOUNDS += SN76496
SOUNDS += POKEY
#SOUNDS += TIA
SOUNDS += NES_APU
SOUNDS += ASTROCADE
#SOUNDS += NAMCO
SOUNDS += NAMCO_15XX
#SOUNDS += NAMCO_CUS30
SOUNDS += NAMCO_52XX
#SOUNDS += NAMCO_63701X
#SOUNDS += T6W28
#SOUNDS += SNKWAVE
#SOUNDS += C140
#SOUNDS += C352
SOUNDS += TMS36XX
#SOUNDS += TMS3615
SOUNDS += TMS5110
SOUNDS += TMS5220
SOUNDS += VLM5030
#SOUNDS += ADPCM
SOUNDS += MSM5205
#SOUNDS += MSM5232
#SOUNDS += OKIM6258
SOUNDS += OKIM6295
#SOUNDS += OKIM6376
#SOUNDS += OKIM9810
SOUNDS += UPD7759
SOUNDS += HC55516
#SOUNDS += TC8830F
SOUNDS += K005289
SOUNDS += K007232
SOUNDS += K051649
SOUNDS += K053260
SOUNDS += K054539
#SOUNDS += K056800
SOUNDS += SEGAPCM
#SOUNDS += MULTIPCM
#SOUNDS += SCSP
#SOUNDS += AICA
SOUNDS += RF5C68
#SOUNDS += RF5C400
#SOUNDS += CEM3394
SOUNDS += QSOUND
#SOUNDS += SAA1099
#SOUNDS += IREMGA20
#SOUNDS += ES5503
SOUNDS += ES5505
#SOUNDS += ES5506
#SOUNDS += BSMT2000
#SOUNDS += GAELCO_CG1V
#SOUNDS += GAELCO_GAE1
#SOUNDS += C6280
SOUNDS += SP0250
#SOUNDS += SPU
SOUNDS += CDDA
SOUNDS += ICS2115
#SOUNDS += I5000_SND
SOUNDS += ST0016
#SOUNDS += NILE
#SOUNDS += X1_010
#SOUNDS += VRENDER0
SOUNDS += VOTRAX
#SOUNDS += ES8712
#SOUNDS += CDP1869
#SOUNDS += S14001A
#SOUNDS += WAVE
#SOUNDS += SID6581
#SOUNDS += SID8580
#SOUNDS += SP0256
SOUNDS += DIGITALKER
#SOUNDS += CDP1863
#SOUNDS += CDP1864
#SOUNDS += ZSG2
#SOUNDS += MOS656X
#SOUNDS += S2636
#SOUNDS += ASC
#SOUNDS += MAS3507D
#SOUNDS += LMC1992
#SOUNDS += AWACS
#SOUNDS += YMZ770

#-------------------------------------------------
# specify available video cores
#-------------------------------------------------

VIDEOS += SEGA315_5124
VIDEOS += SEGA315_5313
VIDEOS += BUFSPRITE
#VIDEOS += CDP1861
#VIDEOS += CDP1862
#VIDEOS += CRT9007
#VIDEOS += CRT9021
#VIDEOS += CRT9212
#VIDEOS += DL1416
#VIDEOS += DM9368
#VIDEOS += EF9340_1
#VIDEOS += H63484
#VIDEOS += HD44102
#VIDEOS += HD44352
#VIDEOS += HD44780
#VIDEOS += HD61830
VIDEOS += HD63484
#VIDEOS += HD66421
#VIDEOS += HUC6202
#VIDEOS += HUC6260
#VIDEOS += HUC6261
#VIDEOS += HUC6270
#VIDEOS += HUC6272
#VIDEOS += I8244
#VIDEOS += I8275
#VIDEOS += K053250
#VIDEOS += M50458
#VIDEOS += MB90082
VIDEOS += MC6845
#VIDEOS += MC6847
#VIDEOS += MSM6255
#VIDEOS += PC_CGA
#VIDEOS += PC_VGA
#VIDEOS += POLY
#VIDEOS += PSX
#VIDEOS += RAMDAC
#VIDEOS += S2636
#VIDEOS += SAA5050
#VIDEOS += SED1330
#VIDEOS += STVVDP
VIDEOS += TLC34076
VIDEOS += TMS34061
#VIDEOS += TMS3556
#VIDEOS += TMS9927
#VIDEOS += TMS9928A
#VIDEOS += UPD3301
#VIDEOS += UPD7220
#VIDEOS += UPD7227
#VIDEOS += V9938
#VIDEOS += VOODOO

#-------------------------------------------------
# specify available machine cores
#-------------------------------------------------

MACHINES += BANKDEV
#MACHINES += NCR53C7XX
#MACHINES += LSI53C810
MACHINES += 6522VIA
#MACHINES += TPI6525
#MACHINES += 6526CIA
MACHINES += RIOT6532
MACHINES += 6821PIA
#MACHINES += 6840PTM
#MACHINES += ACIA6850
MACHINES += 68681
#MACHINES += 7200FIFO
#MACHINES += TTL74123
#MACHINES += TTL74145
#MACHINES += TTL74148
#MACHINES += TTL74153
#MACHINES += TTL74181
#MACHINES += TTL7474
#MACHINES += KBDC8042
MACHINES += I8257
MACHINES += AAKARTDEV
#MACHINES += ADC0808
#MACHINES += ADC083X
#MACHINES += ADC1038
#MACHINES += ADC1213X
#MACHINES += AM53CF96
#MACHINES += AM9517A
#MACHINES += AMIGAFDC
#MACHINES += AT28C16
#MACHINES += AT29040
#MACHINES += AT45DBXX
#MACHINES += ATAFLASH
#MACHINES += AY31015
#MACHINES += BANKDEV
#MACHINES += CDP1852
#MACHINES += CDP1871
#MACHINES += COM8116
#MACHINES += CR589
#MACHINES += CTRONICS
#MACHINES += DS1302
#MACHINES += DS2401
#MACHINES += DS2404
#MACHINES += DS75160A
#MACHINES += DS75161A
#MACHINES += E0516
MACHINES += EEPROMDEV
MACHINES += ER2055
#MACHINES += ER59256
#MACHINES += F3853
MACHINES += I2CMEM
#MACHINES += I8155
#MACHINES += I8212
#MACHINES += I8214
MACHINES += I8243
#MACHINES += I8251
MACHINES += I8255
#MACHINES += I8279
#MACHINES += I8355
#MACHINES += IDE
#MACHINES += IM6402
#MACHINES += INS8154
#MACHINES += INS8250
#MACHINES += INTELFLASH
#MACHINES += JVS
#MACHINES += K033906
#MACHINES += K053252
#MACHINES += K056230
MACHINES += LATCH8
#MACHINES += LC89510
MACHINES += LDPR8210
#MACHINES += LDSTUB
#MACHINES += LDV1000
#MACHINES += LDVP931
#MACHINES += LINFLASH
#MACHINES += M6M80011AP
#MACHINES += MATSUCD
MACHINES += MB14241
#MACHINES += MB3773
MACHINES += MB87078
#MACHINES += MB89371
#MACHINES += MC146818
#MACHINES += MC2661
#MACHINES += MC6843
#MACHINES += MC6846
#MACHINES += MC6852
#MACHINES += MC6854
#MACHINES += MC68901
#MACHINES += MCCS1850
#MACHINES += MCF5206E
#MACHINES += MICROTOUCH
#MACHINES += MM58274C
#MACHINES += MM74C922
#MACHINES += MOS6526
#MACHINES += MOS6529
MACHINES += MIOT6530
#MACHINES += MOS6551
#MACHINES += MSM5832
#MACHINES += MSM58321
#MACHINES += MSM6242
#MACHINES += NCR539x
#MACHINES += NMC9306
#MACHINES += NSCSI
#MACHINES += PCF8593
#MACHINES += PCI
#MACHINES += PCKEYBRD
#MACHINES += PIC8259
#MACHINES += PIT8253
#MACHINES += PLA
#MACHINES += RF5C296
#MACHINES += ROC10937
#MACHINES += RP5C01
#MACHINES += RP5C15
MACHINES += RP5H01
#MACHINES += RTC4543
#MACHINES += RTC65271
#MACHINES += RTC9701
#MACHINES += S3520CF
#MACHINES += S3C2400
#MACHINES += S3C2410
#MACHINES += S3C2440
#MACHINES += SATURN
#MACHINES += SCSI
#MACHINES += SCUDSP
#MACHINES += SEIBU_COP
#MACHINES += SERFLASH
#MACHINES += SMC91C9X
#MACHINES += SMPC
#MACHINES += STVCD
#MACHINES += TC0091LVC
MACHINES += TIMEKPR
#MACHINES += TMP68301
MACHINES += TMS6100
#MACHINES += TMS9901
#MACHINES += TMS9902
MACHINES += UPD1990A
MACHINES += UPD4992
#MACHINES += UPD4701
#MACHINES += UPD7002
#MACHINES += UPD765
MACHINES += V3021
#MACHINES += WD_FDC
#MACHINES += WD11C00_17
#MACHINES += WD17XX
#MACHINES += WD2010
#MACHINES += WD33C93
#MACHINES += X2212
#MACHINES += X76F041
#MACHINES += X76F100
MACHINES += Z80CTC
MACHINES += Z80DART
#MACHINES += Z80DMA
MACHINES += Z80PIO
MACHINES += Z80SIO
#MACHINES += Z80STI
#MACHINES += Z8536
#MACHINES += SECFLASH
#MACHINES += PCCARD


#-------------------------------------------------
# this is the list of driver libaries that
# comprise hbmame
#-------------------------------------------------

DRVLIBS = \
	$(HBMAMEOBJ)/atari.a \
	$(HBMAMEOBJ)/capcom.a \
	$(HBMAMEOBJ)/cave.a \
	$(HBMAMEOBJ)/cinemat.a \
	$(HBMAMEOBJ)/dataeast.a \
	$(HBMAMEOBJ)/exidy.a \
	$(HBMAMEOBJ)/gottlieb.a \
	$(HBMAMEOBJ)/igs.a \
	$(HBMAMEOBJ)/irem.a \
	$(HBMAMEOBJ)/itech.a \
	$(HBMAMEOBJ)/jaleco.a \
	$(HBMAMEOBJ)/kaneko.a \
	$(HBMAMEOBJ)/konami.a \
	$(HBMAMEOBJ)/midway.a \
	$(HBMAMEOBJ)/namco.a \
	$(HBMAMEOBJ)/neogeo.a \
	$(HBMAMEOBJ)/nichibut.a \
	$(HBMAMEOBJ)/nintendo.a \
	$(HBMAMEOBJ)/pacman.a \
	$(HBMAMEOBJ)/phoenix.a \
	$(HBMAMEOBJ)/psikyo.a \
	$(HBMAMEOBJ)/sega.a \
	$(HBMAMEOBJ)/seta.a \
	$(HBMAMEOBJ)/snk.a \
	$(HBMAMEOBJ)/taito.a \
	$(HBMAMEOBJ)/tehkan.a \
	$(HBMAMEOBJ)/toaplan.a \
	$(HBMAMEOBJ)/univers.a \
	$(HBMAMEOBJ)/valadon.a \
	$(HBMAMEOBJ)/shared.a \
	$(HBMAMEOBJ)/hbmame.a 



#-------------------------------------------------
# the following files are general components and
# shared across a number of drivers
#-------------------------------------------------

$(HBMAMEOBJ)/shared.a: \
	$(MAME_MACHINE)/ticket.o \


#-------------------------------------------------
# manufacturer-specific groupings for drivers
#-------------------------------------------------

$(HBMAMEOBJ)/atari.a: \
	$(HBMAME_DRIVERS)/asteroid.o $(MAME_MACHINE)/asteroid.o $(MAME_AUDIO)/asteroid.o \
	$(HBMAME_DRIVERS)/bwidow.o $(MAME_AUDIO)/bwidow.o \
	$(HBMAME_DRIVERS)/centiped.o $(MAME_VIDEO)/centiped.o \
	$(HBMAME_DRIVERS)/gauntlet.o $(MAME_VIDEO)/gauntlet.o \
	$(HBMAME_DRIVERS)/tempest.o \
	$(MAME_MACHINE)/atarigen.o $(MAME_MACHINE)/atari_vg.o $(MAME_MACHINE)/mathbox.o $(MAME_MACHINE)/slapstic.o \
	$(MAME_VIDEO)/atarimo.o $(MAME_VIDEO)/avgdvg.o $(MAME_AUDIO)/llander.o \

$(HBMAMEOBJ)/capcom.a: \
	$(HBMAME_DRIVERS)/1942.o $(MAME_VIDEO)/1942.o \
	$(HBMAME_DRIVERS)/blktiger.o $(MAME_VIDEO)/blktiger.o \
	$(HBMAME_DRIVERS)/commando.o $(MAME_VIDEO)/commando.o \
	$(HBMAME_DRIVERS)/cps1.o $(HBMAME_VIDEO)/cps1.o \
	$(MAME_MACHINE)/kabuki.o $(HBMAME_DRIVERS)/fcrash.o \
	$(HBMAME_DRIVERS)/cps2.o $(MAME_MACHINE)/cps2crpt.o \
	$(HBMAME_DRIVERS)/mitchell.o $(MAME_VIDEO)/mitchell.o 

$(HBMAMEOBJ)/cave.a: \
	$(HBMAME_DRIVERS)/cave.o $(MAME_VIDEO)/cave.o \

$(HBMAMEOBJ)/cinemat.a: \
	$(HBMAME_DRIVERS)/cinemat.o $(MAME_AUDIO)/cinemat.o $(MAME_VIDEO)/cinemat.o \

$(HBMAMEOBJ)/dataeast.a: \
	$(MAME_MACHINE)/decocpu7.o $(MAME_MACHINE)/deco222.o \
	$(HBMAME_DRIVERS)/astrof.o $(MAME_AUDIO)/astrof.o \
	$(HBMAME_DRIVERS)/btime.o $(MAME_MACHINE)/btime.o $(MAME_VIDEO)/btime.o \
	$(HBMAME_DRIVERS)/pcktgal.o $(MAME_VIDEO)/pcktgal.o \
	$(MAME_VIDEO)/decbac06.o $(MAME_VIDEO)/decmxc06.o \
	$(HBMAME_DRIVERS)/dec0.o $(MAME_MACHINE)/dec0.o $(MAME_VIDEO)/dec0.o \

$(HBMAMEOBJ)/exidy.a: \
	$(HBMAME_DRIVERS)/exidy.o $(MAME_AUDIO)/exidy.o $(MAME_VIDEO)/exidy.o \
	$(MAME_AUDIO)/targ.o \

$(HBMAMEOBJ)/gottlieb.a: \
	$(HBMAME_DRIVERS)/gottlieb.o $(MAME_AUDIO)/gottlieb.o $(MAME_VIDEO)/gottlieb.o \

$(HBMAMEOBJ)/igs.a: \
	$(HBMAME_DRIVERS)/pgm.o $(MAME_VIDEO)/pgm.o \
	$(MAME_MACHINE)/pgmcrypt.o \
	$(MAME_MACHINE)/pgmprot_orlegend.o \
	$(MAME_MACHINE)/pgmprot_igs027a_type1.o \
	$(MAME_MACHINE)/pgmprot_igs027a_type2.o \
	$(MAME_MACHINE)/pgmprot_igs027a_type3.o \
	$(MAME_MACHINE)/pgmprot_igs025_igs012.o \
	$(MAME_MACHINE)/pgmprot_igs025_igs022.o \
	$(MAME_MACHINE)/pgmprot_igs025_igs028.o \
	$(MAME_MACHINE)/igs022.o \
	$(MAME_MACHINE)/igs025.o \
	$(MAME_MACHINE)/igs028.o \

$(HBMAMEOBJ)/irem.a: \
	$(HBMAME_DRIVERS)/m52.o $(MAME_VIDEO)/m52.o \
	$(MAME_MACHINE)/irem_cpu.o \
	$(MAME_AUDIO)/irem.o \

$(HBMAMEOBJ)/itech.a: \
	$(HBMAME_DRIVERS)/itech8.o $(MAME_MACHINE)/slikshot.o $(MAME_VIDEO)/itech8.o \

$(HBMAMEOBJ)/jaleco.a: \
	$(HBMAME_DRIVERS)/exerion.o $(MAME_VIDEO)/exerion.o \

$(HBMAMEOBJ)/konami.a: \
	$(MAME_VIDEO)/tutankhm.o \
	$(HBMAME_DRIVERS)/shaolins.o $(MAME_VIDEO)/shaolins.o \
	$(HBMAME_DRIVERS)/trackfld.o $(MAME_MACHINE)/konami1.o $(MAME_AUDIO)/trackfld.o $(MAME_VIDEO)/trackfld.o \
	$(MAME_AUDIO)/hyprolyb.o \
	$(HBMAME_DRIVERS)/nemesis.o $(MAME_VIDEO)/nemesis.o \
	$(HBMAME_DRIVERS)/yiear.o $(MAME_VIDEO)/yiear.o \
	$(HBMAME_DRIVERS)/1943.o $(MAME_VIDEO)/1943.o \
	$(HBMAME_DRIVERS)/tmnt.o $(MAME_VIDEO)/tmnt.o \
	$(MAME_VIDEO)/konami_helper.o \
	$(MAME_VIDEO)/k051960.o \
	$(MAME_VIDEO)/k052109.o \
	$(MAME_VIDEO)/k053244_k053245.o \
	$(MAME_VIDEO)/k053251.o \
	$(MAME_VIDEO)/k053936.o \
	$(MAME_VIDEO)/k054000.o \

$(HBMAMEOBJ)/midway.a: \
	$(HBMAME_DRIVERS)/astrocde.o $(MAME_VIDEO)/astrocde.o \
	$(HBMAME_DRIVERS)/mcr.o $(MAME_MACHINE)/mcr.o $(MAME_AUDIO)/midway.o $(MAME_VIDEO)/mcr.o \
	$(HBMAME_DRIVERS)/mcr3.o $(MAME_VIDEO)/mcr3.o \
	$(MAME_DRIVERS)/mcr68.o $(MAME_VIDEO)/mcr68.o \
	$(HBMAME_DRIVERS)/williams.o $(MAME_MACHINE)/williams.o $(MAME_AUDIO)/williams.o $(MAME_VIDEO)/williams.o \
	$(MAME_AUDIO)/dcs.o $(MAME_AUDIO)/cage.o $(MAME_MACHINE)/midwayic.o \
	$(HBMAME_DRIVERS)/midtunit.o $(MAME_MACHINE)/midtunit.o $(MAME_VIDEO)/midtunit.o \
	$(HBMAME_DRIVERS)/midwunit.o $(MAME_MACHINE)/midwunit.o \
	$(HBMAME_DRIVERS)/midyunit.o $(MAME_MACHINE)/midyunit.o $(MAME_VIDEO)/midyunit.o \
	$(MAME_AUDIO)/gorf.o \
	$(MAME_AUDIO)/wow.o \

$(HBMAMEOBJ)/namco.a: \
	$(HBMAME_DRIVERS)/mappy.o $(MAME_VIDEO)/mappy.o \
	$(HBMAME_DRIVERS)/pacland.o $(MAME_VIDEO)/pacland.o \
	$(MAME_DRIVERS)/polepos.o $(MAME_AUDIO)/polepos.o $(MAME_VIDEO)/polepos.o \
	$(HBMAME_DRIVERS)/rallyx.o $(MAME_VIDEO)/rallyx.o $(MAME_AUDIO)/timeplt.o \
	$(MAME_MACHINE)/namcoio.o $(MAME_MACHINE)/namco50.o \
	$(MAME_AUDIO)/namco52.o $(MAME_AUDIO)/namco54.o $(MAME_VIDEO)/bosco.o $(MAME_VIDEO)/digdug.o \
	$(MAME_MACHINE)/namco06.o $(MAME_MACHINE)/namco51.o $(MAME_MACHINE)/namco53.o \
	$(MAME_MACHINE)/xevious.o $(MAME_VIDEO)/xevious.o \
	$(HBMAME_DRIVERS)/galaga.o $(MAME_AUDIO)/galaga.o $(MAME_VIDEO)/galaga.o \
	$(HBMAME_DRIVERS)/galaxian.o $(MAME_AUDIO)/galaxian.o $(HBMAME_VIDEO)/galaxian.o \

$(HBMAMEOBJ)/neogeo.a: \
	$(HBMAME_DRIVERS)/neogeo.o $(HBMAME_VIDEO)/neogeo.o \
	$(HBMAME_MACHINE)/neoboot.o \
	$(HBMAME_MACHINE)/neocrypt.o \
	$(HBMAME_MACHINE)/neoprot.o \
	$(HBMAME_MACHINE)/ng_memcard.o \
	$(HBMAME_VIDEO)/neogeo_spr.o \
	$(OBJ)/emu/imagedev/cartslot.o \

$(HBMAMEOBJ)/nichibut.a: \
	$(HBMAME_DRIVERS)/cclimber.o $(MAME_MACHINE)/cclimber.o $(MAME_AUDIO)/cclimber.o $(MAME_VIDEO)/cclimber.o \
	$(HBMAME_DRIVERS)/terracre.o $(MAME_VIDEO)/terracre.o \

$(HBMAMEOBJ)/nintendo.a: \
	$(HBMAME_DRIVERS)/dkong.o $(MAME_AUDIO)/dkong.o $(MAME_VIDEO)/dkong.o \
	$(HBMAME_DRIVERS)/mario.o $(MAME_AUDIO)/mario.o $(MAME_VIDEO)/mario.o \
	$(HBMAME_DRIVERS)/playch10.o $(MAME_MACHINE)/playch10.o $(MAME_VIDEO)/playch10.o \
	$(HBMAME_DRIVERS)/vsnes.o $(MAME_MACHINE)/vsnes.o $(MAME_VIDEO)/vsnes.o \
	$(MAME_VIDEO)/ppu2c0x.o $(EMU_MACHINE)/z80dma.o \

$(HBMAMEOBJ)/pacman.a: \
	$(HBMAME_DRIVERS)/jrpacman.o  $(HBMAME_VIDEO)/pacman.o \
	$(HBMAME_DRIVERS)/pacman.o \
	$(HBMAME_DRIVERS)/pengo.o \

$(HBMAMEOBJ)/phoenix.a: \
	$(HBMAME_DRIVERS)/phoenix.o $(MAME_AUDIO)/phoenix.o $(MAME_VIDEO)/phoenix.o \
	$(MAME_AUDIO)/pleiads.o \

$(HBMAMEOBJ)/psikyo.a: \
	$(HBMAME_DRIVERS)/psikyosh.o $(MAME_VIDEO)/psikyosh.o \

$(HBMAMEOBJ)/sega.a: \
	$(HBMAME_DRIVERS)/segac2.o \
	$(HBMAME_DRIVERS)/segag80r.o $(MAME_MACHINE)/segag80.o $(MAME_AUDIO)/segag80r.o $(MAME_VIDEO)/segag80r.o \
	$(HBMAME_DRIVERS)/system1.o $(MAME_VIDEO)/system1.o $(MAME_MACHINE)/mc8123.o \
	$(HBMAME_DRIVERS)/system16.o $(MAME_VIDEO)/system16.o $(MAME_MACHINE)/segaic16.o $(MAME_VIDEO)/segaic16.o \
	$(MAME_VIDEO)/sega16sp.o $(MAME_MACHINE)/segacrp2.o \
	$(MAME_DRIVERS)/megaplay.o $(MAME_MACHINE)/megadriv.o $(MAME_MACHINE)/segacrpt.o \
	$(MAME_DRIVERS)/megatech.o $(MAME_DRIVERS)/segae.o $(MAME_AUDIO)/segasnd.o \
	$(MAME_MACHINE)/fd1089.o $(MAME_MACHINE)/fd1094.o $(MAME_MACHINE)/315_5296.o \
	$(HBMAME_DRIVERS)/segaorun.o $(MAME_VIDEO)/segaorun.o $(MAME_VIDEO)/segaic16_road.o \

$(HBMAMEOBJ)/seta.a: \
	$(HBMAME_DRIVERS)/simple_st0016.o $(MAME_MACHINE)/st0016.o \

$(HBMAMEOBJ)/snk.a: \
	$(HBMAME_DRIVERS)/snk6502.o $(MAME_AUDIO)/snk6502.o $(MAME_VIDEO)/snk6502.o \

$(HBMAMEOBJ)/taito.a: \
	$(HBMAME_DRIVERS)/bublbobl.o $(MAME_MACHINE)/bublbobl.o $(MAME_VIDEO)/bublbobl.o \
	$(HBMAME_DRIVERS)/rastan.o $(MAME_VIDEO)/rastan.o \
	$(HBMAME_DRIVERS)/taito_b.o $(MAME_VIDEO)/taito_b.o \
	$(HBMAME_DRIVERS)/taito_f2.o $(MAME_VIDEO)/taito_f2.o \
	$(HBMAME_DRIVERS)/taito_f3.o $(MAME_VIDEO)/taito_f3.o $(MAME_AUDIO)/taito_en.o \
	$(MAME_MACHINE)/cchip.o $(MAME_MACHINE)/taitoio.o\
	$(MAME_AUDIO)/taitosnd.o \
	$(MAME_VIDEO)/taito_helper.o \
	$(MAME_VIDEO)/pc080sn.o \
	$(MAME_VIDEO)/pc090oj.o \
	$(MAME_VIDEO)/tc0100scn.o \
	$(MAME_VIDEO)/tc0110pcr.o \
	$(MAME_VIDEO)/tc0180vcu.o \
	$(MAME_VIDEO)/tc0280grd.o \
	$(MAME_VIDEO)/tc0360pri.o \
	$(MAME_VIDEO)/tc0480scp.o \
	$(HBMAME_DRIVERS)/volfied.o $(MAME_MACHINE)/volfied.o $(MAME_VIDEO)/volfied.o \
	$(HBMAME_DRIVERS)/retofinv.o $(MAME_MACHINE)/retofinv.o $(MAME_VIDEO)/retofinv.o \
	$(HBMAME_DRIVERS)/arkanoid.o $(HBMAME_MACHINE)/arkanoid.o $(MAME_VIDEO)/arkanoid.o 

$(HBMAMEOBJ)/tehkan.a: \
	$(HBMAME_DRIVERS)/bombjack.o $(MAME_VIDEO)/bombjack.o \
	$(HBMAME_DRIVERS)/tecmo.o $(MAME_VIDEO)/tecmo.o \
	$(HBMAME_DRIVERS)/tehkanwc.o $(MAME_VIDEO)/tehkanwc.o \
	$(MAME_VIDEO)/tecmo_spr.o \

$(HBMAMEOBJ)/toaplan.a: \
	$(HBMAME_DRIVERS)/toaplan2.o $(MAME_VIDEO)/toaplan2.o \
	$(HBMAME_DRIVERS)/twincobr.o $(MAME_MACHINE)/twincobr.o $(MAME_VIDEO)/twincobr.o \
	$(MAME_DRIVERS)/wardner.o  $(MAME_VIDEO)/gp9001.o \
	$(MAME_MACHINE)/nmk112.o $(MAME_VIDEO)/toaplan_scu.o \

$(HBMAMEOBJ)/univers.a: \
	$(HBMAME_DRIVERS)/docastle.o $(MAME_MACHINE)/docastle.o $(MAME_VIDEO)/docastle.o \
	$(HBMAME_DRIVERS)/mrdo.o $(MAME_VIDEO)/mrdo.o \

$(HBMAMEOBJ)/valadon.a: \
	$(HBMAME_DRIVERS)/bagman.o $(MAME_MACHINE)/bagman.o $(MAME_VIDEO)/bagman.o \


$(HBMAMEOBJ)/hbmame.a: \
	$(HBMAME_DRIVERS)/mineswp.o \
	$(HBMAME_DRIVERS)/ir.o $(HBMAME_DRIVERS)/schaser.o $(HBMAME_DRIVERS)/spacmiss.o \
	$(HBMAME_DRIVERS)/monaco.o \
	$(HBMAME_DRIVERS)/atari_s1.o $(MAME_MACHINE)/genpin.o 

#	$(HBMAME_DRIVERS)/ertictac.o $(MAME_MACHINE)/archimds.o $(MAME_VIDEO)/archimds.o 

#-------------------------------------------------
# layout dependencies
#-------------------------------------------------

$(HBMAME_DRIVERS)/atari_s1.o:           $(MAME_LAYOUT)/atari_s1.lh
$(HBMAME_DRIVERS)/spacmiss.o:		$(MAME_LAYOUT)/galactic.lh
$(HBMAME_DRIVERS)/neogeo.o:		$(HBMAME_LAYOUT)/neogeo.lh
$(HBMAME_DRIVERS)/monaco.o:		$(HBMAME_LAYOUT)/monaco.lh


#-------------------------------------------------
# Only needed for compiling
#-------------------------------------------------


$(HBMAME_DRIVERS)/astrocde.o:		$(MAME_LAYOUT)/gorf.lh $(MAME_LAYOUT)/tenpindx.lh \
					$(MAME_LAYOUT)/spacezap.lh $(MAME_LAYOUT)/seawolf2.lh
$(MAME_MACHINE)/megacd.o:		$(MAME_LAYOUT)/megacd.lh
$(HBMAME_DRIVERS)/tehkanwc.o:		$(MAME_LAYOUT)/gridiron.lh
$(HBMAME_DRIVERS)/mcr3.o:		$(MAME_LAYOUT)/turbotag.lh $(MAME_LAYOUT)/spyhunt.lh
$(HBMAME_DRIVERS)/mcr.o:		$(MAME_LAYOUT)/dpoker.lh
$(HBMAME_DRIVERS)/asteroid.o:		$(MAME_LAYOUT)/astdelux.lh
$(HBMAME_DRIVERS)/segaorun.o:		$(MAME_LAYOUT)/outrun.lh
$(MAME_DRIVERS)/polepos.o:		$(MAME_LAYOUT)/polepos.lh $(MAME_LAYOUT)/topracer.lh
$(HBMAME_DRIVERS)/cinemat.o:		$(MAME_LAYOUT)/armora.lh $(MAME_LAYOUT)/barrier.lh \
					$(MAME_LAYOUT)/demon.lh $(MAME_LAYOUT)/solarq.lh \
					$(MAME_LAYOUT)/starcas.lh $(MAME_LAYOUT)/sundance.lh \
					$(MAME_LAYOUT)/tailg.lh $(MAME_LAYOUT)/wotw.lh
$(HBMAME_DRIVERS)/cave.o:		$(MAME_LAYOUT)/ppsatan.lh
$(HBMAME_DRIVERS)/nemesis.o:		$(MAME_LAYOUT)/konamigt.lh
