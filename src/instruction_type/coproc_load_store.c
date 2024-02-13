#include "coproc_load_store.h"

void Coproc_LS_write(word* instr, Coproc_LS_Bitfield bits) {
    *instr =    bits.OFF << 0       | bits.CP_NO << 8   | bits.CRD << 12    |
                bits.RN << 16       | bits.L << 20      | bits.W << 21      |
                bits.N << 22        | bits.U << 23      | bits.P << 24      |
                bits.RES_1 << 25    | bits.COND << 28;
}

Coproc_LS_Bitfield Coproc_LS_read(const word* instr) {
    Coproc_LS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.N = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.CRD = *instr >> 12;
    bits.CP_NO = *instr >> 8;
    bits.OFF = *instr >> 0;
    return bits;
}