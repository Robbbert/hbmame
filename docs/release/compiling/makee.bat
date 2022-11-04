:start
del hbmameui.exe
if exist hbmameui.exe goto start
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmameui.sym
:end
