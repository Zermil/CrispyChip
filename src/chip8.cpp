#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chip8.hpp"

uint8_t font_sprites[80] = {
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

  for (int i = 0; i < 80; ++i) {
    memory[i] = font_sprites[i];
  }

  delay_timer = 0;
  sound_timer = 0;

  render = true;

  // Some instructions/opcodes need random byte
  srand(time(NULL));
}

void Chip8::emulateCycle()
{
  // Get opcode
  uint16_t opcode = memory[PC] << 8 | memory[PC + 1];

  uint16_t addr   = opcode & 0x0FFF;      // address, lowest 12 bits

  uint8_t  n      = opcode & 0x000F;      // lowest 4 bits
  uint8_t  kk     = opcode & 0x00FF;      // lowest 8 bits
  uint8_t  x      = opcode >> 8 & 0x000F; // 4 bit value, lower 4 bits of the high byte
  uint8_t  y      = opcode >> 4 & 0x000F; // 4 bit value, upper 4 bits of the high byte
  
  // OP-codes, implementations
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
          std::cerr << "Unknown opcode: " << opcode << std::endl;
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
          std::cerr << "Unknown opcode: " << opcode << std::endl;
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

    // This is probably the weirdest instruction in CHIP8
    case 0xD000: { // Display n-byte sprite starting at memory location I at (V[x], V[y]), set V[0xF] = collision
      uint8_t Vx = V[x];
      uint8_t Vy = V[y];
      uint8_t pixel; 
      V[0xF] = 0;

      for (int y = 0; y < n; ++y) {
        pixel = memory[I + y];

        for (int x = 0; x < 8; ++x) {
          if ((pixel & (0x80 >> x)) != 0) {
            // index = x + width*y (% loop around)
            int index = ((Vx + x) + ((Vy + y) * 64)) % 2048;

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
      switch (kk) {
        case 0x009E: // Skip instruction if key at V[x] is pressed
          if (keyboard[V[x]] != 0) {
            PC += 2;
          }
          PC += 2;
          break;

        case 0x00A1: // Skip instruction if key at V[x] is NOT pressed
          if (keyboard[V[x]] == 0) {
            PC += 2;
          }
          PC += 2;
          break;

        default:
          std::cerr << "Unknown opcode: " << opcode << std::endl;
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

          for (int i = 0; i < 16; ++i) {
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
          for (int i = 0; i <= x; ++i) {
            memory[I + i] = V[i]; 
          }
          I += x + 1; // Staying true to the original
          PC += 2;
          break;

        case 0x0065: // Read registers V0 through Vx from memory starting at location I 
          for (int i = 0; i <= x; ++i) {
            V[i] = memory[I + i]; 
          }
          I += x + 1; // Staying true to the original
          PC += 2;
          break;

        default:
          std::cerr << "Unknown opcode: " << opcode << std::endl;
      }
    } break;

    default:
      std::cerr << "Unknown opcode: " << opcode << std::endl;
  }

  // Timers, unsigned so that's why the if "guards"
  if (delay_timer > 0) {
    delay_timer--;
  }

  if (sound_timer > 0) {
    if (sound_timer == 1) {
      std::cout << "SOUND\n";
    }

    sound_timer--;
  }
}

bool Chip8::loadROM(const std::string& rom_path)
{
  std::ifstream rom_file(rom_path, std::ios::binary | std::ios::ate);

  if (!rom_file.is_open()) {
    std::cerr << "Invalid file provided\n";
    return false;
  }

  std::streamsize size = rom_file.tellg();

  // Check if file size is appropriate
  if (size > 4096 - 512 || size <= 0) {
    std::cerr << "Invalid file size, make sure the ROM fits CHIP8 memory\n";
    rom_file.close();
    return false;
  }

  uint8_t* buffer = new uint8_t[size];

  if (buffer == nullptr) {
    std::cerr << "Failed to allocate buffer\n";
    rom_file.close();
    return false;
  }
  
  rom_file.seekg(0, std::ios::beg);
  rom_file.read(reinterpret_cast<char*>(buffer), size);

  // First 512 bytes are reserved
  for (int i = 0; i < size; ++i) {
    memory[i + 512] = buffer[i];
  }

  // Clean up
  rom_file.close();
  delete[] buffer;

  return true; 
}
