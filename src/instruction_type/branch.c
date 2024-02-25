#include "branch.h"

void Branch_write(word* instr, Branch_Bitfield bits) {
    *instr =    bits.OFF << 0       | bits.L << 24  |
                bits.RES_1 << 25    | bits.COND << 28;
}

Branch_Bitfield Branch_read(const word* instr) {
    Branch_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.L = *instr >> 24;
    bits.OFF = *instr >> 0;
    return bits;
}

void B_BL(Branch_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.L) *GP_Reg[c_m].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
        word se30;
        if (bits.OFF >> 23) se30 = bits.OFF | (0b111111 << 24);
        else se30 = bits.OFF | (0b000000 << 24);
        *GP_Reg[c_m].regs[PC] += se30 << 2;
    }
}