#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "typedefs.h"
#include "utils.h"

#include "smol_renderer.h"

// @ToDo: Substitute emulation flag with something else to distinguish
// between appropriate draw-calls?

global const char *ROMS[] = {
    "../roms/IBM Logo.ch8",
    "../roms/Maze [David Winter, 199x].ch8",
    "../roms/Sierpinski [Sergey Naydenov, 2010].ch8",
    "../roms/Pong [Paul Vervalin, 1990].ch8",
};

void parse_movement_key_down(Chip8 *crispy, SDL_Keycode key);
void parse_movement_key_up(Chip8 *crispy, SDL_Keycode key);

struct Selector {
    SDL_Rect rect;    
    u32 index;
};

void selector_move_up(Selector *selector)
{
    if (selector->rect.y == 0) {
        selector->index = ARRAY_LEN(ROMS) - 1;
        selector->rect.y = RENDER_HEIGHT_SCALED - selector->rect.h;
    } else {
        selector->index -= 1;
        selector->rect.y -= selector->rect.h;
    }
}

void selector_move_down(Selector *selector)
{
    if (selector->rect.y == RENDER_HEIGHT_SCALED - selector->rect.h) {
        selector->index = 0;
        selector->rect.y = 0;
    } else {
        selector->index += 1;
        selector->rect.y += selector->rect.h;
    }
}

int main(int argc, char **argv)
{
    Chip8 crispy;
    
    Selector selector = {0};
    {
        selector.rect.w = RENDER_WIDTH_SCALED;
        selector.rect.h = (RENDER_HEIGHT_SCALED / ARRAY_LEN(ROMS));
    }
    
    bool emulation = true;
    if (argc > 1) {
        crispy.initialize();

        if (!crispy.load_rom(argv[1])) {
            exit(1);
        }
    } else {
        emulation = false;
    }

    Renderer smol("CrispyChip - CHIP8 Emulator");
    bool should_quit = false;
    
    while (!should_quit) {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    should_quit = true;
                } break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE: {
                            if (!emulation) {
                                should_quit = true;
                            } else {
                                emulation = false;
                            }
                        } break;

                        case SDLK_UP: {
                            if (!emulation) {
                                selector_move_up(&selector);
                            }
                        } break;
                            
                        case SDLK_DOWN: {
                            if (!emulation) {
                                selector_move_down(&selector);
                            }
                        } break;
                            
                        case SDLK_RETURN: {
                            if (!emulation) {
                                crispy.initialize();
                                crispy.load_rom(ROMS[selector.index]);
                                
                                emulation = true;
                            }
                        } break;
                            
                        default: {
                            if (emulation) {
                                parse_movement_key_down(&crispy, event.key.keysym.sym);
                            }
                        } break;
                    }
                } break;

                case SDL_KEYUP: {
                    if (emulation) {
                        parse_movement_key_up(&crispy, event.key.keysym.sym);
                    }
                } break;
            }
        }

        if (emulation) {
            smol.render_emulation(&crispy);
        } else {
            SDL_SetRenderDrawColor(smol.renderer, 0, 0, 0, 255);
            SDL_RenderClear(smol.renderer);
            
            smol.render_selector(selector.rect);
            smol.render_menu(selector.index, ROMS, ARRAY_LEN(ROMS));
        }

        SDL_RenderPresent(smol.renderer);
        SDL_Delay(1);
    }

    return 0;
}

void parse_movement_key_down(Chip8 *crispy, SDL_Keycode key)
{
    switch (key) {
        case SDLK_1:
            crispy->keyboard[0x1] = 1;
            break;
        case SDLK_2:
            crispy->keyboard[0x2] = 1;
            break;
        case SDLK_3:
            crispy->keyboard[0x3] = 1;
            break;
        case SDLK_4:
            crispy->keyboard[0xC] = 1;
            break;

        case SDLK_q:
            crispy->keyboard[0x4] = 1;
            break;
        case SDLK_w:
            crispy->keyboard[0x5] = 1;
            break;
        case SDLK_e:
            crispy->keyboard[0x6] = 1;
            break;
        case SDLK_r:
            crispy->keyboard[0xD] = 1;
            break;

        case SDLK_a:
            crispy->keyboard[0x7] = 1;
            break;
        case SDLK_s:
            crispy->keyboard[0x8] = 1;
            break;
        case SDLK_d:
            crispy->keyboard[0x9] = 1;
            break;
        case SDLK_f:
            crispy->keyboard[0xE] = 1;
            break;

        case SDLK_z:
            crispy->keyboard[0xA] = 1;
            break;
        case SDLK_x:
            crispy->keyboard[0x0] = 1;
            break;
        case SDLK_c:
            crispy->keyboard[0xB] = 1;
            break;
        case SDLK_v:
            crispy->keyboard[0xF] = 1;
            break;
    }
}

void parse_movement_key_up(Chip8 *crispy, SDL_Keycode key)
{
    switch (key) {
        case SDLK_1:
            crispy->keyboard[0x1] = 0;
            break;
        case SDLK_2:
            crispy->keyboard[0x2] = 0;
            break;
        case SDLK_3:
            crispy->keyboard[0x3] = 0;
            break;
        case SDLK_4:
            crispy->keyboard[0xC] = 0;
            break;

        case SDLK_q:
            crispy->keyboard[0x4] = 0;
            break;
        case SDLK_w:
            crispy->keyboard[0x5] = 0;
            break;
        case SDLK_e:
            crispy->keyboard[0x6] = 0;
            break;
        case SDLK_r:
            crispy->keyboard[0xD] = 0;
            break;

        case SDLK_a:
            crispy->keyboard[0x7] = 0;
            break;
        case SDLK_s:
            crispy->keyboard[0x8] = 0;
            break;
        case SDLK_d:
            crispy->keyboard[0x9] = 0;
            break;
        case SDLK_f:
            crispy->keyboard[0xE] = 0;
            break;

        case SDLK_z:
            crispy->keyboard[0xA] = 0;
            break;
        case SDLK_x:
            crispy->keyboard[0x0] = 0;
            break;
        case SDLK_c:
            crispy->keyboard[0xB] = 0;
            break;
        case SDLK_v:
            crispy->keyboard[0xF] = 0;
            break;
    }
}
