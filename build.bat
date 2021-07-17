@echo off

set CC=g++

set FILES=src/main.cpp src/chip8.cpp
set FLAGS=-Os -w

set INCLUDE=-I"include"
set LIBS=-L"libs"

set LINKER=-l"SDL2/SDL2" -l"SDL2/SDL2main"

if not exist build_dir mkdir build_dir
copy "libs\SDL2\SDL2.dll" "build_dir\SDL2.dll" /Y

call %CC% %FILES% %INCLUDE% %LIBS% %FLAGS% %LINKER% -o build_dir/CrispyChip