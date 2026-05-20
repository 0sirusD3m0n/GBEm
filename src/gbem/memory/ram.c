#include "ram.h"


uint8_t ram_wram_read(gb_ram_t *ram, uint16_t addr) {
    return ram->wram[addr - GB_WRAM_START];
}

uint8_t ram_hram_read(gb_ram_t *ram, uint16_t addr) {
    return ram->hram[addr - GB_HRAM_START];
}

void ram_wram_write(gb_ram_t *ram, uint16_t addr, uint8_t value) {
    ram->wram[addr - GB_WRAM_START] = value;
}

void ram_hram_write(gb_ram_t *ram, uint16_t addr, uint8_t value) {
    ram->hram[addr - GB_HRAM_START] = value;
}