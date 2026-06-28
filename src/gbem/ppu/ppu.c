#include "gbem/gb.h"
#include "ppu.h"
#include <string.h>

#define GB_PPU_OAM    2
#define GB_PPU_DRAW   3
#define GB_PPU_HBLANK 0
#define GB_PPU_VBLANK 1

#define GB_OAM_BUFFER_MAX 10

#define VRAM_START 0x8000
#define OAM_START  0xFE00

void compute_y_offsets(gb_t *gb) {
    uint8_t background_y = (gb->ppu.LY + gb->ppu.SCY) % 256;
    gb->ppu.mode3_state.tile_row = background_y / 8;
    gb->ppu.mode3_state.tile_pixel_row = background_y % 8;
}

void compute_x_col(gb_t *gb) {
    gb->ppu.mode3_state.tile_col = (gb->ppu.mode3_state.fetcher_tile_x + (gb->ppu.SCX/8)) & 0x1F;
}

void advance_bg_fetch(gb_t *gb) {
    if(gb->ppu.mode3_state.fetch_dots >= 8 && gb->ppu.mode3_state.bg_fifo_count == 0) {
        
        compute_x_col(gb);

        uint16_t map_base = gb->ppu.bg_tile_map ? 0x9C00 : 0x9800;
        uint16_t tile_map_addr = map_base + ((gb->ppu.mode3_state.tile_row * 32) + gb->ppu.mode3_state.tile_col);
        uint8_t tile_index = gb->ppu.vram[tile_map_addr - VRAM_START];

        uint16_t tile_data_addr = gb->ppu.bg_window_tiles ? 0x8000 + (tile_index * 16) : 0x9000 + ((int8_t)tile_index * 16);

        uint16_t row_addr = tile_data_addr + (gb->ppu.mode3_state.tile_pixel_row * 2);
        uint8_t low_byte = gb->ppu.vram[row_addr - VRAM_START];
        uint8_t high_byte = gb->ppu.vram[row_addr - VRAM_START + 1];
        
        for(uint8_t i = 0; i < 8; i++) {
            uint8_t bit_position = 7 - i;
            uint8_t low_bit = (low_byte >> bit_position) & 0x01;
            uint8_t high_bit = (high_byte >> bit_position) & 0x01;

            gb->ppu.mode3_state.bg_fifo[i] = (high_bit << 1) | low_bit;
        }
        gb->ppu.mode3_state.bg_fifo_count = 8;
        gb->ppu.mode3_state.fetch_dots = 0;
        gb->ppu.mode3_state.fetcher_tile_x++;
        return;
    }
    gb->ppu.mode3_state.fetch_dots++;
}

void render_pixel(gb_t *gb) {

}

void ppu_draw(gb_t *gb) {
    switch(gb->ppu.mode3_state.state) {
        case INIT:
        //Set all of mode3 state to 0. This effectively resets the state on entering mode 3, since we don't need anything from before
        memset(&gb->ppu.mode3_state, 0, sizeof(gb->ppu.mode3_state));
        compute_y_offsets(gb);
        if(gb->ppu.mode3_state.fetch_dots < 8) advance_bg_fetch(gb);
        //perform check for objects and update state
        break;

        case FETCH_RENDER:
        advance_bg_fetch(gb);
        break;

        case OBJECT_FETCH:

        break;
    }
}

void ppu_tick(gb_t *gb, uint8_t m_cycles) {
    for(uint8_t i = 1; i <= m_cycles * 4; i++) {
        switch(gb->ppu.ppu_mode) {
            case GB_PPU_OAM:
                //check dma one byte at a time. Fill object buffer if y value = ly
            if((gb->ppu.oam_dots % 2 == 0) && gb->ppu.oam_buffer_count < GB_OAM_BUFFER_MAX) {
                uint8_t height = gb->ppu.obj_size ? 16 : 8;
                gb_oam_data_t object = gb->ppu.oam[gb->ppu.oam_index++];
                if(gb->ppu.LY >= (object.y_pos - 16) && gb->ppu.LY < (object.y_pos - 16 + height)) {
                    gb->ppu.oam_buffer[gb->ppu.oam_buffer_count++] = object;
                }
            }
            
            //update ppu dot tracker
            gb->ppu.oam_dots++;
            //when at end of line (80 dots) move to mode 3
            if(gb->ppu.oam_dots == 80) {
                gb->ppu.ppu_mode = GB_PPU_DRAW;
            }
            break;

            case GB_PPU_DRAW:
                ppu_draw(gb);
                gb->ppu.ppu_dots++;
            break;

            case GB_PPU_HBLANK:
                gb->ppu.ppu_dots++;
                if (gb->ppu.ppu_dots == 456) {
                    gb->ppu.ppu_dots = 0;
                    gb->ppu.LY++;
                    if (gb->ppu.LY == 144) {
                        gb->ppu.ppu_mode = GB_PPU_VBLANK;
                        gb->interrupts.if_flags.vblank = 1;
                    } else {
                        gb->ppu.ppu_mode = GB_PPU_OAM;
                        gb->ppu.oam_dots = 0;
                        gb->ppu.oam_index = 0;
                        gb->ppu.oam_buffer_count = 0;
                    }
                }
                break;

            case GB_PPU_VBLANK:
                gb->ppu.ppu_dots++;
                if (gb->ppu.ppu_dots == 456) {
                    gb->ppu.ppu_dots = 0;
                    gb->ppu.LY++;
                    if (gb->ppu.LY == 154) {
                        gb->ppu.LY = 0;
                        gb->ppu.ppu_mode = GB_PPU_OAM;
                        gb->ppu.oam_dots = 0;
                        gb->ppu.oam_index = 0;
                        gb->ppu.oam_buffer_count = 0;
                    }
                }
                break;
                    }
    }

}

