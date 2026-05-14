/**
 * gb.h
 * 
 * Header file for the GB CPU. 
 * Includes declarations for public functions and variables
 * 
 * @author T. Aldridge
 * @version 0.10
 * @date 05/13/2026
 */


 #ifndef GB_CPU_FILE
 #define GB_CPU_FILE

 #include <stdint.h>

 #define CPU_CLOCK_SPEED_MHZ 4.194304
 #define HORIZONTAL_SYNC_KHZ 9.198
 #define VERTICAL_SYNC_MHZ   59.73
 
 #define NUM_REGISTERS 10
 
 typedef struct
 {
    uint8_t* registers;
    uint8_t IR;  // Instruction register
    uint8_t IE;  // Interupt Register
    uint16_t PC; // Program counter
    uint16_t SP; // Stack pointer
 } GB_CPU;
 
  #endif