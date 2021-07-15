#include <iostream>

#include <SDL2/SDL.h>
#undef main

#include "chip8.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

bool initializeSDL(const std::string& title, int width, int height)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Failed to initialize SDL\n";
    return false;
  }

  window = SDL_CreateWindow(
    title.c_str(),
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_SHOWN
  );

  if (window == nullptr) {
    std::cerr << "Failed to initialize SDL Window\n";
    return false;
  }

  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );

  if (renderer == nullptr) {
    std::cerr << "Failed to initialize SDL Renderer\n";
    return false;
  }

  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING,
    64, 32
  );

  if (texture == nullptr) {
    std::cerr << "Failed to initialize SDL Texture\n";
    return false;
  }

  return true;
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Please provide ROM you want to run\n";
    return 1;
  }

  Chip8 crispy;
  crispy.initialize();

  if (!crispy.loadROM(argv[1])) {
    return 1;
  }

  const int SCALE = 16;

  int width = 64 * SCALE;
  int height = 32 * SCALE;

  SDL_Event event;
  uint32_t pixelBuffer[2048];

  bool isOpen = initializeSDL("CrispyChip - CHIP8 Emulator", width, height);

  while (isOpen) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isOpen = false;
          break;
      }
    }

    crispy.emulateCycle();

    if (crispy.render) {
      for (int i = 0; i < 2048; ++i) {
        if (crispy.display[i]) {
          pixelBuffer[i] = 0xFFFFFFFF;
        } else {
          pixelBuffer[i] = 0x000000FF;
        }
      }

      SDL_UpdateTexture(texture, nullptr, pixelBuffer, 64 * sizeof(uint32_t));
      SDL_RenderCopy(renderer, texture, nullptr, nullptr);

      crispy.render = false;
    }
    
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }

  // Clean up
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
