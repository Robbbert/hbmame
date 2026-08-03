@echo off
set MINGW64=C:\Mingw\13-2\mingw64
set minpath=%MINGW64%\bin
set oldpath=%Path%
set Path=%minpath%;%oldpath%
echo.|time
%MINGW64%\bin\make PTR64=1 TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
if exist hbmame.exe %minpath%\strip -s hbmame.exe
if exist hbmameui.exe %minpath%\strip -s hbmameui.exe
if exist hbmameui64.exe %minpath%\strip -s hbmameui64.exe
set minpath=

