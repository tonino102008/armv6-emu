#ifndef RAM_H_
#define RAM_H_

#include "types.h"

#define RAM_SIZE (1024 * 512)/sizeof(word)

#define OUT_OF_BOUNDARY()                                                               \
        do {                                                                            \
            printf("Trying to access physical address out of memory boundaries.\n");    \
            exit(1);                                                                    \
        } while(0)

typedef struct {
    word start_addr;
    word size;
    byte is_alloc;
} ARM_RAM_Chunk_Stats;

typedef struct {
    word* memory;
    word base_addr;
    word tot_size;
    word curr_alloc_size;
    word stats_size;
    ARM_RAM_Chunk_Stats* stats;
} ARM_RAM;

extern ARM_RAM* ram;

void ARM_RAM_ctor();
void ARM_RAM_dtor();

word* ARM_RAM_allocate(word);
void ARM_RAM_deallocate(word*);
byte ARM_RAM_alloc_write(word, word);
word ARM_RAM_alloc_read(word);
void ARM_RAM_simple_write(word, word);
word ARM_RAM_simple_read(word);

#endif // RAM_H_