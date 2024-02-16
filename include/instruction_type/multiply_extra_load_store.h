#ifndef MULTIPLY_EXTRA_LOAD_STORE_H_
#define MULTIPLY_EXTRA_LOAD_STORE_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int P      : 1;
    unsigned int U      : 1;
    unsigned int B_UN   : 1;
    unsigned int W_A    : 1;
    unsigned int L_S    : 1;
    unsigned int Q_1    : 4;
    unsigned int Q_2    : 4;
    unsigned int Q_3    : 4;
    unsigned int RES_2  : 2;
    unsigned int H_ST   : 1;
    unsigned int RES_3  : 1;
    unsigned int Q_4    : 4;
} MELS_Bitfield;

void MELS_write(word*, MELS_Bitfield);
MELS_Bitfield MELS_read(const word*);

word offset_imm(MELS_Bitfield);
word offset_reg(MELS_Bitfield);

void LDRD(MELS_Bitfield, word);
void LDREX(MELS_Bitfield, word);
void LDRH(MELS_Bitfield, word);
void LDRSB(MELS_Bitfield, word);
void LDRSH(MELS_Bitfield, word);
void MLA(MELS_Bitfield, word);
void MUL(MELS_Bitfield, word);
void SMLAL(MELS_Bitfield, word);
void SMULL(MELS_Bitfield, word);
void STRD(MELS_Bitfield, word);
void STREX(MELS_Bitfield, word);
void STRH(MELS_Bitfield, word);
void SWP(MELS_Bitfield, word);
void SWPB(MELS_Bitfield, word);
void UMAAL(MELS_Bitfield, word);
void UMLAL(MELS_Bitfield, word);
void UMULL(MELS_Bitfield, word);

#endif // MULTIPLY_EXTRA_LOAD_STORE_H_