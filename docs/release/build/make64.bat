@echo off
set MINGW64=E:\Mingw\10-1-0\mingw64
set minpath=%MINGW64%\bin
set oldpath=%Path%
set Path=%minpath%;%oldpath%
echo.|time
%MINGW64%\bin\make PTR64=1 TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
if exist hbmame64.exe %minpath%\strip -s hbmame64.exe
if exist hbmameui64.exe %minpath%\strip -s hbmameui64.exe
set minpath=

