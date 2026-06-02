#include "handlers.h"

uint8_t gb_handler_adc_r8(gb_t *gb) {
    uint8_t src = (gb->cpu.IR & 0x07);


}

uint8_t gb_handler_inc_r16(gb_t *gb) {
    // Get operand
    uint8_t idx = (gb->cpu.IR >> 4) & 0x07;
    uint16_t val = read_r16(gb, idx);
    write_r16(gb, idx, ++val);
    
    //increment value
    return 2;
}

uint8_t gb_handler_dec_r16(gb_t *gb) {
    // Get operand
    uint8_t idx = (gb->cpu.IR >> 4) & 0x07;
    uint16_t val = read_r16(gb, idx);
    write_r16(gb, idx, --val);
    
    //increment value
    return 2;
}

uint8_t gb_handler_add_hl_r16(gb_t *gb) {
    // Get operrand
    uint8_t idx = (gb->cpu.IR >> 4) & 0x03;

    // Get data in r16
    uint16_t data = read_r16(gb, idx);

    //get data in HL
    uint16_t hl = read_r16(gb, GB_REGISTER16_HL);

    uint32_t val = data + hl;

    gb->cpu.AF.carry        = val > 0xFFFF;
    gb->cpu.AF.half_carry   = ((data & 0x0FFF) + (hl & 0x0FFF) > 0x0FFF);
    gb->cpu.AF.negative     = 0;

    write_r16(gb, GB_REGISTER16_HL, val & 0xFFFF);
}

uint8_t gb_handler_inc_r8(gb_t *gb) {
    // Get operand
    uint8_t idx = (gb->cpu.IR >> 3) & 0x07;

    uint8_t data = read_r8(gb, idx);

    uint16_t val = data + 1;

    gb->cpu.AF.negative = 0;
    gb->cpu.AF.zero = val == 0;
    gb->cpu.AF.half_carry = (data & 0x0F) + 1 > 0x0F;

    write_r8(gb, idx, val & 0xFF);
    return (idx == GB_REGISTER_HL_MEM) ? 3 : 1;
}