#include "ram.h"

#include "stdlib.h"

ARM_RAM* ram;

void ARM_RAM_ctor() {
    ram = malloc(sizeof(ARM_RAM));
    ram->curr_alloc_size = 0;
    ram->tot_size = RAM_SIZE;
    ram->memory = malloc(ram->tot_size * sizeof(word));
    ram->base_addr = &ram->memory;
    ram->stats_size = 0;
    ram->stats = malloc(sizeof(ARM_RAM_Chunk_Stats));
}

void ARM_RAM_dtor() {
    free(ram->memory);
    free(ram->stats);
    free(ram);
}

word* ARM_RAM_allocate(word size) {
    if (ram->curr_alloc_size + size > ram->tot_size) return NULL;
    ram->stats_size += 1;
    ram->stats = realloc(ram->stats, ram->stats_size * sizeof(ARM_RAM_Chunk_Stats));
    ram->stats[ram->stats_size - 1].is_alloc = 1;
    ram->stats[ram->stats_size - 1].size = size;
    ram->stats[ram->stats_size - 1].start_addr = ram->base_addr + ram->curr_alloc_size;
    ram->curr_alloc_size += ram->stats[ram->stats_size - 1].size;
    return &ram->stats[ram->stats_size - 1].start_addr;
}

void ARM_RAM_deallocate(word* ptr) {
    // find start_addr == ptr and then free mem block
    return;
}

byte ARM_RAM_alloc_write(word pa, word val) {
    for (word i = 0; i < ram->stats_size; i++) {
        if (ram->stats[i].start_addr <= pa && 
            (ram->stats[i].start_addr + ram->stats[i].size) >= pa) {
                ram->memory[pa] = val;
                return (byte)1;
            }
    }
    return (byte)0;
}

word ARM_RAM_alloc_read(word pa) {
    for (word i = 0; i < ram->stats_size; i++) {
        if (ram->stats[i].start_addr <= pa && 
            (ram->stats[i].start_addr + ram->stats[i].size) >= pa) return ram->memory[pa];
    }
    return (word)-1;
}

/**
 * @brief Simple unchcecked ram access for write.
 * 
 * @param pa Physical Address - starting from 0.
 * @param val Value to be written at the Physical Address.
 */
void ARM_RAM_simple_write(word pa, word val) {
    if (pa < 0 && pa > ram->tot_size - sizeof(word)) OUT_OF_BOUNDARY();
    ram->memory[pa + ram->base_addr] = val;
}

/**
 * @brief Simple unchcecked ram access for read.
 * 
 * @param pa Physical Address - starting from 0.
 * @return word Value read at the Physical Address.
 */
word ARM_RAM_simple_read(word pa) {
    if (pa < 0 && pa > ram->tot_size - sizeof(word)) OUT_OF_BOUNDARY();
    return ram->memory[pa + ram->base_addr];
}