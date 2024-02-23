#ifndef DCACHE_H_
#define DCACHE_H_

#include "types.h"

#define NSETS_DCACHE 512 // 512 sets to have cache size of 16KB
#define NWAYS_DCACHE 4 // nways -> lines per set
#define NWORDS_PER_LINE_DCACHE 8 // 8 words per line
#define NO_ENTRY_WRITE_BUFFER 3

#define REP_STRAT_DCACHE 1 // Replacement strategy for cache lines - 0 : Random, 1 : Round Robin
#define WRITE_METHOD_DCACHE 1 // Write method for cache lines - 0 : Write Through, 1 : Write_back
// Cache miss: read-allocate -> allocate in main memory only when read/load not when write/store

typedef struct {
    byte nwords;
    word* line;
    word tag;
    byte validity;
    byte dirty;
    byte NS_tag;
} ARM_DCache_Line;

typedef struct {
    byte nways;
    ARM_DCache_Line* lines;
} ARM_DCache_Set;

typedef struct {
    byte no_entries;
    word data[NO_ENTRY_WRITE_BUFFER];
    word address[NO_ENTRY_WRITE_BUFFER];
} ARM_DWrite_Buffer;

typedef struct
{
    byte nsets;
    ARM_DCache_Set* cache;
    ARM_DWrite_Buffer* write_buffer;
} ARM_DCache;

extern ARM_DCache* dcache;

void ARM_DCache_ctor();
void ARM_DCache_dtor();

byte dcache_read(word*, word*); // determine cache hit/miss
void dcache_write(word*);
void dcache_handle_miss();
void dcache_replace();

#endif // DCACHE_H_