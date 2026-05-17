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
 #include "gbem/cpu/cpu.h"
 #include "gbem/memory/ram.h"
 #include "gbem/cart/cart.h"
 
 /* TODO: MOVE TO PPU WHEN FINISHED
    #define HORIZONTAL_SYNC_KHZ 9.198
    #define VERTICAL_SYNC_MHZ   59.73
 */

 
 typedef struct {
    gb_cpu_t cpu;
    gb_ram_t ram;
    gb_cart_t cart;
 } gb_t;

typedef enum {
    GB_OKAY = 0,
    GB_ERROR_FILE_NOT_FOUND,
    GB_ERROR_OUT_OF_MEMORY,
    GB_ERROR_INVALID_ROM,
    GB_ERROR_UNSUPPORTED_MBC,
    GB_ERROR_INVALID_CHECKSUM,
} gb_error_e;

gb_error_e gb_init(gb_t *gb);
  #endif