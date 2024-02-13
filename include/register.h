#ifndef REGISTER_H_
#define REGISTER_H_

#include "types.h"

#include "stdio.h"

#define NO_GP_REG 16
#define NO_CPSR_REG 1
#define NO_SPSR_REG 1

#define SP 13
#define LR 14
#define PC 15

#define ERROR_MODE()                                                    \
        do {                                                            \
            printf("The requested Processor Mode does not exist.\n");   \
            exit(1);                                                    \
        } while(0)

typedef enum {
    USER,
    FIQ,
    IRQ,
    SUPERVISOR,
    ABORT,
    UNDEFINED,
    SYSTEM,
    NO_PROC_MODE
} Proc_Mode;

typedef struct {
    word** regs;
    byte reg_size;
} ARM_Reg;

typedef struct {
    unsigned int N      : 1;
    unsigned int Z      : 1;
    unsigned int C      : 1;
    unsigned int V      : 1;
    unsigned int Q      : 1;
    unsigned int RES_1  : 2;
    unsigned int J      : 1;
    unsigned int RES_2  : 4;
    unsigned int GE     : 4;
    unsigned int RES_3  : 6;
    unsigned int E      : 1;
    unsigned int A      : 1;
    unsigned int I      : 1;
    unsigned int F      : 1;
    unsigned int T      : 1;
    unsigned int M      : 5;
} PSR_Bitfield;

extern byte proc_mode[NO_PROC_MODE];

extern ARM_Reg* GP_Reg;
extern ARM_Reg CPSR_Reg;
extern ARM_Reg* SPSR_Reg;

void ARM_Reg_ctor();
void ARM_Reg_dtor();

int cmp_Proc_Mode(const void*, const void*);
Proc_Mode curr_Proc_Mode(byte);
byte has_SPSR();

void PSR_write(word*, PSR_Bitfield);
PSR_Bitfield PSR_read(const word*);

#endif // REGISTER_H_