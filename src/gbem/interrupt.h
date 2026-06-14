#include "gbem/gb.h"
#include <stdint.h>



typedef struct {
    union {
        uint8_t IE;
        struct {
            uint8_t : 3;
            uint8_t joypad : 1;
            uint8_t serial : 1;
            uint8_t timer  : 1;
            uint8_t lcd    : 1;
            uint8_t vblank : 1;
        } ie_flags;
    };
    union {
        uint8_t IF;
        struct {
            uint8_t : 3;
            uint8_t joypad : 1;
            uint8_t serial : 1;
            uint8_t timer  : 1;
            uint8_t lcd    : 1;
            uint8_t vblank : 1;
        } if_flags;
    };
} gb_interrupt_t;

uint8_t interrupt_read_if(gb_t *gb);
uint8_t interrupt_read_ie(gb_t *gb);
void interrupt_write_if(gb_t *gb, uint8_t value);
void interrupt_write_ie(gb_t *gb, uint8_t value);

