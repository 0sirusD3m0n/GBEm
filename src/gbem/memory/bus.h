
#include <stdint.h>

#include "gbem/gb.h"

#define GB_WRAM_START 0xC000
#define GB_WRAM_END   0xDFFF
#define GB_HRAM_START 0xFF80
#define GB_HRAM_END   0xFFFE

uint8_t mem_read(gb_t * gb, uint16_t addr);