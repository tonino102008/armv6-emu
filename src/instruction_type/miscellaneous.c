#include "miscellaneous.h"

void MISC_write(word* instr, MISC_Bitfield bits) {
    *instr =    bits.Q_5 << 0   | bits.Q_4 << 4     | bits.Q_3 << 8     | 
                bits.Q_2 << 12  | bits.Q_1 << 16    | bits.R_3 << 20    | 
                bits.R_2 << 21  | bits.R_1 << 22    | bits.RES_1 << 23  |
                bits.COND << 28;
}

MISC_Bitfield MISC_read(const word* instr) {
    MISC_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 23;
    bits.R_1 = *instr >> 22;
    bits.R_2 = *instr >> 21;
    bits.R_3 = *instr >> 20;
    bits.Q_1 = *instr >> 16;
    bits.Q_2 = *instr >> 12;
    bits.Q_3 = *instr >> 8;
    bits.Q_4 = *instr >> 4;
    bits.Q_5 = *instr >> 0;
    return bits;
}

void BKPT(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void BX(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void BXJ(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void CLZ(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void MRS(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void MSR(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QADD(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QDADD(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QDSUB(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QSUB(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLA(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLAL(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLAW(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMUL(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMULW(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}