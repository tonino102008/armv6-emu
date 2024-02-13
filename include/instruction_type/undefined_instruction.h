#ifndef UNDEFINED_INSTRUCTION_H_
#define UNDEFINED_INSTRUCTION_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

#define UNDEFINED_INSTR()                       \
        do {                                    \
            printf("Undefined Instruction.\n"); \
            exit(1);                            \
        } while(0)

// READ MANUAL SECTION A2.6.3 AND IMPLEMENT IT

#endif // UNDEFINED_INSTRUCTION_H_