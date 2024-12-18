#include <chrono>
#include <iostream>

#include "chip8.hpp"
#include "platform.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM> [Scale] [Delay]\n";
        std::exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    int scale = (argc > 2) ? std::stoi(argv[2]) : 10;
    int delay = (argc > 3) ? std::stoi(argv[3]) : 3;

    Platform platform{"Chip8", SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale,
                      SCREEN_WIDTH, SCREEN_HEIGHT};

    Chip8 chip8{};
    chip8.loadROM(filename);

    int pitch = sizeof(chip8.display[0]) * SCREEN_WIDTH;

    bool quit = false;
    while (!quit) {
        quit = platform.processInput(chip8.keypad);
        chip8.cycle();

        platform.update(chip8.display, pitch);
        SDL_Delay(delay);
    }

    return 0;
}