@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

SET SDL_PATH=..\thirdparty\SDL2
SET LIB_PATH=..\thirdparty
SET ASSIMP_PATH=..\thirdparty\assimp
SET IMGUI_PATH=..\thirdparty\imgui

SET INCLUDES=/I"%SDL_PATH%\include" /I"%ASSIMP_PATH%\include"  /I"%IMGUI_PATH%"  /I"%LIB_PATH%"
SET COMPILER_FLAGS=-nologo -FC -Zi  /EHsc /std:c++20
SET LINKER_FLAGS=SDL2.lib SDL2main.lib shell32.lib opengl32.lib assimp-vc143-mtd.lib imgui.lib /SUBSYSTEM:CONSOLE 
SET LINK_PATHS=/LIBPATH:"%SDL_PATH%\lib\x64" /LIBPATH:"%ASSIMP_PATH%\lib\Debug" /LIBPATH:"%IMGUI_PATH%\lib"

IF NOT EXIST ..\build mkdir ..\build

pushd ..\build

cl %COMPILER_FLAGS% ..\code\main.cpp %INCLUDES% /link %LINK_PATHS% %LINKER_FLAGS%

popd
