set from=c:\HBMAME
set to=c:\HBMAME\docs\release

rd %to%\src /q /s

md %to%\src\emu

copy /Y %from%\makefile                             %to%\src
copy /Y %from%\src\version.cpp                      %to%\src
copy /Y %from%\src\emu\validity.cpp                 %to%\src\emu
copy /Y %from%\src\emu\digfx.h                      %to%\src\emu
copy /Y %from%\src\emu\emuopts.*                    %to%\src\emu
copy /Y %from%\src\emu\romload.cpp                  %to%\src\emu
copy /Y %from%\src\emu\video.*                      %to%\src\emu

md %to%\src\frontend\mame\ui
copy /Y %from%\src\frontend\mame\audit.*            %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\clifront.cpp       %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\language.cpp       %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\info.cpp           %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\mameopts.*         %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\ui\about.cpp       %to%\src\frontend\mame\ui
copy /Y %from%\src\frontend\mame\ui\inifile.cpp     %to%\src\frontend\mame\ui

md %to%\src\devices\cpu\m68000
copy /Y %from%\src\devices\cpu\m68000\m68kcpu.cpp   %to%\src\devices\cpu\m68000
copy /Y %from%\src\devices\cpu\m68000\m68k_in.cpp   %to%\src\devices\cpu\m68000

md %to%\src\devices\video
copy /Y %from%\src\devices\video\vector.cpp         %to%\src\devices\video

md %to%\src\mame\drivers
copy /Y %from%\src\mame\drivers\cinemat.cpp         %to%\src\mame\drivers
copy /Y %from%\src\mame\drivers\galaxian.cpp        %to%\src\mame\drivers
copy /Y %from%\src\mame\drivers\konamigx.cpp        %to%\src\mame\drivers
copy /Y %from%\src\mame\drivers\model2.cpp          %to%\src\mame\drivers

md %to%\src\mame\includes
copy /Y %from%\src\mame\includes\mhavoc.h           %to%\src\mame\includes

md %to%\src\mame\machine
copy /Y %from%\src\mame\machine\mhavoc.cpp          %to%\src\mame\machine

md %to%\src\mame\video
copy /Y %from%\src\mame\video\avgdvg.cpp            %to%\src\mame\video
copy /Y %from%\src\mame\video\decbac06.cpp          %to%\src\mame\video
copy /Y %from%\src\mame\video\midtunit.cpp          %to%\src\mame\video
copy /Y %from%\src\mame\video\sega16sp.cpp          %to%\src\mame\video

md %to%\src\hbmame
xcopy /E /Y %from%\src\hbmame                       %to%\src\hbmame

md %to%\src\osd\winui
xcopy /E /Y %from%\src\osd\winui                    %to%\src\osd\winui

md %to%\src\osd\modules\render
copy /Y %from%\src\osd\modules\render\drawd3d.cpp   %to%\src\osd\modules\render

md %to%\src\osd\windows
copy /Y %from%\src\osd\windows\winmain.*            %to%\src\osd\windows

rem now save all our stuff to github
copy %from%\*.bat                                   %to%\build
xcopy /E /Y %from%\scripts                          %to%\scripts
