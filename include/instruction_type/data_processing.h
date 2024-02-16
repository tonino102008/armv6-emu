#ifndef DATA_PROCESSING_H_
#define DATA_PROCESSING_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

#define TRY_RESTORE_SPSR()                          \
        do {                                        \
            if (has_SPSR()) CPSR_Reg = *SPSR_Reg;   \
            else UNPREDICTABLE();                   \
        } while(0)

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

Shift_Result shift_32bit_imm(Data_Proc_Bitfield);
Shift_Result shift_imm(Data_Proc_Bitfield);
Shift_Result shift_reg(Data_Proc_Bitfield);

void ADC(Data_Proc_Bitfield, Shift_Result);
void ADD(Data_Proc_Bitfield, Shift_Result);
void AND(Data_Proc_Bitfield, Shift_Result);
void BIC(Data_Proc_Bitfield, Shift_Result);
void CMN(Data_Proc_Bitfield, Shift_Result);
void CMP(Data_Proc_Bitfield, Shift_Result);

// void CPY(Data_Proc_Bitfield, Shift_Result); PARTICULAR CASE THAT FALLS IN MOV

void EOR(Data_Proc_Bitfield, Shift_Result);
void MOV(Data_Proc_Bitfield, Shift_Result);
void MVN(Data_Proc_Bitfield, Shift_Result);
void ORR(Data_Proc_Bitfield, Shift_Result);
void RSB(Data_Proc_Bitfield, Shift_Result);
void RSC(Data_Proc_Bitfield, Shift_Result);
void SBC(Data_Proc_Bitfield, Shift_Result);
void SUB(Data_Proc_Bitfield, Shift_Result);
void TEQ(Data_Proc_Bitfield, Shift_Result);
void TST(Data_Proc_Bitfield, Shift_Result);

#endif // DATA_PROCESSING_H_