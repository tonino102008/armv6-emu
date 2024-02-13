#ifndef MEDIA_INSTRUCTION_H_
#define MEDIA_INSTRUCTION_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 3;
    unsigned int OPCODE : 20;
    unsigned int B      : 1;
    unsigned int Q      : 4;
} MEI_Bitfield;

void MEI_write(word*, MEI_Bitfield);
MEI_Bitfield MEI_read(const word*);

#endif // MEDIA_INSTRUCTION_H_