@echo off
@rem set MINGW64=C:\Mingw\11-2-0\mingw64
 set MINGW64=C:\Mingw\13-2\mingw64
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

