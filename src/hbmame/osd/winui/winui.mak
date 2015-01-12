###########################################################################
#
#   winui.mak
#
#   HBMAME Windows-specific makefile
#
###########################################################################

# build the executable names
RCFLAGS += -DHBMAME

EMU_EXE = $(PREFIX)$(NAME)$(SUFFIX)$(SUFFIX64)$(SUFFIXDEBUG)$(EXE)
BUILD += $(EMU_EXE)

HBMAME_WINUISRC = $(SRC)/hbmame/osd/winui
HBMAME_WINUIOBJ = $(OBJ)/hbmame/osd/winui
WINUISRC = $(SRC)/osd/winui
WINUIOBJ = $(OBJ)/osd/winui

RESFILE = $(HBMAME_WINUIOBJ)/hbmameui.res

INCPATH += \
	-I$(HBMAMESRC)/osd/winui

OBJDIRS += \
	$(HBMAMEOBJ)/osd \
	$(HBMAMEOBJ)/osd/winui


$(HBMAME_WINUIOBJ)/hbmameui.res: $(WINUISRC)/mameui.rc $(HBMAME_WINUISRC)/messui.rc $(WINUISRC)/resource.h $(WINUIOBJ)/mamevers.rc
	@echo Compiling resources $<...
	$(RC) $(RCDEFS) $(RCFLAGS) --include-dir $(WINUISRC) --include-dir $(HBMAME_WINUISRC) --include-dir $(WINUIOBJ) -o $@ -i $(HBMAME_WINUISRC)/messui.rc

$(WINUIOBJ)/mamevers.rc: $(OBJ)/build/verinfo$(EXE) $(SRC)/version.c
	@echo Emitting $@...
	@"$(VERINFO)" -b mame $(SRC)/version.c  > $@

$(LIBOSD): $(OSDOBJS)

$(LIBOCORE): $(OSDCOREOBJS)

$(LIBOCORE_NOMAIN): $(OSDCOREOBJS:$(WINOBJ)/main.o=)

#-------------------------------------------------
# OSD Windows library
#-------------------------------------------------

WINOSDOBJS = \
	$(WINOBJ)/d3d9intf.o \
	$(WINOBJ)/drawd3d.o \
	$(WINOBJ)/drawdd.o \
	$(WINOBJ)/d3dhlsl.o \
	$(WINOBJ)/drawgdi.o \
	$(WINOBJ)/drawnone.o \
	$(WINOBJ)/input.o \
	$(WINOBJ)/output.o \
	$(WINOBJ)/../modules/sound/direct_sound.o \
	$(WINOBJ)/video.o \
	$(WINOBJ)/window.o \
	$(WINOBJ)/../modules/debugger/debugwin.o \
	$(WINOBJ)/winmain.o \
	$(WINOBJ)/winmenu.o \

$(EMU_EXE): $(VERSIONOBJ) $(EMUINFOOBJ) $(DRIVLISTOBJ) $(DRVLIBS) $(WINOSDOBJS) $(LIBBUS) $(LIBOPTIONAL) $(LIBEMU) $(LIBDASM) $(LIBUTIL) $(EXPAT) $(SOFTFLOAT) $(JPEG_LIB) $(FLAC_LIB) $(7Z_LIB) $(FORMATS_LIB) $(LUA_LIB) $(SQLITE3_LIB) $(WEB_LIB) $(ZLIB) $(LIBOCORE) $(MIDI_LIB) $(RESFILE)
	@echo Linking $@...
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
