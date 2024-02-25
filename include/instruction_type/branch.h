#ifndef BRANCH_H_
#define BRANCH_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int L      : 1;
    unsigned int OFF    : 24;
} Branch_Bitfield;

void Branch_write(word*, Branch_Bitfield);
Branch_Bitfield Branch_read(const word*);

void B_BL(Branch_Bitfield);

#endif // BRANCH_H_