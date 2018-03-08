// REGS.H
#ifndef REGS_H
#define REGS_H

#include <cstdint>

// register type
typedef uint32_t reg_t;

// number of registers
const int R_SIZE = 32;

// general purpose registers
extern reg_t R[R_SIZE];

// un-numbered registers
extern reg_t HI, LO, PC;

// floating-point registers ?

// names
enum {
    // zero
    REG_ZERO = 0,
    // reserved for assembler
    REG_AT,
    // results
    REG_V0,
    REG_V1,
    // arguments
    REG_A0,
    REG_A1,
    REG_A2,
    REG_A3,
    // temporary registers
    REG_T0,
    REG_T1,
    REG_T2,
    REG_T3,
    REG_T4,
    REG_T5,
    REG_T6,
    REG_T7,
    // saved registes
    REG_S0,
    REG_S1,
    REG_S2,
    REG_S3,
    REG_S4,
    REG_S5,
    REG_S6,
    REG_S7,
    // temporary registers
    REG_T8,
    REG_T9,
    // reserved for OS
    REG_K0,
    REG_K1,
    // global pointer
    REG_GP,
    // stack pointer
    REG_SP,
    // frame pointer
    REG_FP,
    // return address
    REG_RA
};

#endif
