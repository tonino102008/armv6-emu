#include "ram.h"

#include "stdlib.h"

ARM_Ram* ram;

void ARM_Ram_ctor() {
    ram = malloc(sizeof(ARM_Ram));
    ram->size = RAM_SIZE;
    ram->memory = malloc(ram->size * sizeof(word));
}

void ARM_Ram_dtor() {
    free(ram->memory);
    free(ram);
}