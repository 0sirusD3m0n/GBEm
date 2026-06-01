#ifndef GB_HANDLERS_H
#define GB_HANDLERS_H

#include <stdint.h>
#include <gbem/gb.h>
#include <gbem/cpu/cpu.h>

//misc
uint8_t gb_handler_nop(gb_t *gb);
uint8_t gb_handler_halt(gb_t *gb);
uint8_t gb_handler_stop(gb_t *gb);
uint8_t gb_handler_ei(gb_t *gb);
uint8_t gb_handler_prefix(gb_t *gb);

//alu
uint8_t gb_handler_adc_r8(gb_t *gb);

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

//bit


#endif