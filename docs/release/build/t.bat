@echo off
if %1q==q goto help

git tag -a tag245%1 -m "tag245%1"
git push --tags
goto end

:help
echo off
echo.
echo Push new tag to github
echo.
echo Usage: t [release]
echo.
echo Example: t 29
echo.
:end
 