/**
 * cpu.h
 * 
 * Contains declarations for cpu architecture
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/14/2026
 */

 #ifndef GBEM_CPU_H
 #define GBEM_CPU_H
 
 #include <stdint.h>
 #include <stdbool.h>

typedef struct gb_t gb_t;

 #define GB_CPU_CLOCK_HZ        4194304
 #define GB_CPU_COCCK_M_HZ      1048576
 #define GB_M_CYCLES_PER_FRAME  17556

 #define GB_REGISTER_A 0x07
 #define GB_REGISTER_B 0x00
 #define GB_REGISTER_C 0x01
 #define GB_REGISTER_D 0x02
 #define GB_REGISTER_E 0x03
 #define GB_REGISTER_H 0x04
 #define GB_REGISTER_L 0x05
 #define GB_REGISTER_HL_MEM 0x06
 
 #define GB_REGISTER16_BC 0x00
 #define GB_REGISTER16_DE 0x01
 #define GB_REGISTER16_HL 0x02
 #define GB_REGISTER16_SP 0x03

 #define GB_INTERRUPT_VBLANK 0x40
 #define GB_INTERRUPT_STAT   0x48
 #define GB_INTERRUPT_TIMER  0x50
 #define GB_INTERRUPT_SERIAL 0x58
 #define GB_INTERRUPT_JP     0x60

 typedef union {
    uint16_t word;
    struct {
        union {
            uint8_t low;
            struct {
                uint8_t            : 4; // padding
                uint8_t carry      : 1;
                uint8_t half_carry : 1;
                uint8_t negative   : 1;
                uint8_t zero       : 1;
            };
        };
    uint8_t high;    
    };
 } gb_register_t;

  typedef struct
 {
    gb_register_t AF;
    gb_register_t BC;
    gb_register_t DE;
    gb_register_t HL;
    uint8_t IR;  // Instruction register
    uint16_t PC; // Program counter
    uint16_t SP; // Stack pointer
    bool ime; //interupt register
    bool halted; // check halted status
    bool stopped;
    bool ime_pending;
 } gb_cpu_t;

 typedef uint8_t (*gb_opcode_handler_t)(gb_t *gb);

 extern const gb_opcode_handler_t cb_table[256];

 // Function declaration
 uint8_t fetch (gb_t *gb);
 uint16_t fetch16(gb_t *gb);
 uint8_t cpu_tick(gb_t *gb);
 void write_r8(gb_t *gb, uint8_t idx, uint8_t value);
 uint8_t read_r8(gb_t *gb, uint8_t idx);
 uint16_t read_r16(gb_t *gb, uint8_t idx);
 uint16_t read_r16mem(gb_t *gb, uint8_t idx);
 void write_r16(gb_t *gb, uint8_t idx, uint16_t value);
 uint16_t read_r16_stk(gb_t *gb, uint8_t idx);
 void write_r16_stk(gb_t *gb, uint8_t idx, uint16_t value);
 void stack_push(gb_t *gb, uint16_t val);
 uint16_t stack_pop(gb_t *gb);

 #endif