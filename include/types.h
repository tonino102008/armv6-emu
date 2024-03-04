#ifndef TYPES_H_
#define TYPES_H_

#include "stdio.h"
#include "stdlib.h"

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned int word;

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

#define BIT_CLEAR(var, pos)         ((var) &= (0x0 << (pos)))
#define BIT_SET(var, pos)           ((BIT_CLEAR(var, pos)) |= (0x1 << (pos)))
#define BIT_CHECK(var, pos)         (!!((var) & (0x1 << (pos))))
#define BITMASK_CLEAR(var, mask)    ((var) &= (~mask))
#define BITMASK_SET(var, mask)      ((BITMASK_CLEAR(var, mask)) |= (mask))
#define BITMASK_CHECK(var, mask)    ((var) & (mask))

#endif // TYPES_H_