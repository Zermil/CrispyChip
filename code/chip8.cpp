#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "chip8.h"

void Chip8::initialize() 
{
    memset(keyboard, 0, 16);
    memset(display, 0, 2048);
    memset(stack, 0, 16);
    memset(memory, 0, 4096);
    memset(V, 0, 16);

    PC = 0x200;
    SP = 0;
    I = 0;

    for (u32 i = 0; i < 80; ++i) {
        memory[i] = font_sprites[i];
    }

    delay_timer = 0;
    sound_timer = 0;
    
    render = true;

    // Some instructions/opcodes need random byte
    srand((u32) time(0));
}

void Chip8::emulate_cycle()
{
    u16 opcode = memory[PC] << 8 | memory[PC + 1];

    u16 addr = opcode & 0x0FFF;  // address, lowest 12 bits
    u8 n = opcode & 0x000F;      // lowest 4 bits
    u8 kk = opcode & 0x00FF;     // lowest 8 bits
    u8 x = opcode >> 8 & 0x000F; // 4 bit value, lower 4 bits of the high byte
    u8 y = opcode >> 4 & 0x000F; // 4 bit value, upper 4 bits of the high byte
  
    // opcodes - implementations
    switch (opcode & 0xF000) {
        case 0x0000: {
            switch (n) {
                case 0x0000: // Clear screen
                    memset(display, 0, 2048);
                    render = true;
                    PC += 2;
                    break;

                case 0x000E: // Return from subroutine
                    --SP;
                    PC = stack[SP];
                    PC += 2;
                    break;

                default:
                    assert(0 && "Unknown opcode detected!\n");
            }
        } break;
    
        case 0x1000: // Jump to location
            PC = addr;
            break;

        case 0x2000: // Call subroutine
            stack[SP] = PC;
            ++SP;
            PC = addr;
            break;

        case 0x3000: // Skip next instruction if V[x] == kk
            if (V[x] == kk) {
                PC += 2;
            }
            PC += 2;
            break;

        case 0x4000: // Skip next instruction if V[x] != kk
            if (V[x] != kk) {
                PC += 2;
            } 
            PC += 2;
            break;

        case 0x5000: // Skip next instruction if V[x] == V[y]
            if (V[x] == V[y]) {
                PC += 2;
            }
            PC += 2;
            break;

        case 0x6000: // Set V[x] = kk
            V[x] = kk;
            PC += 2;
            break;

        case 0x7000: // Set V[x] += kk
            V[x] += kk;
            PC += 2;
            break;
    
        case 0x8000: {
            switch (n) {
                case 0x0000: // Set V[x] = V[y]
                    V[x] = V[y];
                    PC += 2;
                    break;

                case 0x0001: // Set V[x] = V[x] | V[y]
                    V[x] |= V[y];
                    PC += 2;
                    break;

                case 0x0002: // Set V[x] = V[x] & V[y]
                    V[x] &= V[y];
                    PC += 2;
                    break;

                case 0x0003: // Set V[x] = V[x] ^ V[y]
                    V[x] ^= V[y];
                    PC += 2;
                    break;

                case 0x0004: // Set V[x] += V[y], set V[0xF] = carry
                    V[0xF] = (V[x] + V[y] > 0xFF) ? 1 : 0;
                    V[x] += V[y];
                    PC += 2;
                    break;

                case 0x0005: // Set V[x] -= V[y], set V[0xF] = NOT borrow
                    V[0xF] = (V[x] > V[y]) ? 1 : 0; 
                    V[x] -= V[y];
                    PC += 2;
                    break;

                case 0x0006: // Set V[x] = V[x] SHR 1
                    V[0xF] = V[x] & 0x1; 
                    V[x] >>= 1;
                    PC += 2;
                    break;

                case 0x0007: // Set V[x] = V[y] - V[x], set V[0xF] = NOT borrow
                    V[0xF] = (V[y] > V[x]) ? 1 : 0; 
                    V[x] = V[y] - V[x];
                    PC += 2;
                    break;

                case 0x000E: // Set V[x] = V[x] SHL 1
                    V[0xF] = V[x] >> 7;
                    V[x] <<= 1;
                    PC += 2;
                    break;

                default:
                    assert(0 && "Unknown opcode detected!\n");
            }
        } break;

        case 0x9000: // Skip next instruction if V[x] != V[y]
            if (V[x] != V[y]) {
                PC += 2;
            }
            PC += 2;
            break;

        case 0xA000: // Set I = addr
            I = addr;
            PC += 2;
            break;

        case 0xB000: // Jump to addr + V[0]
            PC = V[0x0] + addr;
            break;

        case 0xC000: // Set V[x] to random byte & kk
            V[x] = (rand() % 0xFF) & kk;
            PC += 2;
            break;

        case 0xD000: { // Display n-byte sprite starting at memory location I at (V[x], V[y]), set V[0xF] = collision
            u8 Vx = V[x];
            u8 Vy = V[y];
            u8 pixel; 
            V[0xF] = 0;

            for (u32 y0 = 0; y0 < n; ++y0) {
                pixel = memory[I + y0];

                for (u32 x0 = 0; x0 < 8; ++x0) {
                    if ((pixel & (0x80 >> x0)) != 0) {
                        // index = x + width * y (and % loop around)
                        u32 index = ((Vx + x0) + ((Vy + y0) * 64)) % 2048;

                        if (display[index] == 1) {
                            V[0xF] = 1;
                        }

                        display[index] ^= 1;
                    }
                }
            }
      
            render = true;
            PC += 2;
        } break;

        case 0xE000: {
            switch (n) {
                case 0x000E: // Skip instruction if key at V[x] is pressed
                    if (keyboard[V[x]] != 0) {
                        PC += 2;
                    }
                    PC += 2;
                    break;

                case 0x0001: // Skip instruction if key at V[x] is NOT pressed
                    if (keyboard[V[x]] == 0) {
                        PC += 2;
                    }
                    PC += 2;
                    break;

                default:
                    assert(0 && "Unknown opcode detected!\n");
            }
        } break;

        case 0xF000: {
            switch (kk) {
                case 0x0007: // Set Vx = delay timer value
                    V[x] = delay_timer;
                    PC += 2;
                    break;

                case 0x000A: { // Wait for a key press, store the value of the key in Vx
                    bool keyPress = false;

                    for (u8 i = 0; i < 16; ++i) {
                        if (keyboard[i] != 0) {
                            V[x] = i;
                            keyPress = true;
                        }
                    }

                    if (!keyPress) {
                        return;
                    }

                    PC += 2;
                } break;

                case 0x0015: // Set delay timer = Vx
                    delay_timer = V[x];
                    PC += 2;
                    break;

                case 0x0018: // Set sound timer = Vx
                    sound_timer = V[x];
                    PC += 2;
                    break;

                case 0x001E: // Set I = I + Vx
                    V[0xF] = (I + V[x] > 0xFFF) ? 1 : 0;
                    I += V[x];
                    PC += 2;
                    break;

                case 0x0029: // Set I = location of sprite for digit Vx
                    // We multiply by 5 because we want the "starting index" of a sprite
                    I = V[x] * 0x5;
                    PC += 2;
                    break;

                case 0x0033: // Store BCD representation of Vx in memory locations I, I+1, and I+2
                    memory[I]     = V[x] / 100;
                    memory[I + 1] = (V[x] / 10) % 10;
                    memory[I + 2] = V[x] % 10;
                    PC += 2;
                    break;

                case 0x0055: // Store registers V0 through Vx in memory starting at location I 
                    for (u32 i = 0; i <= x; ++i) {
                        memory[I + i] = V[i]; 
                    }
                    I += x + 1; // Staying true to the original
                    PC += 2;
                    break;

                case 0x0065: // Read registers V0 through Vx from memory starting at location I 
                    for (u32 i = 0; i <= x; ++i) {
                        V[i] = memory[I + i]; 
                    }
                    
                    I += x + 1; // Staying true to the original
                    PC += 2;
                    break;

                default:
                    assert(0 && "Unknown opcode detected!\n");
            }
        } break;

        default:
            assert(0 && "Unknown opcode detected!\n");
    }

    // Timers are unsigned
    if (delay_timer > 0) {
        delay_timer -= 1;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            printf("SOUND!\n");
        }

        sound_timer -= 1;
    }
}

