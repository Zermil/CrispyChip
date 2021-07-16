@echo off

set CC=g++

set FILES=src/main.cpp src/chip8.cpp
set FLAGS=-Os -w

set INCLUDE=-I"include"
set LIBS=-L"libs"

set LINKER=-l"SDL2/SDL2" -l"SDL2/SDL2main"

call %CC% %FILES% %INCLUDE% %LIBS% %FLAGS% %LINKER% -o CrispyChip