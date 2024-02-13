#include "branch.h"

void Branch_write(word* instr, Branch_Bitfield bits) {
    *instr =    bits.OFF << 0       | bits.L << 24  |
                bits.RES_1 << 25    | bits.COND << 28;
}

Branch_Bitfield Branch_read(const word* instr) {
    Branch_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.L = *instr >> 24;
    bits.OFF = *instr >> 0;
    return bits;
}