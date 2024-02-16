#ifndef DATA_PROCESSING_H_
#define DATA_PROCESSING_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 2;
    unsigned int I      : 1;
    unsigned int OPCODE : 4;
    unsigned int S      : 1;
    unsigned int RN     : 4;
    unsigned int RD     : 4;
    unsigned int SHIFT  : 12;
} Data_Proc_Bitfield;

typedef struct {
    word shifter_operand;
    byte shift_carry_out;
} Shift_Result;

void Data_Proc_write(word*, Data_Proc_Bitfield);
Data_Proc_Bitfield Data_Proc_read(const word*);

void ADC(const word*); // HAS TO BE STATIC

Shift_Result shift_32bit_imm(Data_Proc_Bitfield);
Shift_Result shift_imm(Data_Proc_Bitfield);
Shift_Result shift_reg(Data_Proc_Bitfield);

#endif // DATA_PROCESSING_H_