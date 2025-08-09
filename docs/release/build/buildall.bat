@echo off
cls
echo.
echo Pre-compile steps
echo.
rem echo 1. HBMAME - CLEAN, update MAKEFILE, update VERSION.CPP
rem echo.
echo 2. ARCADE - CLEAN, update MAKEFILE, check HK, check sets that NEED ROMS
echo.
echo 3. MAMEUI - CLEAN, update MAKEFILE and run SWLIST
rem echo.
echo 4. When this is all done, then
pause
goto arcade

c:

:hbmame
cd\hbmame
call newsrc.bat

rem --- HBMAME 64bit ---
del hbmameui.exe
del hbmameui.sym
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmame.exe
del hbmame.sym
call make64 -j4 %1 %2 %3
if not exist hbmame.exe goto end

:arcade
cd\arcade
call newsrc.bat

rem --- ARCADE 64bit ---
del arcade64.sym
del build\generated\resource\mamevers.rc
del arcade64.exe
copy /Y src\mame\arcade.flt src\mame\arcade.bak
copy /Y src\mame\arcade.txt src\mame\arcade.flt
touch src\mame\arcade.flt
call make64 -j6 "OSD=winui" %1 %2 %3
copy /Y src\mame\arcade.bak src\mame\arcade.flt
del arcade.exe
copy /Y arcade64.exe arcade.exe

if not exist arcade64.exe goto end
goto mameui

:messui
cd\mess
call newsrc.bat

rem --- MESSUI 64bit ---
call makee.bat
if not exist messui.exe goto end
call maker.bat
if not exist mess.exe goto end

:mameui
cd\mameui
call newsrc.bat

rem --- MAMEUI 64bit ---
del mameui.exe
del mameui.sym
call make64 -j8 %1 %2 %3
if not exist mameui.exe goto end

:end


