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

void MMU_TLB_replacement() {

}

MMU_Physical_Address MMU_TLB_insert(word, byte) {

}

void MMU_reset() {
    MMU_Mem_Type mem_type = STRONGLY_ORDERED;
    // UNCACHABLE
    // CP15 reg 1 I = 1 -> INSTRUCTION CACHE -> CACHEABLE, NON SHAREABLE, NORMAL
    // CP15 reg 1 I = 0 -> INSTRUCTION CACHE -> NON CACHEABLE, NON SHAREABLE, NORMAL
    // WHEN MMU DISABLED: NO CHECK OF PERMISSION ATTRIBUTES, NO ABORTS, PA = MVA, FCSE PID = 0,
    // BEFORE RENABLING -> DISABLE AND INVALIDATE INSTRUCTION CACHE, THEN RENABLE WHEN MMU RENABLED
}

// ASID fetched from coproc[reg 13]
void MMU_TLB_search(word mva, byte ASID, MMU_Physical_Address** pa) {
    for (byte i = 0; i < TLB.actual_size; i++) {
        byte N = (byte)log2(TLB.entry[i].page_size);
        // check access with permission and domain bits otherwise memory abort
        // return physical address
        if ((mva >> N) == (TLB.entry[i].mva >> N) && 
            (TLB.entry[i].global || ASID == TLB.entry[i].ASID)) {
                *pa = &(MMU_Physical_Address){.address = TLB.entry[i].pa, .success = 1};
                return;
            }
    }
}

void MMU_page_table_translation(word mva, MMU_Physical_Address** pa) {
    // DEPENDING ON THE EXECUTED MEMORY INTERACTION (DATA OR INSTRUCTION) -> ABORT CAN BE: DATA OR PREFETCH
    // YOU HAVE TO MODIFY BELOW!!!
    if (check_alignment()) {
        if (!is_aligned(mva)) return handle_exception((ARM_Exception_Type)DATA_ABORT);
    }
    word fld = MMU_fetch_fld(mva);
    if (is_external_abort()) return;
    if (is_fld_fault(fld)) return handle_exception((ARM_Exception_Type)DATA_ABORT);
    if (is_section(fld)) {
        byte at = check_domain(fld);
        MMU_Mem_Dom_Acc_Permission dom_acc_type = is_access_type(at);
        if (dom_acc_type == MANAGER) {
            (*pa)->address = MMU_fld_to_pa_section(mva, fld);
            (*pa)->success = 1;
            return;
        } else if (dom_acc_type == CLIENT) {
            byte ap = check_fld_access_permission(fld);
            if (can_access(ap, 0) == NO_ACCESS) return handle_exception((ARM_Exception_Type)DATA_ABORT);
            (*pa)->address = MMU_fld_to_pa_section(mva, fld);
            (*pa)->success = 1;
            return;
        } else {
            return handle_exception((ARM_Exception_Type)DATA_ABORT);
        }
    } else {
        word sld = MMU_fetch_sld(mva, fld);
        if (is_external_abort()) return;
        if (is_sld_fault(sld)) return handle_exception((ARM_Exception_Type)DATA_ABORT);
        byte at = check_domain(fld);
        MMU_Mem_Dom_Acc_Permission dom_acc_type = is_access_type(at);
        if (dom_acc_type == MANAGER) {
            (*pa)->address = MMU_sld_to_pa_section(mva, sld);
            (*pa)->success = 1;
            return;
        } else if (dom_acc_type == CLIENT) {
            byte ap = check_sld_access_permission(sld);
            if (can_access(ap, 0) == NO_ACCESS) return handle_exception((ARM_Exception_Type)DATA_ABORT);
            (*pa)->address = MMU_sld_to_pa_section(mva, sld);
            (*pa)->success = 1;
            return;
        } else {
            return handle_exception((ARM_Exception_Type)DATA_ABORT);
        }
    }
}

word MMU_fetch_fld(word mva) {
    word N = CP15_Reg.regs[PTC][0b10] & 0b111;
    word TTBR;
    word X = N;
    if (N == 0b000) {
        TTBR = CP15_Reg.regs[PTC][0b00];
    }
    else {
        if ((mva >> (32 - N)) == 0) TTBR = CP15_Reg.regs[PTC][0b00];
        else {
            TTBR = CP15_Reg.regs[PTC][0b01];
            X = 0;
        }
    }
    word addr_fld =  (TTBR >> (14 - X)) << (14 - X) | (((mva << X) >> (20 + X)) & ((word)0 << (11 - X))) << 2 & 0b00;
    // perform fetch at addr_fld
    return ARM_RAM_simple_read(addr_fld);
}

word MMU_fetch_sld(word mva, word fld) {
    word sld_addr;
    if ((fld & 0b11) == 0b01) sld_addr =  (fld >>  10) << 10 | (mva << 12) >> 22 & 0b00; // COARSE PAGE TABLE
    else if ((fld & 0b11) == 0b11) sld_addr =  (fld >>  12) << 12 | (mva << 12) >> 22 & 0b00; // FINE PAGE TABLE
    else UNSUPPORTED_TABLE_DIMENSION();
    // perform fetch at addr_sld
    return ARM_RAM_simple_read(sld_addr);
}

word MMU_fld_to_pa_section(word mva, word fld) {
    return (fld >> 20) << 20 | (mva << 11) >> 11;
}

