#include "media_instruction.h"
#include "util.h"

void MEI_write(word* instr, MEI_Bitfield bits) {
    *instr =    bits.Q << 0         | bits.B << 4   | bits.OPCODE << 5  | 
                bits.RES_1 << 25    | bits.COND << 28;
}

MEI_Bitfield MEI_read(const word* instr) {
    MEI_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.OPCODE = *instr >> 5;
    bits.B = *instr >> 4;
    bits.Q = *instr >> 0;
    return bits;
}

// CHECK ALL bits.OPCODE operations!!!!!!! 
// CHECK ALL bits.OPCODE operations!!!!!!! 
// CHECK ALL bits.OPCODE operations!!!!!!! 

void PKHBT(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte shift_imm = (bits.OPCODE >> 2) & 0b11111;
        byte Rd = (bits.OPCODE >> 7) & 0xF;
        byte Rn = (bits.OPCODE >> 11) & 0xF;
        if (bits.Q == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] = (*GP_Reg[c_m].regs[Rn] & 0x0000FFFF) | 
                                ((*GP_Reg[c_m].regs[bits.Q] << shift_imm) & 0xFFFF0000);
    }
}

void PKHTB(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte shift_imm = (bits.OPCODE >> 2) & 0b11111;
        byte Rd = (bits.OPCODE >> 7) & 0xF;
        byte Rn = (bits.OPCODE >> 11) & 0xF;
        if (bits.Q == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        if (shift_imm == 0) {
            if (((*GP_Reg[c_m].regs[bits.Q] >> 31) & 0b1) == 0b0) *GP_Reg[c_m].regs[Rd] = 0x0000;
            else *GP_Reg[c_m].regs[Rd] = 0xFFFF;
        } else {
            word msb = (*GP_Reg[c_m].regs[bits.Q] & (0b1 << 15)) >> 15; // msb is 15 because halfword?
            word ASR = *GP_Reg[c_m].regs[bits.Q] >> shift_imm;
            ASR |= msb << 15;
            *GP_Reg[c_m].regs[Rd] = ASR;
        }
        *GP_Reg[c_m].regs[Rd] |= (*GP_Reg[c_m].regs[Rn] & 0xFFFF0000);
    }
}

void QADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void QADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
    }
}

void QADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void QSUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void QSUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
    }
}

void QSUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                    *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = signed_saturation_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                    (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void REV(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] |= (*GP_Reg[c_m].regs[Rm] & 0xFF) << 23;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF) << 15;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF) << 7;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF);
    }
}

void REV16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] |= (*GP_Reg[c_m].regs[Rm] & 0xFF) << 7;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF);
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF) << 23;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF) << 15;
    }
}

void REVSH(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] |= (*GP_Reg[c_m].regs[Rm] & 0xFF) << 7;
        *GP_Reg[c_m].regs[Rd] |= ((*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF);
        if (((*GP_Reg[c_m].regs[Rm] >> 6) & 0b1) == 1) *GP_Reg[c_m].regs[Rd] |= 0xFFFF << 15;
        else *GP_Reg[c_m].regs[Rd] |= 0x0000 << 15;
    }
}

void SADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 : 0b00;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 : 0b0;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 1 : 0b0 << 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 2 : 0b0 << 2;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 3 : 0b0 << 3;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] & (0xFFFF << 15)) >> 15, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 : 0b00;
        res = signed_op((*GP_Reg[c_m].regs[Rn] & (0xFFFF << 15)) >> 15, 
                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SEL(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] |=    (CPSR_bits.GE & 0b1) ? 
                                    *GP_Reg[c_m].regs[Rn] & 0xFF : 
                                    *GP_Reg[c_m].regs[Rm] & 0xFF;
        *GP_Reg[c_m].regs[Rd] |=    (CPSR_bits.GE & (0b1 << 1)) ? 
                                    *GP_Reg[c_m].regs[Rn] & (0xFF << 7) : 
                                    *GP_Reg[c_m].regs[Rm] & (0xFF << 7);
        *GP_Reg[c_m].regs[Rd] |=    (CPSR_bits.GE & (0b1 << 2)) ? 
                                    *GP_Reg[c_m].regs[Rn] & (0xFF << 15) : 
                                    *GP_Reg[c_m].regs[Rm] & (0xFF << 15);
        *GP_Reg[c_m].regs[Rd] |=    (CPSR_bits.GE & (0b1 << 3)) ? 
                                    *GP_Reg[c_m].regs[Rn] & (0xFF << 23) : 
                                    *GP_Reg[c_m].regs[Rm] & (0xFF << 23);
    }
}

void SHADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFFFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFFFF << 1)) >> 1) << 15;
    }
}

void SHADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 7;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 15;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 23;
    }
}

void SHADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] & (0xFFFF << 15)) >> 15, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFFFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] & (0xFFFF << 15)) >> 15, 
                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFFFF << 1)) >> 1) << 15;
    }
}

void SHSUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFFFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFFFF << 1)) >> 1) << 15;
    }
}

void SHSUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 7;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 15;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFF << 1)) >> 1) << 23;
    }
}

void SHSUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] & (0xFFFF << 15)) >> 15, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result & (0xFFFF << 1)) >> 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] & (0xFFFF << 15)) >> 15, 
                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= ((res.result & (0xFFFF << 1)) >> 1) << 15;
    }
}

void SMLAD(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.OPCODE, 0);
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        byte Rs = (bits.OPCODE >> 2) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC || Rs == PC) UNPREDICTABLE();
        word op2;
        if (x == 1) op2 = *GP_Reg[c_m].regs[Rs] >> 16;
        else op2 = *GP_Reg[c_m].regs[Rs];
        Util_Sat_Op_Result res = signed_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                            BITMASK_CHECK(op2, 0x0000FFFF), 32, MUL);
        Util_Sat_Op_Result res2 = signed_op(BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16, 
                                            BITMASK_CHECK(op2, 0xFFFF0000) >> 16, 32, MUL);
        Util_Sat_Op_Result res3 = signed_op(*GP_Reg[c_m].regs[Rn], res.result, 32, ADD);
        Util_Sat_Op_Result res4 = signed_op(res3.result, res2.result, 32, ADD);
        *GP_Reg[c_m].regs[Rd] = res4.result;
        if (res3.overflow || res4.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SMLALD(MEI_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMLSD(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.OPCODE, 0);
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        byte Rs = (bits.OPCODE >> 2) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC || Rs == PC) UNPREDICTABLE();
        word op2;
        if (x == 1) op2 = *GP_Reg[c_m].regs[Rs] >> 16;
        else op2 = *GP_Reg[c_m].regs[Rs];
        Util_Sat_Op_Result res = signed_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                            BITMASK_CHECK(op2, 0x0000FFFF), 32, MUL);
        Util_Sat_Op_Result res2 = signed_op(BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16, 
                                            BITMASK_CHECK(op2, 0xFFFF0000) >> 16, 32, MUL);
        Util_Sat_Op_Result res3 = signed_op(res.result, res2.result, 32, SUB);
        Util_Sat_Op_Result res4 = signed_op(*GP_Reg[c_m].regs[Rn], res3.result, 32, ADD);
        *GP_Reg[c_m].regs[Rd] = res4.result;
        if (res4.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SMLSLD(MEI_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMMLA(MEI_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMMLS(MEI_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMMUL(MEI_Bitfield bits) {
    NOT_IMPLEMENTED();
}

void SMUAD(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.OPCODE, 0);
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rs = (bits.OPCODE >> 2) & 0xF;
        if (Rm == PC || Rd == PC || Rs == PC) UNPREDICTABLE();
        word op2;
        if (x == 1) op2 = *GP_Reg[c_m].regs[Rs] >> 16;
        else op2 = *GP_Reg[c_m].regs[Rs];
        Util_Sat_Op_Result res = signed_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                            BITMASK_CHECK(op2, 0x0000FFFF), 32, MUL);
        Util_Sat_Op_Result res2 = signed_op(BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16, 
                                            BITMASK_CHECK(op2, 0xFFFF0000) >> 16, 32, MUL);
        Util_Sat_Op_Result res3 = signed_op(res.result, res2.result, 32, ADD);
        *GP_Reg[c_m].regs[Rd] = res3.result;
        if (res3.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SMUSD(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte x = BIT_CHECK(bits.OPCODE, 0);
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rs = (bits.OPCODE >> 2) & 0xF;
        if (Rm == PC || Rd == PC || Rs == PC) UNPREDICTABLE();
        word op2;
        if (x == 1) op2 = *GP_Reg[c_m].regs[Rs] >> 16;
        else op2 = *GP_Reg[c_m].regs[Rs];
        Util_Sat_Op_Result res = signed_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                            BITMASK_CHECK(op2, 0x0000FFFF), 32, MUL);
        Util_Sat_Op_Result res2 = signed_op(BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16, 
                                            BITMASK_CHECK(op2, 0xFFFF0000) >> 16, 32, MUL);
        Util_Sat_Op_Result res3 = signed_op(res.result, res2.result, 32, SUB);
        *GP_Reg[c_m].regs[Rd] = res3.result;
    }
}

void SSAT(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte sh = (bits.OPCODE >> 1) & 0x3;
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        word shift_imm = (bits.OPCODE >> 2) & 0x1F;
        word sat_imm = (bits.OPCODE >> 12) & 0x1F;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        word op;
        if (sh) {
            if (shift_imm == 0) {
                op = arithmetic_shift_right(*GP_Reg[c_m].regs[Rm], 32);
            } else {
                op = arithmetic_shift_right(*GP_Reg[c_m].regs[Rm], shift_imm);
            }
        }
        else op = *GP_Reg[c_m].regs[Rm] << shift_imm;
        Util_Sat_Op_Result res = signed_saturation_check(op, sat_imm + 1);
        *GP_Reg[c_m].regs[Rd] = res.result;
        if (res.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SSAT16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte sh = (bits.OPCODE >> 1) & 0x3;
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        word sat_imm = (bits.OPCODE >> 12) & 0xF;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = signed_saturation_check(   BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                                            sat_imm + 1);
        Util_Sat_Op_Result res2 = signed_saturation_check(  BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16,
                                                            sat_imm + 1);
        *GP_Reg[c_m].regs[Rd] = (res2.result << 16)| res.result;
        if (res.overflow || res2.result) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SSUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 : 0b00;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SSUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 : 0b0;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 1 : 0b0 << 1;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 2 : 0b0 << 2;
        res = signed_op((*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
        CPSR_bits.GE |= res.result >= 0 ? 0b1 << 3 : 0b0 << 3;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SSUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = signed_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] & (0xFFFF << 15)) >> 15, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 : 0b00;
        res = signed_op((*GP_Reg[c_m].regs[Rn] & (0xFFFF << 15)) >> 15, 
                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.result >= 0 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void SXTAB(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = *GP_Reg[c_m].regs[Rn] + sign_extend(BITMASK_CHECK(op, 0x000000FF), 24);
    }
}

void SXTAB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = (BITMASK_CHECK(*GP_Reg[c_m].regs[Rn], 0x0000FFFF) + 
                                sign_extend(BITMASK_CHECK(op, 0x000000FF), 24)) |
                                (BITMASK_CHECK(*GP_Reg[c_m].regs[Rn], 0xFFFF0000) >> 16 + 
                                sign_extend(BITMASK_CHECK(op, 0x00FF0000) >> 16, 24)) << 16;
    }
}

void SXTAH(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = *GP_Reg[c_m].regs[Rn] + sign_extend(BITMASK_CHECK(op, 0x0000FFFF), 16);
    }
}

void SXTB(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = sign_extend(BITMASK_CHECK(op, 0x000000FF), 24);
    }
}

