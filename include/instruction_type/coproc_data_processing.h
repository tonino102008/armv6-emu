#ifndef COPROC_DATA_PROCESSING_H_
#define COPROC_DATA_PROCESSING_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND       : 4;
    unsigned int RES_1      : 4;
    unsigned int OPCODE     : 4;
    unsigned int CRN        : 4;
    unsigned int CRD        : 4;
    unsigned int CP_NO      : 4;
    unsigned int OPCODE2    : 3;
    unsigned int B          : 1;
    unsigned int CRM        : 4;
} Coproc_DP_Bitfield;

void Coproc_DP_write(word*, Coproc_DP_Bitfield);
Coproc_DP_Bitfield Coproc_DP_read(const word*);

void CDP(Coproc_DP_Bitfield);

#endif // COPROC_DATA_PROCESSING_H_