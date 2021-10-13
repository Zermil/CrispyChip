#ifndef SMOL_RENDERER
#define SMOL_RENDERER

#include <string>

#include <SDL2/SDL.h>
#undef main

#include <SDL2/SDL_ttf.h>
#include "chip8.hpp"

static constexpr int SCALE = 16;
static constexpr int WIDTH = 64 * SCALE;
static constexpr int HEIGHT = 32 * SCALE;
static constexpr int FONT_SIZE = 30;

static const std::string ROMS[4] = {
    "../roms/IBM Logo.ch8",
    "../roms/Maze [David Winter, 199x].ch8",
    "../roms/Sierpinski [Sergey Naydenov, 2010].ch8",
    "../roms/Pong [Paul Vervalin, 1990].ch8"
};

struct Renderer {
    Renderer() = delete;
    Renderer(const std::string& title);
    ~Renderer();
    
    void displayTextAt(const std::string& msg_text, SDL_Color& color, int y);
    void renderMenu(size_t index, const SDL_Rect& selector);
    void renderEmulation(Chip8& crispy);
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture_crispy = nullptr;
    TTF_Font* font = nullptr;
    bool error = false;
};

#endif // SMOL_RENDERER
