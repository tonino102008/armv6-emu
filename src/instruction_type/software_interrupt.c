#include "software_interrupt.h"

void SI_write(word* instr, SI_Bitfield bits) {
    *instr =    bits.SWI << 0   | bits.RES_1 << 24  | bits.COND << 28;
}

SI_Bitfield SI_read(const word* instr) {
    SI_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 24;
    bits.SWI = *instr >> 0;
    return bits;
}