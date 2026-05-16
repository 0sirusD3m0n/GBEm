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
            uint8_t high;
        };
    };
 } gbem_register_t;

 typedef enum {
    GB_OPCODE_NOP       = 0x00,
    GB_OPCODE_LD_BC_N16 = 0x01,
    GB_OPCODE_LD_BC_A   = 0x02,
    GB_OPCODE_INC_BC    = 0x03,
    GB_OPCODE_INC_B     = 0x04,
    GB_OPCODE_DEC_B     = 0x05,
    GB_OPCODE_LD_B_N8   = 0x06,
    GB_OPCODE_RLCA      = 0x07,
    GB_OPCODE_LD_A16_SP = 0x10
 } GB_OPCODE_LOOKUP;

 #endif