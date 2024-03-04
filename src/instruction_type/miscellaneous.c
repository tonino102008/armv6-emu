#include "miscellaneous.h"
#include "util.h"

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

void BXJ(MISC_Bitfield bits) {
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

void MSR(MISC_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void QADD(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_5 == PC || bits.Q_2 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_5], 
                                    *GP_Reg[c_m].regs[bits.Q_1], 32, ADD);
        *GP_Reg[c_m].regs[bits.Q_2] = res.result;
        if (res.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void QDADD(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_5 == PC || bits.Q_2 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        Util_Sat_Op_Result Rn2 = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_1], 
                                    (word)2, 32, MUL);
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_5], 
                                    Rn2.result, 32, ADD);
        *GP_Reg[c_m].regs[bits.Q_2] = res.result;
        if (res.overflow || Rn2.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void QDSUB(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_5 == PC || bits.Q_2 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        Util_Sat_Op_Result Rn2 = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_1], 
                                    (word)2, 32, MUL);
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_5], 
                                    Rn2.result, 32, SUB);
        *GP_Reg[c_m].regs[bits.Q_2] = res.result;
        if (res.overflow || Rn2.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void QSUB(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        if (bits.Q_5 == PC || bits.Q_2 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[bits.Q_5], 
                                    *GP_Reg[c_m].regs[bits.Q_1], 32, SUB);
        *GP_Reg[c_m].regs[bits.Q_2] = res.result;
        if (res.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SMLA(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.Q_4, 1);
        byte y = BIT_CHECK(bits.Q_4, 2);
        if (bits.Q_5 == PC || bits.Q_3 == PC || bits.Q_2 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        word op1;
        if (x == 0) op1 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_5], 0x0000FFFF), 16);
        else op1 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_5], 0xFFFF0000) >> 16, 16);
        word op2;
        if (y == 0) op2 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_3], 0x0000FFFF), 16);
        else op2 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_3], 0xFFFF0000) >> 16, 16);
        Util_Sat_Op_Result res = signed_op(op1, op2, 32, MUL);
        Util_Sat_Op_Result res2 = signed_op(res.result, *GP_Reg[c_m].regs[bits.Q_2], 32, ADD);
        *GP_Reg[c_m].regs[bits.Q_1] = res2.result;
        if (res2.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SMLAL(MISC_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMLAW(MISC_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMUL(MISC_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.Q_4, 1);
        byte y = BIT_CHECK(bits.Q_4, 2);
        if (bits.Q_5 == PC || bits.Q_3 == PC || bits.Q_1 == PC) UNPREDICTABLE();
        word op1;
        if (x == 0) op1 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_5], 0x0000FFFF), 16);
        else op1 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_5], 0xFFFF0000) >> 16, 16);
        word op2;
        if (y == 0) op2 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_3], 0x0000FFFF), 16);
        else op2 = sign_extend(BITMASK_CHECK(*GP_Reg[c_m].regs[bits.Q_3], 0xFFFF0000) >> 16, 16);
        Util_Sat_Op_Result res = signed_op(op1, op2, 32, MUL);
        *GP_Reg[c_m].regs[bits.Q_1] = res.result;
    }
}

void SMULW(MISC_Bitfield bits) {
    NOT_IMPLEMENTED();
}