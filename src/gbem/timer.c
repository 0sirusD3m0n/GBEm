

#include "gbem/gb.h"
#include "timer.h"

#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07

#define DIV_OVER 64

void timer_tick(gb_t *gb, uint8_t cycles) {
    gb->timer.total_cycles += cycles;
    if(gb->timer.tac_bits.enable && gb->timer.total_cycles >= gb->timer.clock) {
        gb->timer.total_cycles = 0;
        if(++gb->timer.tima == 0x00) {
            gb->interrupts.if_flags.timer = 1;
            gb->timer.tima = gb->timer.tma;
        }
    }

    gb->timer.div_clock += cycles;
    if (gb->timer.div_clock >= DIV_OVER) { gb->timer.div++; gb->timer.div_clock = 0; }
}

static void tac_write(gb_t *gb, uint8_t value) {
    gb->timer.tac = value;
    switch(gb->timer.tac_bits.select) {
        case 0x00: gb->timer.clock = 256; break;
        case 0x01: gb->timer.clock = 4; break;
        case 0x02: gb->timer.clock = 16; break;
        case 0x03: gb->timer.clock = 64; break;
    }
}

uint8_t timer_read(gb_t *gb, uint16_t addr) {
    switch(addr) {
        case DIV:  return gb->timer.div;
        case TIMA: return gb->timer.tima;
        case TMA:  return gb->timer.tma;
        case TAC:  return gb->timer.tac;
        default:   return 0xFF;
    }
}

void timer_write(gb_t *gb, uint16_t addr, uint8_t value) {
    switch(addr) {
        case DIV:  gb->timer.div = 0; break;
        case TIMA: gb->timer.tima = value; break;
        case TMA:  gb->timer.tma = value; break;
        case TAC:  tac_write(gb, value); break;
    }
}

void timer_init(gb_t *gb) { 
    gb->timer.clock = 256;
}
