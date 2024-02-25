#ifndef MISCELLANEOUS_H_
#define MISCELLANEOUS_H_

#include "types.h"
#include "register.h"
#include "instruction.h"
#include "exception.h"

#define NO_DEBUG_HARDWARE 1

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
} MISC_Bitfield;

void MISC_write(word*, MISC_Bitfield);
MISC_Bitfield MISC_read(const word*);

void BKPT(MISC_Bitfield);
void BLX(MISC_Bitfield);
void BX(MISC_Bitfield);
void BXJ(MISC_Bitfield);
void CLZ(MISC_Bitfield);
void MRS(MISC_Bitfield);
void MSR(MISC_Bitfield);
void QADD(MISC_Bitfield);
void QDADD(MISC_Bitfield);
void QDSUB(MISC_Bitfield);
void QSUB(MISC_Bitfield);
void SMLA(MISC_Bitfield);
void SMLAL(MISC_Bitfield);
void SMLAW(MISC_Bitfield);
void SMUL(MISC_Bitfield);
void SMULW(MISC_Bitfield);

#endif // MISCELLANEOUS_H_