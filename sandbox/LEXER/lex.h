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
#define R_INS_1 122
#define R_INS_2 123
#define R_INS_3 124


int opcode,funct,value;
char value1[15];
