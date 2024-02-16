#include "data_processing.h"

void Data_Proc_write(word* instr, Data_Proc_Bitfield bits) {
    *instr =    bits.SHIFT << 0     | bits.RD << 12     | bits.RN << 16 |
                bits.S << 20        | bits.OPCODE << 21 | bits.I << 25  |
                bits.RES_1 << 26    | bits.COND << 28;
}

Data_Proc_Bitfield Data_Proc_read(const word* instr) {
    Data_Proc_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 26;
    bits.I = *instr >> 25;
    bits.OPCODE = *instr >> 21;
    bits.S = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.RD = *instr >> 12;
    bits.SHIFT =  *instr >> 0;
    return bits;
}

void ADC(const word *instr) {
    Data_Proc_Bitfield Data_bits = Data_Proc_read(instr);
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    if (check_Cond((Cond_Field)Data_bits.COND, CPSR_bits)) {
        *GP_Reg[c_m].regs[Data_bits.RD] = *GP_Reg[c_m].regs[Data_bits.RN] + Data_bits.SHIFT + CPSR_bits.C;
        if (Data_bits.S && Data_bits.RD == PC) {
            TRY_RESTORE_SPSR();
        } else if (Data_bits.S) {
            CPSR_bits.N = *GP_Reg[c_m].regs[Data_bits.RD] >> 31;
            CPSR_bits.Z = *GP_Reg[c_m].regs[Data_bits.RD] == 0 ? 1 : 0;
            CPSR_bits.C = 0; // TO BE ADJUSTED
            CPSR_bits.V = 0; // TO BE ADJUSTED
            PSR_write(*CPSR_Reg.regs, CPSR_bits);
        }
    }
}

Shift_Result shift_32bit_imm(Data_Proc_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    word rot_imm = (bits.SHIFT >> 8) << 1;
    word immed_8 = bits.SHIFT & 0xFF;

    //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION

    Shift_Result out;

    out.shifter_operand = (immed_8 >> rot_imm) | (immed_8 << ((sizeof(word) * 8) - rot_imm));
    if (rot_imm == 0) out.shift_carry_out = CPSR_bits.C;
    else out.shift_carry_out = out.shifter_operand >> 31;

    return out;
}

Shift_Result shift_imm(Data_Proc_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    byte s_imm = bits.SHIFT >> 7;
    byte op = (bits.SHIFT & (0b11 << 5)) >> 5;
    byte Rm = bits.SHIFT & 0xF;

    //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
    //if (Rm == PC) *GP_Reg[c_m].regs[Rm] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION

    Shift_Result out;

    switch (op)
    {
    case 0b00:
        if (s_imm == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = CPSR_bits.C;
        } else {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] << s_imm;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << ((sizeof(word) * 8) - s_imm))) >> ((sizeof(word) * 8) - s_imm);
        }
        break;
    case 0b01:
        if (s_imm == 0) {
            out.shifter_operand = 0;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
        } else {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] >> s_imm;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (s_imm - 1))) >> (s_imm - 1);
        }
        break;
    case 0b10:
        if (s_imm == 0) {
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
            if (((*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31) == 0) {
                out.shifter_operand = 0;
            } else {
                out.shifter_operand = 0xFFFFFFFF;
            }
        } else {
            word msb = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] >> s_imm;
            out.shifter_operand |= msb << 31;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (s_imm - 1))) >> (s_imm - 1);
        }
    case 0b11:
        if (s_imm == 0) {
            out.shifter_operand = (CPSR_bits.C << 31) | (*GP_Reg[c_m].regs[Rm] >> 1);
            out.shift_carry_out = *GP_Reg[c_m].regs[Rm] & 0b1;
        } else {
            out.shifter_operand = (*GP_Reg[c_m].regs[Rm] >> s_imm) | (*GP_Reg[c_m].regs[Rm] << ((sizeof(word) * 8) - s_imm));
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (s_imm - 1))) >> (s_imm - 1);
        }
        break;
    default:
        UNSUPPORTED();
        break;
    }

    return out;
}

Shift_Result shift_reg(Data_Proc_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    byte Rs = bits.SHIFT >> 8;
    byte op = (bits.SHIFT & (0b11 << 5)) >> 5;
    byte Rm = bits.SHIFT & 0xF;

    //if (bits.RN == PC || bits.RD == PC || Rm == PC || Rs == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION

    Shift_Result out;

    switch (op)
    {
    case 0b00:
        word off = *GP_Reg[c_m].regs[Rs] & 0xFF;
        if (off == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = CPSR_bits.C;
        } else if (off < 32) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] << off;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << ((sizeof(word) * 8) - off))) >> ((sizeof(word) * 8) - off);
        } else if (off == 32) {
            out.shifter_operand = 0;
            out.shift_carry_out = *GP_Reg[c_m].regs[Rm] & 0b1;
        } else {
            out.shifter_operand = 0;
            out.shift_carry_out = 0;
        }
        break;
    case 0b01:
        word off = *GP_Reg[c_m].regs[Rs] & 0xFF;
        if (off == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = CPSR_bits.C;
        } else if (off < 32) {
            word msb = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] >> off;
            out.shifter_operand |= msb << 31;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (off - 1))) >> (off - 1);
        } else if (off == 32) {
            out.shifter_operand = 0;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
        } else {
            out.shifter_operand = 0;
            out.shift_carry_out = 0;
        }
        break;
    case 0b10:
        word off = *GP_Reg[c_m].regs[Rs] & 0xFF;
        if (off == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = CPSR_bits.C;
        } else if (off < 32) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm] >> off;
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (off - 1))) >> (off - 1);
        } else {
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
            if (((*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31) == 0) {
                out.shifter_operand = 0;
            } else {
                out.shifter_operand = 0xFFFFFFFF;
            }
        }
        break;
    case 0b11:
        word off = *GP_Reg[c_m].regs[Rs] & 0xFF;
        word off2 = *GP_Reg[c_m].regs[Rs] & 0b11111;
        if (off == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = CPSR_bits.C;
        } else if (off2 == 0) {
            out.shifter_operand = *GP_Reg[c_m].regs[Rm];
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << 31)) >> 31;
        } else {
            out.shifter_operand = (*GP_Reg[c_m].regs[Rm] >> off2) | (*GP_Reg[c_m].regs[Rm] << ((sizeof(word) * 8) - off2));
            out.shift_carry_out = (*GP_Reg[c_m].regs[Rm] & (0b1 << (off2 - 1))) >> (off2 - 1);
        }
        break;
    default:
        UNSUPPORTED();
        break;
    }

    return out;
}