void SXTB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = sign_extend(BITMASK_CHECK(op, 0x000000FF), 24) | 
                                sign_extend(BITMASK_CHECK(op, 0x00FF0000) >> 16, 24);
    }
}

void SXTH(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        word op = *GP_Reg[c_m].regs[Rm] >> (8 * rot);
        *GP_Reg[c_m].regs[Rd] = sign_extend(BITMASK_CHECK(op, 0x0000FFFF), 16);
    }
}

void UADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 : 0b00;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void UADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b1 : 0b0;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        CPSR_bits.GE |= res.overflow == 1 ? 0b1 << 1 : 0b0 << 1;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b1 << 2 : 0b0 << 2;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
        CPSR_bits.GE |= res.overflow == 1 ? 0b1 << 3 : 0b0 << 3;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void UADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 : 0b00;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void UHADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result >> 1;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
    }
}

void UHADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1);
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 7;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 23;
    }
}

void UHADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1);
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
    }
}

void UHSUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result >> 1;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
    }
}

void UHSUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1);
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 7;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 23;
    }
}

void UHSUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1);
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= (res.result >> 1) << 15;
    }
}

void UQADD16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void UQADD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
    }
}

void UQADDSUBX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void UQSUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void UQSUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                        *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                                        (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
    }
}

void UQSUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_saturation_op(*GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                        (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        res = unsigned_saturation_op(   (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                                        *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
    }
}

void USAD8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rs = (bits.OPCODE >> 3) & 0xF;
        byte Rd = (bits.OPCODE >> 12) & 0xF;
        if (Rm == PC || Rd == PC || Rs == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res, res1, res2, res3;
        if (BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF) < 
            BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF)) {
            res = unsigned_op(  BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF), 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF), 8, SUB);
        } else {
            res = unsigned_op(  BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF), 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF), 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8)) {
            res1 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8, 8, SUB);
        } else {
            res1 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8, 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16)) {
            res2 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16, 8, SUB);
        } else {
            res2 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16, 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24)) {
            res3 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24, 8, SUB);
        } else {
            res3 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24, 8, SUB);
        }
        *GP_Reg[c_m].regs[Rd] = res.result + res1.result + res2.result + res3.result;
    }
}

void USADA8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rs = (bits.OPCODE >> 3) & 0xF;
        byte Rn = (bits.OPCODE >> 8) & 0xF;
        byte Rd = (bits.OPCODE >> 12) & 0xF;
        if (Rm == PC || Rd == PC || Rs == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res, res1, res2, res3;
        if (BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF) < 
            BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF)) {
            res = unsigned_op(  BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF), 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF), 8, SUB);
        } else {
            res = unsigned_op(  BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x000000FF), 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x000000FF), 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8)) {
            res1 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8, 8, SUB);
        } else {
            res1 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FF00) >> 8, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x0000FF00) >> 8, 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16)) {
            res2 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16, 8, SUB);
        } else {
            res2 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x00FF0000) >> 16, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0x00FF0000) >> 16, 8, SUB);
        }
        if ((BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24) < 
            (BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24)) {
            res3 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24, 8, SUB);
        } else {
            res3 = unsigned_op( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFF000000) >> 24, 
                                BITMASK_CHECK(*GP_Reg[c_m].regs[Rs], 0xFF000000) >> 24, 8, SUB);
        }
        *GP_Reg[c_m].regs[Rd] = *GP_Reg[c_m].regs[Rn] + res.result + res1.result + res2.result + res3.result;
    }
}

