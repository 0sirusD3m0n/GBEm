#include "handlers.h"

uint8_t gb_handler_jr_n8(gb_t *gb) {
    int8_t offset = (int8_t)fetch(gb);

    gb->cpu.PC += offset;
    return 3;
}

uint8_t gb_handler_jr_cc_n8(gb_t *gb) {
    int8_t offset = (int8_t)fetch(gb);
    uint8_t cc    = (gb->cpu.IR >> 3) & 0x03;

    bool condition;
    switch (cc) {
        case 0: condition = !gb->cpu.AF.zero;  break;  // NZ
        case 1: condition =  gb->cpu.AF.zero;  break;  // Z
        case 2: condition = !gb->cpu.AF.carry; break;  // NC
        case 3: condition =  gb->cpu.AF.carry; break;  // C
    }

    if (condition) {
        gb->cpu.PC += offset;
        return 3;  // branch taken
    }
    return 2;  // branch not taken
}

uint8_t gb_handler_ret_cc(gb_t *gb) {
    uint8_t cc = (gb->cpu.IR >> 3) & 0x03;

    bool condition;
    switch (cc) {
        case 0: condition = !gb->cpu.AF.zero;  break;  // NZ
        case 1: condition =  gb->cpu.AF.zero;  break;  // Z
        case 2: condition = !gb->cpu.AF.carry; break;  // NC
        case 3: condition =  gb->cpu.AF.carry; break;  // C
    }

    if (condition) {
        gb->cpu.PC = stack_pop(gb);
        return 5;
    }

    return 2;
}

uint8_t gb_handler_ret(gb_t *gb) {
    gb->cpu.PC = stack_pop(gb);
    return 4;
}

uint8_t gb_handler_reti(gb_t *gb) {
    gb->cpu.PC = stack_pop(gb);
    gb->cpu.ime = 1;
    return 4;
}

uint8_t gb_handler_jp_cc_n16(gb_t *gb) {
    uint8_t cc = (gb->cpu.IR >> 3) & 0x03;
    uint16_t dst = fetch16(gb);

    bool condition;
    switch (cc) {
        case 0: condition = !gb->cpu.AF.zero;  break;  // NZ
        case 1: condition =  gb->cpu.AF.zero;  break;  // Z
        case 2: condition = !gb->cpu.AF.carry; break;  // NC
        case 3: condition =  gb->cpu.AF.carry; break;  // C
    }

    if (condition) {
        gb->cpu.PC = dst;
        return 4;
    }
    return 3;
}

uint8_t gb_handler_jp_n16(gb_t *gb) {
    uint16_t dst = fetch16(gb);

    gb->cpu.PC = dst;

    return 4;
}

uint8_t gb_handler_jp_hl(gb_t *gb) {
    uint16_t dst = read_r16(gb, GB_REGISTER16_HL);

    gb->cpu.PC = dst;

    return 1;
}

uint8_t gb_handler_call_cc_n16(gb_t *gb) {
    uint8_t cc = (gb->cpu.IR >> 3) & 0x03;
    uint16_t dst = fetch16(gb);

    bool condition;
    switch (cc) {
        case 0: condition = !gb->cpu.AF.zero;  break;  // NZ
        case 1: condition =  gb->cpu.AF.zero;  break;  // Z
        case 2: condition = !gb->cpu.AF.carry; break;  // NC
        case 3: condition =  gb->cpu.AF.carry; break;  // C
    }

    if (condition) {
        stack_push(gb, gb->cpu.PC);
        gb->cpu.PC = dst;
        return 6;
    }
    return 3;
}

uint8_t gb_handler_call_n16(gb_t *gb) {
    uint16_t dst = fetch16(gb);

    stack_push(gb, gb->cpu.PC);
    gb->cpu.PC = dst;

    return 6;
}

uint8_t gb_handler_rst(gb_t *gb) {
    uint8_t vector = gb->cpu.IR & 0x38;  // mask bits 5-3
    stack_push(gb, gb->cpu.PC);
    gb->cpu.PC = vector;
    return 4;
}