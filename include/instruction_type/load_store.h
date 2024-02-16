#ifndef LOAD_STORE_H_
#define LOAD_STORE_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 2;
    unsigned int I      : 1;
    unsigned int P      : 1;
    unsigned int U      : 1;
    unsigned int B      : 1;
    unsigned int W      : 1;
    unsigned int L      : 1;
    unsigned int RN     : 4;
    unsigned int RD     : 4;
    unsigned int OFF    : 12;
} LS_Bitfield;

void LS_write(word*, LS_Bitfield);
LS_Bitfield LS_read(const word*);

word offset_imm(LS_Bitfield);
word offset_reg(LS_Bitfield);
word offset_scaled_reg(LS_Bitfield);

#endif // LOAD_STORE_H_