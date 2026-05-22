/**
 * cpu.c
 * 
 * contains function definitions
 * for gbem_cpu
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/16/2026
 */
 #include "cpu.h"
 #include "handlers/handlers.h"

 
 static const gb_opcode_handler_t uc_table[256]  = {
    [0x00] = gb_handler_nop,
    [0x10] = gb_handler_stop,
    [0x67] = gb_handler_halt,
 };



 uint8_t read_r8(gb_t *gb, uint8_t idx) {
    switch (idx) {
        case 0: return gb->cpu.BC.high; break;
        case 1: return gb->cpu.BC.low; break;
        case 2: return gb->cpu.DE.high; break;
        case 3: return gb->cpu.DE.low; break;
        case 4: return gb->cpu.HL.high; break;
        case 5: return gb->cpu.HL.low; break;
        case 6: return mem_read(gb, gb->cpu.HL.word);
        case 7: return gb->cpu.AF.high; break;
        default: return 0; 
    }
 }

 void write_r8(gb_t *gb, uint8_t idx, uint8_t value) {
    switch (idx) {
        case 0:  gb->cpu.BC.high = value; break;
        case 1:  gb->cpu.BC.low = value; break;
        case 2:  gb->cpu.DE.high = value; break;
        case 3:  gb->cpu.DE.low = value; break;
        case 4:  gb->cpu.HL.high = value; break;
        case 5:  gb->cpu.HL.low = value; break;
        case 6:  mem_write(gb, gb->cpu.HL.word, value);
        case 7:  gb->cpu.AF.high = value; break;
    }
 }

 uint16_t read_r16(gb_t *gb, uint8_t idx) {
   switch(idx) {
      case 0: return gb->cpu.BC.word; break;
      case 1: return gb->cpu.DE.word; break;
      case 2: return gb->cpu.HL.word; break;
      case 3: return gb->cpu.SP; break;
      default: return 0xFFFF;
   }
 }

 void write_r16(gb_t *gb, uint8_t idx, uint16_t value) {
   switch(idx) {
      case 0: gb->cpu.BC.word = value; break;
      case 1: gb->cpu.DE.word = value; break;
      case 2: gb->cpu.HL.word = value; break;
      case 3: gb->cpu.SP = value; break;
      default: return;
   }
 }
uint16_t read_r16mem(gb_t *gb, uint8_t idx) {
   switch(idx) {
      case 0: return gb->cpu.BC.word;
      case 1: return gb->cpu.DE.word;
      case 2: return gb->cpu.HL.word++;
      case 3: return gb->cpu.HL.word--;
      default: return 0xFFFF;
   }
 }
 uint8_t fetch(gb_t *gb) {
   return mem_read(gb, gb->cpu.PC++);
 }

 uint16_t fetch16(gb_t *gb) {
   uint8_t low = fetch(gb);
   uint8_t high = fetch(gb);
   return ((uint16_t)high << 8) | low;
 }
