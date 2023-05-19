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
#define RENDER_FONT_SIZE 2 * RENDER_SCALE

struct Renderer {
    Renderer(const char *title);
    ~Renderer();
    
    void display_text_at(u32 x, u32 y, const char *msg_text, SDL_Color color);
    void render_menu(u32 selected_index, const char *roms[], u32 roms_size);
    void render_selector(SDL_Rect selector_rect);
    void render_emulation(Chip8 *crispy);
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture_crispy;
    TTF_Font *font;
};

#endif // SMOL_RENDERER_H_
