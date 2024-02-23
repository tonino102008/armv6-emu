#ifndef MMU_H_
#define MMU_H_

#include "types.h"
#include "fcse.h"

#define NO_DOMAINS 16

#define SHAREABLE 0
#define CACHEABLE 0
#define BUFFERABLE 0

#define TLB_SIZE 10
#define REP_STRAT_TLB 1 // Replacement strategy for cache lines - 0 : Random, 1 : Round Robin

typedef enum {
    SUPERSECTION = 1 << 24,
    SECTION = 1 << 20,
    LARGE_PAGE = 1 << 16,
    SMALL_PAGE = 1 << 12,
    TINY_PAGE = 1 << 10
} MMU_Mem_Blocks;

typedef enum {
    NO_ACCESS,
    CLIENT,
    RESERVED,
    MANAGER
} MMU_Mem_Dom_Acc_Permission;

typedef enum {
    NO_ACCESS,
    READ_ONLY,
    READ_WRITE
} MMU_Mem_Acc_Type;

typedef enum {
    STRONGLY_ORDERED,
    DEVICE,
    NORMAL
} MMU_Mem_Type;

typedef struct {
    word mva;
    byte page_size;
    word pa;
    byte mem_propr;
    byte ASID;
    byte global;
} MMU_TLB_Entry;

typedef struct {
    byte size;
    byte actual_size;
    MMU_TLB_Entry* entry;
} MMU_TLB;

extern MMU_TLB TLB;

void MMU_TLB_ctor();
void MMU_TLB_dtor();

void MMU_TLB_flush_entry(byte);
void MMU_TLB_flush_all();
void MMU_TLB_check_fault(word);
word MMU_TLB_search(word, byte);
word MMU_page_table_walk(word, word, word);
word MMU_TLB_insert(word, byte);
void MMU_access_memory(word, word);

#endif // MMU_H_