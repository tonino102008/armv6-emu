#ifndef COPROC_REG_TRANSFER_H_
#define COPROC_REG_TRANSFER_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND       : 4;
    unsigned int RES_1      : 4;
    unsigned int OPCODE     : 3;
    unsigned int L          : 1;
    unsigned int CRN        : 4;
    unsigned int CRD        : 4;
    unsigned int CP_NO      : 4;
    unsigned int OPCODE2    : 3;
    unsigned int B          : 1;
    unsigned int CRM        : 4;
} Coproc_RT_Bitfield;

void Coproc_RT_write(word*, Coproc_RT_Bitfield);
Coproc_RT_Bitfield Coproc_RT_read(const word*);

void MCR(Coproc_RT_Bitfield);
void MRC(Coproc_RT_Bitfield);

#endif // COPROC_REG_TRANSFER_H_