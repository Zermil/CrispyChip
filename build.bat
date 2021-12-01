@echo off

set CC=g++

set FILES=src/main.cpp src/chip8.cpp src/smol_renderer.cpp
set FLAGS=-Os -w -s -std=c++17

set INCLUDE=-I"include"
set LIBS=-L"libs"

set LINKER=-l"SDL2/SDL2" -l"SDL2/SDL2main" -l"SDL2_ttf/SDL2_ttf"

if not exist build_dir mkdir build_dir
if not exist "build_dir\SDL2.dll" copy "libs\SDL2\SDL2.dll" "build_dir\SDL2.dll"
if not exist "build_dir\SDL2_ttf.dll" copy "libs\SDL2_ttf\SDL2_ttf.dll" "build_dir\SDL2_ttf.dll"
if not exist "build_dir\libfreetype-6.dll" copy "libs\SDL2_ttf\libfreetype-6.dll" "build_dir\libfreetype-6.dll"
if not exist "build_dir\zlib1.dll" copy "libs\SDL2_ttf\zlib1.dll" "build_dir\zlib1.dll"

call %CC% %FILES% %INCLUDE% %LIBS% %FLAGS% %LINKER% -o build_dir/CrispyChip