#include "move_immediate_status_reg.h"

void MISR_write(word* instr, MISR_Bitfield bits) {
    *instr =    bits.IMM << 0       | bits.ROT << 8     | bits.SBO << 12    | 
                bits.MASK << 16     |bits.RES_2 << 20   | bits.R << 22      | 
                bits.RES_1 << 23    | bits.COND << 28;
}

MISR_Bitfield MISR_read(const word* instr) {
    MISR_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 23;
    bits.R = *instr >> 22;
    bits.RES_2 = *instr >> 20;
    bits.MASK = *instr >> 16;
    bits.SBO = *instr >> 12;
    bits.ROT = *instr >> 8;
    bits.IMM = *instr >> 0;
    return bits;
}

void MSR(MISR_Bitfield) {
    NOT_IMPLEMENTED();
}