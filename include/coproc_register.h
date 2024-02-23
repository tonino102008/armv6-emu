#ifndef COPROC_REGISTER_H_
#define COPROC_REGISTER_H_

#include "types.h"

#define NO_COPROC_REG 16

#define N_TTBR0 7
#define SIZE_TTBR0 1<<(7 + N_TTBR0)
#define SIZE_TTBR1 1<<14

typedef struct {
    word** regs;
    byte reg_size;
} COPROC_Reg;

typedef enum {
    ID,
    SC_BITS,
    PTC,
    DAC,
    RES_1,
    FAULT_S,
    FAULT_A,
    C_WB_C,
    TLB_C,
    C_L,
    TLB_L,
    DMA_C,
    RES_2,
    PROC_ID,
    RES_3,
    RES_4
} COPROC_Reg_Type;

typedef struct {
    unsigned int IMPL   : 8;
    unsigned int VAR    : 4;
    unsigned int ARCH   : 4;
    unsigned int PPN    : 12;
    unsigned int REV    : 4;
} ID_Bitfield;

extern COPROC_Reg CP15_Reg;

void COPROC_Reg_ctor();
void COPROC_Reg_dtor();

#endif // COPROC_REGISTER_H_