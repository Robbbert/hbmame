###########################################################################
#
#   windows.mak
#
#   HBMAME Windows-specific makefile
#
###########################################################################

HBMAME_WINSRC = src/hbmame/osd/windows
HBMAME_WINOBJ = $(OBJ)/hbmame/osd/windows

OBJDIRS += \
	$(HBMAMEOBJ)/osd \
	$(HBMAMEOBJ)/osd/windows

RESFILE = $(HBMAME_WINOBJ)/hbmame.res

$(LIBOSD): $(OSDOBJS)

$(LIBOCORE): $(OSDCOREOBJS)

$(LIBOCORE_NOMAIN): $(OSDCOREOBJS:$(WINOBJ)/main.o=)

#-------------------------------------------------
# generic rules for the resource compiler
#-------------------------------------------------

$(HBMAME_WINOBJ)/%.res: $(HBMAME_WINSRC)/%.rc
	@echo Compiling resources $<...
	$(RC) $(RCDEFS) $(RCFLAGS) --include-dir $(HBMAME_WINOBJ) -o $@ -i $<


#-------------------------------------------------
# rules for resource file
#-------------------------------------------------

$(RESFILE): $(HBMAME_WINSRC)/hbmame.rc $(HBMAME_WINOBJ)/hbmamevers.rc

$(HBMAME_WINOBJ)/hbmamevers.rc: $(BUILDOUT)/verinfo$(BUILD_EXE) $(SRC)/version.c
	@echo Emitting $@...
	@"$(BUILDOUT)/verinfo$(BUILD_EXE)" -b mame $(SRC)/version.c > $@