#pragma once

#include <chrono>
#include <cstdint>
#include <random>

constexpr unsigned int START_ADDRESS = 0x200;
constexpr unsigned int FONTSET_START_ADDRESS = 0x50;

constexpr unsigned int SCREEN_WIDTH = 64;
constexpr unsigned int SCREEN_HEIGHT = 32;

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
    uint16_t opcode{};
    uint16_t stack[16]{};
    uint16_t keypad[16]{};
    // index for display
    uint16_t index{};
    uint32_t display[SCREEN_WIDTH * SCREEN_HEIGHT]{};

    // table that points to functions
    typedef void (Chip8::*Chip8Func)();

    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];

    // other
    std::default_random_engine rand_gen;
    std::uniform_int_distribution<uint8_t> rand_byte;

    inline uint8_t get_vx() const { return (opcode & 0x0F00u) >> 8u; }
    inline uint8_t get_vy() const { return (opcode & 0x00F0u) >> 4u; }
    inline uint8_t get_byte() const { return opcode & 0x00FFu; }
    inline uint8_t get_addr() const { return opcode & 0x0FFFu; }
    inline uint8_t get_nibble() const { return opcode & 0x000Fu; }
    inline uint8_t get_table_idx() const { return (opcode & 0xF000u) >> 12u; }

    void ftable0() {
        (this->*table0[get_nibble()])();
    }

    void ftable8() {
        (this->*table8[get_nibble()])();
    }

    void ftableE() {
        (this->*tableE[get_nibble()])();
    }

    void ftableF() {
        (this->*tableF[get_nibble()])();
    }

    void opNULL() {}

    void op00E0();
    void op00EE();
    void op1nnn();
    void op2nnn();
    void op3xkk();
    void op4xkk();
    void op5xy0();
    void op6xkk();
    void op7xkk();
    void op8xy0();
    void op8xy1();
    void op8xy2();
    void op8xy3();
    void op8xy4();
    void op8xy5();
    void op8xy6();
    void op8xy7();
    void op8xyE();
    void op9xy0();
    void opAnnn();
    void opBnnn();
    void opCxkk();
    void opDxyn();
    void opEx9E();
    void opExA1();
    void opFx07();
    void opFx0A();
    void opFx15();
    void opFx18();
    void opFx1E();
    void opFx29();
    void opFx33();
    void opFx55();
    void opFx65();

   public:
    void loadROM(const char *filename);
    void cycle();
};