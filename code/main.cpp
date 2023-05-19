#include <iostream>

#include "typedefs.h"
#include "smol_renderer.h"

void parseMovementKeyUp(Chip8& crispy, SDL_Keycode key);
void parseMovementKeyDown(Chip8& crispy, SDL_Keycode key);

int main(int argc, char **argv)
{
    SDL_Rect selector = {
        (WIDTH / 4) - 150, (HEIGHT / 4) - 80,
        (WIDTH / 2) + 300, 80
    };
    
    bool emulation = true;
    size_t index = 0;
  
    Chip8 crispy;
  
    if (argc > 1) {
        crispy.initialize();

        if (!crispy.load_rom(argv[1])) {
            exit(1);
        }
    } else {
        emulation = false;
    }

    Renderer renderer("CrispyChip - CHIP8 Emulator");
    bool isOpen = !renderer.error;
    SDL_Event event;
  
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
                                crispy.load_rom(ROMS[index].c_str());

                                emulation = true;
                            }
                            break;
                        default:
                            if (emulation) parseMovementKeyDown(crispy, event.key.keysym.sym);
                    }
                } break;

                case SDL_KEYUP:
                    if (emulation) parseMovementKeyUp(crispy, event.key.keysym.sym);
                    break;
            }
        }

        if (emulation) {
            renderer.renderEmulation(crispy);
        } else {
            renderer.renderMenu(index, selector);
        }
    }

    return 0;
}

void parseMovementKeyDown(Chip8& crispy, SDL_Keycode key)
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

void parseMovementKeyUp(Chip8& crispy, SDL_Keycode key)
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
