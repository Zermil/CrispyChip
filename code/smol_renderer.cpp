#include <stdio.h>

#include "smol_renderer.h"

Renderer::Renderer(const char *title)
{
    ERROR_EXIT(SDL_Init(SDL_INIT_VIDEO) != 0, "Failed to initialize SDL\n");
    ERROR_EXIT(TTF_Init() != 0, "Failed to initialize SDL_ttf\n");
    
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              RENDER_WIDTH * RENDER_SCALE,
                              RENDER_HEIGHT * RENDER_SCALE,
                              SDL_WINDOW_SHOWN);
    ERROR_EXIT(window == NULL, "Failed to initialize SDL Window\n");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    ERROR_EXIT(renderer == NULL, "Failed to initialize SDL Renderer\n");

    texture_crispy = SDL_CreateTexture(renderer,
                                       SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       RENDER_WIDTH, RENDER_HEIGHT);
    ERROR_EXIT(texture_crispy == NULL, "Failed to initialize SDL Texture\n");

    font = TTF_OpenFont("Consolas.ttf", RENDER_FONT_SIZE);
    ERROR_EXIT(font == NULL, "Failed to read font file\n");
}

// @ToDo: Probably a good idea to reuse the font_texture here, instead of creating
// and destroying it constantly?.
void Renderer::display_text_at(u32 x, u32 y, const char *msg_text, SDL_Color color)
{
    SDL_Surface *font_surface = TTF_RenderText_Solid(font, msg_text, color);

    if (font_surface == NULL) {
        fprintf(stderr, "Failed to create SDL_Surface\n");
        return;
    }

    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);

    if (font_texture == NULL) {
        fprintf(stderr, "Failed to create SDL_Texture\n");
        return;
    }

    SDL_Rect font_rect = {0};
    SDL_QueryTexture(font_texture, NULL, NULL, &font_rect.w, &font_rect.h);
    font_rect.x = x - (font_rect.w / 2);
    font_rect.y = y;

    SDL_RenderCopy(renderer, font_texture, NULL, &font_rect);
    SDL_DestroyTexture(font_texture);
}

void Renderer::render_menu(u32 selected_index, const char *roms[], u32 roms_size)
{
    for (u32 i = 0; i < roms_size; ++i) {
        SDL_Color color = color = { 0, 0, 0, 255 };
        
        if (i != selected_index) {
            color = { 255, 255, 255, 255 };
        }

        u32 y = (RENDER_HEIGHT_SCALED / roms_size);
        display_text_at(RENDER_WIDTH_SCALED / 2, (i*y) + (y/2), roms[i], color);
    }
}

void Renderer::render_selector(SDL_Rect selector_rect)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &selector_rect);
}

void Renderer::render_emulation(Chip8 *crispy)
{
    crispy->emulate_cycle();

    // @Note: `render` is set to true after initialization, so it will render at least once
    // in other words: it will clear the screen when first called.
    if (crispy->render) {
        u32 pixelBuffer[CHIP8_DISPLAY];

        for (u32 i = 0; i < CHIP8_DISPLAY; ++i) {
            if (crispy->display[i]) {
                pixelBuffer[i] = 0xFFFFFFFF;
            } else {
                pixelBuffer[i] = 0x000000FF;
            }
        }

        SDL_UpdateTexture(texture_crispy, NULL, pixelBuffer, RENDER_WIDTH * sizeof(u32));
        SDL_RenderCopy(renderer, texture_crispy, NULL, NULL);

        crispy->render = false;
    }
}

Renderer::~Renderer()
{
    SDL_DestroyTexture(texture_crispy);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}
