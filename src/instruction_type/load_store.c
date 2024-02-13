#include "load_store.h"

void LS_write(word* instr, LS_Bitfield bits) {
    *instr =    bits.SHIFT << 0     | bits.RD << 12     | bits.RN << 16 | 
                bits.L << 20        |bits.W << 21       | bits.B << 22  | 
                bits.U << 23        |bits.P << 24       | bits.I << 25  |
                bits.RES_1 << 26    | bits.COND << 28;
}

LS_Bitfield LS_read(const word* instr) {
    LS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 26;
    bits.I = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.B = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.RD = *instr >> 12;
    bits.SHIFT = *instr >> 0;
    return bits;
}