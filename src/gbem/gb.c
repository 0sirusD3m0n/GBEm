/**
 * gb.c
 * Contains the function definitons
 * and "class" objects for the GB CPU
 */

 #include <string.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include "gbem/gb.h"

 static uint8_t* registers;

 /**
  * GB_INIT(cpu)
  * 
  * Initalizes the CPU. 
  * 
  * Allocates memory for the CPU object and the register array
  * 
  */
 void GB_INIT(gb_t* gb) {
    memset(gb, 0, sizeof(gb_t));
    timer_init(gb);
 }

 void GB_DESTROY(gb_cpu_t* cpu)  {
    free(cpu);
 }