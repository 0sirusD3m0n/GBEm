#include "gbem/gb.h"
#include "ppu.h"

#define GB_PPU_OAM    2
#define GB_PPU_DRAW   3
#define GB_PPU_HBLANK 0
#define GB_PPU_VBLANK 1

#define GB_OAM_BUFFER_MAX 10

void ppu_draw(gb_t *gb) {
    
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