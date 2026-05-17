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
 #include "gbem/memory/bus.h"
 #include "gbem/gb.h"

 #define GB_CPU_CLOCK_HZ        4194304
 #define GB_CPU_COCCK_M_HZ      1048576
 #define GB_M_CYCLES_PER_FRAME  17556

 typedef union {
    uint16_t word;
    struct {
        union {
            uint8_t low;
            struct {
                uint8_t            : 4; // padding
                uint8_t carry      : 1;
                uint8_t half_carry : 1;
                uint8_t sub        : 1;
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
 } gb_cpu_t;

 
 // Function declaration
 uint8_t fetch (gb_t *gb);

 #endif