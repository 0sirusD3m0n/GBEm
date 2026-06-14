/**
 * cart.h
 * Contains declarations for cart data and methods
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/16/2026
 */
#ifndef GBEM_CART_H
#define GBEM_CART_H

#include <stdint.h>
#include <stdbool.h>
#include "gbem/gb.h"

typedef struct gb_t gb_t;

typedef enum {
    GB_MBC_TYPE_NONE,
    GB_MBC_TYPE_MBC1,
    GB_MBC_TYPE_MBC2,
    GB_MBC_TYPE_MBC3,
    GB_MBC_TYPE_MBC5
} gb_mbc_type_e;

typedef struct {
    uint8_t *rom;
    uint8_t *ram;
    uint32_t rom_size;
    uint32_t ram_size;
    gb_mbc_type_e mbc_type;

    // MBC state (bank registers etc)
    uint16_t rom_bank;
    uint8_t ram_bank;
    bool ram_enabled;
} gb_cart_t;

void cart_init(gb_t * gb, const char* filename);
void cart_ram_read(gb_t *gb, uint16_t addr);

#endif