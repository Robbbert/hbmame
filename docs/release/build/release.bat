@echo off
echo HAVE YOU UPDATED version.cpp and makefile ???
pause


call newsrc.bat
call clean.bat
call clean.bat
call clean.bat
call clean.bat
del hbmame*.exe
del hbmame*.sym
call make64 -j4 "OSD=winui" "REGENIE=1" %1 %2 %3
if not exist hbmameui64.exe goto end
del hbmameui64.sym
call make64 -j4 "ARCHOPTS='-fuse-ld=lld'" "REGENIE=1" %1 %2 %3
copy hbmame64.exe hbmame.exe
copy hbmameui64.exe hbmameui.exe
:end
