#include "chip8.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "font.hpp"

void Chip8::loadROM(const char *filename) {
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(filename, std::ios::binary | std::ios::ate);
    } catch (const std::ios_base::failure &e) {
        std::cerr << filename << ":" << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    // gets file size to create buffer
    std::streampos size = file.tellg();
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();

    uint8_t *dest = memory + START_ADDRESS;
    std::memcpy(dest, buffer.data(), size);
}

Chip8::Chip8() {
    pc = START_ADDRESS;

    uint8_t *dest = memory + FONTSET_START_ADDRESS;
    std::memcpy(dest, fontset, FONTSET_SIZE);

    rand_gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    rand_byte = std::uniform_int_distribution<uint8_t>(0, 255U);
}