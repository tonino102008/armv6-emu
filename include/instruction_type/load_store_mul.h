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

typedef struct {
    word start_address;
    word end_address;
} Addr_Result;

void LS_Mul_write(word*, LS_Mul_Bitfield);
LS_Mul_Bitfield LS_Mul_read(const word*);

Addr_Result incr_decr(LS_Mul_Bitfield);
byte no_set_bits(hword);

void LDM(LS_Mul_Bitfield, Addr_Result); // TRIPLE IMPLEMENTATION
void STM(LS_Mul_Bitfield, Addr_Result); // DOUBLE IMPLEMENTATION

#endif // LOAD_STORE_MUL_H_