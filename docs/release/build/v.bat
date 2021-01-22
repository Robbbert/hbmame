@echo off
hbmame64 -valid
xmllint --noout -valid hash/*.xml 2>&1 > nul | find /V "c64_flop_clcracked"