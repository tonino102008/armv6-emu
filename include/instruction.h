#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "types.h"
#include "register.h"

#include "stdio.h"
#include "stdlib.h"

#define NOT_IMPLEMENTED() printf("Not yet implemented.\n");
#define UNPREDICTABLE() printf("Unpredictable operation. Behaviour to be defined\n");
#define TRY_RESTORE_SPSR()                          \
        do {                                        \
            if (has_SPSR()) CPSR_Reg = *SPSR_Reg;   \
            else UNPREDICTABLE();                   \
        } while(0)
#define UNSUPPORTED()                           \
        do {                                    \
            printf("Unsupported operation.\n"); \
            exit(1);                            \
        } while(0)

typedef enum {
    DPIS,
    MI_1,
    DPRS,
    MI_2,
    MELS,
    DPI,
    UNDI,
    MISR,
    LSIO,
    LSRO,
    MEI,
    AU,
    LSM,
    BBL,
    CLS,
    CDP,
    CRT,
    SWI,
    UNCI
} Op_Field;

typedef enum {
    EQ      = 0b0000,
    NE      = 0b0001,
    CS_HS   = 0b0010,
    CC_LO   = 0b0011,
    MI      = 0b0100,
    PL      = 0b0101,
    VS      = 0b0110,
    VC      = 0b0111,
    HI      = 0b1000,
    LS      = 0b1001,
    GE      = 0b1010,
    LT      = 0b1011,
    GT      = 0b1100,
    LE      = 0b1101,
    AL      = 0b1110,
    EXTRA   = 0b1111 // UNIQUE Cond_Field that enables operation
} Cond_Field;

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

byte check_Cond(Cond_Field, PSR_Bitfield);

Op_Field check_Op_Type(const word*);

void Data_Proc_write(word*, Data_Proc_Bitfield);
Data_Proc_Bitfield Data_Proc_read(const word*);

void ADC(const word*); // HAS TO BE STATIC

#endif // INSTRUCTION_H_