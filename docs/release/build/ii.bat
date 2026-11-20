@echo off
copy roms\warriorm_v%1.zip e:\incoming\warriorm.zip /Y
hbmame -romident e:\incoming\warriorm.zip
sha1 d e:\incoming\warriorm.zip
pause
call maker
hbmame warriorm
