#ifndef GB_HANDLERS_H
#define GB_HANDLERS_H

#include <stdint.h>
#include <gbem/gb.h>
#include <gbem/cpu/cpu.h>

//misc
uint8_t gb_handler_nop(gb_t *gb);
uint8_t gb_handler_halt(gb_t *gb);
uint8_t gb_handler_stop(gb_t *gb);
uint8_t gb_handler_di(gb_t *gb);
uint8_t gb_handler_ei(gb_t *gb);
uint8_t gb_handler_prefix(gb_t *gb);

//alu
uint8_t gb_handler_inc_r16(gb_t *gb);
uint8_t gb_handler_dec_r16(gb_t *gb);
uint8_t gb_handler_add_hl_r16(gb_t *gb);
uint8_t gb_handler_inc_r8(gb_t *gb);
uint8_t gb_handler_dec_r8(gb_t * gb);
uint8_t gb_handler_add_a_r8(gb_t *gb);
uint8_t gb_handler_adc_a_r8(gb_t *gb);
uint8_t gb_handler_sub_a_r8(gb_t *gb);
uint8_t gb_handler_sbc_a_r8(gb_t *gb);
uint8_t gb_handler_and_a_r8(gb_t *gb);
uint8_t gb_handler_xor_a_r8(gb_t *gb);
uint8_t gb_handler_or_a_r8(gb_t *gb);
uint8_t gb_handler_cp_a_r8(gb_t *gb);
uint8_t gb_handler_add_a_n8(gb_t *gb);
uint8_t gb_handler_adc_a_n8(gb_t *gb);
uint8_t gb_handler_sub_a_n8(gb_t *gb);
uint8_t gb_handler_sbc_a_n8(gb_t *gb);
uint8_t gb_handler_and_a_n8(gb_t *gb);
uint8_t gb_handler_xor_a_n8(gb_t *gb);
uint8_t gb_handler_or_a_n8(gb_t *gb);
uint8_t gb_handler_cp_a_n8(gb_t *gb);
uint8_t gb_handler_add_sp_e8(gb_t *gb);
uint8_t gb_handler_daa(gb_t *gb);
uint8_t gb_handler_cpl(gb_t *gb);
uint8_t gb_handler_scf(gb_t *gb);
uint8_t gb_handler_ccf(gb_t *gb);

//ld/rd
uint8_t gb_handler_ld_r8_r8(gb_t *gb);
uint8_t gb_handler_ld_r8_n8(gb_t *gb);
uint8_t gb_handler_ld_r16_n16(gb_t *gb);
uint8_t gb_handler_ld_r16mem_a(gb_t *gb);
uint8_t gb_handler_ld_a_r16mem(gb_t *gb);
uint8_t gb_handler_ld_n16mem_sp(gb_t *gb);
uint8_t gb_handler_ldh_c_a(gb_t *gb);
uint8_t gb_handler_ldh_n8_a(gb_t *gb);
uint8_t gb_handler_ld_n16mem_a(gb_t *gb);
uint8_t gb_handler_ldh_a_c(gb_t *gb);
uint8_t gb_handler_ldh_a_n8(gb_t *gb);
uint8_t gb_handler_ld_a_n16mem(gb_t *gb);
uint8_t gb_handler_ld_hl_sp_e8(gb_t *gb);
uint8_t gb_handler_ld_sp_hl(gb_t *gb);
uint8_t gb_handler_pop_r16(gb_t *gb);
uint8_t gb_handler_push_r16(gb_t *gb);

//control
uint8_t gb_handler_jr_n8(gb_t *gb);
uint8_t gb_handler_jr_cc_n8(gb_t *gb);
uint8_t gb_handler_ret_cc(gb_t *gb);
uint8_t gb_handler_ret(gb_t *gb);
uint8_t gb_handler_reti(gb_t *gb);
uint8_t gb_handler_jp_cc_n16(gb_t *gb);
uint8_t gb_handler_jp_n16(gb_t *gb);
uint8_t gb_handler_jp_hl(gb_t *gb);
uint8_t gb_handler_call_cc_n16(gb_t *gb);
uint8_t gb_handler_call_n16(gb_t *gb);
uint8_t gb_handler_rst(gb_t *gb);

//bit
uint8_t gb_handler_rlca(gb_t *gb);
uint8_t gb_handler_rrca(gb_t *gb);
uint8_t gb_handler_rla(gb_t *gb);
uint8_t gb_handler_rra(gb_t *gb);
uint8_t gb_handler_rlc_r8(gb_t *gb);
uint8_t gb_handler_rrc_r8(gb_t *gb);
uint8_t gb_handler_rl_r8(gb_t *gb);
uint8_t gb_handler_rr_r8(gb_t *gb);
uint8_t gb_handler_sla_r8(gb_t *gb);
uint8_t gb_handler_sra_r8(gb_t *gb);
uint8_t gb_handler_swap_r8(gb_t *gb);
uint8_t gb_handler_srl_r8(gb_t *gb);
uint8_t gb_handler_bit_u3_r8(gb_t *gb);
uint8_t gb_handler_res_u3_r8(gb_t *gb);
uint8_t gb_handler_set_u3_r8(gb_t *gb);
#endif