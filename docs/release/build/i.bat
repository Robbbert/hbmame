@echo off
hbmame -romident e:\incoming\%1.zip
sha1 d e:\incoming\%1.zip
pause
pause
call maker
hbmame %1
