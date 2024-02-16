#include "multiply_extra_load_store.h"

void MELS_write(word* instr, MELS_Bitfield bits) {
    *instr =    bits.Q_4 << 0       | bits.RES_3 << 4   | bits.H_ST << 5    | 
                bits.RES_2 << 6     | bits.Q_3 << 8     | bits.Q_2 << 12    | 
                bits.Q_1 << 16      | bits.L_S << 20    | bits.W_A << 21    |
                bits.B_UN << 22     | bits.U << 23      | bits.P << 24      |
                bits.RES_1 << 25    | bits.COND << 28;
}

MELS_Bitfield MELS_read(const word* instr) {
    MELS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.B_UN = *instr >> 22;
    bits.W_A = *instr >> 21;
    bits.L_S = *instr >> 20;
    bits.Q_1 = *instr >> 16;
    bits.Q_2 = *instr >> 12;
    bits.Q_3 = *instr >> 8;
    bits.RES_2 = *instr >> 6;
    bits.H_ST = *instr >> 5;
    bits.RES_3 = *instr >> 4;
    bits.Q_4 = *instr >> 0;
    return bits;
}

word offset_imm(MELS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    word address;
    word offset;

    switch(bits.W_A)
    {
    case 0b0:
        switch(bits.P)
        {
        case 0b0:
            //if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            address = *GP_Reg[c_m].regs[bits.Q_1];
            offset = (bits.Q_3 << 4) | bits.Q_4;
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.Q_1] += offset;
                else *GP_Reg[c_m].regs[bits.Q_1] -= offset;
            }  
            break;
        case 0b1:
            //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            offset = (bits.Q_3 << 4) | bits.Q_4;
            if (bits.U) address = *GP_Reg[c_m].regs[bits.Q_1] + offset;
            else address = *GP_Reg[c_m].regs[bits.Q_1] - offset;
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch(bits.P)
        {
        case 0b0:
            break;
        case 0b1:
            //if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            offset = (bits.Q_3 << 4) | bits.Q_4;
            if (bits.U) address = *GP_Reg[c_m].regs[bits.Q_1] + offset;
            else address = *GP_Reg[c_m].regs[bits.Q_1] - offset;
            if (check_Cond(bits.COND, CPSR_bits)) *GP_Reg[c_m].regs[bits.Q_1] = address;
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    default:
        UNSUPPORTED();
        break;
    }

    return address;
}

word offset_reg(MELS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    word address;
    word offset;

    switch(bits.W_A)
    {
    case 0b0:
        switch(bits.P)
        {
        case 0b0:
            //if (bits.RM == PC || Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            address = *GP_Reg[c_m].regs[bits.Q_1];
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.Q_1] += *GP_Reg[c_m].regs[bits.Q_4];
                else *GP_Reg[c_m].regs[bits.Q_1] -= *GP_Reg[c_m].regs[bits.Q_4];
            }
            break;
        case 0b1:
            //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            //if (bits.RM == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.Q_1] + *GP_Reg[c_m].regs[bits.Q_4];
            else address = *GP_Reg[c_m].regs[bits.Q_1] - *GP_Reg[c_m].regs[bits.Q_4];
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch(bits.P)
        {
        case 0b0:
            break;
        case 0b1:
            //if (bits.RM == PC || Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.Q_1] + *GP_Reg[c_m].regs[bits.Q_4];
            else address = *GP_Reg[c_m].regs[bits.Q_1] - *GP_Reg[c_m].regs[bits.Q_4];
            if (check_Cond(bits.COND, CPSR_bits)) *GP_Reg[c_m].regs[bits.Q_1] = address;
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    default:
        UNSUPPORTED();
        break;
    }

    return address;
}

void LDRD(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void LDREX(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void LDRH(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void LDRSB(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void LDRSH(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void MLA(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void MUL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void SMLAL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void SMULL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void STRD(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void STREX(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void STRH(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void SWP(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void SWPB(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void UMAAL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void UMLAL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}

void UMULL(MELS_Bitfield, word) {
    NOT_IMPLEMENTED();
}