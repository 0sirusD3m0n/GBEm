/**
 * gb.c
 * Contains the function definitons
 * and "class" objects for the GB CPU
 */

 #include <stdint.h>
 #include "./include/gb.h"

 static uint8_t* registers;

 /**
  * GB_INIT(cpu)
  * 
  * Initalizes the CPU. 
  * 
  * Allocates memory for the CPU object and the register array
  * 
  */
 void GB_INIT(GB_CPU* cpu) {
    cpu = malloc(sizeof(GB_CPU));

    cpu->registers = malloc (sizeof(uint8_t) * NUM_REGISTERS);
 }

 void GB_DESTROY(GB_CPU* cpu)  {
    free(cpu-registers);
    free(cpu);
 }