#include "gbem/gb.h"

uint8_t interrupt_read_if(gb_t *gb) {
    return gb->interrupts.IF;
}

uint8_t interrupt_read_ie(gb_t *gb) {
    return gb->interrupts.IE;
}

void interrupt_write_if(gb_t *gb, uint8_t value) {
    gb->interrupts.IF = value;
}

void interrupt_write_ie(gb_t *gb, uint8_t value) {
    gb->interrupts.IE = value;
}