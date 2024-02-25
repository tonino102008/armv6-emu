#ifndef UNCONDITIONAL_INSTRUCTION_H_
#define UNCONDITIONAL_INSTRUCTION_H_

#include "types.h"
#include "register.h"
#include "instruction.h"

void BLX(); // COND: 1111
void CDP();
void CPS();
void LDC();
void MCR();
void MCRR();
void MRC();
void MRRC();
void PLD();
void RFE();
void SETEND();
void SRS();

#endif // UNCONDITIONAL_INSTRUCTION_H_