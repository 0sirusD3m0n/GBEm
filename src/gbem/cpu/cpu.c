/**
 * cpu.c
 * 
 * contains function definitions
 * for gbem_cpu
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/16/2026
 */
 #include "gbem/gb.h"
 #include "cpu.h"
 #include "handlers/handlers.h"
 #include "gbem/memory/bus.h"

 static const gb_opcode_handler_t uc_table[256] = {
    [0x00]=gb_handler_nop,       [0x01]=gb_handler_ld_r16_n16,  [0x02]=gb_handler_ld_r16mem_a, [0x03]=gb_handler_inc_r16,     [0x04]=gb_handler_inc_r8,       [0x05]=gb_handler_dec_r8,      [0x06]=gb_handler_ld_r8_n8,    [0x07]=gb_handler_rlca,        [0x08]=gb_handler_ld_n16mem_sp, [0x09]=gb_handler_add_hl_r16,  [0x0A]=gb_handler_ld_a_r16mem,  [0x0B]=gb_handler_dec_r16,     [0x0C]=gb_handler_inc_r8,      [0x0D]=gb_handler_dec_r8,      [0x0E]=gb_handler_ld_r8_n8,    [0x0F]=gb_handler_rrca,
    [0x10]=gb_handler_stop,      [0x11]=gb_handler_ld_r16_n16,  [0x12]=gb_handler_ld_r16mem_a, [0x13]=gb_handler_inc_r16,     [0x14]=gb_handler_inc_r8,       [0x15]=gb_handler_dec_r8,      [0x16]=gb_handler_ld_r8_n8,    [0x17]=gb_handler_rla,         [0x18]=gb_handler_jr_n8,        [0x19]=gb_handler_add_hl_r16,  [0x1A]=gb_handler_ld_a_r16mem,  [0x1B]=gb_handler_dec_r16,     [0x1C]=gb_handler_inc_r8,      [0x1D]=gb_handler_dec_r8,      [0x1E]=gb_handler_ld_r8_n8,    [0x1F]=gb_handler_rra,
    [0x20]=gb_handler_jr_cc_n8,  [0x21]=gb_handler_ld_r16_n16,  [0x22]=gb_handler_ld_r16mem_a, [0x23]=gb_handler_inc_r16,     [0x24]=gb_handler_inc_r8,       [0x25]=gb_handler_dec_r8,      [0x26]=gb_handler_ld_r8_n8,    [0x27]=gb_handler_daa,         [0x28]=gb_handler_jr_cc_n8,     [0x29]=gb_handler_add_hl_r16,  [0x2A]=gb_handler_ld_a_r16mem,  [0x2B]=gb_handler_dec_r16,     [0x2C]=gb_handler_inc_r8,      [0x2D]=gb_handler_dec_r8,      [0x2E]=gb_handler_ld_r8_n8,    [0x2F]=gb_handler_cpl,
    [0x30]=gb_handler_jr_cc_n8,  [0x31]=gb_handler_ld_r16_n16,  [0x32]=gb_handler_ld_r16mem_a, [0x33]=gb_handler_inc_r16,     [0x34]=gb_handler_inc_r8,       [0x35]=gb_handler_dec_r8,      [0x36]=gb_handler_ld_r8_n8,    [0x37]=gb_handler_scf,         [0x38]=gb_handler_jr_cc_n8,     [0x39]=gb_handler_add_hl_r16,  [0x3A]=gb_handler_ld_a_r16mem,  [0x3B]=gb_handler_dec_r16,     [0x3C]=gb_handler_inc_r8,      [0x3D]=gb_handler_dec_r8,      [0x3E]=gb_handler_ld_r8_n8,    [0x3F]=gb_handler_ccf,
    [0x40]=gb_handler_ld_r8_r8,  [0x41]=gb_handler_ld_r8_r8,    [0x42]=gb_handler_ld_r8_r8,    [0x43]=gb_handler_ld_r8_r8,    [0x44]=gb_handler_ld_r8_r8,     [0x45]=gb_handler_ld_r8_r8,    [0x46]=gb_handler_ld_r8_r8,    [0x47]=gb_handler_ld_r8_r8,    [0x48]=gb_handler_ld_r8_r8,     [0x49]=gb_handler_ld_r8_r8,    [0x4A]=gb_handler_ld_r8_r8,     [0x4B]=gb_handler_ld_r8_r8,    [0x4C]=gb_handler_ld_r8_r8,    [0x4D]=gb_handler_ld_r8_r8,    [0x4E]=gb_handler_ld_r8_r8,    [0x4F]=gb_handler_ld_r8_r8,
    [0x50]=gb_handler_ld_r8_r8,  [0x51]=gb_handler_ld_r8_r8,    [0x52]=gb_handler_ld_r8_r8,    [0x53]=gb_handler_ld_r8_r8,    [0x54]=gb_handler_ld_r8_r8,     [0x55]=gb_handler_ld_r8_r8,    [0x56]=gb_handler_ld_r8_r8,    [0x57]=gb_handler_ld_r8_r8,    [0x58]=gb_handler_ld_r8_r8,     [0x59]=gb_handler_ld_r8_r8,    [0x5A]=gb_handler_ld_r8_r8,     [0x5B]=gb_handler_ld_r8_r8,    [0x5C]=gb_handler_ld_r8_r8,    [0x5D]=gb_handler_ld_r8_r8,    [0x5E]=gb_handler_ld_r8_r8,    [0x5F]=gb_handler_ld_r8_r8,
    [0x60]=gb_handler_ld_r8_r8,  [0x61]=gb_handler_ld_r8_r8,    [0x62]=gb_handler_ld_r8_r8,    [0x63]=gb_handler_ld_r8_r8,    [0x64]=gb_handler_ld_r8_r8,     [0x65]=gb_handler_ld_r8_r8,    [0x66]=gb_handler_ld_r8_r8,    [0x67]=gb_handler_ld_r8_r8,    [0x68]=gb_handler_ld_r8_r8,     [0x69]=gb_handler_ld_r8_r8,    [0x6A]=gb_handler_ld_r8_r8,     [0x6B]=gb_handler_ld_r8_r8,    [0x6C]=gb_handler_ld_r8_r8,    [0x6D]=gb_handler_ld_r8_r8,    [0x6E]=gb_handler_ld_r8_r8,    [0x6F]=gb_handler_ld_r8_r8,
    [0x70]=gb_handler_ld_r8_r8,  [0x71]=gb_handler_ld_r8_r8,    [0x72]=gb_handler_ld_r8_r8,    [0x73]=gb_handler_ld_r8_r8,    [0x74]=gb_handler_ld_r8_r8,     [0x75]=gb_handler_ld_r8_r8,    [0x76]=gb_handler_halt,        [0x77]=gb_handler_ld_r8_r8,    [0x78]=gb_handler_ld_r8_r8,     [0x79]=gb_handler_ld_r8_r8,    [0x7A]=gb_handler_ld_r8_r8,     [0x7B]=gb_handler_ld_r8_r8,    [0x7C]=gb_handler_ld_r8_r8,    [0x7D]=gb_handler_ld_r8_r8,    [0x7E]=gb_handler_ld_r8_r8,    [0x7F]=gb_handler_ld_r8_r8,
    [0x80]=gb_handler_add_a_r8,  [0x81]=gb_handler_add_a_r8,    [0x82]=gb_handler_add_a_r8,    [0x83]=gb_handler_add_a_r8,    [0x84]=gb_handler_add_a_r8,     [0x85]=gb_handler_add_a_r8,    [0x86]=gb_handler_add_a_r8,    [0x87]=gb_handler_add_a_r8,    [0x88]=gb_handler_adc_a_r8,     [0x89]=gb_handler_adc_a_r8,    [0x8A]=gb_handler_adc_a_r8,     [0x8B]=gb_handler_adc_a_r8,    [0x8C]=gb_handler_adc_a_r8,    [0x8D]=gb_handler_adc_a_r8,    [0x8E]=gb_handler_adc_a_r8,    [0x8F]=gb_handler_adc_a_r8,
    [0x90]=gb_handler_sub_a_r8,  [0x91]=gb_handler_sub_a_r8,    [0x92]=gb_handler_sub_a_r8,    [0x93]=gb_handler_sub_a_r8,    [0x94]=gb_handler_sub_a_r8,     [0x95]=gb_handler_sub_a_r8,    [0x96]=gb_handler_sub_a_r8,    [0x97]=gb_handler_sub_a_r8,    [0x98]=gb_handler_sbc_a_r8,     [0x99]=gb_handler_sbc_a_r8,    [0x9A]=gb_handler_sbc_a_r8,     [0x9B]=gb_handler_sbc_a_r8,    [0x9C]=gb_handler_sbc_a_r8,    [0x9D]=gb_handler_sbc_a_r8,    [0x9E]=gb_handler_sbc_a_r8,    [0x9F]=gb_handler_sbc_a_r8,
    [0xA0]=gb_handler_and_a_r8,  [0xA1]=gb_handler_and_a_r8,    [0xA2]=gb_handler_and_a_r8,    [0xA3]=gb_handler_and_a_r8,    [0xA4]=gb_handler_and_a_r8,     [0xA5]=gb_handler_and_a_r8,    [0xA6]=gb_handler_and_a_r8,    [0xA7]=gb_handler_and_a_r8,    [0xA8]=gb_handler_xor_a_r8,     [0xA9]=gb_handler_xor_a_r8,    [0xAA]=gb_handler_xor_a_r8,     [0xAB]=gb_handler_xor_a_r8,    [0xAC]=gb_handler_xor_a_r8,    [0xAD]=gb_handler_xor_a_r8,    [0xAE]=gb_handler_xor_a_r8,    [0xAF]=gb_handler_xor_a_r8,
    [0xB0]=gb_handler_or_a_r8,   [0xB1]=gb_handler_or_a_r8,     [0xB2]=gb_handler_or_a_r8,     [0xB3]=gb_handler_or_a_r8,     [0xB4]=gb_handler_or_a_r8,      [0xB5]=gb_handler_or_a_r8,     [0xB6]=gb_handler_or_a_r8,     [0xB7]=gb_handler_or_a_r8,     [0xB8]=gb_handler_cp_a_r8,      [0xB9]=gb_handler_cp_a_r8,     [0xBA]=gb_handler_cp_a_r8,      [0xBB]=gb_handler_cp_a_r8,     [0xBC]=gb_handler_cp_a_r8,     [0xBD]=gb_handler_cp_a_r8,     [0xBE]=gb_handler_cp_a_r8,     [0xBF]=gb_handler_cp_a_r8,
    [0xC0]=gb_handler_ret_cc,    [0xC1]=gb_handler_pop_r16,     [0xC2]=gb_handler_jp_cc_n16,   [0xC3]=gb_handler_jp_n16,      [0xC4]=gb_handler_call_cc_n16,  [0xC5]=gb_handler_push_r16,    [0xC6]=gb_handler_add_a_n8,    [0xC7]=gb_handler_rst,         [0xC8]=gb_handler_ret_cc,       [0xC9]=gb_handler_ret,         [0xCA]=gb_handler_jp_cc_n16,    [0xCB]=gb_handler_prefix,      [0xCC]=gb_handler_call_cc_n16, [0xCD]=gb_handler_call_n16,    [0xCE]=gb_handler_adc_a_n8,    [0xCF]=gb_handler_rst,
    [0xD0]=gb_handler_ret_cc,    [0xD1]=gb_handler_pop_r16,     [0xD2]=gb_handler_jp_cc_n16,   [0xD3]=0,                      [0xD4]=gb_handler_call_cc_n16,  [0xD5]=gb_handler_push_r16,    [0xD6]=gb_handler_sub_a_n8,    [0xD7]=gb_handler_rst,         [0xD8]=gb_handler_ret_cc,       [0xD9]=gb_handler_reti,        [0xDA]=gb_handler_jp_cc_n16,    [0xDB]=0,                      [0xDC]=gb_handler_call_cc_n16, [0xDD]=0,                      [0xDE]=gb_handler_sbc_a_n8,    [0xDF]=gb_handler_rst,
    [0xE0]=gb_handler_ldh_n8_a,  [0xE1]=gb_handler_pop_r16,     [0xE2]=gb_handler_ldh_c_a,     [0xE3]=0,                      [0xE4]=0,                       [0xE5]=gb_handler_push_r16,    [0xE6]=gb_handler_and_a_n8,    [0xE7]=gb_handler_rst,         [0xE8]=gb_handler_add_sp_e8,    [0xE9]=gb_handler_jp_hl,       [0xEA]=gb_handler_ld_n16mem_a,  [0xEB]=0,                      [0xEC]=0,                      [0xED]=0,                      [0xEE]=gb_handler_xor_a_n8,    [0xEF]=gb_handler_rst,
    [0xF0]=gb_handler_ldh_a_n8,  [0xF1]=gb_handler_pop_r16,     [0xF2]=gb_handler_ldh_a_c,     [0xF3]=gb_handler_di,          [0xF4]=0,                       [0xF5]=gb_handler_push_r16,    [0xF6]=gb_handler_or_a_n8,     [0xF7]=gb_handler_rst,         [0xF8]=gb_handler_ld_hl_sp_e8,  [0xF9]=gb_handler_ld_sp_hl,    [0xFA]=gb_handler_ld_a_n16mem,  [0xFB]=gb_handler_ei,          [0xFC]=0,                      [0xFD]=0,                      [0xFE]=gb_handler_cp_a_n8,     [0xFF]=gb_handler_rst,
};
 


