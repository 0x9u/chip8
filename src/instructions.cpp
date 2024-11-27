#include "chip8.hpp"
#include <cstring>

void Chip8::opCls() {
    std::memset(display, 0, sizeof(display));
}

void Chip8::opRet() {
    pc = stack[--sp];
}

void Chip8::opJp(uint16_t address) {
    pc = address;
}

void Chip8::opCall(uint16_t address) {
    stack[sp++] = pc;
    pc = address;
}

void Chip8::opSei(uint8_t x, uint8_t value) {
    if (registers[x] == value) pc += 2;
}

void Chip8::opSnei(uint8_t x, uint8_t value) {
    if (registers[x] != value) pc += 2;
}

void Chip8::opSe(uint8_t x, uint8_t y) {
    if (registers[x] == registers[y]) pc += 2;
}

void Chip8::opLdi(uint8_t x, uint8_t value) {
    registers[x] = value;
}

void Chip8::opAddi(uint8_t x, uint8_t value) {
    registers[x] += value;
}

void Chip8::opLd(uint8_t x, uint8_t y) {
    registers[x] = registers[y];
}

void Chip8::opOr(uint8_t x, uint8_t y) {
    registers[x] |= registers[y];
}

void Chip8::opAnd(uint8_t x, uint8_t y) {
    registers[x] &= registers[y];
}

void Chip8::opXor(uint8_t x, uint8_t y) {
    registers[x] ^= registers[y];
}

void Chip8::opAdd(uint8_t x, uint8_t y) {
    // shouldnt complain with sanitizer
    // for overflow
    // cause its well defined
    uint8_t rX = registers[x];
    uint8_t rY = registers[y];
    uint8_t result = rX + rY;
    // From hackers delight - overflow detection
    registers[VF] = ((rX & rY) | ((rX | rY) & ~result)) >> 7u;
    registers[x] = result;
}


void Chip8::opSub(uint8_t x, uint8_t y) {
    uint8_t rX = registers[x];
    uint8_t rY = registers[y];
    registers[VF] = rX > rY;
    registers[x] -= rY;
}

void Chip8::opShr(uint8_t x) {
    registers[VF] = (registers[x] & 0x1u);
    registers[x] >>= 1;
}

void Chip8::opSubn(uint8_t x, uint8_t y) {
    uint8_t rX = registers[x];
    uint8_t rY = registers[y];
    registers[VF] = rX > rY;
    registers[x] = rY - rX;
}

void Chip8::opShl(uint8_t x) {
    registers[VF] = (registers[x] & 0x80u) >> 7u;
    registers[x] <<= 1;
}

void Chip8::opSne(uint8_t x, uint8_t y) {
    if (registers[x] != registers[y]) pc += 2;
}

void Chip8::opLda(uint16_t addr) {
    index = addr;
}

void Chip8::opJpo(uint16_t addr) {
    pc = registers[V0] + addr;
}

void Chip8::opRnd(uint8_t x, uint8_t value) {
    registers[x] = rand_byte(rand_gen) & value;
}