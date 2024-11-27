#pragma once

#include <cstdint>
#include <chrono>
#include <random>

constexpr unsigned int START_ADDRESS = 0x200;
constexpr unsigned int FONTSET_START_ADDRESS = 0x50;


class State {
    private:
        uint8_t registers[16]{};
        // All programs start at memory address 0x200
        uint8_t memory[4096]{};
        uint8_t display[64 * 32]{};
        // specified as 16 bits
        // but we are using a array hence 8 bits
        uint8_t sp{};
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        uint16_t pc{};
        uint16_t stack[16]{};

        // other
        std::default_random_engine rand_gen;
        std::uniform_int_distribution<uint8_t> rand_byte;
    public:
        void LoadROM(const char *filename);
};