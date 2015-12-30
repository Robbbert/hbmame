@echo off
set MINGW32=E:\Mingw\4-9-1\mingw64-w32
set minpath=%MINGW32%\bin;%MINGW32%\opt\bin;%MINGW32%\Qt\bin
set oldpath=%Path%
set Path=%minpath%;%oldpath%
echo.|time
%MINGW32%\bin\make PTR64=0 TARGET=hbmame NO_SYMBOLS=1 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
set minpath=

