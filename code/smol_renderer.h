#ifndef SMOL_RENDERER_H_
#define SMOL_RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "utils.h"
#include "typedefs.h"

#include "chip8.h"

#ifndef RENDER_SCALE
# define RENDER_SCALE 16
#endif // RENDER_SCALE

#define RENDER_WIDTH 64
#define RENDER_HEIGHT 32
#define RENDER_WIDTH_SCALED 64 * RENDER_SCALE
#define RENDER_HEIGHT_SCALED 32 * RENDER_SCALE
#define RENDER_FONT_SIZE 30

global const char *ROMS[] = {
    "../roms/IBM Logo.ch8",
    "../roms/Maze [David Winter, 199x].ch8",
    "../roms/Sierpinski [Sergey Naydenov, 2010].ch8",
    "../roms/Pong [Paul Vervalin, 1990].ch8",
};

struct Renderer {
    Renderer(const char *title);
    ~Renderer();
    
    void display_text_at(u32 x, u32 y, const char *msg_text, SDL_Color color);
    void render_menu(u32 index, const SDL_Rect selector);
    void render_emulation(Chip8 *crispy);
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture_crispy;
    TTF_Font *font;
};

#endif // SMOL_RENDERER_H_
