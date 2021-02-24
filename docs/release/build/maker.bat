del hbmame.sym
:start1
del hbmame.exe
if exist hbmame.exe goto start1
call make64 "ARCHOPTS='-fuse-ld=lld'" -j4 %1 %2 %3
if not exist hbmame.exe goto end
hbmame -valid
:end
