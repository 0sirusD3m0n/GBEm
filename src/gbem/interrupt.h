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

