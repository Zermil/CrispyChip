#include <iostream>
#include "smol_renderer.h"

Renderer::Renderer(const std::string& title)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL\n";
        error = true;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Failed to initialize SDL_ttf\n";
        error = true;
    }

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
        );

    if (window == nullptr) {
        std::cerr << "Failed to initialize SDL Window\n";
        error = true;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
        );

    if (renderer == nullptr) {
        std::cerr << "Failed to initialize SDL Renderer\n";
        error = true;
    }

    texture_crispy = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64, 32
        );

    if (texture_crispy == nullptr) {
        std::cerr << "Failed to initialize SDL Texture\n";
        error = true;
    }

    font = TTF_OpenFont("Consolas.ttf", FONT_SIZE);
}

void Renderer::displayTextAt(const std::string& msg_text, SDL_Color& color, int y)
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

void Renderer::renderMenu(size_t index, const SDL_Rect& selector)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &selector);

    for (int i = 0; i < 4; ++i) {
        SDL_Color color;

        if (i == index) color = { 0, 0, 0, 255 };
        else color = { 255, 255, 255, 255 };

        displayTextAt(ROMS[i], color, 75 + (i * 120));
    }

    SDL_RenderPresent(renderer);
}

void Renderer::renderEmulation(Chip8& crispy)
{
    crispy.emulate_cycle();

    // Render is set to true after initialization, so it will render at least once, 
    // in other words: it will clear the screen when first called.
    if (crispy.render) {
        uint32_t pixelBuffer[2048];

        for (int i = 0; i < 2048; ++i) {
            if (crispy.display[i]) pixelBuffer[i] = 0xFFFFFFFF;
            else pixelBuffer[i] = 0x000000FF;
        }

        SDL_UpdateTexture(texture_crispy, nullptr, pixelBuffer, 64 * sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture_crispy, nullptr, nullptr);

        crispy.render = false;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}

Renderer::~Renderer()
{
    // Clean up
    SDL_DestroyTexture(texture_crispy);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}
