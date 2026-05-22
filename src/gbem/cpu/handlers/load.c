#include "handlers.h"
#include "./cpu.h"

// LD r8, r8
uint8_t gb_handler_ld_r8_r8(gb_t *gb) {
    uint8_t dest = (gb->cpu.IR >> 3) & 0x07;
    uint8_t src = gb->cpu.IR & 0x07;
    write_r8(gb, dest, read_r8(gb, src));
    return (dest == 6 || src == 6) ? 2 : 1;
}

// LD r8, n8
uint8_t gb_handler_ld_r8_n8(gb_t *gb) {
    uint8_t dest = (gb->cpu.IR >> 3) & 0x07;
    uint8_t data = fetch(gb);
    write_r8(gb, dest, data);
    return (dest == 6) ? 3 : 2;
}

// LD r16, n16
uint8_t gb_handler_ld_r16_n16(gb_t *gb) {
    uint8_t dest = (gb->cpu.IR >> 4) & 0x03;
    uint16_t data = fetch16(gb);
    write_r16(gb, dest, data);
    return 3;
}

// LD [r16mem], A
uint8_t gb_handler_ld_r16mem_a(gb_t *gb) {
    uint8_t dest = (gb->cpu.IR >> 4) & 0x03;
    mem_write(gb, read_r16mem(gb, dest), gb->cpu.AF.high);
    return 2;
}

// LD A, [r16mem]
uint8_t gb_handler_ld_a_r16mem(gb_t *gb) {
    uint16_t src = (gb->cpu.IR >> 4) & 0x03;
    write_r8(gb, GB_REGISTER_A, read_r16mem(gb, src));
    return 2;
}

// LD [n16], SP
uint8_t gb_handler_ld_n16mem_sp(gb_t *gb) {
    uint16_t data = fetch16(gb);
    mem_write(gb, data, gb->cpu.SP & 0xFF);
    mem_write(gb, data + 1, (gb->cpu.SP >> 8) & 0xFF);
    return 5;
}

// LDH [C], A
uint8_t gb_handler_ldh_c_a(gb_t *gb) {
    uint16_t addr = 0xFF00 | gb->cpu.BC.low;
    mem_write(gb, addr, gb->cpu.AF.high);
    return 2;
}

// LDH [n8], A
uint8_t gb_handler_ldh_n8_a(gb_t *gb) {
    uint16_t addr = 0xFF00 | fetch(gb);
    mem_write(gb, addr, gb->cpu.AF.high);
    return 3;
}

// LD [n16], A
uint8_t gb_handler_ld_n16mem_a(gb_t *gb) {
    uint16_t addr = fetch16(gb);
    mem_write(gb, addr, gb->cpu.AF.high);
    return 4;
}

// LDH A, [C]
uint8_t gb_handler_ldh_a_c(gb_t *gb) {
    uint16_t addr = 0xFF00 | gb->cpu.BC.low;
    uint8_t data = mem_read(gb, addr);
    write_r8(gb, GB_REGISTER_A, data);
    return 2;
}

// LDH A, [n8]
uint8_t gb_handler_ldh_a_n8(gb_t *gb) {
    uint16_t addr = 0xFF00 | fetch(gb);
    uint8_t data = mem_read(gb, addr);
    write_r8(gb, GB_REGISTER_A, data);
    return 3;
}