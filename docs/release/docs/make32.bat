@echo off
set MINGW32=E:\Mingw\4-9-1\mingw64-w32
set minpath=%MINGW32%\bin
set minpath2=%minpath%;%MINGW32%\opt\bin;%MINGW32%\Qt\bin
set oldpath=%Path%
set Path=%minpath2%;%oldpath%
echo.|time
%minpath%\make TARGET=hbmame PTR64=0 %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
set minpath=
set minpath2=
set MINGW32=
