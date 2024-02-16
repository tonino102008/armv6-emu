#include "coproc_load_store.h"

void Coproc_LS_write(word* instr, Coproc_LS_Bitfield bits) {
    *instr =    bits.OFF << 0       | bits.CP_NO << 8   | bits.CRD << 12    |
                bits.RN << 16       | bits.L << 20      | bits.W << 21      |
                bits.N << 22        | bits.U << 23      | bits.P << 24      |
                bits.RES_1 << 25    | bits.COND << 28;
}

Coproc_LS_Bitfield Coproc_LS_read(const word* instr) {
    Coproc_LS_Bitfield bits;
    bits.COND = *instr >> 28;
    bits.RES_1 = *instr >> 25;
    bits.P = *instr >> 24;
    bits.U = *instr >> 23;
    bits.N = *instr >> 22;
    bits.W = *instr >> 21;
    bits.L = *instr >> 20;
    bits.RN = *instr >> 16;
    bits.CRD = *instr >> 12;
    bits.CP_NO = *instr >> 8;
    bits.OFF = *instr >> 0;
    return bits;
}

Addr_Result offset_imm(Coproc_LS_Bitfield bits) {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    Addr_Result out;
    word address;

    switch (bits.W)
    {
    case 0b0:
        break;
        switch(bits.P)
        {
        case 0b0:
            //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            if (check_Cond(bits.COND, CPSR_bits)) {
                out.start_address = *GP_Reg[c_m].regs[bits.RN];
                address = out.start_address;
                while (Coproc_sig(bits.CP_NO)) { // FUNCTION TO BE DEFINED: COMM BETWEEN CPU AND COPROCESSOR
                    address += 4;
                }
                out.end_address = address;
            }
            break;
        case 0b1:
            //if (bits.RN == PC) *GP_Reg[c_m].regs[bits.RN] += 0x8; TO BE PLACED IN THE ACTUAL OPERATION
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) address = *GP_Reg[c_m].regs[bits.RN] + (bits.OFF * 4);
                else address = *GP_Reg[c_m].regs[bits.RN] - (bits.OFF * 4);
                out.start_address = address;
                while (Coproc_sig(bits.CP_NO)) { // FUNCTION TO BE DEFINED: COMM BETWEEN CPU AND COPROCESSOR
                    address += 4;
                }
                out.end_address = address;
            }
            break;
        default:
            UNPREDICTABLE();
            break;
        }
    case 0b1:
        switch(bits.P)
        {
        case 0b0:
            //if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (check_Cond(bits.COND, CPSR_bits)) {
                out.start_address = *GP_Reg[c_m].regs[bits.RN];
                if (bits.U) *GP_Reg[c_m].regs[bits.RN] += (bits.OFF * 4);
                else *GP_Reg[c_m].regs[bits.RN] -= (bits.OFF * 4);
                address = out.start_address;
                while (Coproc_sig(bits.CP_NO)) { // FUNCTION TO BE DEFINED: COMM BETWEEN CPU AND COPROCESSOR
                    address += 4;
                }
                out.end_address = address;
            }
            break;
        case 0b1:
            //if (bits.RN == PC) UNPREDICTABLE(); TO BE PLACED IN THE ACTUAL OPERATION
            if (check_Cond(bits.COND, CPSR_bits)) {
                if (bits.U) *GP_Reg[c_m].regs[bits.RN] += (bits.OFF * 4);
                else *GP_Reg[c_m].regs[bits.RN] -= (bits.OFF * 4);
                out.start_address = *GP_Reg[c_m].regs[bits.RN];
                address = out.start_address;
                while (Coproc_sig(bits.CP_NO)) { // FUNCTION TO BE DEFINED: COMM BETWEEN CPU AND COPROCESSOR
                    address += 4;
                }
                out.end_address = address;
            }
            break;
        default:
            UNPREDICTABLE();
            break;
        }
        break;
    default:
        UNPREDICTABLE();
        break;
    }

    return out;
}