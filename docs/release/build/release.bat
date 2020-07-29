@echo off
echo HAVE YOU UPDATED version.cpp and makefile ???
pause


call newsrc.bat
call clean.bat
call clean.bat
call clean.bat
call clean.bat
goto 64bit

rem --- 32bit ---
del hbmameui.exe
del hbmameui.sym
call make32 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmame.exe
del hbmame.sym
call make32 -j4 %1 %2 %3

:64bit
rem --- 64bit ---
del hbmameui64.exe
del hbmameui64.sym
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui64.exe goto end
del hbmame64.exe
del hbmame64.sym
call make64 -j4 %1 %2 %3
:end
