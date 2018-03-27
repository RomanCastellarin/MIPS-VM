// INSTRUCTIONS.H
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/* 
 * This module implements all the processor's (and coprocessors') instructions
 */

#include <cstdint>

// INSTRUCTION TYPE
typedef uint32_t instruction;

// R TYPE INTRUCTION
typedef int (*inst_r_t)(int32_t rs, int32_t rt, int32_t rd, int32_t shamt);

// I TYPE INTRUCTION
typedef int (*inst_i_t)(int32_t rs, int32_t rt, int32_t imm);

// J TYPE INTRUCTION
typedef int (*inst_j_t)(int32_t addr);

// DECODE FUNCTION
int decode(instruction i);

// Status codes
enum {
    ST_ERROR        = -1,
    ST_SUCCESS      =  0,
    ST_NOADVANCE    =  1,
};

#endif
