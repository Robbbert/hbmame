@echo off
set MINGW64=E:\Mingw\10-1-0\mingw64
set minpath=%MINGW64%\bin
set oldpath=%Path%
set Path=%minpath%;%oldpath%
touch src\version.cpp
echo.|time
%MINGW64%\bin\make PTR64=1 TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
if exist hbmame.exe %minpath%\strip -s hbmame.exe
if exist hbmameui.exe %minpath%\strip -s hbmameui.exe
set minpath=