void USAT(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte sh = (bits.OPCODE >> 1) & 0x3;
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        word shift_imm = (bits.OPCODE >> 2) & 0x1F;
        word sat_imm = (bits.OPCODE >> 12) & 0x1F;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        word op;
        if (sh) {
            if (shift_imm == 0) {
                op = arithmetic_shift_right(*GP_Reg[c_m].regs[Rm], 32);
            } else {
                op = arithmetic_shift_right(*GP_Reg[c_m].regs[Rm], shift_imm);
            }
        }
        else op = *GP_Reg[c_m].regs[Rm] << shift_imm;
        Util_Sat_Op_Result res = unsigned_saturation_check(op, sat_imm);
        *GP_Reg[c_m].regs[Rd] = res.result;
        if (res.overflow) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void USAT16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte sh = (bits.OPCODE >> 1) & 0x3;
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        word sat_imm = (bits.OPCODE >> 12) & 0xF;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        Util_Sat_Op_Result res = unsigned_saturation_check( BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0x0000FFFF), 
                                                            sat_imm);
        Util_Sat_Op_Result res2 = unsigned_saturation_check(BITMASK_CHECK(*GP_Reg[c_m].regs[Rm], 0xFFFF0000) >> 16,
                                                            sat_imm);
        *GP_Reg[c_m].regs[Rd] = (res2.result << 16)| res.result;
        if (res.overflow || res2.result) CPSR_bits.Q = 1;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void USUB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b00 : 0b11;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b00 << 2 : 0b11 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void USUB8(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFF, 
                                                *GP_Reg[c_m].regs[Rm] & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b0 : 0b1;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 7) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 7) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 7;
        CPSR_bits.GE |= res.overflow == 1 ? 0b0 << 1 : 0b1 << 1;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b0 << 2 : 0b1 << 2;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 23) & 0xFF, 
                            (*GP_Reg[c_m].regs[Rm] >> 23) & 0xFF, 8, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 23;
        CPSR_bits.GE |= res.overflow == 1 ? 0b0 << 3 : 0b1 << 3;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void USUBADDX(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 10) & 0xF;
        byte Rn = (bits.OPCODE >> 6) & 0xF;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        Util_Sig_Op_Result res = unsigned_op(   *GP_Reg[c_m].regs[Rn] & 0xFFFF, 
                                                (*GP_Reg[c_m].regs[Rm] >> 15) & 0xFFFF, 16, ADD);
        *GP_Reg[c_m].regs[Rd] |= res.result;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 : 0b00;
        res = unsigned_op(  (*GP_Reg[c_m].regs[Rn] >> 15) & 0xFFFF, 
                            *GP_Reg[c_m].regs[Rm] & 0xFFFF, 16, SUB);
        *GP_Reg[c_m].regs[Rd] |= res.result << 15;
        CPSR_bits.GE |= res.overflow == 1 ? 0b11 << 2 : 0b00 << 2;
        PSR_write(*CPSR_Reg.regs, CPSR_bits);
    }
}

void UXTAB(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op2 = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x000000FF;
        *GP_Reg[c_m].regs[Rd] = *GP_Reg[c_m].regs[Rn] + op2;
    }
}

void UXTAB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op2 = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x00FF00FF;
        *GP_Reg[c_m].regs[Rd] = (*GP_Reg[c_m].regs[Rn] & 0x0000FFFF) + (op2 & 0x0000FFFF);
        *GP_Reg[c_m].regs[Rd] |= (((*GP_Reg[c_m].regs[Rn] >> 16) & 0x0000FFFF) + ((op2 >> 16) & 0x000000FF)) << 16;
    }
}

void UXTAH(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte Rn = (bits.OPCODE >> 12) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC || Rn == PC) UNPREDICTABLE();
        word op2 = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x0000FFFF;
        *GP_Reg[c_m].regs[Rd] = *GP_Reg[c_m].regs[Rn] + op2;
    }
}

void UXTB(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x000000FF;
    }
}

void UXTB16(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x00FF00FF;
    }
}

void UXTH(MEI_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)bits.COND, CPSR_bits)) {
        byte Rm = bits.Q;
        byte Rd = (bits.OPCODE >> 8) & 0xF;
        byte rot = (bits.OPCODE >> 6) & 0x3;
        if (Rm == PC || Rd == PC) UNPREDICTABLE();
        *GP_Reg[c_m].regs[Rd] = (*GP_Reg[c_m].regs[Rm] >> (8 * rot)) & 0x0000FFFF;
    }
}