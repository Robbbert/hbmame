@if exist scripts\minimaws\minimaws.sqlite3 del scripts\minimaws\minimaws.sqlite3
@del hbmameui64.sym
:start
del hbmameui64.exe
if exist hbmameui64.exe goto start
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui64.exe goto end
@copy /Y hbmameui64.exe hbmameui.exe
@copy /Y hbmameui64.sym hbmameui.sym
@del hbmame64.sym
:start1
del hbmame64.exe
if exist hbmame64.exe goto start1
call make64 -j4 %1 %2 %3
if not exist hbmame64.exe goto end
@copy /Y hbmame64.exe hbmame.exe
@copy /Y hbmame64.sym hbmame.sym
@call v.bat
:end
