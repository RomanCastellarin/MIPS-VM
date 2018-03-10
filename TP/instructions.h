// INSTRUCTIONS.H
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

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

#endif
