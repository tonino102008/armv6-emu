#ifndef RAM_H_
#define RAM_H_

#include "types.h"

#define RAM_SIZE (1024 * 512)/sizeof(word)

typedef struct
{
    word* memory;
    byte size;
} ARM_Ram;

extern ARM_Ram* ram;

void ARM_Ram_ctor();
void ARM_Ram_dtor();

#endif // RAM_H_