%{
    #include    "lex.h"
%}

id 			[a-zA-Z][a-zA-Z0-9]*
char  		(\\['"\?\\abfenrtv])|([a-zA-Z0-9])
hexnumber 	(0[xX][0-9a-fA-F]+)|([0-9]+)|('{char}')
ws			[ \t\v\n\f]+
string  	\"[^\n"]+\"
label   	{id}:

%option yylineno
%%


"#".*       { strcpy(value1, yytext); return T_COMMENT; }

{string}    { yytext[strlen(yytext)-1] = 0; strcpy(value1, yytext+1); return STRING; }

".data"		{ return T_DATA_DIRECTIVE;}
".byte"		{ return T_BYTE_DIRECTIVE;}
".half"		{ return T_HALF_DIRECTIVE;}
".word"		{ return T_WORD_DIRECTIVE;}
".text"		{ return T_TEXT_DIRECTIVE;}
".globl"	{ return T_GLOBL_DIRECTIVE;}
".end"		{ return T_END_DIRECTIVE;}
".asciiz"	{ return T_ASCIIZ_DIRECTIVE;}

("$zero"|"$0")	{ value = 0; return T_REG;}
"$at"			{ value = 1; return T_REG;}
"$v0"			{ value = 2; return T_REG;}
"$v1"			{ value = 3; return T_REG;}

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

"sll"       { opcode = 0; funct = 0; return R_INS_1;}
"srl"       { opcode = 0; funct = 2; return R_INS_1;}
"sra"       { opcode = 0; funct = 3; return R_INS_1;}

"sllv"      { opcode = 0; funct = 4; return R_INS_2;}
"srlv"      { opcode = 0; funct = 6; return R_INS_2;}
"srav"      { opcode = 0; funct = 7; return R_INS_2;}
"add"		{ opcode = 0; funct = 32; return R_INS_2;}
"addu"		{ opcode = 0; funct = 33; return R_INS_2;}
"sub"		{ opcode = 0; funct = 34; return R_INS_2;}
"subu"		{ opcode = 0; funct = 35; return R_INS_2;}
"and"		{ opcode = 0; funct = 36; return R_INS_2;}
"or"		{ opcode = 0; funct = 37; return R_INS_2;}
"xor"		{ opcode = 0; funct = 38; return R_INS_2;}
"nor"		{ opcode = 0; funct = 39; return R_INS_2;}
"slt"		{ opcode = 0; funct = 42; return R_INS_2;}
"sltu"		{ opcode = 0; funct = 43; return R_INS_2;}

"jalr"		{ opcode = 0; funct = 9; return R_INS_3;}
"mult"		{ opcode = 0; funct = 24; return R_INS_3;}
"multu"		{ opcode = 0; funct = 25; return R_INS_3;}
"div"		{ opcode = 0; funct = 26; return R_INS_3;}
"divu"		{ opcode = 0; funct = 27; return R_INS_3;}

"jr"		{ opcode = 0; funct = 8; return R_INS_4;}
"mfhi"		{ opcode = 0; funct = 16; return R_INS_4;}
"mthi"		{ opcode = 0; funct = 17; return R_INS_4;}
"mflo"		{ opcode = 0; funct = 18; return R_INS_4;}
"mtlo"		{ opcode = 0; funct = 19; return R_INS_4;}

"syscall"	{ opcode = 0; funct = 12; return SYSCALL;}

"beq"     	{ opcode = 4; return I_INS_1;}
"bne"    	{ opcode = 5; return I_INS_1;}
"addi"      { opcode = 8; return I_INS_1;}
"addiu"     { opcode = 9; return I_INS_1;}
"slti"      { opcode = 10; return I_INS_1;}
"sltiu"     { opcode = 11; return I_INS_1;}
"andi"      { opcode = 12; return I_INS_1;}
"ori"       { opcode = 13; return I_INS_1;}
"xori"      { opcode = 14; return I_INS_1;}

"blez"      { opcode = 6; return I_INS_2;}
"bgtz"      { opcode = 7; return I_INS_2;}
"lui"       { opcode = 15; return I_INS_2;}

"lb"      	{ opcode = 32; return I_INS_3;}
"lh"      	{ opcode = 33; return I_INS_3;}
"lw"      	{ opcode = 34; return I_INS_3;}
"lbu"      	{ opcode = 36; return I_INS_3;}
"lhu"      	{ opcode = 37; return I_INS_3;}
"sb"      	{ opcode = 40; return I_INS_3;}
"sh"      	{ opcode = 41; return I_INS_3;}
"sw"      	{ opcode = 43; return I_INS_3;}

"j"   	    { opcode = 2; return J_INS_1;}
"jal"       { opcode = 3; return J_INS_1;}

%{/* Pseudo Instructions */%}

"blt"		{ return INST_BLT;}
"bgt"		{ return INST_BGT;}
"ble"		{ return INST_BLE;}
"neg"		{ return INST_NEG;}
"not"		{ return INST_NOT;}
"bge"		{ return INST_BGE;}
"li"		{ return INST_LI;}
"la"		{ return INST_LA;}
"move"		{ return INST_MOVE;}
"sge"		{ return INST_SGE;}
"sgt"		{ return INST_SGT;}
"nop"		{ return INST_NOP;}


"("         { return LPAR;}
")"         { return RPAR;}
":"         { return COLON;}
"+"			{ return PLUS;}
("\'"|"\\")	{ return STRAY; }
","         ;
{ws}        ;


{label}     { yytext[ strlen(yytext)-1 ] = 0; strcpy(value1, yytext); return LABEL; }

{id} 		{ strcpy(value1, yytext); return T_ID;}
{hexnumber}	{ if( yytext[1] == '\\' ){	// Escaped character
				switch( yytext[2] ){
					case '\'': 	strcpy(value1,"0x27"); break;
					case '\"': 	strcpy(value1,"0x22"); break;
					case '\?': 	strcpy(value1,"0x3f"); break;
					case '\\': 	strcpy(value1,"0x5c"); break;
					case 'a': 	strcpy(value1,"0x07"); break;
					case 'b': 	strcpy(value1,"0x08"); break;
					case 'f': 	strcpy(value1,"0x0c"); break;
					case 'n': 	strcpy(value1,"0x0a"); break;
					case 'r': 	strcpy(value1,"0x0d"); break;
					case 't': 	strcpy(value1,"0x09"); break;
					case 'v': 	strcpy(value1,"0x0b"); break;
				}
			  }
			  else if( yytext[0] == '\'' ) sprintf(value1, "%d", yytext[1]); // Keep the character between both '
			  else strcpy(value1, yytext); // Normal integer / hex number

			  return T_HEX_NUM;
			}

%%

int yywrap()
{
	return 1;
}



