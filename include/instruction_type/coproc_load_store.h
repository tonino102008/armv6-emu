#ifndef COPROC_LOAD_STORE_H_
#define COPROC_LOAD_STORE_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int P      : 1;
    unsigned int U      : 1;
    unsigned int N      : 1;
    unsigned int W      : 1;
    unsigned int L      : 1;
    unsigned int RN     : 4;
    unsigned int CRD    : 4;
    unsigned int CP_NO  : 4;
    unsigned int OFF    : 8;
} Coproc_LS_Bitfield;

typedef struct {
    word start_address;
    word end_address;
} Addr_Result;

void Coproc_LS_write(word*, Coproc_LS_Bitfield);
Coproc_LS_Bitfield Coproc_LS_read(const word*);

Addr_Result offset_imm(Coproc_LS_Bitfield);

void LDC(Coproc_LS_Bitfield, Addr_Result);
void MCRR(Coproc_LS_Bitfield, Addr_Result);
void MRRC(Coproc_LS_Bitfield, Addr_Result);
void STC(Coproc_LS_Bitfield, Addr_Result);

#endif // COPROC_LOAD_STORE_H_