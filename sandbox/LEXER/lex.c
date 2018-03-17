#include    "lex.h"
#include    <stdio.h>
#include    <stdlib.h>
#define     B_R_3(op,rd,rs,rt,funct)  (((op)<<26) + ((rs)<<21) + ((rt)<<16) + ((rd)<<11) + (funct))
extern yylex();

int f_r3(int tok)
{
    int tok1,tok2,tok3;
    int rs,rd,rt;
    tok1 = yylex();
    rd = value;
    tok2 = yylex();
    rs = value;
    tok3 = yylex();
    rt = value;
    if(tok1 == tok2 && tok2 == tok3 )
        return B_R_3(opcode,rd,rs,rt,funct);
    exit(-1);
}




int main()
{
	int tok;
	while((tok = yylex()) != EOF){
	    switch(tok){
            case R_INS_3:  printf("%04X\n",f_r3(tok) );   break;
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
