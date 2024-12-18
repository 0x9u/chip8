#include "platform.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <unordered_map>

Platform::Platform(const char *title, unsigned int window_width,
                   unsigned int window_height, unsigned int texture_width,
                   unsigned int texture_height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                  << "\n";
        std::exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, window_width,
                              window_height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << "\n";
        std::exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << "\n";
        std::exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                SDL_TEXTUREACCESS_STREAMING, texture_width,
                                texture_height);
    if (texture == nullptr) {
        std::cerr << "Texture could not be created! SDL_Error: "
                  << SDL_GetError() << "\n";
        std::exit(EXIT_FAILURE);
    }
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::update(const void *buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::processInput(uint8_t *keys) {
    bool quit = false;

    SDL_Event e;

    const std::unordered_map<SDL_Keycode, uint8_t> keymap = {
        {SDLK_x, 0x0}, {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_a, 0x7},
        {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_z, 0xA}, {SDLK_c, 0xB},
        {SDLK_4, 0xC}, {SDLK_r, 0xD}, {SDLK_f, 0xE}, {SDLK_v, 0xF}};

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else if (keymap.count(e.key.keysym.sym)) {
                    keys[keymap.at(e.key.keysym.sym)] = 1;
                }
                break;
            case SDL_KEYUP:
                if (keymap.count(e.key.keysym.sym)) {
                    keys[keymap.at(e.key.keysym.sym)] = 0;
                }
                break;
            default:
                break;
        }
    }
    return quit;
}