bool Chip8::load_rom(const char *rom_path)
{
    FILE *rom_file = fopen(rom_path, "rb");
    u8 *buffer = NULL;
    
    if (rom_file == NULL) {
        fprintf(stderr, "Invalid path to ROM provided\n");
        goto fail;
    }

    if (fseek(rom_file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Could not find the end of file\n");
        goto fail;
    }
    
    u64 size = ftell(rom_file);
    
    if (fseek(rom_file, 0, SEEK_SET) !=0) {
        fprintf(stderr, "Could not find the beginning of the file\n");
        goto fail;
    }
    
    if (size > CHIP8_MEMORY - CHIP8_RESERVED || size <= 0) {
        fprintf(stderr, "Invalid file size, make sure the ROM fits CHIP8 memory\n");
        goto fail;
    }

    buffer = (u8 *) malloc(size);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        goto fail;
    }

    if (fread(buffer, 1, size, rom_file) != size) {
        fprintf(stderr, "Could not read entire ROM file\n");
        goto fail;
    }
    
    for (u32 i = 0; i < size; ++i) {
        memory[i + CHIP8_RESERVED] = buffer[i];
    }

    return(true);
    
fail:
    if (rom_file) {
        fclose(rom_file);
    }

    if (buffer) {
        free((void *) buffer);
    }
    
    return(false);
}
