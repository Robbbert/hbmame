set from=c:\HBMAME
set to=c:\HBMAME\docs\release

rd %to%\src /q /s

md %to%\src\emu
copy /Y %from%\src\version.cpp %to%\src\version.cpp
copy /Y %from%\src\emu\validity.cpp %to%\src\emu
copy /Y %from%\src\emu\digfx.h %to%\src\emu
copy /Y %from%\src\emu\video.* %to%\src\emu
copy /Y %from%\src\version.cpp %to%\src

md %to%\src\frontend\mame
copy /Y %from%\src\frontend\mame\audit.* %to%\src\frontend\mame

md %to%\src\devices\cpu\m68000
copy /Y %from%\src\devices\cpu\m68000\m68kcpu.cpp %to%\src\devices\cpu\m68000

md %to%\src\mame\drivers
md %to%\src\mame\includes
md %to%\src\mame\video
copy /Y %from%\src\mame\drivers\jrpacman.cpp %to%\src\mame\drivers
copy /Y %from%\src\mame\drivers\neogeo_noslot.cpp %to%\src\mame\drivers
copy /Y %from%\src\mame\includes\cps3.h %to%\src\mame\includes
copy /Y %from%\src\mame\video\neogeo_spr.cpp %to%\src\mame\video

md %to%\src\hbmame
rem does osd as well
xcopy /E /Y %from%\src\hbmame %to%\src\hbmame

md %to%\src\osd\winui
xcopy /E /Y %from%\src\osd\winui %to%\src\osd\winui

rem now save all our stuff to github
copy %from%\*.bat %to%\build
xcopy /E /Y %from%\scripts %to%\scripts

rem convert all the unix documents to windows format for notepad
type %from%\docs\BSD3Clause.txt    | MORE /P > %to%\docs\BSD3Clause.txt
type %from%\docs\LICENSE           | MORE /P > %to%\docs\license.txt
type %from%\docs\winui_license.txt | MORE /P > %to%\docs\winui_license.txt

pause
echo off
cls
echo.
echo RAR up everything.
echo.

pause
