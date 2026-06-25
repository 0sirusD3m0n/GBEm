#include <stdint.h>

typedef struct gb_t gb_t;

typedef struct {
    uint8_t y_pos;
    uint8_t x_pos;
    uint8_t index;
    union {
        uint8_t flags;
        struct {
            uint8_t : 4;
            uint8_t dmb_palette : 1;
            uint8_t x_flip : 1;
            uint8_t y_flip : 1;
            uint8_t priority : 1;
        };
    };
} gb_oam_data_t;

typedef struct {
    uint8_t vram[8192];
    uint8_t frame_buffer[160 * 144];
    gb_oam_data_t oam[40];
    gb_oam_data_t oam_buffer[10];
    uint8_t LY;
    uint8_t LYC;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t WX;
    uint8_t WY;
    uint16_t ppu_dots;
    uint8_t oam_index;
    uint8_t oam_buffer_count;
    uint8_t oam_dots;
    uint8_t oam_index;
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

void ppu_tick(gb_t *gb, uint8_t m_cycles);
void ppu_init(gb_ppu_t *ppu);