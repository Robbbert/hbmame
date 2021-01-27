@echo off
hbmame -valid
xmllint --noout -valid hash/*.xml 2>&1 > nul