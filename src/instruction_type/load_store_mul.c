#include "load_store_mul.h"

void LS_Mul_write(word* instr, LS_Mul_Bitfield bits) {
    *instr =    bits.RL << 0    | bits.RN << 16     | bits.L << 20      |
                bits.W << 21    | bits.S << 22      | bits.U << 23      |
                bits.P << 24    | bits.RES_1 << 25  | bits.COND << 28;
}

LS_Mul_Bitfield LS_Mul_read(const word* instr) {
    LS_Mul_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.S = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.RL = *instr >> 0;
    return bits;
}

Addr_Result incr_decr(LS_Mul_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    Addr_Result out;

    // PAY ATTENTION TO S AND L FLAGS TO DETERMINE IF UNPREDICTABLE()
    // if (bits.RL == 0) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION

    switch (bits.U)
    {
    case 0b0:
        switch (bits.P)
        {
        case 0b0:
            out.start_address = *GP_Reg[c_m].regs[bits.RN] - (no_set_bits(bits.RL) * 4) + 4;
            out.end_address = *GP_Reg[c_m].regs[bits.RN];
            if (check_Cond(bits.COND, CPSR_bits) && bits.W) {
                *GP_Reg[c_m].regs[bits.RN] -= (no_set_bits(bits.RL) * 4);
            }
            break;
        case 0b1:
            out.start_address = *GP_Reg[c_m].regs[bits.RN] - (no_set_bits(bits.RL) * 4);
            out.end_address = *GP_Reg[c_m].regs[bits.RN] - 4;
            if (check_Cond(bits.COND, CPSR_bits) && bits.W) {
                *GP_Reg[c_m].regs[bits.RN] -= (no_set_bits(bits.RL) * 4);
            }
            break;
        default:
            UNSUPPORTED();
            break;
        }
        break;
    case 0b1:
        switch (bits.P)
        {
        case 0b0:
            out.start_address = *GP_Reg[c_m].regs[bits.RN];
            out.end_address = *GP_Reg[c_m].regs[bits.RN] + (no_set_bits(bits.RL) * 4) - 4;
            if (check_Cond(bits.COND, CPSR_bits) && bits.W) {
                *GP_Reg[c_m].regs[bits.RN] += (no_set_bits(bits.RL) * 4);
            }
            break;
        case 0b1:
            out.start_address = *GP_Reg[c_m].regs[bits.RN] + 4;
            out.end_address = *GP_Reg[c_m].regs[bits.RN] + (no_set_bits(bits.RL) * 4);
            if (check_Cond(bits.COND, CPSR_bits) && bits.W) {
                *GP_Reg[c_m].regs[bits.RN] += (no_set_bits(bits.RL) * 4);
            }
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
    
    return out;
}

byte no_set_bits(hword n) {
    byte out = 0;

    while (n) {
        n &= (n - 1);
        out += 1;
    }

    return out;
}

void LDM(LS_Mul_Bitfield, Addr_Result) { // TRIPLE IMPLEMENTATION
    NOT_IMPLEMENTED();
}

void STM(LS_Mul_Bitfield, Addr_Result) { // DOUBLE IMPLEMENTATION
    NOT_IMPLEMENTED();
}