const gb_opcode_handler_t cb_table[256] = {
    [0x00]=gb_handler_rlc_r8,    [0x01]=gb_handler_rlc_r8,    [0x02]=gb_handler_rlc_r8,    [0x03]=gb_handler_rlc_r8,    [0x04]=gb_handler_rlc_r8,    [0x05]=gb_handler_rlc_r8,    [0x06]=gb_handler_rlc_r8,    [0x07]=gb_handler_rlc_r8,    [0x08]=gb_handler_rrc_r8,    [0x09]=gb_handler_rrc_r8,    [0x0A]=gb_handler_rrc_r8,    [0x0B]=gb_handler_rrc_r8,    [0x0C]=gb_handler_rrc_r8,    [0x0D]=gb_handler_rrc_r8,    [0x0E]=gb_handler_rrc_r8,    [0x0F]=gb_handler_rrc_r8,
    [0x10]=gb_handler_rl_r8,     [0x11]=gb_handler_rl_r8,     [0x12]=gb_handler_rl_r8,     [0x13]=gb_handler_rl_r8,     [0x14]=gb_handler_rl_r8,     [0x15]=gb_handler_rl_r8,     [0x16]=gb_handler_rl_r8,     [0x17]=gb_handler_rl_r8,     [0x18]=gb_handler_rr_r8,     [0x19]=gb_handler_rr_r8,     [0x1A]=gb_handler_rr_r8,     [0x1B]=gb_handler_rr_r8,     [0x1C]=gb_handler_rr_r8,     [0x1D]=gb_handler_rr_r8,     [0x1E]=gb_handler_rr_r8,     [0x1F]=gb_handler_rr_r8,
    [0x20]=gb_handler_sla_r8,    [0x21]=gb_handler_sla_r8,    [0x22]=gb_handler_sla_r8,    [0x23]=gb_handler_sla_r8,    [0x24]=gb_handler_sla_r8,    [0x25]=gb_handler_sla_r8,    [0x26]=gb_handler_sla_r8,    [0x27]=gb_handler_sla_r8,    [0x28]=gb_handler_sra_r8,    [0x29]=gb_handler_sra_r8,    [0x2A]=gb_handler_sra_r8,    [0x2B]=gb_handler_sra_r8,    [0x2C]=gb_handler_sra_r8,    [0x2D]=gb_handler_sra_r8,    [0x2E]=gb_handler_sra_r8,    [0x2F]=gb_handler_sra_r8,
    [0x30]=gb_handler_swap_r8,   [0x31]=gb_handler_swap_r8,   [0x32]=gb_handler_swap_r8,   [0x33]=gb_handler_swap_r8,   [0x34]=gb_handler_swap_r8,   [0x35]=gb_handler_swap_r8,   [0x36]=gb_handler_swap_r8,   [0x37]=gb_handler_swap_r8,   [0x38]=gb_handler_srl_r8,    [0x39]=gb_handler_srl_r8,    [0x3A]=gb_handler_srl_r8,    [0x3B]=gb_handler_srl_r8,    [0x3C]=gb_handler_srl_r8,    [0x3D]=gb_handler_srl_r8,    [0x3E]=gb_handler_srl_r8,    [0x3F]=gb_handler_srl_r8,
    [0x40]=gb_handler_bit_u3_r8, [0x41]=gb_handler_bit_u3_r8, [0x42]=gb_handler_bit_u3_r8, [0x43]=gb_handler_bit_u3_r8, [0x44]=gb_handler_bit_u3_r8, [0x45]=gb_handler_bit_u3_r8, [0x46]=gb_handler_bit_u3_r8, [0x47]=gb_handler_bit_u3_r8, [0x48]=gb_handler_bit_u3_r8, [0x49]=gb_handler_bit_u3_r8, [0x4A]=gb_handler_bit_u3_r8, [0x4B]=gb_handler_bit_u3_r8, [0x4C]=gb_handler_bit_u3_r8, [0x4D]=gb_handler_bit_u3_r8, [0x4E]=gb_handler_bit_u3_r8, [0x4F]=gb_handler_bit_u3_r8,
    [0x50]=gb_handler_bit_u3_r8, [0x51]=gb_handler_bit_u3_r8, [0x52]=gb_handler_bit_u3_r8, [0x53]=gb_handler_bit_u3_r8, [0x54]=gb_handler_bit_u3_r8, [0x55]=gb_handler_bit_u3_r8, [0x56]=gb_handler_bit_u3_r8, [0x57]=gb_handler_bit_u3_r8, [0x58]=gb_handler_bit_u3_r8, [0x59]=gb_handler_bit_u3_r8, [0x5A]=gb_handler_bit_u3_r8, [0x5B]=gb_handler_bit_u3_r8, [0x5C]=gb_handler_bit_u3_r8, [0x5D]=gb_handler_bit_u3_r8, [0x5E]=gb_handler_bit_u3_r8, [0x5F]=gb_handler_bit_u3_r8,
    [0x60]=gb_handler_bit_u3_r8, [0x61]=gb_handler_bit_u3_r8, [0x62]=gb_handler_bit_u3_r8, [0x63]=gb_handler_bit_u3_r8, [0x64]=gb_handler_bit_u3_r8, [0x65]=gb_handler_bit_u3_r8, [0x66]=gb_handler_bit_u3_r8, [0x67]=gb_handler_bit_u3_r8, [0x68]=gb_handler_bit_u3_r8, [0x69]=gb_handler_bit_u3_r8, [0x6A]=gb_handler_bit_u3_r8, [0x6B]=gb_handler_bit_u3_r8, [0x6C]=gb_handler_bit_u3_r8, [0x6D]=gb_handler_bit_u3_r8, [0x6E]=gb_handler_bit_u3_r8, [0x6F]=gb_handler_bit_u3_r8,
    [0x70]=gb_handler_bit_u3_r8, [0x71]=gb_handler_bit_u3_r8, [0x72]=gb_handler_bit_u3_r8, [0x73]=gb_handler_bit_u3_r8, [0x74]=gb_handler_bit_u3_r8, [0x75]=gb_handler_bit_u3_r8, [0x76]=gb_handler_bit_u3_r8, [0x77]=gb_handler_bit_u3_r8, [0x78]=gb_handler_bit_u3_r8, [0x79]=gb_handler_bit_u3_r8, [0x7A]=gb_handler_bit_u3_r8, [0x7B]=gb_handler_bit_u3_r8, [0x7C]=gb_handler_bit_u3_r8, [0x7D]=gb_handler_bit_u3_r8, [0x7E]=gb_handler_bit_u3_r8, [0x7F]=gb_handler_bit_u3_r8,
    [0x80]=gb_handler_res_u3_r8, [0x81]=gb_handler_res_u3_r8, [0x82]=gb_handler_res_u3_r8, [0x83]=gb_handler_res_u3_r8, [0x84]=gb_handler_res_u3_r8, [0x85]=gb_handler_res_u3_r8, [0x86]=gb_handler_res_u3_r8, [0x87]=gb_handler_res_u3_r8, [0x88]=gb_handler_res_u3_r8, [0x89]=gb_handler_res_u3_r8, [0x8A]=gb_handler_res_u3_r8, [0x8B]=gb_handler_res_u3_r8, [0x8C]=gb_handler_res_u3_r8, [0x8D]=gb_handler_res_u3_r8, [0x8E]=gb_handler_res_u3_r8, [0x8F]=gb_handler_res_u3_r8,
    [0x90]=gb_handler_res_u3_r8, [0x91]=gb_handler_res_u3_r8, [0x92]=gb_handler_res_u3_r8, [0x93]=gb_handler_res_u3_r8, [0x94]=gb_handler_res_u3_r8, [0x95]=gb_handler_res_u3_r8, [0x96]=gb_handler_res_u3_r8, [0x97]=gb_handler_res_u3_r8, [0x98]=gb_handler_res_u3_r8, [0x99]=gb_handler_res_u3_r8, [0x9A]=gb_handler_res_u3_r8, [0x9B]=gb_handler_res_u3_r8, [0x9C]=gb_handler_res_u3_r8, [0x9D]=gb_handler_res_u3_r8, [0x9E]=gb_handler_res_u3_r8, [0x9F]=gb_handler_res_u3_r8,
    [0xA0]=gb_handler_res_u3_r8, [0xA1]=gb_handler_res_u3_r8, [0xA2]=gb_handler_res_u3_r8, [0xA3]=gb_handler_res_u3_r8, [0xA4]=gb_handler_res_u3_r8, [0xA5]=gb_handler_res_u3_r8, [0xA6]=gb_handler_res_u3_r8, [0xA7]=gb_handler_res_u3_r8, [0xA8]=gb_handler_res_u3_r8, [0xA9]=gb_handler_res_u3_r8, [0xAA]=gb_handler_res_u3_r8, [0xAB]=gb_handler_res_u3_r8, [0xAC]=gb_handler_res_u3_r8, [0xAD]=gb_handler_res_u3_r8, [0xAE]=gb_handler_res_u3_r8, [0xAF]=gb_handler_res_u3_r8,
    [0xB0]=gb_handler_res_u3_r8, [0xB1]=gb_handler_res_u3_r8, [0xB2]=gb_handler_res_u3_r8, [0xB3]=gb_handler_res_u3_r8, [0xB4]=gb_handler_res_u3_r8, [0xB5]=gb_handler_res_u3_r8, [0xB6]=gb_handler_res_u3_r8, [0xB7]=gb_handler_res_u3_r8, [0xB8]=gb_handler_res_u3_r8, [0xB9]=gb_handler_res_u3_r8, [0xBA]=gb_handler_res_u3_r8, [0xBB]=gb_handler_res_u3_r8, [0xBC]=gb_handler_res_u3_r8, [0xBD]=gb_handler_res_u3_r8, [0xBE]=gb_handler_res_u3_r8, [0xBF]=gb_handler_res_u3_r8,
    [0xC0]=gb_handler_set_u3_r8, [0xC1]=gb_handler_set_u3_r8, [0xC2]=gb_handler_set_u3_r8, [0xC3]=gb_handler_set_u3_r8, [0xC4]=gb_handler_set_u3_r8, [0xC5]=gb_handler_set_u3_r8, [0xC6]=gb_handler_set_u3_r8, [0xC7]=gb_handler_set_u3_r8, [0xC8]=gb_handler_set_u3_r8, [0xC9]=gb_handler_set_u3_r8, [0xCA]=gb_handler_set_u3_r8, [0xCB]=gb_handler_set_u3_r8, [0xCC]=gb_handler_set_u3_r8, [0xCD]=gb_handler_set_u3_r8, [0xCE]=gb_handler_set_u3_r8, [0xCF]=gb_handler_set_u3_r8,
    [0xD0]=gb_handler_set_u3_r8, [0xD1]=gb_handler_set_u3_r8, [0xD2]=gb_handler_set_u3_r8, [0xD3]=gb_handler_set_u3_r8, [0xD4]=gb_handler_set_u3_r8, [0xD5]=gb_handler_set_u3_r8, [0xD6]=gb_handler_set_u3_r8, [0xD7]=gb_handler_set_u3_r8, [0xD8]=gb_handler_set_u3_r8, [0xD9]=gb_handler_set_u3_r8, [0xDA]=gb_handler_set_u3_r8, [0xDB]=gb_handler_set_u3_r8, [0xDC]=gb_handler_set_u3_r8, [0xDD]=gb_handler_set_u3_r8, [0xDE]=gb_handler_set_u3_r8, [0xDF]=gb_handler_set_u3_r8,
    [0xE0]=gb_handler_set_u3_r8, [0xE1]=gb_handler_set_u3_r8, [0xE2]=gb_handler_set_u3_r8, [0xE3]=gb_handler_set_u3_r8, [0xE4]=gb_handler_set_u3_r8, [0xE5]=gb_handler_set_u3_r8, [0xE6]=gb_handler_set_u3_r8, [0xE7]=gb_handler_set_u3_r8, [0xE8]=gb_handler_set_u3_r8, [0xE9]=gb_handler_set_u3_r8, [0xEA]=gb_handler_set_u3_r8, [0xEB]=gb_handler_set_u3_r8, [0xEC]=gb_handler_set_u3_r8, [0xED]=gb_handler_set_u3_r8, [0xEE]=gb_handler_set_u3_r8, [0xEF]=gb_handler_set_u3_r8,
    [0xF0]=gb_handler_set_u3_r8, [0xF1]=gb_handler_set_u3_r8, [0xF2]=gb_handler_set_u3_r8, [0xF3]=gb_handler_set_u3_r8, [0xF4]=gb_handler_set_u3_r8, [0xF5]=gb_handler_set_u3_r8, [0xF6]=gb_handler_set_u3_r8, [0xF7]=gb_handler_set_u3_r8, [0xF8]=gb_handler_set_u3_r8, [0xF9]=gb_handler_set_u3_r8, [0xFA]=gb_handler_set_u3_r8, [0xFB]=gb_handler_set_u3_r8, [0xFC]=gb_handler_set_u3_r8, [0xFD]=gb_handler_set_u3_r8, [0xFE]=gb_handler_set_u3_r8, [0xFF]=gb_handler_set_u3_r8,
};


 uint8_t read_r8(gb_t *gb, uint8_t idx) {
    switch (idx) {
        case 0: return gb->cpu.BC.high; break;
        case 1: return gb->cpu.BC.low; break;
        case 2: return gb->cpu.DE.high; break;
        case 3: return gb->cpu.DE.low; break;
        case 4: return gb->cpu.HL.high; break;
        case 5: return gb->cpu.HL.low; break;
        case 6: return mem_read(gb, gb->cpu.HL.word);
        case 7: return gb->cpu.AF.high; break;
        default: return 0; 
    }
 }

 void write_r8(gb_t *gb, uint8_t idx, uint8_t value) {
    switch (idx) {
        case 0:  gb->cpu.BC.high = value; break;
        case 1:  gb->cpu.BC.low = value; break;
        case 2:  gb->cpu.DE.high = value; break;
        case 3:  gb->cpu.DE.low = value; break;
        case 4:  gb->cpu.HL.high = value; break;
        case 5:  gb->cpu.HL.low = value; break;
        case 6:  mem_write(gb, gb->cpu.HL.word, value); break;
        case 7:  gb->cpu.AF.high = value; break;
    }
 }

 uint16_t read_r16(gb_t *gb, uint8_t idx) {
   switch(idx) {
      case 0: return gb->cpu.BC.word; break;
      case 1: return gb->cpu.DE.word; break;
      case 2: return gb->cpu.HL.word; break;
      case 3: return gb->cpu.SP; break;
      default: return 0xFFFF;
   }
 }

 void write_r16(gb_t *gb, uint8_t idx, uint16_t value) {
   switch(idx) {
      case 0: gb->cpu.BC.word = value; break;
      case 1: gb->cpu.DE.word = value; break;
      case 2: gb->cpu.HL.word = value; break;
      case 3: gb->cpu.SP = value; break;
      default: return;
   }
 }

