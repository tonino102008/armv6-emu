#include "exception.h"

void handle_exception(ARM_Exception_Type exc) {
    switch (exc) {
        case RESET:
            handle_reset();
            break;
        case UNDEFINED_INSTR:
            handle_undefined_instruction();
            break;
        case SOFTWARE_INT:
            handle_software_interrupt();
            break;
        case PREFETCH_ABORT:
            handle_prefetch_abort();
            break;
        case DATA_ABORT:
            handle_data_abort();
            break;
        case IRQ:
            handle_irq();
            break;
        case FIQ:
            handle_fiq();
            break;
        default:
            UNSUPPORTED_EXCEPTION();
    }
}

void handle_reset() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    CPSR_bits.M = proc_mode[SUPERVISOR];
    CPSR_bits.T = 0;
    CPSR_bits.F = 1;
    CPSR_bits.I = 1;
    CPSR_bits.A = 1;
    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
        *GP_Reg[SUPERVISOR].regs[PC] = 0xFFFF0000;
    } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
        *GP_Reg[SUPERVISOR].regs[PC] = 0x00000000;
    } else UNSUPPORTED_EXCEPTION_ADRRESS();
}

void handle_undefined_instruction() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[UNDEFINED].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    PSR_write(*SPSR_Reg[UNDEFINED].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[UNDEFINED];
    CPSR_bits.T = 0;

    CPSR_bits.I = 1;

    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
        *GP_Reg[UNDEFINED].regs[PC] = 0xFFFF0004;
    } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
        *GP_Reg[UNDEFINED].regs[PC] = 0x00000004;
    } else UNSUPPORTED_EXCEPTION_ADRRESS();
    // RETURN TO EXECUTION FLOW
}

void handle_software_interrupt() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[SUPERVISOR].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    PSR_write(*SPSR_Reg[SUPERVISOR].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[SUPERVISOR];
    CPSR_bits.T = 0;

    CPSR_bits.I = 1;

    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
        *GP_Reg[SUPERVISOR].regs[PC] = 0xFFFF0008;
    } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
        *GP_Reg[SUPERVISOR].regs[PC] = 0x00000008;
    } else UNSUPPORTED_EXCEPTION_ADRRESS();
    // RETURN TO EXECUTION FLOW
}

void handle_prefetch_abort() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[ABORT].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    PSR_write(*SPSR_Reg[ABORT].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[ABORT];
    CPSR_bits.T = 0;

    CPSR_bits.I = 1;
    CPSR_bits.A = 1;
    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
        *GP_Reg[ABORT].regs[PC] = 0xFFFF000C;
    } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
        *GP_Reg[ABORT].regs[PC] = 0x0000000C;
    } else UNSUPPORTED_EXCEPTION_ADRRESS();
    // RETURN TO EXECUTION FLOW
}

void handle_data_abort() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[ABORT].regs[LR] = *GP_Reg[c_m].regs[PC] + 8;
    PSR_write(*SPSR_Reg[ABORT].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[ABORT];
    CPSR_bits.T = 0;

    CPSR_bits.I = 1;
    CPSR_bits.A = 1;
    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
        *GP_Reg[ABORT].regs[PC] = 0xFFFF0010;
    } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
        *GP_Reg[ABORT].regs[PC] = 0x00000010;
    } else UNSUPPORTED_EXCEPTION_ADRRESS();
    // RETURN TO EXECUTION FLOW
}

void handle_irq() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[IRQ].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    PSR_write(*SPSR_Reg[IRQ].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[IRQ];
    CPSR_bits.T = 0;

    CPSR_bits.I = 1;
    CPSR_bits.A = 1;
    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 24) & 0b1 == 0) {
        if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
            *GP_Reg[IRQ].regs[PC] = 0xFFFF0018;
        } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
            *GP_Reg[IRQ].regs[PC] = 0x00000018;
        } else UNSUPPORTED_EXCEPTION_ADRRESS();
    } else {
        UNSUPPORTED_VECTORED_INTERRUPTS();
    }
    // RETURN TO EXECUTION FLOW
}

void handle_fiq() {
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    Proc_Mode c_m = curr_Proc_Mode(CPSR_bits.M);

    *GP_Reg[FIQ].regs[LR] = *GP_Reg[c_m].regs[PC] + 4;
    PSR_write(*SPSR_Reg[FIQ].regs, CPSR_bits);

    CPSR_bits.M = proc_mode[FIQ];
    CPSR_bits.T = 0;
    CPSR_bits.F = 1;
    CPSR_bits.I = 1;
    CPSR_bits.A = 1;
    CPSR_bits.E = (CP15_Reg.regs[SC_BITS][0b000] >> 25) & 0b1; // EE bit: EXCEPTION ENDIANNES BIT
    PSR_write(*CPSR_Reg.regs, CPSR_bits);

    if ((CP15_Reg.regs[SC_BITS][0b000] >> 24) & 0b1 == 0) {
        if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 1) {
            *GP_Reg[FIQ].regs[PC] = 0xFFFF001C;
        } else if ((CP15_Reg.regs[SC_BITS][0b000] >> 13) & 0b1 == 0) {
            *GP_Reg[FIQ].regs[PC] = 0x0000001C;
        } else UNSUPPORTED_EXCEPTION_ADRRESS();
    } else {
        UNSUPPORTED_VECTORED_INTERRUPTS();
    }
    // RETURN TO EXECUTION FLOW
}