#include "unconditional_instruction.h"

void BLX() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[c_m].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    CPSR_bits.T = 1;
    //word se30;
    //if (bits.OFF >> 23) se30 = bits.OFF | (0b111111 << 24);
    //else se30 = bits.OFF | (0b000000 << 24);
    //*GP_Reg[c_m].regs[PC] += se30 << 2;
    PSR_write(*CPSR_Reg.regs, CPSR_bits);
    NOT_IMPLEMENTED();
}

void CDP() {
    NOT_IMPLEMENTED();
}

void CPS() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);
    NOT_IMPLEMENTED();
}

void LDC() {
    NOT_IMPLEMENTED();
}

void MCR() {
    NOT_IMPLEMENTED();
}

void MCRR() {
    NOT_IMPLEMENTED();
}

void PLD() {
    NOT_IMPLEMENTED();
}

void RFE() {
    NOT_IMPLEMENTED();
}

void SETEND() {
    NOT_IMPLEMENTED();
}

void SRS() {
    NOT_IMPLEMENTED();
}