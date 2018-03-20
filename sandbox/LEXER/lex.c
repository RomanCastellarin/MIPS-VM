#include    <cstdio>
#include    <cstdlib>
#include    <map>
#include    <string>

#define     OP(x)                   ((x)<<26)
#define     RS(x)                   ((x)<<21)
#define     RT(x)                   ((x)<<16)
#define     RD(x)                   ((x)<<11)
#define     SH(x)                   ((x)<<6)

#define     START_TEXT_SEGMENT      0x00400000
#define     START_DATA_SEGMENT      0x10000000

extern int32_t yylex();

int32_t DATA_SEGMENT_SIZE = 0;      /* Required to generate the executable */
int32_t TEXT_SEGMENT_SIZE = 0;

int32_t DC = START_DATA_SEGMENT;    /* Needed to keep track of labels */
int32_t PC = START_TEXT_SEGMENT;

std::map<std::string, int32_t> LABEL_ADRESS;  /* References labels' memory adresses */

/***  ***/

/* R-type Instructions */

int32_t f_r4()
{
    // TODO
    int32_t tok1;
    int32_t rs;

    tok1 = yylex(); rs = value;

    if( tok1 == T_REG )
        return (OP(opcode) + RS(rs) + (funct));

    exit(-1);
}

int32_t f_r3()
{
    int32_t tok1, tok2;
    int32_t rs, rt;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + (funct));

    exit(-1);
}

int32_t f_r2()
{
    int32_t tok1, tok2, tok3;
    int32_t rs, rd, rt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + RD(rd) + (funct));

    exit(-1);
}

int32_t f_r1()
{
    int32_t tok1, tok2, tok3;
    int32_t rs, rt;
    int32_t shamt;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); shamt = 0x3F & atoi(value1); // Possible error check: if value1 is bigger than 0x3F

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_INT_NUM )
        return OP(opcode) + RT(rs) + RD(rt) + SH(shamt) + (funct);

    exit(-1);
}

/* I-Type Instructions */

int32_t f_i3()
{
    // TODO
    return 0;

    exit(-1);
}

int32_t f_i2()
{
    bool using_rt = (value1 == "lui"); // There might be a better way to check this

    int32_t tok1,tok2;
    int32_t reg;
    int32_t imm;

    tok1 = yylex(); reg = value;
    tok2 = yylex(); imm = 0xFFFF & atoi(value1); // Possible error check: if value1 is bigger than 0xFFFF

    if( tok1 == T_REG && tok2 == T_INT_NUM ){
        return OP(opcode) + ( using_rt ? RT(reg) : RS(reg) ) + (imm);
    }

    exit(-1);
}

int32_t f_i1()
{

    int32_t tok1,tok2,tok3;
    int32_t rs,rt;
    int32_t imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); imm = 0xFFFF & atoi(value1); // Possible error check: if value1 is bigger than 0xFFFF

    if( tok1 == T_REG && tok2 == T_REG  && tok3 == T_INT_NUM )
        return OP(opcode) + RS(rs) + RT(rt) + (imm);

    exit(-1);
}

/* J-Type Instructions */

int32_t f_j1()
{
    // TODO
    int32_t mem_address = 0x12345678 & 0x07FFFFFF; // Placeholder
    return OP(opcode) + mem_address;

    exit(-1);
}


/***  ***/



int main()
{
    int32_t tok;
    while( tok = yylex() ){
        switch(tok){
            case R_INS_4:   printf("%#010x\n", f_r4() );    break;
            case R_INS_3:   printf("%#010x\n", f_r3() );    break;
            case R_INS_2:   printf("%#010x\n", f_r2() );    break;
            case R_INS_1:   printf("%#010x\n", f_r1() );    break;

            case I_INS_3:   printf("%#010x\n", f_i3() );    break;
            case I_INS_2:   printf("%#010x\n", f_i2() );    break;
            case I_INS_1:   printf("%#010x\n", f_i1() );    break;

            case J_INS_1:   printf("%#010x\n", f_j1() );    break;

            case T_COMMENT: printf("Comment: %s\n", value1); break;

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

            case COLON: printf("token = %d : T_COLON\n", COLON); break;

            case T_REG: printf("token = %d : T_REG : reg_no = %d\n", T_REG, value); break;
            case T_ID: printf("token = %d : T_ID : %s\n", T_ID, value1); break;
            case T_INT_NUM: printf("token = %d : T_INT_NUM : %s\n", T_INT_NUM, value1); break;
        }
    }
}
