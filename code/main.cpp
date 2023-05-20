#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "typedefs.h"
#include "utils.h"

#include "smol_renderer.h"

global const char *ROMS[] = {
    "../roms/IBM Logo.ch8",
    "../roms/Maze [David Winter, 199x].ch8",
    "../roms/Sierpinski [Sergey Naydenov, 2010].ch8",
    "../roms/Pong [Paul Vervalin, 1990].ch8",
};

struct Selector {
    SDL_Rect rect;    
    u32 index;
};

// @ToDo: Could probably add something like MEM_DEBUG mode or something
// similar for fun?
enum Mode {
    MENU = 0,
    EMULATION,
};

internal void parse_movement_key_down(Chip8 *crispy, SDL_Keycode key);
internal void parse_movement_key_up(Chip8 *crispy, SDL_Keycode key);

internal void selector_move_up(Selector *selector)
{
    if (selector->rect.y == 0) {
        selector->index = ARRAY_LEN(ROMS) - 1;
        selector->rect.y = RENDER_HEIGHT_SCALED - selector->rect.h;
    } else {
        selector->index -= 1;
        selector->rect.y -= selector->rect.h;
    }
}

internal void selector_move_down(Selector *selector)
{
    if (selector->rect.y == RENDER_HEIGHT_SCALED - selector->rect.h) {
        selector->index = 0;
        selector->rect.y = 0;
    } else {
        selector->index += 1;
        selector->rect.y += selector->rect.h;
    }
}

internal Selector create_selector(u32 x, u32 y, u32 w, u32 h)
{
    Selector selector = {0};
    selector.rect.x = x;
    selector.rect.y = y;
    selector.rect.w = w;
    selector.rect.h = h;
    
    return(selector);
}

internal void handle_menu_events(Selector *selector, SDL_Event event)
{
    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_UP: selector_move_up(selector); break;
                case SDLK_DOWN: selector_move_down(selector); break;
            }
        } break;
    }
}

internal void handle_emulation_events(Chip8 *crispy, SDL_Event event)
{
    switch (event.type) {
        case SDL_KEYUP: {            
            parse_movement_key_up(crispy, event.key.keysym.sym);
        } break;
            
        case SDL_KEYDOWN: {
            parse_movement_key_down(crispy, event.key.keysym.sym);
        } break;
    }
}

int main(int argc, char **argv)
{
    Chip8 crispy;
    Mode current_mode = MENU;
    Renderer smol("CrispyChip - CHIP8 Emulator");
    Selector selector = create_selector(0, 0, RENDER_WIDTH_SCALED, RENDER_HEIGHT_SCALED / ARRAY_LEN(ROMS));
    
    if (argc > 1) {
        crispy.initialize();
        
        if (crispy.load_rom(argv[1])) {
            current_mode = EMULATION;
        }
    }
    
    SDL_Event event;
    bool should_quit = false;

    while (!should_quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    should_quit = true;
                } break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE: {
                            switch (current_mode) {
                                case MENU: should_quit = true; break;
                                case EMULATION: current_mode = MENU; break;
                            }
                        } break;

                        case SDLK_RETURN: {
                            // @Note: Yes, this could be inside handle_menu_events() function, however I think that would
                            // ruin the simplicity of the function itself, besides this is a small project and a small little
                            // thing that we know won't grow much so it's fine to just leave this little if in here.
                            if (current_mode == MENU) {                                
                                crispy.initialize();
                                
                                if (crispy.load_rom(ROMS[selector.index])) {
                                    current_mode = EMULATION;
                                }
                            }
                        } break;
                    }
                } break;
            }

            switch (current_mode) {
                case MENU: handle_menu_events(&selector, event); break;
                case EMULATION: handle_emulation_events(&crispy, event); break;
            }
        }

        switch (current_mode) {
            case MENU: {
                SDL_SetRenderDrawColor(smol.renderer, 0, 0, 0, 255);
                SDL_RenderClear(smol.renderer);
                
                smol.render_selector(selector.rect);
                smol.render_menu(selector.index, ROMS, ARRAY_LEN(ROMS));
            } break;
                
            case EMULATION: smol.render_emulation(&crispy); break;
        }
        
        SDL_RenderPresent(smol.renderer);
        SDL_Delay(1);
    }

    return 0;
}

internal void parse_movement_key_down(Chip8 *crispy, SDL_Keycode key)
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

internal void parse_movement_key_up(Chip8 *crispy, SDL_Keycode key)
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
