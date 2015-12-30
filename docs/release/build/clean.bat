@echo off
if %1q==q rd /s /q build
if %1q==q goto end

if exist ini\%1.ini del ini\%1.ini
if exist cfg\%1.cfg del cfg\%1.cfg
if exist nvram\%1.nv del nvram\%1.nv
if exist nvram\%1 rd /s /q nvram\%1
:end
echo %1 cleaned

