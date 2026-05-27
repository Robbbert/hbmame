cd scripts\minimaws
del minimaws.sqlite3
python minimaws.py load --executable ..\..\hbmame.exe
python minimaws.py serve
cd..
cd..
