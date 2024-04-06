@echo off
echo Current directory: %cd%
set "DATA_PATH=..\data"
set "DATA_PATH=..\data"
set "PATH=%PATH%;%DATA_PATH%"
copy /Y "..\code\assimp-vc143-mtd.dll" "..\build"
cd /d "..\build"
start main.exe