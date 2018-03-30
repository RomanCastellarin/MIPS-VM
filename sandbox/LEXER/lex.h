#include    <cstdio>
#include    <cstdlib>
#include    <string>
#include    <cstring>
#include    <map>

#define     TRUE    1
#define     FALSE   0

#define     OP(x)                   ((x)<<26)
#define     RS(x)                   ((x)<<21)
#define     RT(x)                   ((x)<<16)
#define     RD(x)                   ((x)<<11)
#define     SH(x)                   ((x)<<6)
#define     IMM(x)                  ((x)&0xFFFF)

#define     START_TEXT_SEGMENT      0x00400000
#define     START_DATA_SEGMENT      0x10000000

#define STRAY	78
#define PLUS	79
#define LPAR	80
#define RPAR	81
#define COLON	82
#define COMMA	83
#define T_COMMENT 84

#define T_DATA_DIRECTIVE 100
#define T_BYTE_DIRECTIVE 101
#define T_HALF_DIRECTIVE 102
#define T_WORD_DIRECTIVE 103
#define T_TEXT_DIRECTIVE 104
#define T_GLOBL_DIRECTIVE 105
#define T_END_DIRECTIVE 106
#define T_ASCIIZ_DIRECTIVE 107

#define T_ID 108
#define T_INT_NUM 109
#define T_HEX_NUM 110
#define T_REG 111
#define T_CHAR 112
#define T_ID_HEX 113

#define T_4 114		// placeholder
#define T_5 115		// placeholder
#define T_6 116		// placeholder
#define T_7 117		// placeholder
#define T_8 118		// placeholder
#define T_9 119		// placeholder

#define LABEL 120
#define STRING 121

#define SYSCALL 122

#define R_INS_1 123 	// INSTR T_REG, T_REG, SHAMT
#define R_INS_2 124 	// INSTR T_REG, T_REG, T_REG
#define R_INS_3 125 	// INSTR T_REG, T_REG
#define R_INS_4 126 	// INSTR T_REG

#define I_INS_1 127 	// INSTR T_REG, T_REG, IMM/ID/ID+IMM
#define I_INS_2 128 	// INSTR T_REG, IMM/ID
#define I_INS_3 129 	// INSTR T_REG, IMM(T_REG)
							// or  
							// INSTR T_REG, T_ID+IMM(T_REG)

#define J_INS_1 130		// INSTR ADRESS


/* PSEUDO INSTRUCTIONS */
#define INST_BLT 131
#define INST_BGT 132
#define INST_BLE 133
#define INST_NEG 134
#define INST_NOT 135
#define INST_BGE 136
#define INST_LI 137
#define INST_LA 138
#define INST_MOVE 139
#define INST_SGE 140
#define INST_SGT 141
#define INST_NOP 142

int opcode, funct, value;
char value1[256], value2[256];
