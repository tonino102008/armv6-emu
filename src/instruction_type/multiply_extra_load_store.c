#include "multiply_extra_load_store.h"

void MELS_write(word* instr, MELS_Bitfield bits) {
    *instr =    bits.Q_4 << 0       | bits.RES_3 << 4   | bits.H_ST << 5    | 
                bits.RES_2 << 6     | bits.Q_3 << 8     | bits.Q_2 << 12    | 
                bits.Q_1 << 16      | bits.L_S << 20    | bits.W_A << 21    |
                bits.B_UN << 22     | bits.U << 23      | bits.P << 24      |
                bits.RES_1 << 25    | bits.COND << 28;
}

MELS_Bitfield MELS_read(const word* instr) {
    MELS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.B_UN = *instr >> 22;
    bits.W_A = *instr >> 21;
    bits.L_S = *instr >> 20;
    bits.Q_1 = *instr >> 16;
    bits.Q_2 = *instr >> 12;
    bits.Q_3 = *instr >> 8;
    bits.RES_2 = *instr >> 6;
    bits.H_ST = *instr >> 5;
    bits.RES_3 = *instr >> 4;
    bits.Q_4 = *instr >> 0;
    return bits;
}