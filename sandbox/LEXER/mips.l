%{
    #include    "lex.h"
%}

id 		[a-zA-Z][a-zA-Z0-9]*
number		[0-9]+
ws		[ \t\n]+
string  \"[^\n"]+\"
label   {id}:


%%

"#".*       { strcpy(value1, yytext); return T_COMMENT; }

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

"add"		{ opcode = 0; funct = 32; return R_INS_3;}
"addu"		{ opcode = 0; funct = 33; return R_INS_3;}
"sub"		{ opcode = 0; funct = 34; return R_INS_3;}
"subu"		{ opcode = 0; funct = 35; return R_INS_3;}
"and"		{ opcode = 0; funct = 36; return R_INS_3;}
"or"		{ opcode = 0; funct = 37; return R_INS_3;}
"xor"		{ opcode = 0; funct = 38; return R_INS_3;}
"nor"		{ opcode = 0; funct = 39; return R_INS_3;}
"slt"		{ opcode = 0; funct = 42; return R_INS_3;}
"sltu"		{ opcode = 0; funct = 43; return R_INS_3;}



"li"		{ return T_LOAD_IMMEDIATE;}
"lw"		{ return T_LOAD_WORD;}
"sw"		{ return T_STORE_WORD;}
":"         { return COLON;}
","         ;
{ws}        ;


{label}     { yytext[strlen(yytext)-1] = 0; strcpy(value1, yytext); return LABEL; }

{id} 		{ strcpy(value1, yytext); return T_ID;}
{number}	{ strcpy(value1, yytext); return T_INT_NUM;}

%%

int yywrap()
{
	return 1;
}
