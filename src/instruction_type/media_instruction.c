#include "media_instruction.h"

void MEI_write(word* instr, MEI_Bitfield bits) {
    *instr =    bits.Q << 0         | bits.B << 4   | bits.OPCODE << 5  | 
                bits.RES_1 << 25    | bits.COND << 28;
}

MEI_Bitfield MEI_read(const word* instr) {
    MEI_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.OPCODE = *instr >> 5;
    bits.B = *instr >> 4;
    bits.Q = *instr >> 0;
    return bits;
}