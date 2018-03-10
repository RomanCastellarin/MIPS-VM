// INSTRUCTIONS.H
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

// TODO: decide width of integer type for rs, rt, shamt, etc. eg: int32_t

// INSTRUCTION TYPE
typedef uint32_t instruction;

// R TYPE INTRUCTION
typedef int (*inst_r_t)(int rs, int rt, int rd, int shamt);

// I TYPE INTRUCTION
typedef int (*inst_i_t)(int rs, int rt, int imm);

// J TYPE INTRUCTION
typedef int (*inst_j_t)(int addr);

// DECODE FUNCTION
int decode(instruction i);

#endif
