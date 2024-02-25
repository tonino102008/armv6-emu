#include "miscellaneous.h"

void MISC_write(word* instr, MISC_Bitfield bits) {
    *instr =    bits.Q_5 << 0   | bits.Q_4 << 4     | bits.Q_3 << 8     | 
                bits.Q_2 << 12  | bits.Q_1 << 16    | bits.R_3 << 20    | 
                bits.R_2 << 21  | bits.R_1 << 22    | bits.RES_1 << 23  |
                bits.COND << 28;
}

MISC_Bitfield MISC_read(const word* instr) {
    MISC_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 23;
    bits.R_1 = *instr >> 22;
    bits.R_2 = *instr >> 21;
    bits.R_3 = *instr >> 20;
    bits.Q_1 = *instr >> 16;
    bits.Q_2 = *instr >> 12;
    bits.Q_3 = *instr >> 8;
    bits.Q_4 = *instr >> 4;
    bits.Q_5 = *instr >> 0;
    return bits;
}

void BKPT(MISC_Bitfield bits) {
    if (NO_DEBUG_HARDWARE) {
        return handle_exception(PREFETCH_ABORT);
    }
}

void BLX(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if ((bits.Q_5 & 0b11) == 0b10) UNPREDICTABLE();
        word target = *GP_Reg[c_m].regs[bits.Q_5];
        *GP_Reg[c_m].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
        CPSR_bits.T = target & 0b1;
        *GP_Reg[c_m].regs[PC] = target & 0xFFFFFFFE;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void BX(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if ((bits.Q_5 & 0b11) == 0b10) UNPREDICTABLE();
        CPSR_bits.T = *GP_Reg[c_m].regs[bits.Q_5] & 0b1;
        *GP_Reg[c_m].regs[PC] = *GP_Reg[c_m].regs[bits.Q_5] & 0xFFFFFFFE;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void BXJ(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void CLZ(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_5 == PC || bits.Q_2 == PC) UNPREDICTABLE();
        if ((bits.Q_5 & 0b1) == 0b0) *GP_Reg[c_m].regs[bits.Q_2] = 32;
        else {
            byte res = 31;
            while (((*GP_Reg[c_m].regs[bits.Q_5] >> res) & 0b1) == 0) res -= 1;
            *GP_Reg[c_m].regs[bits.Q_2] = 31 - res;
        }
    }
}

void MRS(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_2 == PC) UNPREDICTABLE();
        if (bits.R_1) {
            if (c_m == USER || c_m == SYSTEM) UNPREDICTABLE();
            *GP_Reg[c_m].regs[bits.Q_2] = *SPSR_Reg[c_m].regs;
        }
        else *GP_Reg[c_m].regs[bits.Q_2] = *CPSR_Reg.regs;
    }
}

void MSR(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QADD(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QDADD(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QDSUB(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void QSUB(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLA(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLAL(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMLAW(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMUL(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}

void SMULW(MISC_Bitfield) {
    NOT_IMPLEMENTED();
}