
#include "handlers.h"
#include "gbem/gb.h"

uint8_t gb_handler_nop(gb_t *gb) {
    return 1;
}

uint8_t gb_handler_halt(gb_t *gb) {
    gb->cpu.halted = true;
    return 1;
}

uint8_t gb_handler_stop(gb_t *gb) {
       gb->cpu.stopped = true;
       return 1;
}

uint8_t gb_handler_ei(gb_t *gb) {
    gb->cpu.ime_pending = true;
    return 1;
}

uint8_t gb_handler_di(gb_t *gb) {
    gb->cpu.ime = false;
    gb->cpu.ime_pending = false;
    return 1;
}

uint8_t gb_handler_prefix(gb_t *gb) {
    gb->cpu.IR = fetch(gb);
    return cb_table[gb->cpu.IR](gb);
}