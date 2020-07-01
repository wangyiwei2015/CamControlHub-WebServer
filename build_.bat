@echo off
set /p row=<".\main.py"
set ver=%row:~3,-1%
echo Building version %ver%
echo.
REM pyinstaller -w -F .\main.py -i .\icon.ico -p .\venv\Lib\site-packages\
pyinstaller -w -F .\main.py -p .\venv\Lib\site-packages\
move .\dist\main.exe C:\Users\%USERNAME%\Desktop\CamControlHub_%ver%.exe
del .\build\* /F /S /Q