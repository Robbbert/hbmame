rd /s /q g:\package
md g:\package
cd g:\package
md g:artwork
echo 1 >> g:artwork\test.txt
if not exist g:\package\artwork\test.txt goto end
del g:artwork\test.txt
copy c:\hbmame\README.md g:
copy c:\hbmame\COPYING g:
xcopy /e c:\hbmame\artwork g:artwork
md g:bgfx
xcopy /e c:\hbmame\bgfx g:bgfx
md g:ctrlr
xcopy /e e:\mame\ctrlr g:ctrlr
xcopy /e /y c:\hbmame\ctrlr g:ctrlr
md g:hlsl
xcopy /e c:\hbmame\hlsl g:hlsl
md g:language
xcopy /e c:\hbmame\language g:language
md g:plugins
xcopy /e c:\hbmame\plugins g:plugins
md g:roms
xcopy /e c:\mame\roms g:roms
md g:samples
md g:ui
md g:hash
@echo Done!
@rem package for hbmame
