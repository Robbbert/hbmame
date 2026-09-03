set from=c:\HBMAME
set to=c:\HBMAME\docs\release

rd %to%\src /q /s
echo D | rd %to%\scripts /q /s

md %to%\src\emu

copy /Y %from%\makefile                             %to%
copy /Y %from%\src\emu\validity.cpp                 %to%\src\emu
copy /Y %from%\src\emu\digfx.h                      %to%\src\emu
copy /Y %from%\src\emu\gamedrv.h                    %to%\src\emu
copy /Y %from%\src\emu\ioport.*                     %to%\src\emu
copy /Y %from%\src\emu\main.h                       %to%\src\emu
copy /Y %from%\src\emu\mconfig.h                    %to%\src\emu

md %to%\src\frontend\mame\ui
copy /Y %from%\src\frontend\mame\audit.*            %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\mame.*             %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\ui\about.cpp       %to%\src\frontend\mame\ui
copy /Y %from%\src\frontend\mame\ui\auditmenu.cpp   %to%\src\frontend\mame\ui
copy /Y %from%\src\frontend\mame\ui\selgame.cpp     %to%\src\frontend\mame\ui

md %to%\src\mame\atari
copy /Y %from%\src\mame\atari\atarigt.h             %to%\src\mame\atari
copy /Y %from%\src\mame\atari\cball.cpp             %to%\src\mame\atari
md %to%\src\mame\bally
copy /Y %from%\src\mame\bally\mcr.h                 %to%\src\mame\bally
md %to%\src\mame\dataeast
copy /Y %from%\src\mame\dataeast\cninja.h           %to%\src\mame\dataeast
copy /Y %from%\src\mame\dataeast\rohga.cpp          %to%\src\mame\dataeast
md %to%\src\mame\dynax
copy /Y %from%\src\mame\dynax\ddenlovr.cpp          %to%\src\mame\dynax
copy /Y %from%\src\mame\dynax\dynax.cpp             %to%\src\mame\dynax
md %to%\src\mame\exidy
copy /Y %from%\src\mame\exidy\exidy.cpp             %to%\src\mame\exidy
md %to%\src\mame\galaxian
copy /Y %from%\src\mame\galaxian\galaxian.h         %to%\src\mame\galaxian
md %to%\src\mame\gottlieb
copy /Y %from%\src\mame\gottlieb\gottlieb.h         %to%\src\mame\gottlieb
md %to%\src\mame\igs
copy /Y %from%\src\mame\igs\igs011.cpp              %to%\src\mame\igs
copy /Y %from%\src\mame\igs\pgmprot_igs027a_type1.cpp  %to%\src\mame\igs
md %to%\src\mame\konami
copy /Y %from%\src\mame\konami\konamigx.cpp         %to%\src\mame\konami
copy /Y %from%\src\mame\konami\pooyan.cpp           %to%\src\mame\konami
copy /Y %from%\src\mame\konami\timeplt.cpp          %to%\src\mame\konami
md %to%\src\mame\misc
copy /Y %from%\src\mame\misc\timelimt.cpp           %to%\src\mame\misc
md %to%\src\mame\sega
copy /Y %from%\src\mame\sega\model1.cpp             %to%\src\mame\sega
copy /Y %from%\src\mame\sega\model1io2.cpp          %to%\src\mame\sega
md %to%\src\mame\williams
copy /Y %from%\src\mame\williams\midtunit.h         %to%\src\mame\williams
copy /Y %from%\src\mame\williams\midtunit_v.cpp     %to%\src\mame\williams
copy /Y %from%\src\mame\williams\midwayic.h         %to%\src\mame\williams

md %to%\src\hbmame
xcopy /E /Y %from%\src\hbmame                       %to%\src\hbmame

md %to%\src\osd\winui
xcopy /E /Y %from%\src\osd\winui                    %to%\src\osd\winui

md %to%\src\osd\modules\render
copy /Y %from%\src\osd\modules\osdwindow.cpp        %to%\src\osd\modules
copy /Y %from%\src\osd\modules\render\drawd3d.cpp   %to%\src\osd\modules\render
md %to%\src\osd\modules\diagnostics
copy /Y %from%\src\osd\modules\diagnostics\diagnostics_win32.cpp        %to%\src\osd\modules\diagnostics
md %to%\src\osd\modules\lib
copy /Y %from%\src\osd\modules\lib\osdobj_common.cpp    %to%\src\osd\modules\lib
md %to%\src\osd\modules\sound
copy /Y %from%\src\osd\modules\sound\direct_sound.cpp   %to%\src\osd\modules\sound

md %to%\src\osd\windows
copy /Y %from%\src\osd\windows\winmain.*            %to%\src\osd\windows

rem now save all our stuff to github
md %to%\build
md %to%\scripts
copy %from%\*.bat                                   %to%\build
xcopy /E /Y %from%\scripts                          %to%\scripts
