#ifndef MEDIA_INSTRUCTION_H_
#define MEDIA_INSTRUCTION_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int OPCODE : 20;
    unsigned int B      : 1;
    unsigned int Q      : 4;
} MEI_Bitfield;

void MEI_write(word*, MEI_Bitfield);
MEI_Bitfield MEI_read(const word*);

void PKHBT(MEI_Bitfield);
void PKHTB(MEI_Bitfield);
void QADD16(MEI_Bitfield);
void QADD8(MEI_Bitfield);
void QADDSUBX(MEI_Bitfield);
void QSUB16(MEI_Bitfield);
void QSUB8(MEI_Bitfield);
void QSUBADDX(MEI_Bitfield);
void REV(MEI_Bitfield);
void REV16(MEI_Bitfield);
void REVSH(MEI_Bitfield);
void SADD16(MEI_Bitfield);
void SADD8(MEI_Bitfield);
void SADDSUBX(MEI_Bitfield);
void SEL(MEI_Bitfield);
void SHADD16(MEI_Bitfield);
void SHADD8(MEI_Bitfield);
void SHADDSUBX(MEI_Bitfield);
void SHSUB16(MEI_Bitfield);
void SHSUB8(MEI_Bitfield);
void SHSUBADDX(MEI_Bitfield);
void SMLAD(MEI_Bitfield);
void SMLALD(MEI_Bitfield);
void SMLSD(MEI_Bitfield);
void SMLSLD(MEI_Bitfield);
void SMMLA(MEI_Bitfield);
void SMMLS(MEI_Bitfield);
void SMMUL(MEI_Bitfield);
void SMUAD(MEI_Bitfield);
void SMUSD(MEI_Bitfield);
void SSAT(MEI_Bitfield);
void SSAT16(MEI_Bitfield);
void SSUB16(MEI_Bitfield);
void SSUB8(MEI_Bitfield);
void SSUBADDX(MEI_Bitfield);
void SXTAB(MEI_Bitfield);
void SXTAB16(MEI_Bitfield);
void SXTAH(MEI_Bitfield);
void SXTB(MEI_Bitfield);
void SXTB16(MEI_Bitfield);
void SXTH(MEI_Bitfield);
void UADD16(MEI_Bitfield);
void UADD8(MEI_Bitfield);
void UADDSUBX(MEI_Bitfield);
void UHADD16(MEI_Bitfield);
void UHADD8(MEI_Bitfield);
void UHADDSUBX(MEI_Bitfield);
void UHSUB16(MEI_Bitfield);
void UHSUB8(MEI_Bitfield);
void UHSUBADDX(MEI_Bitfield);
void UQADD16(MEI_Bitfield);
void UQADD8(MEI_Bitfield);
void UQADDSUBX(MEI_Bitfield);
void UQSUB16(MEI_Bitfield);
void UQSUB8(MEI_Bitfield);
void UQSUBADDX(MEI_Bitfield);
void USAD8(MEI_Bitfield);
void USADA8(MEI_Bitfield);
void USAT(MEI_Bitfield);
void USAT16(MEI_Bitfield);
void USUB16(MEI_Bitfield);
void USUB8(MEI_Bitfield);
void USUBADDX(MEI_Bitfield);
void UXTAB(MEI_Bitfield);
void UXTAB16(MEI_Bitfield);
void UXTAH(MEI_Bitfield);
void UXTB(MEI_Bitfield);
void UXTB16(MEI_Bitfield);
void UXTH(MEI_Bitfield);

#endif // MEDIA_INSTRUCTION_H_