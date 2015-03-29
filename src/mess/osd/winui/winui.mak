###########################################################################
#
#   winui.mak
#
#   MESS Windows-specific makefile
#
###########################################################################

# build the executable names
RCFLAGS += -DMESS

EMU_EXE = $(PREFIX)$(NAME)$(SUFFIX)$(SUFFIX64)$(SUFFIXDEBUG)$(EXE)
BUILD += $(EMU_EXE)

MESS_WINUISRC = $(SRC)/mess/osd/winui
MESS_WINUIOBJ = $(OBJ)/mess/osd/winui
WINUISRC = $(SRC)/osd/winui
WINUIOBJ = $(OBJ)/osd/winui
OSDOBJ = $(OBJ)/osd

RESFILE = $(MESS_WINUIOBJ)/messui.res

INCPATH += \
	-I$(MESSSRC)/osd/winui

OBJDIRS += \
	$(MESSOBJ)/osd \
	$(MESSOBJ)/osd/winui


$(MESS_WINUIOBJ)/messui.res:	$(WINUISRC)/mameui.rc $(MESS_WINUISRC)/messui.rc $(WINUISRC)/resource.h $(WINUISRC)/resourcems.h $(WINUIOBJ)/mamevers.rc
	@echo Compiling resources $<...
	$(RC) $(RCDEFS) $(RCFLAGS) --include-dir $(WINUISRC) --include-dir $(MESS_WINUISRC) --include-dir $(WINUIOBJ) -o $@ -i $(MESS_WINUISRC)/messui.rc

$(WINUIOBJ)/mamevers.rc: $(SRC)/build/verinfo.py $(SRC)/version.c
	@echo Emitting $@...
	$(PYTHON) $(SRC)/build/verinfo.py -b mess $(SRC)/version.c > $@

$(LIBOSD): $(OSDOBJS)

$(LIBOCORE): $(OSDCOREOBJS)

$(LIBOCORE_NOMAIN): $(OSDCOREOBJS:$(WINOBJ)/main.o=)

#-------------------------------------------------
# OSD Windows library
#-------------------------------------------------

WINOSDOBJS = \
	$(OSDOBJ)/modules/render/drawd3d.o \
	$(OSDOBJ)/modules/render/d3d/d3d9intf.o \
	$(OSDOBJ)/modules/render/d3d/d3dhlsl.o \
	$(OSDOBJ)/modules/render/drawdd.o \
	$(OSDOBJ)/modules/render/drawgdi.o \
	$(OSDOBJ)/modules/render/drawbgfx.o \
	$(OSDOBJ)/modules/render/drawnone.o \
	$(WINOBJ)/input.o \
	$(WINOBJ)/output.o \
	$(OSDOBJ)/modules/lib/osdlib_win32.o \
	$(WINOBJ)/video.o \
	$(WINOBJ)/window.o \
	$(OSDOBJ)/modules/debugger/debugwin.o \
	$(OSDOBJ)/modules/lib/osdobj_common.o \
	$(OSDOBJ)/modules/midi/portmidi.o \
	$(OSDOBJ)/modules/midi/none.o \
	$(OSDOBJ)/modules/sync/work_osd.o \
	$(OSDOBJ)/modules/osdmodule.o \
	$(OSDOBJ)/modules/sound/js_sound.o  \
	$(OSDOBJ)/modules/sound/direct_sound.o  \
	$(OSDOBJ)/modules/sound/sdl_sound.o  \
	$(OSDOBJ)/modules/sound/none.o  \
	$(OSDOBJ)/modules/font/font_sdl.o \
	$(OSDOBJ)/modules/font/font_windows.o \
	$(OSDOBJ)/modules/font/font_osx.o \
	$(OSDOBJ)/modules/font/font_none.o \
	$(OSDOBJ)/modules/debugger/debugwin.o \
	$(OSDOBJ)/modules/debugger/debugint.o \
	$(OSDOBJ)/modules/debugger/debugqt.o \
	$(OSDOBJ)/modules/debugger/none.o \
	$(OSDOBJ)/modules/debugger/win/consolewininfo.o \
	$(OSDOBJ)/modules/debugger/win/debugbaseinfo.o \
	$(OSDOBJ)/modules/debugger/win/debugviewinfo.o \
	$(OSDOBJ)/modules/debugger/win/debugwininfo.o \
	$(OSDOBJ)/modules/debugger/win/disasmbasewininfo.o \
	$(OSDOBJ)/modules/debugger/win/disasmviewinfo.o \
	$(OSDOBJ)/modules/debugger/win/disasmwininfo.o \
	$(OSDOBJ)/modules/debugger/win/editwininfo.o \
	$(OSDOBJ)/modules/debugger/win/logwininfo.o \
	$(OSDOBJ)/modules/debugger/win/memoryviewinfo.o \
	$(OSDOBJ)/modules/debugger/win/memorywininfo.o \
	$(OSDOBJ)/modules/debugger/win/pointswininfo.o \
	$(OSDOBJ)/modules/debugger/win/uimetrics.o \
	$(OSDOBJ)/modules/netdev/pcap.o \
	$(OSDOBJ)/modules/netdev/taptun.o \
	$(OSDOBJ)/modules/netdev/none.o \
	$(WINOBJ)/winmain.o \
	$(UIOBJ)/newui.o \

ifdef USE_OPENGL
WINOSDOBJS += \
	$(OSDOBJ)/modules/render/drawogl.o \
	$(OSDOBJ)/modules/opengl/gl_shader_tool.o \
	$(OSDOBJ)/modules/opengl/gl_shader_mgr.o
endif

ifndef DONT_USE_NETWORK
DEFS +=	-DSDLMAME_NET_PCAP
endif

BGFX_LIB = $(OBJ)/libbgfx.a
INCPATH += -I$(3RDPARTY)/bgfx/include -I$(3RDPARTY)/bx/include

$(EMU_EXE): $(VERSIONOBJ) $(EMUINFOOBJ) $(DRIVLISTOBJ) $(DRVLIBS) $(WINOSDOBJS) $(LIBBUS) $(LIBOPTIONAL) $(LIBEMU) $(LIBDASM) $(LIBUTIL) $(EXPAT) $(SOFTFLOAT) $(JPEG_LIB) $(FLAC_LIB) $(7Z_LIB) $(FORMATS_LIB) $(LUA_LIB) $(SQLITE3_LIB) $(WEB_LIB) $(BGFX_LIB) $(ZLIB) $(LIBOCORE) $(MIDI_LIB) $(RESFILE)
	@echo Linking $@...
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
#ifeq ($(TARGETOS),win32)
#ifdef SYMBOLS
#ifndef MSVC_BUILD
#	$(OBJDUMP) --section=.text --line-numbers --syms --demangle $@ >$(FULLNAME).sym
#endif
#endif
#endif
