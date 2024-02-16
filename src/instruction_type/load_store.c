#include "load_store.h"

void LS_write(word* instr, LS_Bitfield bits) {
    *instr =    bits.OFF << 0     | bits.RD << 12     | bits.RN << 16 | 
                bits.L << 20        |bits.W << 21       | bits.B << 22  | 
                bits.U << 23        |bits.P << 24       | bits.I << 25  |
                bits.RES_1 << 26    | bits.COND << 28;
}

LS_Bitfield LS_read(const word* instr) {
    LS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 26;
    bits.I = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.B = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.RD = *instr >> 12;
    bits.OFF = *instr >> 0;
    return bits;
}

word offset_imm(LS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    word address;

    switch (bits.W)
    {
    case 0b0:
        switch (bits.P)
        {
        case 0b0:
            // if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            address = *GP_Reg[c_m].regs[bits.RN];
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.RN] += bits.OFF;
                else *GP_Reg[c_m].regs[bits.RN] -= bits.OFF;
            }
            break;
        case 0b1:
            // if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + bits.OFF;
            else address = *GP_Reg[c_m].regs[bits.RN] - bits.OFF;
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch (bits.P)
        {
        case 0b1:
            // if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + bits.OFF;
            else address = *GP_Reg[c_m].regs[bits.RN] - bits.OFF;
            if (check_Cond(bits.COND, CPSR_bits)) *GP_Reg[c_m].regs[bits.RN] = address;
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

word offset_reg(LS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    byte Rm = bits.OFF & 0xF;

    word address;

    switch (bits.W)
    {
    case 0b0:
        switch (bits.P)
        {
        case 0b0:
            // if (Rn == PC || Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            address = *GP_Reg[c_m].regs[bits.RN];
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.RN] += *GP_Reg[c_m].regs[Rm];
                else *GP_Reg[c_m].regs[bits.RN] -= *GP_Reg[c_m].regs[Rm];
            }
            break;
        case 0b1:
            // if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            // if (Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + *GP_Reg[c_m].regs[Rm];
            else address = *GP_Reg[c_m].regs[bits.RN] - *GP_Reg[c_m].regs[Rm];
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch (bits.P)
        {
        case 0b1:
            // if (Rn == PC || Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + *GP_Reg[c_m].regs[Rm];
            else address = *GP_Reg[c_m].regs[bits.RN] - *GP_Reg[c_m].regs[Rm];
            if (check_Cond(bits.COND, CPSR_bits)) *GP_Reg[c_m].regs[bits.RN] = address;
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

word offset_scaled_reg(LS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    byte s_imm = bits.OFF >> 7;
    byte op = (bits.OFF & (0b11 << 5)) >> 5;
    byte Rm = bits.OFF & 0xF;

    word address;
    word index;

    switch (bits.W)
    {
    case 0b0:
        switch (bits.P)
        {
        case 0b0:
            // if (Rn == PC | Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            address = *GP_Reg[c_m].regs[bits.RN];
            switch (op) {
            case 0b00:
                index = *GP_Reg[c_m].regs[Rm] << s_imm;
                break;
            case 0b01:
                if (s_imm == 0) index = 0;
                else index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                break;
            case 0b10:
                if (s_imm == 0) {
                    if ((*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31) index = 0xFFFFFFFF;
                    else index = 0;
                } else {
                    word msb = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
                    index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                    index |= msb << 31;
                }
                break;
            case 0b11:
                if (s_imm == 0) index = (CPSR_bits.C << 31) | (*GP_Reg[c_m].regs[Rm] >> 1);
                else index = (*GP_Reg[c_m].regs[Rm] >> s_imm) | (*GP_Reg[c_m].regs[Rm] << ((sizeof(word) * 8) - s_imm));
                break;
            default:
                UNSUPPORTED();
                break;
            }
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.RN] += index;
                else *GP_Reg[c_m].regs[bits.RN] -= index;
            }
            break;
        case 0b1:
            // if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            // if (Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            switch (op) {
            case 0b00:
                index = *GP_Reg[c_m].regs[Rm] << s_imm;
                break;
            case 0b01:
                if (s_imm == 0) index = 0;
                else index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                break;
            case 0b10:
                if (s_imm == 0) {
                    if ((*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31) index = 0xFFFFFFFF;
                    else index = 0;
                } else {
                    word msb = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
                    index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                    index |= msb << 31;
                }
                break;
            case 0b11:
                if (s_imm == 0) index = (CPSR_bits.C << 31) | (*GP_Reg[c_m].regs[Rm] >> 1);
                else index = (*GP_Reg[c_m].regs[Rm] >> s_imm) | (*GP_Reg[c_m].regs[Rm] << ((sizeof(word) * 8) - s_imm));
                break;
            default:
                UNSUPPORTED();
                break;
            }
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + index;
            else address = *GP_Reg[c_m].regs[bits.RN] - index;
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch (bits.P)
        {
        case 0b1:
            // if (Rn == PC || Rm == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            switch (op) {
            case 0b00:
                index = *GP_Reg[c_m].regs[Rm] << s_imm;
                break;
            case 0b01:
                if (s_imm == 0) index = 0;
                else index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                break;
            case 0b10:
                if (s_imm == 0) {
                    if ((*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31) index = 0xFFFFFFFF;
                    else index = 0;
                } else {
                    word msb = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
                    index = *GP_Reg[c_m].regs[Rm] >> s_imm;
                    index |= msb << 31;
                }
                break;
            case 0b11:
                if (s_imm == 0) index = (CPSR_bits.C << 31) | (*GP_Reg[c_m].regs[Rm] >> 1);
                else index = (*GP_Reg[c_m].regs[Rm] >> s_imm) | (*GP_Reg[c_m].regs[Rm] << ((sizeof(word) * 8) - s_imm));
                break;
            default:
                UNSUPPORTED();
                break;
            }
            if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + index;
            else address = *GP_Reg[c_m].regs[bits.RN] - index;
            if (check_Cond(bits.COND, CPSR_bits)) *GP_Reg[c_m].regs[bits.RN] = address;
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