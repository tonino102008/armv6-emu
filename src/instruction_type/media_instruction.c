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

void PKHBT(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void PKHTB(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QSUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QSUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void QSUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void REV(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void REV16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void REVSH(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SEL(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHSUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHSUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SHSUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLAD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLALD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLSD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLSLD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMMLA(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMMLS(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMMUL(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMUAD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMUSD(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SSAT(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SSAT16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SSUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SSUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SSUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTAB(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTAB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTAH(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTB(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void SXTH(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHSUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHSUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UHSUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQADD16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQADD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQADDSUBX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQSUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQSUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UQSUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USAD8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USADA8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USAT(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USAT16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USUB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USUB8(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void USUBADDX(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTAB(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTAB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTAH(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTB(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTB16(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}

void UXTH(MEI_Bitfield) {
    NOT_IMPLEMENTED();
}