word MMU_sld_to_pa_page(word mva, word sld) {
    if ((sld & 0b11) == 0b01) return (sld >> 16) << 16 | (mva << 16) >> 16; // Large Page
    else if ((sld & 0b11) == 0b10) return (sld >> 12) << 12 | (mva << 20) >> 20; // Small Page
    else if ((sld & 0b11) == 0b11) return (sld >> 10) << 10 | (mva << 22) >> 22; // Tiny Page
    else UNSUPPORTED_TRANSLATION();
}

MMU_Physical_Address MMU_access_memory(word mva, word ASID) {
    // lookup ASID, Secure-NonSecure, virtual address in Data/Instruction Micro TLB
    // if fail, look in main TLB
    // if fail, translation table walk and the TLB insertion

    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    byte E = CPSR_bits.E; // SHOULD BE LITTLE ENDIAN
    byte B = (CP15_Reg.regs[SC_BITS][0b000] >> 7) & 0b1; // SHOULD BE LITTLE ENDIAN
    byte U = (CP15_Reg.regs[SC_BITS][0b000] >> 22) & 0b1; // SHOULD BE UNALIGNED MIXED ENDIAN

    // switch(U) { // MemoryAccess(B-bit, E-bit) to detemine load/store in Little/Big Endian
    //     switch (B) {
    //         switch (E) {

    //         }
    //     }
    // }

    MMU_Physical_Address* pa = malloc(sizeof(MMU_Physical_Address));
    MMU_TLB_search(mva, ASID, &pa);
    if (pa->success) return *pa;
    MMU_page_table_translation(mva, &pa);
    if (pa->success) return *pa;
    else (MMU_Physical_Address){.address = -1, .success = 0};
}

byte check_alignment() {
    return (*CP15_Reg.regs[SC_BITS] & 0b1) == 0b1;
}

byte is_aligned(word mva) {
    return (mva & 0b11) == 0b00;
}

byte is_external_abort() {
    return 0;
}

byte is_fld_fault(word fld) {
    return ((fld & 0b11) == 0b00 || (fld & 0b11) == 0b11) ? 1 : 0;
}

byte is_sld_fault(word sld) {
    return (sld & 0b11) == 0b00;
}

byte is_section(word fld) {
    return (fld & 0b11) == 0b10;
}

byte check_domain(word fld) {
    byte domain = (fld >> 5) & 0xF;
    return (*CP15_Reg.regs[DAC] >> (domain * 2)) & 0b11;
}

MMU_Mem_Dom_Acc_Permission is_access_type(byte at) {
    switch (at) {
        case 0b00:
            return NO_ACCESS;
        case 0b01:
            return CLIENT;
        case 0b10:
            return RESERVED;
        case 0b11:
            return MANAGER;
        default:
            UNSUPPORTED_DOMAIN_ACCESS();
    }
}

byte check_fld_access_permission(word fld) {
    return (fld >> 10) & 0b11;
}

byte check_sld_access_permission(word sld) {
    return (sld >> 4) & 0b11;
}

MMU_Mem_Acc_Type can_access(byte ap, byte apx) {
    byte S = (CP15_Reg.regs[SC_BITS][0b000] >> 8) & 0b1;
    byte R = (CP15_Reg.regs[SC_BITS][0b000] >> 9) & 0b1;
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);
    switch (ap) {
        case 0b00:
            switch (apx) {
                case 0b0:
                    switch (R) {
                        case 0b0:
                            switch (S) {
                                case 0b0:
                                    return NO_ACCESS;
                                    break;
                                case 0b1:
                                    if (c_m == USER) return NO_ACCESS;
                                    return READ_ONLY;
                                    break;
                                default:
                                    UNSUPPORTED_PERMISSION();
                            }
                            break;
                        case 0b1:
                            switch (S) {
                                case 0b0:
                                    return READ_ONLY;
                                    break;
                                default:
                                    UNSUPPORTED_PERMISSION();
                            }
                            break;
                        default:
                            UNSUPPORTED_PERMISSION();
                    }
                    break;
                default:
                    UNSUPPORTED_PERMISSION();
            }
            break;
        case 0b01:
            switch (apx) {
                case 0b0:
                    if (c_m == USER) return NO_ACCESS;
                    return READ_WRITE;
                    break;
                case 0b1:
                    switch (R) {
                        case 0b0:
                            switch (S) {
                                case 0b0:
                                    if (c_m == USER) return NO_ACCESS;
                                    return READ_ONLY;
                                    break;
                                default:
                                    UNSUPPORTED_PERMISSION();
                            }
                            break;
                        default:
                            UNSUPPORTED_PERMISSION();
                    }
                    break;
                default:
                    UNSUPPORTED_PERMISSION();
            }
            break;
        case 0b10:
            switch (apx) {
                case 0b0:
                    if (c_m == USER) return READ_ONLY;
                    return READ_WRITE;
                    break;
                case 0b1:
                    switch (R) {
                        case 0b0:
                            switch (S) {
                                case 0b0:
                                    return READ_ONLY;
                                    break;
                                default:
                                    UNSUPPORTED_PERMISSION();
                            }
                            break;
                        default:
                            UNSUPPORTED_PERMISSION();
                    }
                    break;
                default:
                    UNSUPPORTED_PERMISSION();
            }
            break;
        case 0b11:
            switch (apx) {
                case 0b0:
                    return READ_WRITE;
                    break;
                default:
                    UNSUPPORTED_PERMISSION();
            }
            break;
        default:
            UNSUPPORTED_PERMISSION();
    }
}