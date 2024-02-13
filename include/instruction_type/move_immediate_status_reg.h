#ifndef MOVE_IMMEDIATE_STATUS_REG_H_
#define MOVE_IMMEDIATE_STATUS_REG_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 5;
    unsigned int R      : 1;
    unsigned int RES_2  : 2;
    unsigned int MASK   : 4;
    unsigned int SBO    : 4;
    unsigned int ROT    : 4;
    unsigned int IMM    : 8;
} MISR_Bitfield;

void MISR_write(word*, MISR_Bitfield);
MISR_Bitfield MISR_read(const word*);

#endif // MOVE_IMMEDIATE_STATUS_REG_H_