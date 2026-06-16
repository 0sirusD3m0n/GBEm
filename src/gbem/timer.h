#include <stdint.h>

typedef struct gb_t gb_t;

typedef struct {
    uint8_t div;
    uint8_t tima;
    uint8_t tma;
    union {
        uint8_t tac;
        struct {
            uint8_t select : 2;
            uint8_t enable : 1;
            uint8_t        : 5;
        } tac_bits;
    };
    uint16_t total_cycles;
    uint16_t clock;
    uint8_t div_clock;
} gb_timer_t;

uint8_t timer_read(gb_t *gb, uint16_t addr);
void timer_write(gb_t *gb, uint16_t addr, uint8_t value);

void timer_tick(gb_t *gb, uint8_t cycles);
void timer_init(gb_t *gb);