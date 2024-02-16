#ifndef SOFTWARE_INTERRUPT_H_
#define SOFTWARE_INTERRUPT_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

typedef struct {
    unsigned int COND   : 4;
    unsigned int RES_1  : 4;
    unsigned int SWI    : 24;
} SI_Bitfield;

void SI_write(word*, SI_Bitfield);
SI_Bitfield SI_read(const word*);

void SWI(SI_Bitfield);

#endif // SOFTWARE_INTERRUPT_H_