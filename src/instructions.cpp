#include <cstring>

#include "chip8.hpp"

void Chip8::op00E0() { std::memset(display, 0, sizeof(display)); }

void Chip8::op00EE() { pc = stack[--sp]; }

void Chip8::op1nnn() { pc = get_addr(); }

void Chip8::op2nnn() {
    stack[sp++] = pc;
    pc = get_addr();
}

void Chip8::op3xkk() { pc += 2 * (registers[get_vx()] == get_byte()); }

void Chip8::op4xkk() { pc += 2 * (registers[get_vx()] != get_byte()); }

void Chip8::op5xy0() { pc += 2 * (registers[get_vx()] == registers[get_vy()]); }

void Chip8::op6xkk() { registers[get_vx()] = get_byte(); }

void Chip8::op7xkk() { registers[get_vx()] += get_byte(); }

void Chip8::op8xy0() { registers[get_vx()] = registers[get_vy()]; }

void Chip8::op8xy1() { registers[get_vx()] |= registers[get_vy()]; }

void Chip8::op8xy2() { registers[get_vx()] &= registers[get_vy()]; }

void Chip8::op8xy3() { registers[get_vx()] ^= registers[get_vy()]; }

void Chip8::op8xy4() {
    // shouldnt complain with sanitizer
    // for overflow
    // cause its well defined
    uint8_t rx = registers[get_vx()];
    uint8_t ry = registers[get_vy()];
    uint16_t result = rx + ry;
    // From hackers delight - overflow detection
    registers[VF] = ((rx & ry) | ((rx | ry) & ~result)) >> 15u;
    registers[get_vx()] = result;
}

void Chip8::op8xy5() {
    uint8_t rx = registers[get_vx()];
    uint8_t ry = registers[get_vy()];
    registers[VF] = rx > ry;
    registers[get_vx()] -= ry;
}

void Chip8::op8xy6() {
    registers[VF] = (registers[get_vx()] & 0x1u);
    registers[get_vx()] >>= 1;
}

void Chip8::op8xy7() {
    uint8_t rx = registers[get_vx()];
    uint8_t ry = registers[get_vy()];
    registers[VF] = rx > ry;
    registers[get_vx()] = ry - rx;
}

void Chip8::op8xyE() {
    registers[VF] = (registers[get_vx()] & 0x80u) >> 7u;
    registers[get_vx()] <<= 1;
}

void Chip8::op9xy0() { pc += 2 * (registers[get_vx()] != registers[get_vy()]); }

void Chip8::opAnnn() { index = get_addr(); }

void Chip8::opBnnn() { pc = registers[V0] + get_addr(); }

void Chip8::opCxkk() { registers[get_vx()] = rand_byte(rand_gen) & get_byte(); }

void Chip8::opDxyn() {
    uint8_t ypos = registers[get_vy()] % SCREEN_HEIGHT;
    uint8_t xpos = registers[get_vx()] % SCREEN_WIDTH;
    uint8_t height = get_nibble();

    registers[VF] = 0;  
    for (size_t row = 0; row < height; row++) {
        uint8_t sprite_byte = memory[index + row];
        for (size_t col = 0; col < 8; col++) {
            uint8_t sprite_pixel = sprite_byte & (0x80u >> col);
            uint32_t index = (ypos + row) * SCREEN_WIDTH + (xpos + col);
            if (index >= SCREEN_WIDTH * SCREEN_HEIGHT) continue;
            uint32_t* display_pixel =
                &display[(ypos + row) * SCREEN_WIDTH + (xpos + col)];            

            bool cond = sprite_pixel && *display_pixel == 0xFFFFFFFF;
            registers[VF] = cond + registers[VF] * !cond;

            *display_pixel ^= 0xFFFFFFFF * (bool)sprite_pixel;   

        }
    }
}
void Chip8::opEx9E() { pc += 2 * (keypad[registers[get_vx()]] != 0); }

void Chip8::opExA1() { pc += 2 * (keypad[registers[get_vx()]] == 0); }

void Chip8::opFx07() { registers[get_vx()] = delay_timer; }

void Chip8::opFx0A() {
    for (size_t i = 0; i < 16; i++) {
        if (keypad[i]) {
            std::cout << "Key pressed: " << i << std::endl;
            registers[get_vx()] = i;
            return;
        }
    }
    pc -= 2;
}

void Chip8::opFx15() { delay_timer = registers[get_vx()]; }

void Chip8::opFx18() { sound_timer = registers[get_vx()]; }

void Chip8::opFx1E() {
    index += registers[get_vx()];
}

void Chip8::opFx29() {
    index = FONTSET_START_ADDRESS + registers[get_vx()] * 5;
}

void Chip8::opFx33() {
    uint8_t value = registers[get_vx()];

    memory[index + 2] = value % 10;
    value /= 10;
    memory[index + 1] = value % 10;
    value /= 10;
    memory[index] = value % 10;
}

void Chip8::opFx55() {
    for (size_t i = 0; i <= get_vx(); i++) {
        memory[index + i] = registers[i];
    }
}

void Chip8::opFx65() {
    for (size_t i = 0; i <= get_vx(); i++) {
        registers[i] = memory[index + i];
    }
}
