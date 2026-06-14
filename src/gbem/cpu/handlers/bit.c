#include "gbem/gb.h"
#include "handlers.h"

uint8_t gb_handler_rlca(gb_t *gb) {
    uint8_t a = read_r8(gb, GB_REGISTER_A);
    uint8_t c = (a >> 7) & 0x01;

    a = a << 1 | c;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.carry = c;
    write_r8(gb, GB_REGISTER_A, a);
    return 1;
}

uint8_t gb_handler_rrca(gb_t *gb) {
    uint8_t a = read_r8(gb, GB_REGISTER_A);
    uint8_t c = a & 0x01;

    a = a >> 1 | (c << 7);

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.carry = c;
    write_r8(gb, GB_REGISTER_A, a);
    return 1;
}

uint8_t gb_handler_rla(gb_t *gb) {
    uint8_t a = read_r8(gb, GB_REGISTER_A);
    uint8_t c = gb->cpu.AF.carry;
    uint8_t car = (a >> 7) & 0x01;

    a = a << 1 | c;
    
    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.carry = car;
    write_r8(gb, GB_REGISTER_A, a);
    return 1;
}

uint8_t gb_handler_rra(gb_t *gb) {
    uint8_t a = read_r8(gb, GB_REGISTER_A);
    uint8_t c = gb->cpu.AF.carry;
    uint8_t car = a & 0x01;

    a = (a >> 1) | (c << 7);
    
    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.carry = car;
    write_r8(gb, GB_REGISTER_A, a);
    return 1;
}

uint8_t gb_handler_rlc_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b7 = (data >> 7) & 0x01;

    data = (data << 1) | b7;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b7;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_rrc_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b0 = data & 0x01;

    data = (data >> 1) | (b0 << 7);

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b0;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_rl_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b7 = (data >> 7) & 0x01;
    uint8_t carry = gb->cpu.AF.carry;

    data = (data << 1) | carry;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b7;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_rr_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b0 = data & 0x01;
    uint8_t carry = gb->cpu.AF.carry;

    data = (data >> 1) | (carry << 7);

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b0;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_sla_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b7 = (data >> 7) & 0x01;

    data = data << 1;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b7;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_sra_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b7 = (data >> 7 ) & 0x01;
    uint8_t b0 = data & 0x01;
    data = (data >> 1) | (b7 << 7);

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b0;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_swap_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;
    uint8_t data = read_r8(gb, idx);

    uint8_t high = (data >> 4) & 0x0F;
    uint8_t low = data & 0x0F;

    uint8_t result = (low << 4) | high;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = result == 0;

    write_r8(gb, idx, result);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_srl_r8(gb_t *gb) {
     uint8_t idx = gb->cpu.IR & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t b0 = data & 0x01;

    data = data >> 1;

    gb->cpu.AF.low = 0x00;
    gb->cpu.AF.zero = data == 0;
    gb->cpu.AF.carry = b0;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_bit_u3_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;
    uint8_t data = read_r8(gb, idx);
    uint8_t bit = (gb->cpu.IR >> 3) & 0x07;

    gb->cpu.AF.zero = !((data >> bit) & 0x01);
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 1;
    return (idx == GB_REGISTER_HL_MEM) ? 3 : 2;
}

uint8_t gb_handler_res_u3_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;
    uint8_t bit = (gb->cpu.IR >> 3) & 0x07;
    uint8_t data = read_r8(gb, idx);

    data &= ~(1 << bit);

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}

uint8_t gb_handler_set_u3_r8(gb_t *gb) {
    uint8_t idx = gb->cpu.IR & 0x07;
    uint8_t bit = (gb->cpu.IR >> 3) & 0x07;
    uint8_t data = read_r8(gb, idx);

    data |= 1 << bit;

    write_r8(gb, idx, data);
    return (idx == GB_REGISTER_HL_MEM) ? 4 : 2;
}