#include "dcache.h"

#include "stdlib.h"

ARM_DCache* dcache;

void ARM_DCache_ctor() {
    dcache = malloc(sizeof(ARM_DCache));
    dcache->nsets = NSETS_DCACHE;
    dcache->cache = malloc(dcache->nsets * sizeof(ARM_DCache_Set));
    for (byte i = 0; i < dcache->nsets; i++) {
        dcache->cache[i].nways = NWAYS_DCACHE;
        dcache->cache[i].lines = malloc(dcache->cache[i].nways * sizeof(ARM_DCache_Line));
        for (byte j = 0; j < dcache->cache[i].nways; j++) {
            dcache->cache[i].lines[j].nwords = NWORDS_PER_LINE_DCACHE;
            dcache->cache[i].lines[j].dirty = 0;
            dcache->cache[i].lines[j].NS_tag = 0;
            dcache->cache[i].lines[j].validity = 0;
            dcache->cache[i].lines[j].tag = 0;
            dcache->cache[i].lines[j].line = calloc(dcache->cache[i].lines[j].nwords, sizeof(word));
        }
    }
    dcache->write_buffer = malloc(sizeof(ARM_DWrite_Buffer));
    dcache->write_buffer->no_entries = NO_ENTRY_WRITE_BUFFER;
}

void ARM_DCache_dtor() {
    free(dcache->write_buffer);
    for (byte i = 0; i < dcache->nsets; i++) {
        for (byte j = 0; j < dcache->cache[i].nways; j++) {
            free(dcache->cache[i].lines[j].line);
        }
        free(dcache->cache[i].lines);
    }
    free(dcache->cache);
    free(dcache);
}

byte dcache_read(word* address, word* data) {
    byte hit = 0; // 1 = hit, 0 = miss
    for (byte i = 0; i < dcache->nsets; i++) {
        for (byte j = 0; j < dcache->cache[i].nways; j++) {
            if (dcache->cache[i].lines[j].tag == *address) {
                data = dcache->cache[i].lines[j].line[0];
                hit = 1;
                return hit;
            }
        }
    }

    // Miss case handling


    return hit;
}

void dcache_write(word*) {

}