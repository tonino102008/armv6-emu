#ifndef MISCELLANEOUS_H_
#define MISCELLANEOUS_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 5;
    unsigned int R_1    : 1;
    unsigned int R_2    : 1;
    unsigned int R_3    : 1;
    unsigned int Q_1    : 4;
    unsigned int Q_2    : 4;
    unsigned int Q_3    : 4;
    unsigned int Q_4    : 4;
    unsigned int Q_5    : 4;
} MISC_1_Bitfield;

void MISC_1_write(word*, MISC_1_Bitfield);
MISC_1_Bitfield MISC_1_read(const word*);

#endif // MISCELLANEOUS_H_