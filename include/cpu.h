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

 #endif