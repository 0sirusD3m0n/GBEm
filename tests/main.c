#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gbem/gb.h"
#include "gbem/cpu/cpu.h"
#include "gbem/memory/bus.h"
#include "cJSON.h"



bool run_test(gb_t *gb, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        fprintf(stderr, "Error opening file %s", filename);
        return false;
    }

    fseek(f, 0, SEEK_END);
    const long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(file_size + 1);
    buffer[file_size] = '\0';
    fread(buffer, 1, file_size, f);

    fclose(f);

    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    cJSON *tests;
    cJSON_ArrayForEach(tests, root) {
        memset(gb, 0, sizeof(gb_t));
        bool pass = true;
        const char *name = cJSON_GetObjectItem(tests, "name")->valuestring;
        cJSON *initial = cJSON_GetObjectItem(tests, "initial");
        cJSON *final   = cJSON_GetObjectItem(tests, "final");

        gb->cpu.PC       = (uint16_t)cJSON_GetObjectItem(initial, "pc")->valueint;
        gb->cpu.SP       = (uint16_t)cJSON_GetObjectItem(initial, "sp")->valueint;    
        gb->cpu.AF.high  = (uint8_t)cJSON_GetObjectItem(initial, "a")->valueint;
        gb->cpu.AF.low   = (uint8_t)cJSON_GetObjectItem(initial, "f")->valueint;
        gb->cpu.BC.high  = (uint8_t)cJSON_GetObjectItem(initial, "b")->valueint;
        gb->cpu.BC.low   = (uint8_t)cJSON_GetObjectItem(initial, "c")->valueint;
        gb->cpu.DE.high  = (uint8_t)cJSON_GetObjectItem(initial, "d")->valueint;
        gb->cpu.DE.low   = (uint8_t)cJSON_GetObjectItem(initial, "e")->valueint;
        gb->cpu.HL.high  = (uint8_t)cJSON_GetObjectItem(initial, "h")->valueint;
        gb->cpu.HL.low   = (uint8_t)cJSON_GetObjectItem(initial, "l")->valueint;
        gb->cpu.ime      = (uint8_t)cJSON_GetObjectItem(initial, "ime")->valueint;
        gb->interrupts.IE = (uint8_t)cJSON_GetObjectItem(initial, "ie")->valueint;

        cJSON *mem = cJSON_GetObjectItem(initial, "ram");
        cJSON *entry;

        cJSON_ArrayForEach(entry, mem) {
            uint16_t addr = (uint16_t)cJSON_GetArrayItem(entry, 0)->valueint;
            uint8_t  data = (uint8_t)cJSON_GetArrayItem(entry, 1)->valueint;
            mem_write(gb, addr, data);
        }

        cpu_tick(gb);

        const char *result = "PASS";
       
        if (gb->cpu.PC != cJSON_GetObjectItem(final, "pc")->valueint) { 
            printf("Failed at: PC. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "pc")->valueint, gb->cpu.PC);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.SP != cJSON_GetObjectItem(final, "sp")->valueint) { 
            printf("Failed at: SP. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "sp")->valueint, gb->cpu.SP);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.AF.high != cJSON_GetObjectItem(final, "a")->valueint) { 
            printf("Failed at: A. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "a")->valueint, gb->cpu.AF.high);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.AF.low  != cJSON_GetObjectItem(final, "f")->valueint) { 
            printf("Failed at: F. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "f")->valueint, gb->cpu.AF.low);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.BC.high != cJSON_GetObjectItem(final, "b")->valueint) { 
            printf("Failed at: B. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "b")->valueint, gb->cpu.BC.high);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.BC.low  != cJSON_GetObjectItem(final, "c")->valueint) { 
            printf("Failed at: C. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "c")->valueint, gb->cpu.BC.low);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.DE.high != cJSON_GetObjectItem(final, "d")->valueint) { 
            printf("Failed at: D. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "d")->valueint, gb->cpu.DE.high);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.DE.low  != cJSON_GetObjectItem(final, "e")->valueint) { 
            printf("Failed at: E. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "e")->valueint, gb->cpu.DE.low);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.HL.high != cJSON_GetObjectItem(final, "h")->valueint) { 
            printf("Failed at: H. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "h")->valueint, gb->cpu.HL.high);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.HL.low  != cJSON_GetObjectItem(final, "l")->valueint) { 
            printf("Failed at: L. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "l")->valueint, gb->cpu.HL.low);
            result = "FAIL"; 
            pass = false; 
        }
        if (gb->cpu.ime     != cJSON_GetObjectItem(final, "ime")->valueint) { 
            printf("Failed at: IME. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "ime")->valueint, gb->cpu.ime);
            result = "FAIL"; 
            pass = false; 
        }
        cJSON *final_ie = cJSON_GetObjectItem(final, "ie");
        if (final_ie && gb->interrupts.IE != final_ie->valueint) { 
            printf("Failed at: IE. Expected: %d. Got: %d\n", cJSON_GetObjectItem(final, "ie")->valueint, gb->interrupts.IE);
            result = "FAIL"; 
            pass = false; 
        }

        cJSON *final_mem = cJSON_GetObjectItem(final, "ram");
        cJSON *item;
        cJSON_ArrayForEach(item, final_mem) {
            uint16_t addr = cJSON_GetArrayItem(item, 0)->valueint;
            uint8_t data = cJSON_GetArrayItem(item, 1)->valueint;
            if (mem_read(gb, addr) != data) {
                printf("Failed at: Memory addr %d. Expected: %d. Got: %d\n", addr, data, mem_read(gb, addr));
                result = "FAIL";
                pass = false;
            }
        }
        printf("%s: %s\n", name, result);
        if(!pass) { 
            cJSON_Delete(root); 
            return false; }
    }
    cJSON_Delete(root);
    return true;
}

int main(int, char**){
    gb_t gb;
    char filename[64];
    for (int idx = 0x00; idx <= 0xFF; idx++) {
        if (idx == 0xCB) continue;
        if (idx == 0xD3 || idx == 0xDB || idx == 0xDD ||
            idx == 0xE3 || idx == 0xE4 || idx == 0xEB ||
            idx == 0xEC || idx == 0xED || idx == 0xF4 ||
            idx == 0xFC || idx == 0xFD || idx == 0xCB) continue;
        snprintf(filename, sizeof(filename), TEST_DATA_PATH"%02x.json", idx);
        if(!run_test(&gb, filename)) { 
            return 1;
        }
    }

    for (int idx = 0x00; idx <= 0xFF; idx++) {
        snprintf(filename, sizeof(filename), TEST_DATA_PATH"cb %02x.json", idx);
        if(!run_test(&gb, filename)) { 
            return 1;
        }
    }
    return 0;
}
