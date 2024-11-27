#pragma once

#include <cstdint>
#include <chrono>
#include <random>

constexpr unsigned int START_ADDRESS = 0x200;
constexpr unsigned int FONTSET_START_ADDRESS = 0x50;

enum Registers {
    V0,
    V1,
    V2,
    V3,
    V4,
    V5,
    V6,
    V7,
    V8,
    V9,
    VA,
    VB,
    VC,
    VD,
    VE,
    VF
};

class Chip8 {
    private:
        uint8_t registers[16]{};
        // All programs start at memory address 0x200
        uint8_t memory[4096]{};
        // specified as 16 bits
        // but we are using a array hence 8 bits
        uint8_t sp{};
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        uint16_t pc{};
        uint16_t stack[16]{};
        // index for display
        uint16_t index; 
        uint32_t display[64 * 32]{};

        // other
        std::default_random_engine rand_gen;
        std::uniform_int_distribution<uint8_t> rand_byte;
    public:
        void loadROM(const char *filename);

        void opCls();
        void opRet();
        void opJp(uint16_t address);
        void opCall(uint16_t address);
        void opSei(uint8_t x, uint8_t value);
        void opSnei(uint8_t x, uint8_t value);
        void opSe(uint8_t x, uint8_t y);
        void opLdi(uint8_t x, uint8_t value);
        void opAddi(uint8_t x, uint8_t value);
        void opLd(uint8_t x, uint8_t y);
        void opOr(uint8_t x, uint8_t y);
        void opAnd(uint8_t x, uint8_t y);
        void opXor(uint8_t x, uint8_t y);
        void opAdd(uint8_t x, uint8_t y);
        void opSub(uint8_t x, uint8_t y);
        void opShr(uint8_t x);
        void opSubn(uint8_t x, uint8_t y);
        void opShl(uint8_t x);
        void opSne(uint8_t x, uint8_t y);
        void opLda(uint16_t addr);
        void opJpo(uint16_t addr);
        void opRnd(uint8_t x, uint8_t value);
};