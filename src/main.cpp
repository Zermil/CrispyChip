#include <iostream>

#include <SDL2/SDL.h>
#undef main

#include "chip8.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture_crispy = nullptr;

Chip8 crispy;

bool initializeSDL(const std::string& title, int width, int height);
void parseMovementKeyDown(SDL_Keycode key);
void parseMovementKeyUp(SDL_Keycode key);

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Please provide a valid path to a ROM you want to run\n";
    return 1;
  }

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
    crispy.emulateCycle();
    
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        isOpen = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isOpen = false;
        } else {
          parseMovementKeyDown(event.key.keysym.sym);
        }
        break;
      case SDL_KEYUP:
        parseMovementKeyUp(event.key.keysym.sym);
        break;
      }
    }
    
    if (crispy.render) {
      for (int i = 0; i < 2048; ++i) {
        if (crispy.display[i]) {
          pixelBuffer[i] = 0xFFFFFFFF;
        } else {
          pixelBuffer[i] = 0x000000FF;
        }
      }

      SDL_UpdateTexture(texture_crispy, nullptr, pixelBuffer, 64 * sizeof(uint32_t));
      SDL_RenderCopy(renderer, texture_crispy, nullptr, nullptr);

      crispy.render = false;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }

  // Clean up
  SDL_DestroyTexture(texture_crispy);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

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

  texture_crispy = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING,
    64, 32
  );

  if (texture_crispy == nullptr) {
    std::cerr << "Failed to initialize SDL Texture\n";
    return false;
  }

  return true;
}

void parseMovementKeyDown(SDL_Keycode key)
{
  switch (key) {
    case SDLK_1:
      crispy.keyboard[0x1] = 1;
      break;
    case SDLK_2:
      crispy.keyboard[0x2] = 1;
      break;
    case SDLK_3:
      crispy.keyboard[0x3] = 1;
      break;
    case SDLK_4:
      crispy.keyboard[0xC] = 1;
      break;

    case SDLK_q:
      crispy.keyboard[0x4] = 1;
      break;
    case SDLK_w:
      crispy.keyboard[0x5] = 1;
      break;
    case SDLK_e:
      crispy.keyboard[0x6] = 1;
      break;
    case SDLK_r:
      crispy.keyboard[0xD] = 1;
      break;

    case SDLK_a:
      crispy.keyboard[0x7] = 1;
      break;
    case SDLK_s:
      crispy.keyboard[0x8] = 1;
      break;
    case SDLK_d:
      crispy.keyboard[0x9] = 1;
      break;
    case SDLK_f:
      crispy.keyboard[0xE] = 1;
      break;

    case SDLK_z:
      crispy.keyboard[0xA] = 1;
      break;
    case SDLK_x:
      crispy.keyboard[0x0] = 1;
      break;
    case SDLK_c:
      crispy.keyboard[0xB] = 1;
      break;
    case SDLK_v:
      crispy.keyboard[0xF] = 1;
      break;
  }
}

void parseMovementKeyUp(SDL_Keycode key)
{
  switch (key) {
    case SDLK_1:
      crispy.keyboard[0x1] = 0;
      break;
    case SDLK_2:
      crispy.keyboard[0x2] = 0;
      break;
    case SDLK_3:
      crispy.keyboard[0x3] = 0;
      break;
    case SDLK_4:
      crispy.keyboard[0xC] = 0;
      break;

    case SDLK_q:
      crispy.keyboard[0x4] = 0;
      break;
    case SDLK_w:
      crispy.keyboard[0x5] = 0;
      break;
    case SDLK_e:
      crispy.keyboard[0x6] = 0;
      break;
    case SDLK_r:
      crispy.keyboard[0xD] = 0;
      break;

    case SDLK_a:
      crispy.keyboard[0x7] = 0;
      break;
    case SDLK_s:
      crispy.keyboard[0x8] = 0;
      break;
    case SDLK_d:
      crispy.keyboard[0x9] = 0;
      break;
    case SDLK_f:
      crispy.keyboard[0xE] = 0;
      break;

    case SDLK_z:
      crispy.keyboard[0xA] = 0;
      break;
    case SDLK_x:
      crispy.keyboard[0x0] = 0;
      break;
    case SDLK_c:
      crispy.keyboard[0xB] = 0;
      break;
    case SDLK_v:
      crispy.keyboard[0xF] = 0;
      break;
  }
}
