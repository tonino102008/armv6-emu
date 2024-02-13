#include "instruction.h"

/**
 * @brief Find instruction type.
 * 
 * @param instr Instruction to be executed.
 * @return Op_Field Instruction Type.
 */
Op_Field check_Op_Type(const word* instr) {

    byte cond = (*instr & (0xF << 28)) >> 28;

    switch (cond) {
        case 0b1111:
            return (Op_Field)UNCI;
        default:
            byte b27_26_25 = (*instr & (0b111 << 25)) >> 25;
            switch (b27_26_25) {
                case 0b000:
                    {
                    byte b4 = (*instr & (0b1 << 4)) >> 4;
                    switch (b4) {
                        case 0b0:
                            byte b24_23 = (*instr & (0b11 << 23)) >> 23;
                            byte b20 = (*instr & (0b1 << 20)) >> 20;
                            if (b24_23 == 0b10 && b20 == 0b0) return (Op_Field)MI_1;
                            else return (Op_Field)DPIS;
                        case 0b1:
                            byte b7 = (*instr & (0b1 << 7)) >> 7;
                            switch (b7) {
                                case 0b0:
                                    byte b24_23 = (*instr & (0b11 << 23)) >> 23;
                                    byte b20 = (*instr & (0b1 << 20)) >> 20;
                                    if (b24_23 == 0b10 && b20 == 0b0) return (Op_Field)MI_2;
                                    else return (Op_Field)DPRS;
                                case 0b1:
                                    return (Op_Field)MELS;
                                default:
                                    UNSUPPORTED();
                            }
                        default:
                            UNSUPPORTED();
                    }
                    }
                case 0b001:
                    byte b24_23 = (*instr & (0b11 << 23)) >> 23;
                    byte b20 = (*instr & (0b1 << 20)) >> 20;
                    if (b24_23 == 0b10 && b20 == 0b0) {
                        byte b21 = (*instr & (0b1 << 21)) >> 21;
                        switch (b21) {
                            case 0b0:
                                return (Op_Field)UNDI;
                            case 0b1:
                                return (Op_Field)MISR;
                            default:
                                UNSUPPORTED();
                        }
                    } else return (Op_Field)DPI;
                case 0b010:
                    return (Op_Field)LSIO;
                case 0b011:
                    byte b4 = (*instr & (0b1 << 4)) >> 4;
                    switch (b4) {
                        case 0b0:
                            return (Op_Field)LSRO;
                        case 0b1:
                            byte b24_23_22_21_20 = (*instr & (0b11111 << 20)) >> 20;
                            byte b7_6_5 = (*instr & (0b111 << 5)) >> 5;
                            if (b24_23_22_21_20 == 0b11111 && b7_6_5 == 0b111) return (Op_Field)AU;
                            else return (Op_Field)MEI;
                        default:
                            UNSUPPORTED();
                    }
                case 0b100:
                    return (Op_Field)LSM;
                case 0b101:
                    return (Op_Field)BBL;
                case 0b110:
                    return (Op_Field)CLS;
                case 0b111:
                    byte b24 = (*instr & (0b1 << 24)) >> 24;
                    switch (b24) {
                        case 0b0:
                            byte b4 = (*instr & (0b1 << 4)) >> 4;
                            switch (b4) {
                                case 0b0:
                                    return (Op_Field)CDP;
                                case 0b1:
                                    return (Op_Field)CRT;
                                default:
                                    UNSUPPORTED();
                            }
                        case 0b1:
                            return (Op_Field)SWI;
                        default:
                            UNSUPPORTED();
                    }
                default:
                    UNSUPPORTED();
            }
    }

}

/**
 * @brief Returns wheter the current instruction has to be executed or not.
 * 
 * @param cond Condition to satisfy for execution.
 * @param CPSR_bits Bit field of the CPSR register. 
 * @return byte 1 = To be executed - 0 = NOP.
 */
byte check_Cond(Cond_Field cond, PSR_Bitfield CPSR_bits) {
    byte tbe = 0;
    switch (cond)
    {
    case EQ:
        tbe = CPSR_bits.Z == 1 ? 1 : 0;
        break;
    case NE:
        tbe = CPSR_bits.Z == 0 ? 1 : 0;
        break;
    case CS_HS:
        tbe = CPSR_bits.C == 1 ? 1 : 0;
        break;
    case CC_LO:
        tbe = CPSR_bits.C == 0 ? 1 : 0;
        break;
    case MI:
        tbe = CPSR_bits.N == 1 ? 1 : 0;
        break;
    case PL:
        tbe = CPSR_bits.N == 0 ? 1 : 0;
        break;
    case VS:
        tbe = CPSR_bits.V == 1 ? 1 : 0;
        break;
    case VC:
        tbe = CPSR_bits.V == 0 ? 1 : 0;
        break;
    case HI:
        tbe = (CPSR_bits.C == 1 && CPSR_bits.Z == 0) ? 1 : 0;
        break;
    case LS:
        tbe = (CPSR_bits.C == 0 || CPSR_bits.Z == 1) ? 1 : 0;
        break;
    case GE:
        tbe = (CPSR_bits.N == CPSR_bits.V) ? 1 : 0;
        break;
    case LT:
        tbe = (CPSR_bits.N != CPSR_bits.V) ? 1 : 0;
        break;
    case GT:
        tbe = (CPSR_bits.Z == 0 && (CPSR_bits.N == CPSR_bits.V)) ? 1 : 0;
        break;
    case LE:
        tbe = (CPSR_bits.Z == 1 || (CPSR_bits.N != CPSR_bits.V)) ? 1 : 0;
        break;
    case AL:
        tbe = 1;
        break;
    case EXTRA:
        NOT_IMPLEMENTED();
        break;
    default:
        break;
    }
    return tbe;
}