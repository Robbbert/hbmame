del hbmameui.exe
call make32 -j4 "OSD=winui" %1 %2 %3
if not exist hbmameui.exe goto end
del hbmame.exe
call make32 -j4 %1 %2 %3
:end
