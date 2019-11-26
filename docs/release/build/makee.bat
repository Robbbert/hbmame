@del hbmameui64.sym
:start
del hbmameui64.exe
if exist hbmameui64.exe goto start
call make64 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui64.exe goto end
copy /Y hbmameui64.exe hbmameui.exe
:end
