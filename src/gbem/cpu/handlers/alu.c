#include "gbem/gb.h"
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

uint8_t gb_handler_dec_r8(gb_t *gb) {
    uint8_t idx = (gb->cpu.IR >> 3) & 0x07;

    uint8_t data = read_r8(gb, idx);
    uint8_t val = data - 1;

    gb->cpu.AF.negative = 1;
    gb->cpu.AF.zero = val == 0;
    gb->cpu.AF.half_carry = (data & 0x0F) == 0x00;

    write_r8(gb, idx, val);
    return (idx == GB_REGISTER_HL_MEM) ? 3 : 1;
}

uint8_t gb_handler_add_a_r8(gb_t *gb) {
    // get src
    uint8_t src = gb->cpu.IR & 0x07;

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = read_r8(gb, src);

    uint16_t result = a_data + src_data;

    gb->cpu.AF.zero = result & 0xFF == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = (a_data & 0x0F) + (src_data & 0x0F) > 0x0F;
    gb->cpu.AF.carry  = result > 0xFF;

     write_r8(gb, GB_REGISTER_A, result & 0xFF);
     return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_adc_a_r8(gb_t *gb) {
    // get src
    uint8_t src = gb->cpu.IR & 0x07;

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = read_r8(gb, src);

    uint16_t result = (a_data + src_data) + gb->cpu.AF.carry;

    gb->cpu.AF.zero = result & 0xFF == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = (a_data & 0x0F) + (src_data & 0x0F) + gb->cpu.AF.carry > 0x0F;
    gb->cpu.AF.carry  = result > 0xFF;

     write_r8(gb, GB_REGISTER_A, result & 0xFF);
     return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_sub_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = read_r8(gb, src);

    uint8_t result = a_data - src_data;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data > a_data;

    write_r8(gb, GB_REGISTER_A, result);
    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_sbc_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = read_r8(gb, src);

    uint8_t result = a_data - src_data - gb->cpu.AF.carry;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) + gb->cpu.AF.carry > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data + gb->cpu.AF.carry > a_data;

    write_r8(gb, GB_REGISTER_A, result);
    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_and_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;
    
    uint8_t src_data = read_r8(gb, src);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data & a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 1;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_xor_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;
    
    uint8_t src_data = read_r8(gb, src);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data ^ a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 0;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_or_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;
    
    uint8_t src_data = read_r8(gb, src);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data | a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 0;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_cp_a_r8(gb_t *gb) {
    uint8_t src = gb->cpu.IR & 0x07;

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = read_r8(gb, src);

    uint8_t result = a_data - src_data;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data > a_data;

    return (src == GB_REGISTER_HL_MEM) ? 2 : 1;
}

uint8_t gb_handler_add_a_n8(gb_t *gb) {
    // get src
        
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = fetch(gb);

    uint16_t result = a_data + src_data;

    gb->cpu.AF.zero = (result & 0xFF) == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = (a_data & 0x0F) + (src_data & 0x0F) > 0x0F;
    gb->cpu.AF.carry  = result > 0xFF;

     write_r8(gb, GB_REGISTER_A, result & 0xFF);
     return 2;
}

uint8_t gb_handler_adc_a_n8(gb_t *gb) {

    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = fetch(gb);

    uint16_t result = (a_data + src_data) + gb->cpu.AF.carry;

    gb->cpu.AF.zero = (result & 0xFF) == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = (a_data & 0x0F) + (src_data & 0x0F) + gb->cpu.AF.carry > 0x0F;
    gb->cpu.AF.carry  = result > 0xFF;

     write_r8(gb, GB_REGISTER_A, result & 0xFF);
     return 2;
}

uint8_t gb_handler_sub_a_n8(gb_t *gb) {
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = fetch(gb);

    uint8_t result = a_data - src_data;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data > a_data;

    write_r8(gb, GB_REGISTER_A, result);
    return 2;
}

uint8_t gb_handler_sbc_a_n8(gb_t *gb) {
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = fetch(gb);

    uint8_t result = a_data - src_data - gb->cpu.AF.carry;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) + gb->cpu.AF.carry > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data + gb->cpu.AF.carry > a_data;

    write_r8(gb, GB_REGISTER_A, result);
    return 2;
}

uint8_t gb_handler_and_a_n8(gb_t *gb) {
    uint8_t src_data = fetch(gb);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data & a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 1;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return 2;
}

uint8_t gb_handler_xor_a_n8(gb_t *gb) {
    uint8_t src_data = fetch(gb);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data ^ a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 0;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return 2;
}

uint8_t gb_handler_or_a_n8(gb_t *gb) {    
    uint8_t src_data = fetch(gb);
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);

    uint8_t result = src_data | a_data;

    gb->cpu.AF.zero = result == 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = 0;
    gb->cpu.AF.carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return 2;
}

uint8_t gb_handler_cp_a_n8(gb_t *gb) {
    uint8_t a_data = read_r8(gb, GB_REGISTER_A);
    uint8_t src_data = fetch(gb);

    uint8_t result = a_data - src_data;

    gb->cpu.AF.zero     = result == 0;
    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = (src_data & 0x0F) > (a_data & 0x0F);
    gb->cpu.AF.carry = src_data > a_data;

    return 2;
}

uint8_t gb_handler_add_sp_e8(gb_t *gb) {
    int8_t e = (int8_t)fetch(gb);
    uint16_t sp = read_r16(gb, GB_REGISTER16_SP);
    uint16_t result = e + sp;

    gb->cpu.AF.zero = 0;
    gb->cpu.AF.negative = 0;
    gb->cpu.AF.half_carry = (e & 0x0F) + (sp & 0x0F) > 0x0F;
    gb->cpu.AF.carry =  (e & 0xFF) + (sp & 0xFF) > 0xFF;

    write_r16(gb, GB_REGISTER16_SP, result);
    return 4;
}

uint8_t gb_handler_daa(gb_t *gb) {
    uint8_t data = read_r8(gb, GB_REGISTER_A);
    uint8_t adj = 0;
    uint8_t result = data;
    if (gb->cpu.AF.negative ) {
        if (gb->cpu.AF.half_carry) {adj += 0x06;}
        if (gb->cpu.AF.carry)      {adj += 0x60;}
        result -= adj;
    } else {
        if (gb->cpu.AF.half_carry ||  (data & 0x0F) > 0x09) {adj += 0x06;}
        if (gb->cpu.AF.carry || (data > 0x99)) { adj += 0x60; gb->cpu.AF.carry = 1; }
        result += adj;
    }

    gb->cpu.AF.zero = result == 0x00;
    gb->cpu.AF.half_carry = 0;

    write_r8(gb, GB_REGISTER_A, result);
    return 1;
}

uint8_t gb_handler_cpl(gb_t *gb) {
    gb->cpu.AF.high = ~gb->cpu.AF.high;

    gb->cpu.AF.negative = 1;
    gb->cpu.AF.half_carry = 1;

    return 1;
}

uint8_t gb_handler_scf(gb_t *gb) {
    gb->cpu.AF.carry = 1;
    gb->cpu.AF.negative   = 0;
    gb->cpu.AF.half_carry = 0;
    return 1;
}

uint8_t gb_handler_ccf(gb_t *gb) {
    gb->cpu.AF.carry ^= 1;
    gb->cpu.AF.negative   = 0;
    gb->cpu.AF.half_carry = 0;
    return 1;
}