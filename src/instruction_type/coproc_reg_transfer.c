#include "coproc_reg_transfer.h"

void Coproc_RT_write(word* instr, Coproc_RT_Bitfield bits) {
    *instr =    bits.CRM << 0       | bits.B << 4       | bits.OPCODE2 << 5 | 
                bits.CP_NO << 8     | bits.CRD << 12    | bits.CRN << 16    | 
                bits.L << 17        | bits.OPCODE << 20 | bits.RES_1 << 24  |
                bits.COND << 28;
}

Coproc_RT_Bitfield Coproc_RT_read(const word* instr) {
    Coproc_RT_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 24;
    bits.OPCODE = *instr >> 20;
    bits.L = *instr >> 17;
    bits.CRN = *instr >> 16;
    bits.CRD = *instr >> 12;
    bits.CP_NO = *instr >> 8;
    bits.OPCODE2 = *instr >> 5;
    bits.B = *instr >> 4;
    bits.CRM = *instr >> 0;
    return bits;
}