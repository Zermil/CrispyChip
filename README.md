# CrispyChip

Chip8 emulator in C++ with [SDL2](https://www.libsdl.org/) for rendering/drawing. If you want to
create your own version check out [This technical reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) by "Cowgod".

You can get CHIP8 roms from here: [ROMS](https://github.com/kripod/chip8-roms)

![pong](./img/pong.jpg)
![ibm](./img/ibm_logo.jpg)
![maze](./img/maze.jpg)

## Quick Start/Instructions

To build this project you need to link SDL `.lib` and `.dll` files, if you're using Visual Studio
(Community) on Windows here is a quick instruction.

- In Project->Properties->Configuration Properties->VC++ Directories | Add to: Include Directories -> `include` folder, Library Directories -> `libs` folder.
- In Project->Properties->Linker->Input->Additional Dependencies | Add: `SDL2/SDL2.lib`, `SDL2/SDL2main.lib` and `SDL2_ttf/SDL2_ttf.lib`
- After you build your solution / project, add all DLLs (`SDL2.dll`, `SDL2_ttf.dll`, `libfreetype-6.dll`, `zlib1.dll`) to the folder containing your `.exe` file

Otherwise, you can use the provided `build.bat` file, you can specify your compiler (g++ by default) under the "CC" variable and then:

### Compile/Build

```console
> build 
```

### Run

```console
> cd build_dir
> CripsyChip.exe "../roms/[ROM]" 
```

**NOTE**: Make sure all DLLs (`SDL2.dll`, `SDL2_ttf.dll`, `libfreetype-6.dll`, `zlib1.dll`) are in the same place as `CrispyChip.exe` (`build.bat` copies them all automatically)
