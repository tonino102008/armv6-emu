#include "coproc_register.h"

COPROC_Reg CP15_Reg;

/**
 * @brief Constructor for CP15 register (global variable).
 * 
 */
void COPROC_Reg_ctor() {
    CP15_Reg.reg_size = NO_COPROC_REG;
    CP15_Reg.regs = malloc(CP15_Reg.reg_size * sizeof(word*));
    CP15_Reg.regs[ID] = calloc(8, sizeof(word));
    CP15_Reg.regs[SC_BITS] = calloc(8, sizeof(word));
    CP15_Reg.regs[PTC] = calloc(4, sizeof(word));
    for (COPROC_Reg_Type i = DAC; i < C_WB_C; i++) CP15_Reg.regs[i] = calloc(1, sizeof(word));
    CP15_Reg.regs[C_WB_C] = calloc(4, sizeof(word));
    CP15_Reg.regs[TLB_C] = calloc(1, sizeof(word));
    CP15_Reg.regs[C_L] = calloc(6, sizeof(word));
    CP15_Reg.regs[TLB_L] = calloc(1, sizeof(word));
    CP15_Reg.regs[DMA_C] = calloc(16, sizeof(word));
    CP15_Reg.regs[RES_2] = calloc(1, sizeof(word));
    CP15_Reg.regs[PROC_ID] = calloc(1, sizeof(word));
    for (COPROC_Reg_Type i = RES_3; i < NO_COPROC_REG; i++) CP15_Reg.regs[i] = calloc(1, sizeof(word));
    // REG 0 SHOULD BE READ ONLY
    CP15_Reg.regs[ID][0b000] = 0x41 << 24; // ARM ID IMPLEMENTOR CODE
    CP15_Reg.regs[ID][0b000] |= 0x7 << 16; // ARMv6 ARCHITECTURE CODE
    CP15_Reg.regs[ID][0b000] |= 0x1 << 12; // NECESSARY

    CP15_Reg.regs[ID][0b001] = 0b000 << 29; // CACHE TYPE REGISTER

    CP15_Reg.regs[ID][0b010] = 0b000 << 29; // TCM TYPE REGISTER

    CP15_Reg.regs[ID][0b011] = 0b000 << 29; // TLB TYPE REGISTER

    CP15_Reg.regs[ID][0b100] = 0b000 << 29; // MPU TYPE REGISTER

    CP15_Reg.regs[ID][0b101] = CP15_Reg.regs[ID][0b000]; // NECESSARY
    CP15_Reg.regs[ID][0b111] = CP15_Reg.regs[ID][0b000]; // NECESSARY

    CP15_Reg.regs[SC_BITS][0b000] = 0b111 << 4;
    CP15_Reg.regs[SC_BITS][0b000] |= 0b1 << 16;
    CP15_Reg.regs[SC_BITS][0b000] |= 0b0 << 17;
    CP15_Reg.regs[SC_BITS][0b000] |= 0b1 << 18;
    CP15_Reg.regs[SC_BITS][0b000] |= 0b0 << 19;
    CP15_Reg.regs[SC_BITS][0b000] |= 0b0 << 20;
}

/**
 * @brief Destructor for CP15 register (global variable).
 * 
 */
void COPROC_Reg_dtor() {
    for (byte i = 0; i < CP15_Reg.reg_size; i++) free(CP15_Reg.regs[i]);
    free(CP15_Reg.regs);
}