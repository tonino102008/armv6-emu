#ifndef ARCHITECTURE_UNDEFINED_H_
#define ARCHITECTURE_UNDEFINED_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

#define ARCH_UNDEFINED()                                    \
        do {                                                \
            printf("Architecture Undefined Operation.\n");  \
            exit(1);                                        \
        } while(0)

#endif // ARCHITECTURE_UNDEFINED_H_