#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gbem/gb.h"
#include "gbem/cpu/cpu.h"
#include "gbem/memory/bus.h"
#include "cJSON.h"

void run_test(gb_t *gb, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        fprintf(stderr, "Error opening file %s", filename);
        return 2;
    }

    fseek(f, 0, SEEK_END);
    const long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(file_size + 1);
    buffer[file_size] = '\0';
    fread(buffer, 1, file_size, f);

    fclose(f);

    cJSON *root = cJSON_Parse(buffer);
}

int main(int, char**){
    
}
