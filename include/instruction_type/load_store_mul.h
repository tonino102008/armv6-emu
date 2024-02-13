#ifndef LOAD_STORE_MUL_H_
#define LOAD_STORE_MUL_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int P      : 1;
    unsigned int U      : 1;
    unsigned int S      : 1;
    unsigned int W      : 1;
    unsigned int L      : 1;
    unsigned int RN     : 4;
    unsigned int RL     : 16;
} LS_Mul_Bitfield;

void LS_Mul_write(word*, LS_Mul_Bitfield);
LS_Mul_Bitfield LS_Mul_read(const word*);

#endif // LOAD_STORE_MUL_H_