uint16_t read_r16mem(gb_t *gb, uint8_t idx) {
   switch(idx) {
      case 0: return gb->cpu.BC.word;
      case 1: return gb->cpu.DE.word;
      case 2: return gb->cpu.HL.word++;
      case 3: return gb->cpu.HL.word--;
      default: return 0xFFFF;
   }
 }

 uint16_t read_r16_stk(gb_t *gb, uint8_t idx) {
   switch(idx) {
      case 0: return gb->cpu.BC.word; break;
      case 1: return gb->cpu.DE.word; break;
      case 2: return gb->cpu.HL.word; break;
      case 3: return gb->cpu.AF.word; break;
      default: return 0xFFFF;
   }
 }

 void write_r16_stk(gb_t *gb, uint8_t idx, uint16_t value) {
   switch(idx) {
      case 0: gb->cpu.BC.word = value; break;
      case 1: gb->cpu.DE.word = value; break;
      case 2: gb->cpu.HL.word = value; break;
      case 3: gb->cpu.AF.word = value; break;
      default: return;
   }
 }

 uint8_t fetch(gb_t *gb) {
   return mem_read(gb, gb->cpu.PC++);
 }

 uint16_t fetch16(gb_t *gb) {
   uint8_t low = fetch(gb);
   uint8_t high = fetch(gb);
   return ((uint16_t)high << 8) | low;
 }

 uint16_t stack_pop(gb_t *gb) {
   uint8_t low = mem_read(gb, gb->cpu.SP++);
   uint8_t high = mem_read(gb, gb->cpu.SP++);

   return ((uint16_t)high << 8) | low;
 }

 void stack_push(gb_t *gb, uint16_t val) {
   mem_write(gb, --gb->cpu.SP, (val >> 8) & 0xFF);
   mem_write(gb, --gb->cpu.SP, val & 0xFF);
 }

 void cpu_tick(gb_t *gb) {
   //process interupts

   //set interupts if ime_pending == 1
   if(gb->cpu.ime_pending) gb->cpu.ime = true;

   // Fetch the next opcode.
   uint8_t opcode = fetch(gb);

   //process task
   gb_opcode_handler_t handler = uc_table[gb->cpu.IR];
   if(handler) handler(gb);
 }
