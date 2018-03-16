#include "regs.h"
#include <limits>
#include <cassert>

// general purpose registers
reg_t R[R_SIZE];

// un-numbered registers
reg_t HI, LO, PC;

// floating-point registers
reg_t F[F_SIZE];

constexpr bool check(){
    return
            std::numeric_limits<float>::is_iec559
        and
            std::numeric_limits<double>::is_iec559;
}

static_assert( check(), "Need be IEEE754 compliant");

