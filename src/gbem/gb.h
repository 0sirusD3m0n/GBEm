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
 #include <gbem/cpu/cpu.h>

 
 #define HORIZONTAL_SYNC_KHZ 9.198
 #define VERTICAL_SYNC_MHZ   59.73
 
 typedef struct {
    gb_cpu_t cpu;
 } gb_t;
  #endif