#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "types.h"
#include "register.h"
#include "coproc_register.h"

#include "stdio.h"
#include "stdlib.h"

#define UNSUPPORTED_EXCEPTION_ADRRESS()                 \
        do {                                            \
            printf("Unsupported exception address.\n"); \
            exit(1);                                    \
        } while(0)

#define UNSUPPORTED_EXCEPTION()                 \
        do {                                    \
            printf("Unsupported exception.\n"); \
            exit(1);                            \
        } while(0)

#define UNSUPPORTED_VECTORED_INTERRUPTS()                   \
        do {                                                \
            printf("Unsupported vectored interrupts.\n");   \
            exit(1);                                        \
        } while(0)

typedef enum {
    RESET,
    UNDEFINED_INSTR,
    SOFTWARE_INT,
    PREFETCH_ABORT,
    DATA_ABORT,
    IRQ,
    FIQ
} ARM_Exception_Type;

void handle_exception(ARM_Exception_Type);
void handle_reset();
void handle_undefined_instruction();
void handle_software_interrupt();
void handle_prefetch_abort();
void handle_data_abort();
void handle_irq();
void handle_fiq();

#endif // EXCEPTION_H_