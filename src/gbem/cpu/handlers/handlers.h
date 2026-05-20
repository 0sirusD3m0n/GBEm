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

//control

//bit
#endif