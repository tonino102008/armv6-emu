#ifndef UTIL_H_
#define UTIL_H_

#include "types.h"

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} Util_Op_Type;

typedef struct {
    word result;
    byte overflow;
} Util_Sat_Op_Result;

typedef Util_Sat_Op_Result Util_Sig_Op_Result;

Util_Sat_Op_Result signed_saturation_op(word, word, byte, Util_Op_Type);
Util_Sig_Op_Result signed_op(word, word, byte, Util_Op_Type);
Util_Sat_Op_Result unsigned_saturation_op(word, word, byte, Util_Op_Type);
Util_Sig_Op_Result unsigned_op(word, word, byte, Util_Op_Type);
Util_Sat_Op_Result signed_saturation_check(word, word);
Util_Sat_Op_Result unsigned_saturation_check(word, word);
word sign_extend(word, byte);
word arithmetic_shift_right(word, byte);

#endif // UTIL_H_