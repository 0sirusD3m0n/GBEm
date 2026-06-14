
#ifndef GBEM_BUS_H
#define GBEM_BUS_H
#include <stdint.h>

typedef struct gb_t gb_t;

#define GB_CART_ROM_START   0x0000
#define GB_CART_ROM_END     0x7FFF
#define GB_VRAM_START       0x8000
#define GB_VRAM_END         0x9FFF
#define GB_CART_RAM_START   0xA000
#define GB_CART_RAM_END     0xBFFF
#define GB_WRAM_START       0xC000
#define GB_WRAM_END         0xDFFF
#define GB_ECHO_START       0xE000
#define GB_ECHO_END         0xFDFF
#define GB_OAM_START        0xFE00
#define GB_OAM_END          0xFE9F
#define GB_IO_REG_START     0xFF00
#define GB_IO_REG_END       0xFF7F
#define GB_HRAM_START       0xFF80
#define GB_HRAM_END         0xFFFE
#define GB_IF_REG           0xFF0F
#define GB_IE_REG           0xFFFF

#define GB_ECHO_OFFSET      0x2000

uint8_t mem_read(gb_t * gb, uint16_t addr);
void mem_write(gb_t *gb, uint16_t addr, uint8_t value);
#endif