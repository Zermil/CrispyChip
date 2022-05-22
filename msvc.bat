@echo off

set MY_PATH=%~dp0

REM Change this to your visual studio's 'vcvars64.bat' script path
set MSVC_PATH="MY_MSVC_PATH"
call %MSVC_PATH%\vcvars64.bat

pushd %MY_PATH%\build_dir

set SDL2_LINK="..\libs\SDL2\SDL2.lib" "..\libs\SDL2\SDL2main.lib" "..\libs\SDL2_ttf\SDL2_ttf.lib"

cl ..\src\main.cpp ..\src\chip8.cpp ..\src\smol_renderer.cpp /I"..\include" /std:c++17 /EHsc %* /Fe:CrispyChip.exe /link user32.lib %SDL2_LINK% /SUBSYSTEM:windows /ENTRY:mainCRTStartup

popd