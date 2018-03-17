#include    "lex.h"
#include    <stdio.h>
#include    <stdlib.h>
#define     OP(x)   ((x)<<26)
#define     RS(x)   ((x)<<21)
#define     RT(x)   ((x)<<16)
#define     RD(x)   ((x)<<11)
#define     SH(x)   ((x)<<6)
extern yylex();

int32_t f_r3()
{
    int32_t tok1,tok2,tok3;
    int32_t rs,rd,rt;
    tok1 = yylex();
    rd = value;
    tok2 = yylex();
    rs = value;
    tok3 = yylex();
    rt = value;
    if(tok1 == tok2 && tok2 == tok3 )
        return (OP(opcode) + RS(rs) + RT(rt) + RD(rd) + (funct));
    exit(-1);
}
int32_t f_i2()
{
    int32_t tok1,tok2,tok3;
    int32_t rs,rt;
    int32_t imm;
    tok1 = yylex();
    rt = value;
    tok2 = yylex();
    rs = value;
    tok3 = yylex();
    imm = 0xFFFF & atoi(value1);
    if(tok1 == T_REG && tok2 == T_REG  && tok3 == T_INT_NUM )
        return OP(opcode) + RS(rs) + RT(rt) + (imm);
    exit(-1);
}
int32_t f_i1()
{
    int32_t tok1,tok2,tok3;
    int32_t rs,rt;
    int32_t shamt;
    tok1 = yylex();
    rt = value;
    tok2 = yylex();
    rs = value;
    tok3 = yylex();
    shamt = 0x3F & atoi(value1);
    if(tok1 == T_REG && tok2 == T_REG  && tok3 == T_INT_NUM)
        return OP(opcode) + RT(rs) + RD(rt) + SH(shamt) + (funct);
    exit(-1);
}





int main()
{
	int32_t tok;
	while( tok = yylex() ){
	    switch(tok){
            case R_INS_3:   printf("%04X\n",f_r3() );    break;
            case R_INS_2:   printf("%04X\n",f_i2() );    break;
            case R_INS_1:   printf("%04X\n",f_i1() );    break;
            case T_COMMENT: printf("token = %d : Comment %s\n", T_COMMENT, value1); break;
            case STRING: printf("token = %d : String %s\n", STRING, value1); break;
            case LABEL: printf("token = %d : Label \"%s\"\n", LABEL, value1); break;
			case T_DATA_DIRECTIVE: printf("token = %d : T_DATA_DIRECTIVE\n", T_DATA_DIRECTIVE); break;
			case T_BYTE_DIRECTIVE: printf("token = %d : T_BYTE_DIRECTIVE\n", T_BYTE_DIRECTIVE); break;
			case T_HALF_DIRECTIVE: printf("token = %d : T_HALF_DIRECTIVE\n", T_HALF_DIRECTIVE); break;
			case T_WORD_DIRECTIVE: printf("token = %d : T_WORD_DIRECTIVE\n", T_WORD_DIRECTIVE); break;
			case T_TEXT_DIRECTIVE: printf("token = %d : T_TEXT_DIRECTIVE\n", T_TEXT_DIRECTIVE); break;
			case T_GLOBL_DIRECTIVE: printf("token = %d : T_GLOBL_DIRECTIVE\n", T_GLOBL_DIRECTIVE); break;
			case T_ASCIIZ_DIRECTIVE: printf("token = %d : T_ASCIIZ_DIRECTIVE\n", T_ASCIIZ_DIRECTIVE); break;
			case T_END_DIRECTIVE: printf("token = %d : T_END_DIRECTIVE\n", T_END_DIRECTIVE); break;
			case T_LOAD_WORD: printf("token = %d : T_LOAD_WORD\n", T_LOAD_WORD); break;
			case T_LOAD_IMMEDIATE: printf("token = %d : T_LOAD_IMMEDIATE\n", T_LOAD_IMMEDIATE); break;
			case T_STORE_WORD: printf("token = %d : T_STORE_WORD\n", T_STORE_WORD); break;
			case COLON: printf("token = %d : T_COLON\n", COLON); break;

			case T_REG: printf("token = %d : T_REG : reg_no = %d\n", T_REG, value); break;
			case T_ID: printf("token = %d : T_ID : %s\n", T_REG, value1); break;
			case T_INT_NUM: printf("token = %d : T_INT_NUM : %s\n", T_REG, value1); break;
		}
	}
}
