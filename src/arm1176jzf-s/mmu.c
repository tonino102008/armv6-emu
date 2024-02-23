#include "mmu.h"

#include "stdlib.h"
#include "math.h"

MMU_TLB TLB;

void MMU_TLB_ctor() {
    TLB.size = TLB_SIZE;
    TLB.actual_size = 0;
    TLB.entry = malloc(TLB.size * sizeof(MMU_TLB_Entry));
    MMU_TLB_flush_all();
}

void MMU_TLB_dtor() {
    free(TLB.entry);
}

void MMU_TLB_flush_entry(byte i) {
    TLB.entry[i] = (MMU_TLB_Entry){ .ASID = 0,
                                    .global = 0,
                                    .mem_propr = 0,
                                    .mva = 0,
                                    .pa = 0,
                                    .page_size = 0};
}

void MMU_TLB_flush_all() {
    for (byte i = 0; i < TLB.size; i++) {
        MMU_TLB_flush_entry(i);
    }
}

void MMU_TLB_check_fault(word mva) {
    if (checkAlignment()) {
        if (!isAligned()) return;
    }
    getFisrtLevelDescriptor();
    if (isExternalAbort()) return;
    if (isDescriptorFault()) return;
    if (isSection()) {

    } else {
        checkDomain();
        if (!isAccessType()) return;
        if (canAccess())         
    }
}

// ASID fetched from coproc[reg 13]
word MMU_TLB_search(word mva, byte ASID) {
    for (byte i = 0; i < TLB.actual_size; i++) {
        byte N = (byte)log2(TLB.entry[i].page_size);
        // check access with permission and domain bits otherwise memory abort
        // return physical address
        if ((mva >> N) == (TLB.entry[i].mva >> N) && 
            (TLB.entry[i].global || ASID == TLB.entry[i].ASID)) return TLB.entry[i].pa;
    }
    // return hardware translation table walk
    word ttbr = 0; // fetch from coprocessor CP15 reg 2
    byte X = 0; // fetch from coprocessor CP15 reg 2 - ttbr type 0 ? 1
    return MMU_PTW_first_level_fetch(ttbr, mva, X);
}

word MMU_PTW_first_level_fetch(word ttbr, word mva, byte X) {
    // if TTBR1 -> X = 0 if TTBR0 -> X = N_TTBR0
    // return address of first level descriptor
    word addr_fld =  (ttbr >> (14 - X)) << (14 - X) | (((mva << X) >> (20 + X)) & ((word)0 << (11 - X))) << 2 & 0b00;
    // perform fetch at addr_fld
    word fld;
    if ((fld & 0b11) == 0b10) {
        // SECTION / SUPERSECTIONS
        // check access permission bits
        // return physical address
        return (fld >>  20) << 20 | (mva << 11) >> 11;
    }
    if ((fld & 0b11) == 0b01) {
        // COARSE PAGE TABLE
        // check access permission bits
        // return pointer address for second level fetch
        word addr_sld =  (fld >>  10) << 10 | (mva << 12) >> 22 & 0b00;
    }
    if ((fld & 0b11) == 0b11) {
        // FINE PAGE TABLE
        // check access permission bits
        // return pointer address for second level fetch
        word addr_sld =  (fld >>  12) << 12 | (mva << 12) >> 22 & 0b00;
    }
}

word MMU_PTW_second_level_fetch(word addr_sld, word mva) {
    // perform fetch at addr_sld
    word sld;
    if ((sld & 0b11) == 0b01) {
        // Large Page
        // check access permission bits
        // return physical address
        return (sld >> 16) << 16 | (mva << 16) >> 16;
    }
    if ((sld & 0b11) == 0b10) {
        // Small Page
        // check access permission bits
        // return physical address
        return (sld >> 12) << 12 | (mva << 20) >> 20;
    }
    if ((sld & 0b11) == 0b11) {
        // Tiny Page
        // check access permission bits
        // return physical address
        return (sld >> 10) << 10 | (mva << 22) >> 22;
    }
}

void MMU_access_memory(word mva, word ASID) {
    // lookup ASID, Secure-NonSecure, virtual address in Data/Instruction Micro TLB
    // if fail, look in main TLB
    // if fail, translation table walk and the TLB insertion
    MMU_TLB_search(mva, ASID);
}