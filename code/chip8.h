#ifndef CHIP_8_H_
#define CHIP_8_H_

#include "typedefs.h"
#include "utils.h"

#define CHIP8_REGISTERS 16
#define CHIP8_STACK 16
#define CHIP8_KEYBOARD 16
#define CHIP8_DISPLAY 64*32
#define CHIP8_MEMORY 4096
#define CHIP8_RESERVED 512

global u8 font_sprites[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct Chip8 {
    void initialize();
    void emulate_cycle();
    bool load_rom(const char *rom_path);
    
    bool render;
    u8 keyboard[CHIP8_KEYBOARD];
    u8 display[CHIP8_DISPLAY];
    
    // @Note: Below are variables specific for the Chip8 CPU to work
    // there's really no need to access them so they _could_ be private or something,
    // however I want it to be easily extendable (like adding a memory viewer) therefore
    // I prefer to leave them public in a struct like this. (also personally not a huge
    // fan of public/private)
    
    u16 stack[CHIP8_STACK];  // Stack - returns after subroutine
    u8 memory[CHIP8_MEMORY]; // 4Kb memory
    u8 V[CHIP8_REGISTERS];   // 16 registers

    u16 I;  // I register - stores memory addresses
    u16 PC; // Program counter
    u8 SP;  // Stack pointer
  
    u8 delay_timer;
    u8 sound_timer;
};

#endif // CHIP_8_H_
