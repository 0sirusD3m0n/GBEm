/**
 * gb.c
 * Contains the function definitons
 * and "class" objects for the GB CPU
 */

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
 void GB_INIT(gbem_cpu_t* cpu) {
    cpu = malloc(sizeof(gbem_cpu_t));

 }

 void GB_DESTROY(gbem_cpu_t* cpu)  {
    free(cpu);
 }