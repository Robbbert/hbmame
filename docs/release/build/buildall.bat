@echo off
cls
echo.
echo Pre-compile steps
echo.
echo 1. HBMAME - CLEAN, update MAKEFILE, update VERSION.CPP
echo.
echo 2. ARCADE - CLEAN, update MAKEFILE, update WINUI.H
echo.
echo 3. MESSUI - CLEAN, update MAKEFILE, update VERSION.CPP
echo.
echo 4. MAMEUI - CLEAN, update MAKEFILE, update VERSION.CPP
echo.
echo 5. When this is all done, then
pause
goto arcade

c:

:hbmame
cd\hbmame
call newsrc.bat

rem --- HBMAME 64bit ---
del hbmameui64.exe
del hbmameui64.sym
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui64.exe goto end
del hbmame64.exe
del hbmame64.sym
call make64 -j4 %1 %2 %3
if not exist hbmame64.exe goto end

:arcade
cd\arcade
call newsrc.bat

rem --- ARCADE 64bit ---
del arcade64.exe
del arcade64.sym
call make64 -j4 %1 %2 %3
if not exist arcade64.exe goto end

:messui
cd\mess
call newsrc.bat

rem --- MESSUI 64bit ---
call makee.bat
if not exist messui64.exe goto end
call maker.bat
if not exist mess64.exe goto end

:mameui
cd\mameui
call newsrc.bat

rem --- MAMEUI 64bit ---
del mameui64.exe
del mameui64.sym
call make64 -j4 %1 %2 %3
if not exist mameui64.exe goto end

:end


