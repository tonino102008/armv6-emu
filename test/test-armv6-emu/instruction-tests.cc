#include "../googletest/googletest/include/gtest/gtest.h"

extern "C" {
    #include "types.h"
    #include "register.h"
    #include "instruction.h"
}

namespace {

TEST(InstructionTests, check_Cond) {
    ARM_Reg_ctor();
    PSR_Bitfield CPSR_bits = PSR_read(*CPSR_Reg.regs);
    EXPECT_EQ(check_Cond(VC, CPSR_bits), 1);
    EXPECT_EQ(check_Cond(VS, CPSR_bits), 0);
}

TEST(InstructionTests,  check_Op_Type) {
    word* instr = (word*)malloc(sizeof(word));
    *instr = 0b11100000101000010010000000000000;
    EXPECT_EQ(check_Op_Type(instr), (Op_Field)DPIS);
    *instr = 0b11100000101000010010000000010000;
    EXPECT_EQ(check_Op_Type(instr), (Op_Field)DPRS);
    free(instr);
    ARM_Reg_dtor();
}

// TEST(InstructionTests, ADC) {
//     **CPSR_Reg.regs &= !**CPSR_Reg.regs;
//     **CPSR_Reg.regs |= 0b10000;
//     word* instr = (word*)malloc(sizeof(word));
//     *instr = 0b11100000101000010010000000000000;
//     ADC(instr);
//     free(instr);
//     ARM_Reg_dtor();
// }

}