#include "util.h"
#include "math.h"

Util_Sat_Op_Result signed_saturation_op(word r1, word r2, byte shift, Util_Sat_Op_Type op) {
    switch (op) {
        case ADD:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 +
                                        (signed long) r2;
                    word out = r1 + r2;
                    byte ov = 0;
                    if ((signed long)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFFFFFF;
                        if (test < 0) out = 0x80000000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 +
                                        (signed int) r2;
                    hword out = r1 + r2;
                    byte ov = 0;
                    if ((signed int)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFF;
                        if (test < 0) out = 0x8000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 +
                                        (signed short) r2;
                    byte out = r1 + r2;
                    byte ov = 0;
                    if ((signed short)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7F;
                        if (test < 0) out = 0x80;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case SUB:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 -
                                        (signed long) r2;
                    word out = r1 - r2;
                    byte ov = 0;
                    if ((signed long)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFFFFFF;
                        if (test < 0) out = 0x80000000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 -
                                        (signed int) r2;
                    hword out = r1 - r2;
                    byte ov = 0;
                    if ((signed int)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFF;
                        if (test < 0) out = 0x8000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 -
                                        (signed short) r2;
                    byte out = r1 - r2;
                    byte ov = 0;
                    if ((signed short)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7F;
                        if (test < 0) out = 0x80;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case MUL:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 *
                                        (signed long) r2;
                    word out = r1 * r2;
                    byte ov = 0;
                    if ((signed long)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFFFFFF;
                        if (test < 0) out = 0x80000000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 *
                                        (signed int) r2;
                    hword out = r1 * r2;
                    byte ov = 0;
                    if ((signed int)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFF;
                        if (test < 0) out = 0x8000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 *
                                        (signed short) r2;
                    byte out = r1 * r2;
                    byte ov = 0;
                    if ((signed short)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7F;
                        if (test < 0) out = 0x80;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case DIV:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 /
                                        (signed long) r2;
                    word out = r1 / r2;
                    byte ov = 0;
                    if ((signed long)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFFFFFF;
                        if (test < 0) out = 0x80000000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 /
                                        (signed int) r2;
                    hword out = r1 / r2;
                    byte ov = 0;
                    if ((signed int)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7FFF;
                        if (test < 0) out = 0x8000;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 /
                                        (signed short) r2;
                    byte out = r1 / r2;
                    byte ov = 0;
                    if ((signed short)out != test) {
                        ov = 1;
                        if (test > 0) out = 0x7F;
                        if (test < 0) out = 0x80;
                    }
                    return (Util_Sat_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        default:
            UNSUPPORTED();
    }
}

Util_Sig_Op_Result signed_op(word r1, word r2, byte shift, Util_Sat_Op_Type op) {
    switch (op) {
        case ADD:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 +
                                        (signed long) r2;
                    word out = r1 + r2;
                    byte ov = 0;
                    if ((signed long)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 +
                                        (signed int) r2;
                    hword out = r1 + r2;
                    byte ov = 0;
                    if ((signed int)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 +
                                        (signed short) r2;
                    byte out = r1 + r2;
                    byte ov = 0;
                    if ((signed short)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case SUB:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 -
                                        (signed long) r2;
                    word out = r1 - r2;
                    byte ov = 0;
                    if ((signed long)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 -
                                        (signed int) r2;
                    hword out = r1 - r2;
                    byte ov = 0;
                    if ((signed int)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 -
                                        (signed short) r2;
                    byte out = r1 - r2;
                    byte ov = 0;
                    if ((signed short)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case MUL:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 *
                                        (signed long) r2;
                    word out = r1 * r2;
                    byte ov = 0;
                    if ((signed long)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 *
                                        (signed int) r2;
                    hword out = r1 * r2;
                    byte ov = 0;
                    if ((signed int)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 *
                                        (signed short) r2;
                    byte out = r1 * r2;
                    byte ov = 0;
                    if ((signed short)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        case DIV:
            switch (shift) {
                case 32:
                    signed long test =  (signed long) r1 /
                                        (signed long) r2;
                    word out = r1 / r2;
                    byte ov = 0;
                    if ((signed long)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 16:
                    signed int test =   (signed int) r1 /
                                        (signed int) r2;
                    hword out = r1 / r2;
                    byte ov = 0;
                    if ((signed int)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                case 8:
                    signed short test = (signed short) r1 /
                                        (signed short) r2;
                    byte out = r1 / r2;
                    byte ov = 0;
                    if ((signed short)out != test) ov = 1;
                    return (Util_Sig_Op_Result){.result = (word)out, .overflow = ov};
                    break;
                default:
                    UNSUPPORTED();
            }
            break;
        default:
            UNSUPPORTED();
    }
}

word sign_extend(word r, byte shift) {
    byte msb = 0, n = 0;
    while (n < shift) {
        if (BIT_CHECK(r, 31 - n)) {
            msb = 1;
            break;
        }
        n++;
    }
    return (r | (((msb << n) - 1) << (31 - n)));
}

Util_Sat_Op_Result signed_saturation_check(word r, word sat) {
    signed int lim = (sat - 1) == 0 ? 1 : (signed int)1 << (sat - 1); 
    if ((signed int) r < -lim) return (Util_Sig_Op_Result){.result = (word)-lim, .overflow = 1};
    else if ((signed int) r > (lim - 1)) return (Util_Sig_Op_Result){.result = (word)(lim - 1), .overflow = 1};
    else return (Util_Sig_Op_Result){.result = r, .overflow = 0};
}

word arithmetic_shift_right(word r, byte shift) {
    word msb = (r & (0b1 << 31)) >> 31;
    word out = r >> shift;
    out |= msb << 31;
    return out;
}

void is_carry_op() {
    NOT_IMPLEMENTED(); 
}

void is_overflow_op() {
    NOT_IMPLEMENTED();
}