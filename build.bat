@echo off

REM Change this to your visual studio's 'vcvars64.bat' script path
set MSVC_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build"
set CXXFLAGS=/std:c++14 /EHsc /W4 /WX /FC /MT /wd4996 /wd4201 /nologo %*
set INCLUDES=/I"deps\include"
set LIBS="deps\lib\SDL2\SDL2.lib" "deps\lib\SDL2\SDL2main.lib" "deps\lib\SDL2_ttf\SDL2_ttf.lib" shell32.lib
set FILES="code\*.cpp"

call %MSVC_PATH%\vcvars64.bat

pushd %~dp0
if not exist .\build mkdir build
cl %CXXFLAGS% %INCLUDES% %FILES% /Fo:build\ /Fe:build\CrispyChip.exe /link %LIBS% /SUBSYSTEM:CONSOLE

cd build
del *.obj
cd ..
popd