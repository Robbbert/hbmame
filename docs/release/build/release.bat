@echo off
echo HAVE YOU UPDATED the MAKEFILE ???
echo Also update the date in HBNEW ??
echo --AND-- disabled file creation in ngneo ?? neogeo.cpp, line 2087
pause


call newsrc.bat
call clean.bat
call clean.bat
call clean.bat
call clean.bat
del hbmame*.exe
del hbmame*.sym
call make64 -j4 "OSD=winui" "REGENIE=1" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmameui.sym
call make64 -j4 "ARCHOPTS='-fuse-ld=lld'" "REGENIE=1" %1 %2 %3
:end
