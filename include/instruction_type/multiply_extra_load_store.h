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

#endif // MULTIPLY_EXTRA_LOAD_STORE_H_