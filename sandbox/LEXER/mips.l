%{
	#include <stdio.h>

	#define T_DATA_DIRECTIVE 100
	#define T_BYTE_DIRECTIVE 101
	#define T_HALF_DIRECTIVE 102
	#define T_WORD_DIRECTIVE 103
	#define T_TEXT_DIRECTIVE 104
	#define T_GLOBL_DIRECTIVE 105
	#define T_END_DIRECTIVE 106
	#define T_ASCIIZ_DIRECTIVE 120

	#define T_REG 107
	#define T_ADD 108
	#define T_SUB 109
	#define T_LOAD_WORD 110
	#define T_LOAD_IMMEDIATE 111
	#define T_STORE_WORD 112
	#define T_ID 113
	#define T_INT_NUM 114

	#define COLON 115
	#define COMMA 116
	#define WS 117
    
    #define LABEL 118
    #define STRING 119

	int value;
	char value1[15];
%}

id 		[a-zA-Z][a-zA-Z0-9]*
number		[0-9]+
ws		[ \t\n]+
string  \".+\"
label   {id}:


%%

{string}    { strcpy(value1, yytext); return STRING; } 

".data"		{ return T_DATA_DIRECTIVE;}
".byte"		{ return T_BYTE_DIRECTIVE;}
".half"		{ return T_HALF_DIRECTIVE;}
".word"		{ return T_WORD_DIRECTIVE;}
".text"		{ return T_TEXT_DIRECTIVE;}
".globl"	{ return T_GLOBL_DIRECTIVE;}
".end"		{ return T_END_DIRECTIVE;}
".asciiz"	{ return T_ASCIIZ_DIRECTIVE;}

"$zero"		{ value = 0; return T_REG;}
"$at"		{ value = 1; return T_REG;}
"$v0"		{ value = 2; return T_REG;}
"$v1"		{ value = 3; return T_REG;}

"$a0"		{ value = 4; return T_REG;}
"$a1"		{ value = 5; return T_REG;}
"$a2"		{ value = 6; return T_REG;}
"$a3"		{ value = 7; return T_REG;}

"$t0"		{ value = 8; return T_REG;}
"$t1"		{ value = 9; return T_REG;}
"$t2"		{ value = 10; return T_REG;}
"$t3"		{ value = 11; return T_REG;}
"$t4"		{ value = 12; return T_REG;}
"$t5"		{ value = 13; return T_REG;}
"$t6"		{ value = 14; return T_REG;}
"$t7"		{ value = 15; return T_REG;}

"$s0"		{ value = 16; return T_REG;}
"$s1"		{ value = 17; return T_REG;}
"$s2"		{ value = 18; return T_REG;}
"$s3"		{ value = 19; return T_REG;}
"$s4"		{ value = 20; return T_REG;}
"$s5"		{ value = 21; return T_REG;}
"$s6"		{ value = 22; return T_REG;}
"$s7"		{ value = 23; return T_REG;}

"$t8"		{ value = 24; return T_REG;}
"$t9"		{ value = 25; return T_REG;}

"$k0"		{ value = 26; return T_REG;}
"$k1"		{ value = 27; return T_REG;}

"$gp"		{ value = 28; return T_REG;}
"$sp"		{ value = 29; return T_REG;}
"$fp"		{ value = 30; return T_REG;}
"$ra"		{ value = 31; return T_REG;}

"add"		{ return T_ADD;}
"sub"		{ return T_SUB;}
"li"		{ return T_LOAD_IMMEDIATE;}
"lw"		{ return T_LOAD_WORD;}
"sw"		{ return T_STORE_WORD;}
":"		{ return COLON;}
","		{ return COMMA;}
{ws}		{ return WS;}


{label}     { yytext[strlen(yytext)-1] = 0; strcpy(value1, yytext); return LABEL; }

{id} 		{ strcpy(value1, yytext); return T_ID;}
{number}	{ strcpy(value1, yytext); return T_INT_NUM;}

%%

int yywrap()
{
	return 1;
}

int main()
{
	int tok;
	while((tok = yylex()) != EOF)
	{
		switch(tok)
		{
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
			case T_ADD: printf("token = %d : T_ADD\n", T_ADD); break;
			case T_SUB: printf("token = %d : T_SUB\n", T_SUB); break;
			case T_LOAD_WORD: printf("token = %d : T_LOAD_WORD\n", T_LOAD_WORD); break;
			case T_LOAD_IMMEDIATE: printf("token = %d : T_LOAD_IMMEDIATE\n", T_LOAD_IMMEDIATE); break;
			case T_STORE_WORD: printf("token = %d : T_STORE_WORD\n", T_STORE_WORD); break;
			case COLON: printf("token = %d : T_COLON\n", COLON); break;
			case COMMA: printf("token = %d : T_COMMA\n", COMMA); break;
			
			case T_REG: printf("token = %d : T_REG : reg_no = %d\n", T_REG, value); break;
			case T_ID: printf("token = %d : T_ID : %s\n", T_REG, value1); break;
			case T_INT_NUM: printf("token = %d : T_INT_NUM : %s\n", T_REG, value1); break;
		}
	}
}