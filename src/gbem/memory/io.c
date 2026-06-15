

#include <stdint.h>
#include "gbem/gb.h"
#include "gbem/interrupt.h"
#include "io.h"

uint8_t io_read(gb_t *gb, uint16_t addr) {
    if(addr == GB_JP_REG) return 0xFF; //TODO; Joypad io
    else if (addr >= GB_SERIAL_START && addr <= GB_SERIAL_END) return 0xFF; //TODO: Serial port
    else if(addr >= GB_TIMER_RANGE_START && addr <= GB_TIMER_RANGE_END) return                 0xFF; //TODO: Timer register read/writes
    else if(addr == GB_IF_REG)                                          return                 interrupt_read_if(gb);
    else if (addr >= GB_AUDIO_START && addr <= GB_AUDIO_END) return 0xFF; //TODO: Audio
    else if (addr >= GB_WAVE_START && addr <= GB_WAVE_END) return 0xFF; //TODO: Audio
    else if (addr >= GB_PPU_START && addr <= GB_PPU_END) return 0xFF; //TODO: PPU
    else if (addr == GB_BOOT_REG) return 0xFF; //TODO: Boot rom
    return 0xFF;
}

void io_write(gb_t *gb, uint16_t addr, uint8_t value) {
    if(addr == GB_JP_REG) return; //TODO; Joypad io
    else if (addr >= GB_SERIAL_START && addr <= GB_SERIAL_END) return; //TODO: Serial port
    else if(addr >= GB_TIMER_RANGE_START && addr <= GB_TIMER_RANGE_END) return; //TODO: Timer register read/writes
    else if(addr == GB_IF_REG) { interrupt_write_if(gb, value); return; }
    else if (addr >= GB_AUDIO_START && addr <= GB_AUDIO_END) return; //TODO: Audio
    else if (addr >= GB_WAVE_START && addr <= GB_WAVE_END) return; //TODO: Audio
    else if (addr >= GB_PPU_START && addr <= GB_PPU_END) return; //TODO: PPU
    else if (addr == GB_BOOT_REG) return; //TODO: Boot rom
    return;
}