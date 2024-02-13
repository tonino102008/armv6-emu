#include "register.h"

#include "stdio.h"
#include "stdlib.h"

byte proc_mode[NO_PROC_MODE] = {0b10000, 0b10001, 0b10010, 0b10011, 0b10111, 0b11011, 0b11111}; // SORTED

ARM_Reg* GP_Reg;
ARM_Reg CPSR_Reg;
ARM_Reg* SPSR_Reg;

/**
 * @brief Constructor for ARM registers (global variables).
 * 
 */
void ARM_Reg_ctor() {

    GP_Reg = malloc(NO_PROC_MODE * sizeof(ARM_Reg));
    for (Proc_Mode p_m = USER; p_m < NO_PROC_MODE; p_m++) {
        GP_Reg[p_m].reg_size = NO_GP_REG;
    }

    GP_Reg[USER].regs = malloc(GP_Reg[USER].reg_size * sizeof(word*));
    for (byte i = 0; i < GP_Reg[USER].reg_size; i++) GP_Reg[USER].regs[i] = calloc(1, sizeof(word));

    GP_Reg[FIQ].regs = malloc(GP_Reg[USER].reg_size * sizeof(word*));
    byte stop_ref_fiq = 8;
    for (byte i = 0; i < stop_ref_fiq; i++) GP_Reg[FIQ].regs[i] = GP_Reg[USER].regs[i];
    for (byte i = stop_ref_fiq; i < PC; i++) GP_Reg[FIQ].regs[i] = calloc(1, sizeof(word));
    GP_Reg[FIQ].regs[PC] = GP_Reg[USER].regs[PC];

    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) {
        GP_Reg[p_m].regs = malloc(GP_Reg[p_m].reg_size * sizeof(word*));
        for (byte i = 0; i < SP; i++) GP_Reg[p_m].regs[i] = GP_Reg[USER].regs[i];
        for (byte i = SP; i < PC; i++) GP_Reg[p_m].regs[SP] = calloc(1, sizeof(word));
        GP_Reg[p_m].regs[PC] = GP_Reg[USER].regs[PC];
    }

    GP_Reg[SYSTEM].regs = GP_Reg[USER].regs;

    CPSR_Reg.reg_size = NO_CPSR_REG;
    CPSR_Reg.regs = malloc(CPSR_Reg.reg_size * sizeof(word*));
    *CPSR_Reg.regs = calloc(1, sizeof(word));

    SPSR_Reg = malloc(NO_PROC_MODE * sizeof(ARM_Reg));
    SPSR_Reg[USER].reg_size = 0;
    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) SPSR_Reg[p_m].reg_size = NO_SPSR_REG;
    SPSR_Reg[SYSTEM].reg_size = 0;
    SPSR_Reg[USER].regs = NULL;
    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) {
        SPSR_Reg[p_m].regs = malloc(SPSR_Reg[p_m].reg_size * sizeof(word*));
        *SPSR_Reg[p_m].regs = calloc(1, sizeof(word));
    }
    SPSR_Reg[SYSTEM].regs = NULL;

}

/**
 * @brief Destructor for ARM registers (global variables).
 * 
 */
void ARM_Reg_dtor() {

    for (byte i = 0; i < GP_Reg[USER].reg_size; i++) free(GP_Reg[USER].regs[i]);

    byte stop_ref_fiq = 8;
    for (byte i = stop_ref_fiq; i < PC; i++) free(GP_Reg[FIQ].regs[i]);

    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) {
        for (byte i = SP; i < PC; i++) free(GP_Reg[p_m].regs[i]);
    }
    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) free(GP_Reg[p_m].regs);
    free(GP_Reg[FIQ].regs);
    free(GP_Reg[USER].regs);
    free(GP_Reg);

    free(*CPSR_Reg.regs);
    free(CPSR_Reg.regs);

    for (Proc_Mode p_m = IRQ; p_m < SYSTEM; p_m++) {
        free(*SPSR_Reg[p_m].regs);
        free(SPSR_Reg[p_m].regs);
    }
    free(SPSR_Reg);

}

/**
 * @brief Compare Function to find a given Processor Mode.
 * 
 * @param v1 First Processor Mode.
 * @param v2 Second Processor Mode.
 * @return int Distance between the compared values.
 */
int cmp_Proc_Mode(const void* v1, const void* v2) {
    return (*(byte*)v1 - *(byte*)v2);
}

/**
 * @brief Find Current Processor Mode.
 * 
 * @param mode inary representation of Current Mode.
 * @return Proc_Mode Current Processor Mode.
 */
Proc_Mode curr_Proc_Mode(byte mode) {
    byte* curr_mode = bsearch(&mode, proc_mode, NO_PROC_MODE, sizeof(byte), cmp_Proc_Mode);
    if (curr_mode == NULL) ERROR_MODE();
    return (Proc_Mode)(curr_mode - proc_mode);
}

/**
 * @brief Check if current processor mode has a SPSR.
 * 
 * @return byte 1 = True - 0 = False
 */
byte has_SPSR() {
    byte cur_mode = **CPSR_Reg.regs & 0b11111;
    if (cur_mode == proc_mode[USER] || cur_mode == proc_mode[SYSTEM]) return 0;
    return 1;
}

/**
 * @brief Write configuration to a generic PSR (CPSR/SPSR) from bitfield.
 * 
 * @param reg Generic PSR register.
 * @param bits Bitfield to write.
 */
void PSR_write(word* reg, PSR_Bitfield bits) {
    *reg =  bits.M << 0         | bits.T << 5       | bits.F << 6       |
            bits.I << 7         | bits.A << 8       | bits.E << 9       |
            bits.RES_3 << 10    | bits.GE << 16     | bits.RES_2 << 20  |
            bits.J << 24        | bits.RES_1 << 25  | bits.Q << 27      |
            bits.V << 28        | bits.C << 29      | bits.Z << 30      |
            bits.N << 31;
}

/**
 * @brief Read configuration from generic PSR (CPSR/SPSR) and dump it into bitfield.
 * 
 * @param reg Generic PSR register.
 * @return PSR_Bitfield Bitfield representation of PSR.
 */
PSR_Bitfield PSR_read(const word* reg) {
    PSR_Bitfield bits;
    bits.N = *reg >> 31;
    bits.Z = *reg >> 30;
    bits.C = *reg >> 29;
    bits.V = *reg >> 28;
    bits.Q = *reg >> 27;
    bits.RES_1 = *reg >> 25;
    bits.J = *reg >> 24;
    bits.RES_2 = *reg >> 20;
    bits.GE = *reg >> 16;
    bits.RES_3 = *reg >> 10;
    bits.E = *reg >> 9;
    bits.A = *reg >> 8;
    bits.I = *reg >> 7;
    bits.F = *reg >> 6;
    bits.T = *reg >> 5;
    bits.M = *reg >> 0;
    return bits;
}