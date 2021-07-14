# CrispyChip

Chip8 emulator in C++ with [SDL2](https://www.libsdl.org/) for rendering/drawing. If you want to
create your own version check out [This technical reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) by "Cowgod".

You can get CHIP8 roms from here: [ROMS](https://github.com/kripod/chip8-roms)

![pong](./img/pong.jpg)
![ibm](./img/ibm_logo.jpg)

## Quick Start/Instructions

To build this project you need to link SDL `.lib` and `.dll` files, if you're using Visual Studio
(Community) on Windows here is a quick instruction.

- In Project->Properties->Configuration Properties->VC++ Directories | Add to: Include Directories -> `include` folder, Library Directories -> `libs` folder.
- In Project->Properties->Linker->Input->Additional Dependencies | Add: `SDL2/SDL2.lib` and `SDL2/SDL2main.lib`
- After you build your solution / project, add `SDL2.dll` to the folder containing your `.exe` file

Otherwise, you need to link those files when compiling.
