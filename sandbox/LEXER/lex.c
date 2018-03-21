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

#define     START_TEXT_SEGMENT      0x00400000
#define     START_DATA_SEGMENT      0x10000000

extern int32_t  yylex();
extern void     yyerror(const char*);

int32_t IS_DATA_SEGMENT = TRUE;         /* Tells us whether we are on the data segment or the text segment */
int32_t IS_GLOBAL_MAIN = FALSE;         /* Tells us if '.global main' has been specified */
int32_t PADDING_BYTES_SIZE = 4;         /* Data padding - this means every data should start at an address
                                            multiple of PADDING_BYTES_SIZE */

int32_t DATA_SEGMENT_SIZE = 0;          /* Required to generate the executable's header */
int32_t TEXT_SEGMENT_SIZE = 0;
int32_t START_MAIN_ADDRESS = 0;

int32_t DC = START_DATA_SEGMENT;        /* Needed to keep track of labels */
int32_t PC = START_TEXT_SEGMENT;

std::map<std::string, int32_t> LABEL_ADDRESS;  /* References labels' memory addresses */

/***  ***/


/* R-type Instructions */

int32_t f_r4(){
    int32_t tok1;
    int32_t rs;

    tok1 = yylex(); rs = value;

    if( tok1 == T_REG ){
        PC += 4;
        return (OP(opcode) + RS(rs) + (funct));
    }

    yyerror("");
}

int32_t f_r3(){
    int32_t tok1, tok2;
    int32_t rs, rt;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG ){
        PC += 4;
        return (OP(opcode) + RS(rs) + RT(rt) + (funct));
    }

    yyerror("");
}

int32_t f_r2(){
    int32_t tok1, tok2, tok3;
    int32_t rs, rd, rt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_REG ){
        PC += 4;
        return (OP(opcode) + RS(rs) + RT(rt) + RD(rd) + (funct));
    }

    yyerror("");
}

int32_t f_r1(){
    int32_t tok1, tok2, tok3;
    int32_t rd, rt;
    int32_t shamt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rt = value;
    tok3 = yylex(); shamt = 0x3F & atoi(value1); // Possible check: if value1 is bigger than 0x3F

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_INT_NUM ){
        PC += 4;
        return OP(opcode) + RD(rd) + RT(rt) + SH(shamt) + (funct);
    }

    yyerror("");
}

int32_t f_syscall(){ return 0xC; }

/* I-Type Instructions */

int32_t f_i3(){
    // TODO
    int32_t tok1,tok2,tok3,tok4,tok5;
    int32_t rt, rs;
    int32_t imm;
    
    tok1 = yylex(); rt = value;
    tok2 = yylex(); 

    if( tok1 != T_REG ) yyerror("First argument of a Type-I Instruction should be a register.");

    if( tok2 == T_ID ){ // INST T_REG, T_ID
        if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");

        rs = 0x0;
        imm = 0xFFFF & LABEL_ADDRESS[value1]; // Possible check: if value1 address is bigger than 0xFFFF

        PC += 4;
        return OP(opcode) + RT(rt) + RS(rs) + (imm);      
    }

    if( tok2 == T_INT_NUM ){ // INST T_REG, IMM(T_REG)
        imm = 0xFFFF & atoi(value1); // Possible check: if value1 address is bigger than 0xFFFF 

        tok3 = yylex();
        tok4 = yylex(); rs = value;
        tok5 = yylex();

        if( not (tok3 == LPAR && tok4 == T_REG && tok5 == RPAR) ) yyerror("Syntax error.");

        PC += 4;
        return OP(opcode) + RT(rt) + RS(rs) + (imm); 
    }

    yyerror("");
}

int32_t f_i2(){
    bool using_rt = (value1 == "lui");

    int32_t tok1,tok2;
    int32_t reg;
    int32_t imm;

    tok1 = yylex(); reg = value;
    tok2 = yylex(); imm = 0xFFFF & atoi(value1); // Possible check: if value1 is bigger than 0xFFFF

    if( tok1 == T_REG && tok2 == T_INT_NUM ){
        PC += 4;
        return OP(opcode) + ( using_rt ? RT(reg) : RS(reg) ) + (imm);
    }

    yyerror("");
}

int32_t f_i1(){
    int32_t tok1,tok2,tok3;
    int32_t rs,rt;
    int32_t imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); imm = 0xFFFF & atoi(value1); // Possible check: if value1 is bigger than 0xFFFF

    if( tok1 == T_REG && tok2 == T_REG  && tok3 == T_INT_NUM ){
        PC += 4;
        return OP(opcode) + RS(rs) + RT(rt) + (imm);
    }

    yyerror("");
}


/* J-Type Instructions */

int32_t f_j1(){
    int32_t tok1;
    int32_t addr;

    tok1 = yylex();
    if(tok1 == T_ID){ // INST T_ID
        if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");
        addr = 0x3FFFFFF & (LABEL_ADDRESS[value1] >> 2);

        PC += 4;
        return OP(opcode) + addr;
    }

    yyerror("");
}


