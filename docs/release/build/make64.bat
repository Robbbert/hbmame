@echo off
set MINGW64=E:\Mingw\4-9-1\mingw64-w64
set minpath=%MINGW64%\bin;%MINGW64%\opt\bin;%MINGW64%\Qt\bin
set oldpath=%Path%
set Path=%minpath%;%oldpath%
echo.|time
%MINGW64%\bin\make PTR64=1 TARGET=hbmame NO_SYMBOLS=1 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
set minpath=

