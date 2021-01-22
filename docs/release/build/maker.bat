del hbmame64.sym
:start1
del hbmame64.exe
if exist hbmame64.exe goto start1
call make64 "ARCHOPTS='-fuse-ld=lld'" -j4 %1 %2 %3
if not exist hbmame64.exe goto end
@copy /Y hbmame64.exe hbmame.exe
@copy /Y hbmame64.sym hbmame.sym
hbmame64 -valid
:end