/* Pseudo-Instructions */

int32_t f_nop(){ PC+=4; return 0x0; }
// ...


/* Utility Functions */

void yyerror(const char *err){ fputs(err, stderr); exit(-1); }    /* Handle the 'err' error and exit the program */ 

/***  ***/



int main(){
    int32_t tok, tok1, tok2, tok3;

    while( tok = yylex() ){
        switch(tok){
            /*** Regular instructions ***/
            case R_INS_4:   printf("Write %#010x to text (PC: %#010x)\n", f_r4(), PC );    break;
            case R_INS_3:   printf("Write %#010x to text (PC: %#010x)\n", f_r3(), PC );    break;
            case R_INS_2:   printf("Write %#010x to text (PC: %#010x)\n", f_r2(), PC );    break;
            case R_INS_1:   printf("Write %#010x to text (PC: %#010x)\n", f_r1(), PC );    break;

            case I_INS_3:   printf("Write %#010x to text (PC: %#010x)\n", f_i3(), PC );    break;
            case I_INS_2:   printf("Write %#010x to text (PC: %#010x)\n", f_i2(), PC );    break;
            case I_INS_1:   printf("Write %#010x to text (PC: %#010x)\n", f_i1(), PC );    break;

            case J_INS_1:   printf("Write %#010x to text (PC: %#010x)\n", f_j1(), PC );    break;


            /*** Pseudo-instructions ***/
            case INST_NOP:  printf("Write %#010x to text (PC: %#010x)\n", f_nop(), PC );   break;
            // case ...


            /*** Directives ***/
            case T_ASCIIZ_DIRECTIVE:    if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != STRING ) yyerror("Asciiz directive should be followed by a string.");
                                        printf("Write \'%s\' to data (DC: %#010x)\n", value1, DC);
                                        DC += strlen(value1) + 1;
                                        break;

            case T_WORD_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_INT_NUM ) yyerror("Word directive should be followed by an integer.");
                                        printf("Write %d to data (DC: %#010x)\n", atoi(value1), DC);
                                        DC += 4;
                                        break;

            case T_BYTE_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_INT_NUM && (tok1=yylex()) != T_CHAR ) yyerror("Byte directive should be followed by an integer or a char.");
                                        if( tok1 == T_INT_NUM ) printf("Write %d to data (DC: %#010x)\n", atoi(value1), DC);
                                        else                    printf("Write \'%c\' to data (DC: %#010x)\n", value1[0], DC);
                                        DC+=1;
                                        break;

            case T_HALF_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_INT_NUM ) yyerror("Half directive should be followed by an integer.");
                                        printf("Write %d to data (DC: %#010x)\n", atoi(value1), DC);
                                        DC+=2;
                                        break;
            
            case T_GLOBL_DIRECTIVE:     if((tok1=yylex()) != T_ID) yyerror("Global directive should be followed by an ID.");
                                        if(strcmp(value1, "main") == 0) IS_GLOBAL_MAIN = TRUE;
                                        // do something ?
                                        break;
                                    
            case T_END_DIRECTIVE:       if((tok1=yylex()) != T_ID) yyerror("End directive should be followed by an ID.");
                                        if(LABEL_ADDRESS.count(value1) == 0) yyerror("End's ID not found.");
                                        // do something ?
                                        break;

            case T_DATA_DIRECTIVE: IS_DATA_SEGMENT = TRUE;  break;
            case T_TEXT_DIRECTIVE: IS_DATA_SEGMENT = FALSE; break;


            /*** Labels ***/
            case LABEL:     if( LABEL_ADDRESS.count(value1) ) yyerror("Label already exists");
                            LABEL_ADDRESS[value1] = (IS_DATA_SEGMENT ? DC : PC);
                            printf("Label \'%s\' address: %#010x\n", value1, LABEL_ADDRESS[value1]);   break;


            /*** Bad syntax ***/
            case LPAR:
            case RPAR:
            case COLON:
            case T_ID:
            case T_INT_NUM:
            case T_REG:
                            yyerror("Syntax error or not supported instructions."); break;


            /* Do nothing */
            case T_COMMENT: break;
        }

        /* Data padding */
        if( DC % PADDING_BYTES_SIZE )
            DC += PADDING_BYTES_SIZE - DC % PADDING_BYTES_SIZE;
    }

    DATA_SEGMENT_SIZE = DC - START_DATA_SEGMENT;
    TEXT_SEGMENT_SIZE = PC - START_TEXT_SEGMENT;
    START_MAIN_ADDRESS = LABEL_ADDRESS["main"];

    puts("");
    printf("Data segment size: %d\n", DATA_SEGMENT_SIZE);
    printf("Text segment size: %d\n", TEXT_SEGMENT_SIZE);
    printf("Main address: %#010x\n", START_MAIN_ADDRESS);

    return 0;
}
