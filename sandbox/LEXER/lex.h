#define T_DATA_DIRECTIVE 100
#define T_BYTE_DIRECTIVE 101
#define T_HALF_DIRECTIVE 102
#define T_WORD_DIRECTIVE 103
#define T_TEXT_DIRECTIVE 104
#define T_GLOBL_DIRECTIVE 105
#define T_END_DIRECTIVE 106
#define T_REG 107
#define T_LOAD_WORD 110
#define T_LOAD_IMMEDIATE 111
#define T_STORE_WORD 112
#define T_ID 113
#define T_INT_NUM 114

#define COLON 115
#define COMMA 116

#define LABEL 118
#define STRING 119
#define T_ASCIIZ_DIRECTIVE 120

#define T_COMMENT 121

#define SYSCALL 122

#define R_INS_1 123 	// INSTR T_REG, T_REG, SHAMT
#define R_INS_2 124 	// INSTR T_REG, T_REG, T_REG
#define R_INS_3 125 	// INSTR T_REG, T_REG
#define R_INS_4 126 	// INSTR T_REG

#define I_INS_1 127 	// INSTR T_REG, T_REG, IMM
#define I_INS_2 128 	// INSTR T_REG, IMM
#define I_INS_3 129 	// INSTR T_REG, IMM($T_REG)
							// or  
							// INSTR T_REG, ADDRESS

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


int opcode,funct,value;
char value1[15];
