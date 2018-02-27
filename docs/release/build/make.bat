//goto start0
@if exist scripts\minimaws\minimaws.sqlite3 del scripts\minimaws\minimaws.sqlite3
@del hbmameui.sym
:start
del hbmameui.exe
if exist hbmameui.exe goto start
call make32 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
:start0
del hbmame.sym
:start1
del hbmame.exe
if exist hbmame.exe goto start1
call make32 -j4 %1 %2 %3
:end
hbmame -valid
