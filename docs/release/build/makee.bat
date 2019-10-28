@del hbmameui.sym
:start
del hbmameui.exe
if exist hbmameui.exe goto start
call make32 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
:end

