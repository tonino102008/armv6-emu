#include "../googletest/googletest/include/gtest/gtest.h"

extern "C" {
    #include "types.h"
    #include "register.h"
}

namespace {

TEST(RegisterTests, ARM_Reg_ctor) {
    ARM_Reg_ctor();
    EXPECT_EQ(*GP_Reg[FIQ].regs[12], (word)0);
    EXPECT_EQ(GP_Reg[USER].regs[12], GP_Reg[SYSTEM].regs[12]);
    EXPECT_NE(GP_Reg[IRQ].regs[14], GP_Reg[USER].regs[14]);
}

TEST(RegisterTests, PSR_write) {
    PSR_Bitfield bits;
    bits.N = 0b1;
    bits.Z = 0b0;
    bits.C = 0b1;
    bits.V = 0b0;
    bits.Q = 0b1;
    bits.RES_1 = 0b00;
    bits.J = 0b1;
    bits.RES_2 = 0b0000;
    bits.GE = 0b1001;
    bits.RES_3 = 0b000000;
    bits.E = 0b1;
    bits.A = 0b0;
    bits.I = 0b0;
    bits.F = 0b0;
    bits.T = 0b1;
    bits.M = 0b10001;

    PSR_write(*CPSR_Reg.regs, bits);
    EXPECT_EQ(**CPSR_Reg.regs, (word)0b10101001000010010000001000110001);
}

TEST(RegisterTests, PSR_read) {
    **CPSR_Reg.regs = 0b10101001000010010000001000110001;
    PSR_Bitfield bits = PSR_read(*CPSR_Reg.regs);    
    EXPECT_EQ(bits.N, (unsigned int)0b1);
    EXPECT_EQ(bits.Z, (unsigned int)0b0);
    EXPECT_EQ(bits.C, (unsigned int)0b1);
    EXPECT_EQ(bits.V, (unsigned int)0b0);
    EXPECT_EQ(bits.Q, (unsigned int)0b1);
    EXPECT_EQ(bits.RES_1, (unsigned int)0b00);
    EXPECT_EQ(bits.J, (unsigned int)0b1);
    EXPECT_EQ(bits.RES_2, (unsigned int)0b0000);
    EXPECT_EQ(bits.GE, (unsigned int)0b1001);
    EXPECT_EQ(bits.RES_3, (unsigned int)0b000000);
    EXPECT_EQ(bits.E, (unsigned int)0b1);
    EXPECT_EQ(bits.A, (unsigned int)0b0);
    EXPECT_EQ(bits.I, (unsigned int)0b0);
    EXPECT_EQ(bits.F, (unsigned int)0b0);
    EXPECT_EQ(bits.T, (unsigned int)0b1);
    EXPECT_EQ(bits.M, (unsigned int)0b10001);
}

TEST(RegisterTests, curr_Proc_Mode) {
    EXPECT_EQ(curr_Proc_Mode(0b10001), 1);
    EXPECT_EQ(curr_Proc_Mode(0b11111), 6);
}

TEST(RegisterTests, has_SPSR) {
    EXPECT_EQ(has_SPSR(), 1);
    **CPSR_Reg.regs &= !**CPSR_Reg.regs;
    **CPSR_Reg.regs |= 0b10000;
    EXPECT_EQ(has_SPSR(), 0);
}

TEST(RegisterTests, ARM_Reg_dtor) {
    ARM_Reg_dtor();
}

}