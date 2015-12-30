@echo off
set MINGW64=E:\Mingw\4-9-1\mingw64-w64
set minpath=%MINGW64%\bin
set minpath2=%minpath%;%MINGW64%\opt\bin;%MINGW64%\Qt\bin
set oldpath=%Path%
set Path=%minpath2%;%oldpath%
echo.|time
%minpath%\make TARGET=hbmame %1 %2 %3 %4
echo.|time
set Path=%oldpath%
set oldpath=
set minpath=
set minpath2=
set MINGW64=
