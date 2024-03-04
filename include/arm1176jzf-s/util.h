#ifndef UTIL_H_
#define UTIL_H_

#include "types.h"

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} Util_Sat_Op_Type;

typedef struct {
    word result;
    byte overflow;
} Util_Sat_Op_Result;

typedef Util_Sat_Op_Result Util_Sig_Op_Result;

Util_Sat_Op_Result signed_saturation_op(word, word, byte, Util_Sat_Op_Type);
Util_Sig_Op_Result signed_op(word, word, byte, Util_Sat_Op_Type);
word sign_extend(word, byte);
Util_Sat_Op_Result signed_saturation_check(word, word);
word arithmetic_shift_right(word, byte);

void is_carry_op();
void is_overflow_op();

#endif // UTIL_H_