/**
 * bus.c
 * Contains the function definitons
 * and for the bus
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/16/2026
 */

#include "gbem/gb.h"
#include "gbem/interrupt.h"
#include "bus.h"
#include "io.h"

#ifdef TESTING 
static uint8_t test_ram[0x10000];
#endif

uint8_t mem_read(gb_t * gb, uint16_t addr) {
    #ifdef TESTING
    return test_ram[addr];
    #endif

    if (addr <= GB_CART_ROM_END) return                                         0xFF; //TODO: Update to rom_read in cart.c
    else if (addr >= GB_VRAM_START && addr <= GB_VRAM_END) return               0xFF; // TODO: Update to ppu_vram_read() in ppu.c
    else if (addr >= GB_CART_RAM_START && addr <= GB_CART_RAM_END) return       0xFF; // TODO: Update to cart_ram_read in cart.c
    else if (addr >= GB_WRAM_START && addr <= GB_WRAM_END) { return             ram_wram_read(&gb->ram, addr); }
    else if (addr >= GB_ECHO_START && addr <= GB_ECHO_END) { return             ram_wram_read(&gb->ram, addr - GB_ECHO_OFFSET); }
    else if (addr >= GB_OAM_START && addr <= GB_OAM_END) return                 0xFF; // TODO: Update to oam_read in ppu.c
    else if (addr >= GB_IO_REG_START && addr <= GB_IO_REG_END) return           io_read(gb, addr); // TODO: Update to io_read in joypad.c 
    else if (addr >= GB_HRAM_START && addr <= GB_HRAM_END) { return             ram_hram_read(&gb->ram, addr);  }
    else if (addr == GB_IE_REG) return                                          interrupt_read_ie(gb);
    else return 0xFF; // unusable region 0xFEA0-0xFEFF and any other unmapped addresses
    }

void mem_write(gb_t *gb, uint16_t addr, uint8_t value) {
    #ifdef TESTING
    test_ram[addr] = value;
    return;
    #endif

    if (addr <= GB_CART_ROM_END) return; //TODO: Update to mbc_write in cart.c
    else if (addr >= GB_VRAM_START && addr <= GB_VRAM_END) return; // TODO: Update to ppu_vram_write in cart.c
    else if (addr >= GB_CART_RAM_START && addr <= GB_CART_RAM_END) return; // TODO: Update to cart_ram_write in cart.c
    else if (addr >= GB_WRAM_START && addr <= GB_WRAM_END)  { ram_wram_write(&gb->ram, addr, value); return; }
    else if (addr >= GB_ECHO_START && addr <= GB_ECHO_END) { ram_wram_write(&gb->ram, addr - GB_ECHO_OFFSET, value); return; }
    else if (addr >= GB_OAM_START && addr <= GB_OAM_END) return; // TODO: Update to oam_write in ppu.c
    else if (addr >= GB_IO_REG_START && addr <= GB_IO_REG_END) { io_write(gb, addr, value); return; } // TODO: Update to io_write in joypad.c 
    else if (addr >= GB_HRAM_START && addr <= GB_HRAM_END) { ram_hram_write(&gb->ram, addr, value); return; }
    else if (addr == GB_IE_REG) { interrupt_write_ie(gb, value); return; }
    else return; 
}