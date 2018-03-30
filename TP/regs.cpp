#include "regs.h"
#include <limits>
#include <cassert>
#include <cstring>

// general purpose registers
reg_t R[R_SIZE];

// un-numbered registers
reg_t HI, LO, PC;

// floating-point registers
reg_t F[F_SIZE];

// clear registers except PC
void clear_registers(){
    memset(R, 0, sizeof(R));
    memset(F, 0, sizeof(F));
    HI = LO = 0x00;
}

constexpr bool check(){
    return
            std::numeric_limits<float>::is_iec559
        and
            std::numeric_limits<double>::is_iec559;
}

// check that the floating types representation is correct
static_assert( check(), "Need be IEEE754 compliant" );

