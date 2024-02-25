#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "types.h"
#include "register.h"

#include "stdio.h"
#include "stdlib.h"

#define NOT_IMPLEMENTED()                       \
        do {                                    \
            printf("Not yet implemented.\n");   \
            exit(1);                            \
        } while(0)

#define UNPREDICTABLE()                             \
        do {                                        \
            printf("Unpredictable operation.\n");   \
            exit(1);                                \
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

byte check_Cond(Cond_Field, PSR_Bitfield);
Op_Field check_Op_Type(const word*);

#endif // INSTRUCTION_H_