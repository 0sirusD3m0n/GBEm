
#include <stdint.h>
#include "gbem/gb.h"
#include "gbem/memory/bus.h"

#define GB_WRAM_SIZE 0x2000
#define GB_HRAM_SIZE 0x7F

typedef struct {
    uint8_t wram[GB_WRAM_SIZE];
    uint8_t hram[GB_HRAM_SIZE];
} gb_ram_t;

uint8_t ram_wram_read(gb_ram_t * ram, uint16_t addr);