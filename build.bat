@echo off

set CC=g++

set FILES=src/main.cpp src/chip8.cpp src/smol_renderer.cpp
set FLAGS=-Os -w -s -std=c++17 -static-libgcc -static-libstdc++

set INCLUDE=-I"include"
set LIBS=-L"libs"

set LINKER=-l"SDL2/SDL2" -l"SDL2/SDL2main" -l"SDL2_ttf/SDL2_ttf" -lwinpthread

call %CC% %FILES% %INCLUDE% %LIBS% %FLAGS% %LINKER% -o build_dir/CrispyChip