#include "coproc_data_processing.h"

void Coproc_DP_write(word* instr, Coproc_DP_Bitfield bits) {
    *instr =    bits.CRM << 0       | bits.B << 4       | bits.OPCODE2 << 5 | 
                bits.CP_NO << 8     | bits.CRD << 12    | bits.CRN << 16    | 
                bits.OPCODE << 20   | bits.RES_1 << 24  | bits.COND << 28;
}

Coproc_DP_Bitfield Coproc_DP_read(const word* instr) {
    Coproc_DP_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 24;
    bits.OPCODE = *instr >> 20;
    bits.CRN = *instr >> 16;
    bits.CRD = *instr >> 12;
    bits.CP_NO = *instr >> 8;
    bits.OPCODE2 = *instr >> 5;
    bits.B = *instr >> 4;
    bits.CRM = *instr >> 0;
    return bits;
}