void ppu_init(gb_ppu_t *ppu) {
    ppu->ppu_dots = 0;
    ppu->oam_dots = 0;
    ppu->oam_index = 0;
    ppu->oam_buffer_count = 0;
    ppu->ppu_mode = GB_PPU_OAM;
}

uint8_t vram_read(gb_t *gb, uint16_t addr) {
    if(gb->ppu.ppu_mode == GB_PPU_DRAW) return 0xFF;
    return gb->ppu.vram[addr - VRAM_START];
}

void vram_write(gb_t *gb, uint16_t addr, uint8_t value) {
    if(gb->ppu.ppu_mode == GB_PPU_DRAW) return;
    gb->ppu.vram[addr - VRAM_START] = value;
}

uint8_t oam_read(gb_t *gb, uint16_t addr) {
    if(gb->ppu.ppu_mode == GB_PPU_OAM || gb->ppu.ppu_mode == GB_PPU_DRAW) return 0xFF;
    
    uint16_t byte_offset = addr - OAM_START;
    uint8_t sprite_index = byte_offset / 4;
    uint8_t byte_data = byte_offset % 4;

    gb_oam_data_t *data = &gb->ppu.oam[sprite_index];

    switch(byte_data) {
        case 0: return data->y_pos;
        case 1: return data->x_pos;
        case 2: return data->index;
        case 3: return data->flags;
        default: return 0xFF;
    }
}

void oam_write(gb_t *gb, uint16_t addr, uint8_t value) {
    if(gb->ppu.ppu_mode == GB_PPU_OAM || gb->ppu.ppu_mode == GB_PPU_DRAW) return;
    
    uint16_t byte_offset = addr - OAM_START;
    uint8_t sprite_index = byte_offset / 4;
    uint8_t byte_data = byte_offset % 4;

    gb_oam_data_t *data = &gb->ppu.oam[sprite_index];

    switch(byte_data) {
        case 0: data->y_pos = value; break;
        case 1: data->x_pos = value; break;
        case 2: data->index = value; break;
        case 3: data->flags = value; break;
        default: return;
    }
}

uint8_t ppu_register_read(gb_t *gb, uint16_t addr) {
    switch(addr) {
        case 0xFF40: return gb->ppu.lcdc;
        case 0xFF41: return gb->ppu.stat;
        case 0xFF42: return gb->ppu.SCY;
        case 0xFF43: return gb->ppu.SCX;
        case 0xFF44: return gb->ppu.LY;
        case 0xFF45: return gb->ppu.LYC;
        case 0xFF46: return 0xFF;
        case 0xFF47: return gb->ppu.BGP;
        case 0xFF48: return gb->ppu.obp0;
        case 0xFF49: return gb->ppu.obp1;
        case 0xFF4A: return gb->ppu.WY;
        case 0xFF4B: return gb->ppu.WX;
        default: return 0xFF;
    }
}

void ppu_register_write(gb_t *gb, uint16_t addr, uint8_t value) {
        switch(addr) {
        case 0xFF40: { gb->ppu.lcdc = value; return; }
        case 0xFF41: { gb->ppu.stat = (gb->ppu.stat & 0x07) | (value & 0xF8); return; }
        case 0xFF42: { gb->ppu.SCY = value; return; }
        case 0xFF43: { gb->ppu.SCX = value; return; }
        case 0xFF44: return; // LY is RO.
        case 0xFF45: { gb->ppu.LYC = value; return; }
        case 0xFF46: { gb->ppu.DMA = value; gb->ppu.dma_active = true; gb->ppu.dma_dots = 0; return; };
        case 0xFF47: { gb->ppu.BGP = value; return; }
        case 0xFF48: { gb->ppu.obp0 = value; return; }
        case 0xFF49: { gb->ppu.obp1 = value; return; }
        case 0xFF4A: { gb->ppu.WY = value; return; }
        case 0xFF4B: { gb->ppu.WX = value; return; }
        default: return;
    }
}

