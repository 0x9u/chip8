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

    table[0x0] = &ftable0;
    table[0x1] = &op1nnn;
    table[0x2] = &op2nnn;
    table[0x3] = &op3xkk;
    table[0x4] = &op4xkk;
    table[0x5] = &op5xy0;
    table[0x6] = &op6xkk;
    table[0x7] = &op7xkk;
    table[0x8] = &ftable8;
    table[0x9] = &op9xy0;
    table[0xA] = &opAnnn;
    table[0xB] = &opBnnn;
    table[0xC] = &opCxkk;
    table[0xD] = &opDxyn;
    table[0xE] = &ftableE;

    for (size_t i = 0; i <= 0xE; i++) {
        table0[i] = &opNULL;
        table8[i] = &opNULL;
        tableE[i] = &opNULL;
    }
    
    table0[0x0] = &op00E0;
    table0[0xE] = &op00EE;
    
    table8[0x0] = &op8xy0;
    table8[0x1] = &op8xy1;
    table8[0x2] = &op8xy2;
    table8[0x3] = &op8xy3;
    table8[0x4] = &op8xy4;
    table8[0x5] = &op8xy5;
    table8[0x6] = &op8xy6;
    table8[0x7] = &op8xy7;
    table8[0xE] = &op8xyE;

    tableE[0x1] = &opExA1;
    tableE[0xE] = &opEx9E;

    for (size_t i = 0; i <= 0x65; i++) {
        tableF[i] = &opNULL;
    }

    tableF[0x07] = &opFx07;
    tableF[0x0A] = &opFx0A;
    tableF[0x15] = &opFx15;
    tableF[0x18] = &opFx18;
    tableF[0x1E] = &opFx1E;
    tableF[0x29] = &opFx29;
    tableF[0x33] = &opFx33;
    tableF[0x65] = &opFx65;
}

void Chip8::cycle() {
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    pc += 2;
    (this->*table[get_table_idx()])();

    delay_timer -= delay_timer > 0;
    sound_timer -= sound_timer > 0;
}