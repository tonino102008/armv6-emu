#ifndef MMU_H_
#define MMU_H_

#include "types.h"
#include "coproc_register.h"
#include "exception.h"
#include "fcse.h"
#include "ram.h"

#define NO_DOMAINS 16

#define SHAREABLE 0
#define CACHEABLE 0
#define BUFFERABLE 0

#define TLB_SIZE 10
#define REP_STRAT_TLB 1 // Replacement strategy for TLB - 0 : Random, 1 : Round Robin

// SUBPAGES ENABLED, FINE PAGE TABLES SUPPORT

#define UNSUPPORTED_DOMAIN_ACCESS()                 \
        do {                                        \
            printf("Unsupported domain access.\n"); \
            exit(1);                                \
        } while(0)

#define UNSUPPORTED_PERMISSION()                    \
        do {                                        \
            printf("Unsupported permission.\n");    \
            exit(1);                                \
        } while(0)

#define UNSUPPORTED_TABLE_DIMENSION()                   \
        do {                                            \
            printf("Unsupported table dimension.\n");   \
            exit(1);                                    \
        } while(0)

#define UNSUPPORTED_TRANSLATION()                   \
        do {                                        \
            printf("Unsupported translation.\n");   \
            exit(1);                                \
        } while(0)

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

typedef struct {
    word address;
    byte success;
} MMU_Physical_Address;

extern MMU_TLB TLB;

void MMU_TLB_ctor();
void MMU_TLB_dtor();
void MMU_TLB_flush_entry(byte);
void MMU_TLB_flush_all();
void MMU_reset();

void MMU_TLB_replacement();
void MMU_page_table_translation(word, MMU_Physical_Address**);
void MMU_TLB_search(word, byte, MMU_Physical_Address**);
MMU_Physical_Address MMU_page_table_walk(word, word, word);
MMU_Physical_Address MMU_TLB_insert(word, byte);
MMU_Physical_Address MMU_access_memory(word, word);
word MMU_fetch_fld(word);
word MMU_fetch_sld(word, word);
word MMU_fld_to_pa_section(word, word);
word MMU_sld_to_pa_section(word, word);

byte check_alignment();
byte is_aligned(word);
byte is_external_abort();
byte is_fld_descriptor_fault(word);
byte is_sld_descriptor_fault(word);
byte is_section(word);
byte check_domain(word);
MMU_Mem_Dom_Acc_Permission is_access_type(byte);
byte check_fld_access_permission(word);
byte check_sld_access_permission(word);
MMU_Mem_Acc_Type can_access(byte, byte);

#endif // MMU_H_