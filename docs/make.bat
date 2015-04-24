del hbmameui.exe
del build\generated\resource\hbmamevers.rc
call make32 -j4 "OSD=winui" %1 %2 %3 %4
if exist hbmameui.exe call make32 -j4 %1 %2 %3 %4
