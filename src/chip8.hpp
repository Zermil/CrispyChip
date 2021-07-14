#ifndef CHIP_8
#define CHIP_8

#include <cstdint>
#include <string>

class Chip8 {
public:
  Chip8() = default;
  ~Chip8() = default;

  void initialize();
  void emulateCycle();
  bool loadROM(const std::string& rom_path);

public:
  bool render;

  uint8_t keyboard[16];
  uint8_t display[64*32]; // Total = 2048

private:
  uint16_t stack[16];   // Stack, returns after subroutine
  uint16_t I;           // I register, store memory addresses
  uint16_t PC;          // Program counter

  uint8_t memory[4096]; // 4K Memory
  uint8_t V[16];        // Registers
  uint8_t SP;           // Stack pointer
  
  // Timers
  uint8_t delay_timer;
  uint8_t sound_timer;
};

#endif // CHIP_8
