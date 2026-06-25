#include <stdint.h>

typedef enum {
    mode_0,
    mode_1,
    mode_2,
    mode_3,
} mode;

typedef struct {
    uint8_t vram[8192];
    uint8_t oam[160];
    uint8_t LY;
    uint8_t LYC;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t WX;
    uint8_t WY;
    uint8_t ppu_dots;
    mode mode;
    union {
        uint8_t bg_pallet_data;
        struct {
            uint8_t id_0 : 2;
            uint8_t id_1 : 2;
            uint8_t id_2 : 2; 
            uint8_t id_3 : 2;
        };
    };
    union {
        uint8_t obp0;
        struct {
            uint8_t : 2;
            uint8_t id_1 : 2;
            uint8_t id_2 : 2; 
            uint8_t id_3 : 2;
        };
    };
    union {
        uint8_t obp1;
        struct {
            uint8_t : 2;
            uint8_t id_1 : 2;
            uint8_t id_2 : 2; 
            uint8_t id_3 : 2;
        };
    };
    union {
        uint8_t stat;
        struct {
            uint8_t ppu_mode : 2;
            uint8_t lyc_lc : 1;
            uint8_t mode0_select : 1;
            uint8_t mode1_select : 1;
            uint8_t mode2_select : 1;
            uint8_t lyc_int_select : 1;
        };
    };
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