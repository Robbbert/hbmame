@echo off
echo HAVE YOU UPDATED version.cpp ???
pause


call newsrc.bat
call clean.bat
call clean.bat
call clean.bat
call clean.bat

rem --- 32bit ---
del hbmameui.exe
call make32 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmame.exe
call make32 -j4 %1 %2 %3

rem --- 64bit ---
del hbmameui64.exe
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui64.exe goto end
del hbmame64.exe
call make64 -j4 %1 %2 %3
:end
