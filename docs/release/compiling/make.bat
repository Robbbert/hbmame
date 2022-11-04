@if exist scripts\minimaws\minimaws.sqlite3 del scripts\minimaws\minimaws.sqlite3
del build\generated\resource\hbmamevers.rc
:start
del hbmameui.exe
if exist hbmameui.exe goto start
call make64 -j4 "OSD=winui" %1 %2 %3
color
if not exist hbmameui.exe goto end
@del hbmameui.sym
@del hbmame.sym
:start1
del hbmame.exe
if exist hbmame.exe goto start1
call make64 "ARCHOPTS='-fuse-ld=lld'" -j4 %1 %2 %3
color
if not exist hbmame.exe goto end
@call v.bat
:end
