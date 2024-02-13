#include "load_store_mul.h"

void LS_Mul_write(word* instr, LS_Mul_Bitfield bits) {
    *instr =    bits.RL << 0    | bits.RN << 16     | bits.L << 20      |
                bits.W << 21    | bits.S << 22      | bits.U << 23      |
                bits.P << 24    | bits.RES_1 << 25  | bits.COND << 28;
}

LS_Mul_Bitfield LS_Mul_read(const word* instr) {
    LS_Mul_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.S = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.RL = *instr >> 0;
    return bits;
}