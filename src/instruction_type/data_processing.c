#include "data_processing.h"

void Data_Proc_write(word* instr, Data_Proc_Bitfield bits) {
    *instr =    bits.SHIFT << 0     | bits.RD << 12     | bits.RN << 16     |
                bits.S << 20        | bits.OPCODE << 21 | bits.I << 25      |
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