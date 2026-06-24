#include <stdint.h>


typedef struct {
    uint8_t vram[8192];
    uint8_t oam[160];
    union {
        uint8_t lcdc;
        struct {
            uint8_t bg_window_enable : 1;
            uint8_t obj_enable : 1;
            uint8_t obj_size : 1;
            uint8_t bg_tile_map : 1;
            uint8_t bfg_window_tiles : 1;
            uint8_t window_enable : 1;
            uint8_t window_tile_map : 1;
            uint8_t ppu_enable : 1;
        };
    };
} gb_ppu_t;