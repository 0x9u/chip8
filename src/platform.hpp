#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

class Platform {
    public:
    Platform(const char *title,
        unsigned int window_width,
        unsigned int window_height,
        unsigned int texture_width,
        unsigned int texture_height);
    ~Platform();
    void update(const void* buffer, int pitch);
    bool processInput(uint8_t *keys);
    private:
            SDL_Window *window{};
            SDL_Renderer* renderer{};
            SDL_Texture* texture{};
};