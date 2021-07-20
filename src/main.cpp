#include <iostream>

#include <SDL2/SDL.h>
#undef main

#include <SDL2/SDL_ttf.h>
#include "chip8.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture_crispy = nullptr;
TTF_Font* font = nullptr;

const int FONT_SIZE = 30;

const int SCALE = 16;
const int WIDTH = 64 * SCALE;
const int HEIGHT = 32 * SCALE;

const std::string ROMS[4] = {
  "../roms/IBM Logo.ch8",
  "../roms/Maze [David Winter, 199x].ch8",
  "../roms/Sierpinski [Sergey Naydenov, 2010].ch8",
  "../roms/Pong [Paul Vervalin, 1990].ch8"
};

SDL_Rect selector = { (WIDTH / 4) - 150, (HEIGHT / 4) - 80, (WIDTH / 2) + 300, 80 };

int index = 0;

Chip8 crispy;

bool initializeSDL(const std::string& title, int width, int height);
void displayTextAt(int y, const std::string& msg_text, SDL_Color color);
void parseMovementKeyDown(SDL_Keycode key);
void parseMovementKeyUp(SDL_Keycode key);
void renderMenu();
void renderEmulation();

int main(int argc, char* argv[])
{
  bool emulation = true;
  
  if (argc > 1) {
    crispy.initialize();

    if (!crispy.loadROM(argv[1])) {
      exit(1);
    }
  } else {
    emulation = false;
  }

  SDL_Event event;
  bool isOpen = initializeSDL("CrispyChip - CHIP8 Emulator", WIDTH, HEIGHT);

  while (isOpen) {
    while (SDL_PollEvent(&event)) {

      switch (event.type) {
      case SDL_QUIT:
        isOpen = false;
        break;

      case SDL_KEYDOWN: {

        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          if (!emulation) isOpen = false;
          else emulation = false;
          break;
        case SDLK_UP:
          if (!emulation) {
            if (selector.y == 48) {
              selector.y = 408;
              index = 3;
            } else {
              index--;
              selector.y -= 120;
            }
          }
          break;
        case SDLK_DOWN:
          if (!emulation) {
            if (selector.y == 408) {
              selector.y = 48;
              index = 0;
            } else {
              index++;
              selector.y += 120;
            }
          }
          break;
        case SDLK_RETURN:
          if (!emulation) {
            crispy.initialize();
            crispy.loadROM(ROMS[index]);

            emulation = true;
          }
          break;
        default:
          if (emulation) parseMovementKeyDown(event.key.keysym.sym);
        }

      } break;

      case SDL_KEYUP:
        if (emulation) parseMovementKeyUp(event.key.keysym.sym);
        break;
      }
    }

    if (emulation) {
      renderEmulation();
    } else {
      renderMenu();
    }
  }

  // Clean up
  SDL_DestroyTexture(texture_crispy);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_Quit();

  return 0;
}

bool initializeSDL(const std::string& title, int width, int height)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Failed to initialize SDL\n";
    return false;
  }

  if (TTF_Init() != 0) {
    std::cerr << "Failed to initialize SDL_ttf\n";
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

  font = TTF_OpenFont("../resources/Consolas.ttf", FONT_SIZE);
  return true;
}

void displayTextAt(int y, const std::string& msg_text, SDL_Color color)
{
  SDL_Surface* font_surface = TTF_RenderText_Solid(font, msg_text.c_str(), color);

  if (font_surface == nullptr) {
    std::cerr << "Failed to create SDL_Surface\n";
    return;
  }

  SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
  SDL_FreeSurface(font_surface);

  if (font_texture == nullptr) {
    std::cerr << "Failed to create SDL_Texture\n";
    return;
  }

  SDL_Rect font_rect = {};
  SDL_QueryTexture(font_texture, nullptr, nullptr, &font_rect.w, &font_rect.h);

  font_rect.x = (WIDTH / 2) - (font_rect.w / 2);
  font_rect.y = y;

  SDL_RenderCopy(renderer, font_texture, nullptr, &font_rect);
  SDL_DestroyTexture(font_texture);
}

void renderMenu()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &selector);

  for (int i = 0; i < 4; ++i) {
    SDL_Color color;

    if (i == index) color = { 0, 0, 0, 255 };
    else color = { 255, 255, 255, 255 };

    displayTextAt(75 + (i * 120), ROMS[i], color);
  }

  SDL_RenderPresent(renderer);
}

void renderEmulation() {
  crispy.emulateCycle();

  // Render is set to true after initialization, so it will render at least once, 
  // in other words: it will clear the screen.
  if (crispy.render) {
    uint32_t pixelBuffer[2048];

    for (int i = 0; i < 2048; ++i) {
      if (crispy.display[i]) {
        pixelBuffer[i] = 0xFFFFFFFF;
      }
      else {
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
