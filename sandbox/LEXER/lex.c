

extern int32_t  yylex();
extern void     yyerror(const char*);
extern void     rformatnum(int32_t*);

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

FILE*   executable;                     /* Handler of the exeutable file */
/***  ***/


/* R-type Instructions */

int32_t f_r4(){
    int32_t tok1;
    int32_t rs;

    tok1 = yylex(); rs = value;

    if( tok1 == T_REG )
        return (OP(opcode) + RS(rs) + (funct));

    yyerror("");
}

int32_t f_r3(){
    int32_t tok1, tok2;
    int32_t rs, rt;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + (funct));

    yyerror("");
}

int32_t f_r2(){
    int32_t tok1, tok2, tok3;
    int32_t rs, rd, rt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + RD(rd) + (funct));

    yyerror("");
}

int32_t f_r1(){
    int32_t tok1, tok2, tok3;
    int32_t rd, rt;
    int32_t shamt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rt = value;
    tok3 = yylex(); rformatnum(&shamt);

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_HEX_NUM )
        return OP(opcode) + RD(rd) + RT(rt) + SH(shamt & 0x3F) + (funct);

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
        imm = LABEL_ADDRESS[value1];

        return OP(opcode) + RT(rt) + RS(rs) + IMM(imm);
    }

    if( tok2 == T_HEX_NUM ){ // INST T_REG, IMM(T_REG)
        rformatnum(&imm);

        tok3 = yylex();
        tok4 = yylex(); rs = value;
        tok5 = yylex();

        if( not (tok3 == LPAR && tok4 == T_REG && tok5 == RPAR) ) yyerror("Syntax error.");

        return OP(opcode) + RT(rt) + RS(rs) + IMM(imm);
    }

    yyerror("");
}

int32_t f_i2(){
    bool using_rt = (strcmp(value1,"lui") == 0);

    int32_t tok1,tok2;
    int32_t reg;
    int32_t imm;

    tok1 = yylex(); reg = value;
    tok2 = yylex(); rformatnum(&imm);

    if( tok1 == T_REG && tok2 == T_HEX_NUM )
        return OP(opcode) + ( using_rt ? RT(reg) : RS(reg) ) + IMM(imm);

    yyerror("");
}

int32_t f_i1(){
    int32_t tok1,tok2,tok3;
    int32_t rs,rt;
    int32_t imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex();

    if( tok1 == T_REG && tok2 == T_REG ){
        if( tok3 == T_HEX_NUM ){
            rformatnum(&imm);
            return OP(opcode) + RS(rs) + RT(rt) + IMM(imm);
        }
        if( tok3 == T_ID ){
            if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");
            imm = LABEL_ADDRESS[value1];
            return OP(opcode) + RS(rs) + RT(rt) + IMM(imm);
        }
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
        addr = LABEL_ADDRESS[value1] >> 2;
        return OP(opcode) + addr;
    }

    yyerror("Wrong J-Type instruction formatting, make sure to be using INST_J ID.");
}


/* Pseudo-Instructions */

void f_nop(){ int32_t val = 0; fwrite(&val,sizeof(int32_t),1,executable); PC+=4; }
void f_li(){
    int32_t instruction;
    int32_t tok1, tok2;
    int32_t rt, rs, imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rformatnum(&imm);

    if( tok1 != T_REG or tok2 != T_HEX_NUM ) yyerror("Wrong LI instruction formatting. make sure to be using LI $REG, IMM_VALUE.");

    if( imm >> 16 ){
        // IMM is a 32-bit value
        // LI T_REG, T_HEX_NUM is gonna be split into two instructions:
        // LUI T_REG, T_HEX_NUM_hi
        // ORI T_REG, T_REG, T_HEX_NUM_lo

        opcode = 15; // LUI opcode
        instruction = OP(opcode) + RT(rt) + IMM(imm>>16);
        fwrite(&instruction,sizeof(int32_t),1,executable); PC+=4;

        opcode = 13; // ORI opcode
        rs = rt;
        instruction = OP(opcode) + RT(rt) + RS(rs) + IMM(imm);
        fwrite(&instruction,sizeof(int32_t),1,executable); PC+=4;

    } else {
        // IMM is a 16-bit value
        // We can directly use: ADDIU T_REG, $0, T_HEX_NUM

        opcode = 9; // ADDIU opcode
        rs = 0x0;
        instruction = OP(opcode) + RS(rs) + RT(rt) + IMM(imm);

        fwrite(&instruction,sizeof(int32_t),1,executable); PC+=4;
    }
}

void f_la(){
    int32_t instruction;
    int32_t tok1, tok2;
    int32_t rt, rs;

    tok1 = yylex(); rt = value;
    tok2 = yylex();

    if( tok1 != T_REG or tok2 != T_ID ) yyerror("Wrong LA instruction formatting. make sure to be using LI $REG, ID.");

    // LA T_REG, T_ID is gonna be split into two instructions:
    // LUI T_REG, T_ID_hi
    // ORI T_REG, T_REG, T_ID_lo

    opcode = 15; // LUI opcode
    instruction = OP(opcode) + RT(rt) + IMM(LABEL_ADDRESS[value1]>>16);
    fwrite(&instruction,sizeof(int32_t),1,executable); PC+=4;

    opcode = 13; // ORI opcode
    rs = rt;
    instruction = OP(opcode) + RT(rt) + RS(rs) + IMM(LABEL_ADDRESS[value1]);
    fwrite(&instruction,sizeof(int32_t),1,executable); PC+=4;
}

void f_move(){
    //TODO
    PC+=4;
}


// ...

/* Utility Functions */

void yyerror(const char *err){ fprintf(stderr, "ERROR: %s\nLINE: %d.\n", err, yylineno); exit(-1); }/* Handle the 'err' error and exit the program */

void rformatnum(int32_t *num){ sscanf(value1, "%i", num); }                                         /* Reads a T_HEX_NUM as 0x... or integer */

/***  ***/

void switch_1(){
    int32_t tok, tok1;
    int32_t val;
    while( tok = yylex() ){
        switch(tok){
            /*** Regular instructions ***/
            case R_INS_4:   PC+= 4; break;
            case R_INS_3:   PC+= 4; break;
            case R_INS_2:   PC+= 4; break;
            case R_INS_1:   PC+= 4; break;

            case I_INS_3:   PC+= 4; break;
            case I_INS_2:   PC+= 4; break;
            case I_INS_1:   PC+= 4; break;

            case J_INS_1:   PC+= 4; break;

            case SYSCALL:   PC+= 4; break;


            /*** Pseudo-instructions ***/
            case INST_NOP:  PC+= 4;    break;
            case INST_LI:   yylex();yylex();rformatnum(&val);if(val >>16) PC+= 8; else PC+= 4;
                            break;
            case INST_LA:   PC+= 8;     break;
            case INST_MOVE: PC+= 4;     break;
            // case ...


            /*** Directives ***/
            case T_ASCIIZ_DIRECTIVE:    yylex(); DC += strlen(value1) + 1;
                                        break;

            case T_WORD_DIRECTIVE:      DC += 4;
                                        break;

            case T_BYTE_DIRECTIVE:      DC+=1;
                                        break;

            case T_HALF_DIRECTIVE:      DC+=2;
                                        break;
            case T_COMMENT:             break;

            case T_DATA_DIRECTIVE:      IS_DATA_SEGMENT = TRUE;  break;
            case T_TEXT_DIRECTIVE:      IS_DATA_SEGMENT = FALSE; break;
            case LABEL:     if( LABEL_ADDRESS.count(value1)) yyerror("Label already exists");
                            LABEL_ADDRESS[value1] = (IS_DATA_SEGMENT ? DC : PC); break;
        }
        /* Data padding */
        if( DC % PADDING_BYTES_SIZE )
            DC += PADDING_BYTES_SIZE - DC % PADDING_BYTES_SIZE;
    }
}

void switch_2(){
    int32_t tok, tok1;
    int32_t val;
    while( tok = yylex() ){
        switch(tok){
            /*** Regular instructions ***/
            case R_INS_4:   val = f_r4();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;
            case R_INS_3:   val = f_r3();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;
            case R_INS_2:   val = f_r2();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;
            case R_INS_1:   val = f_r1();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;

            case I_INS_3:   val = f_i3();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;
            case I_INS_2:   val = f_i2();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;
            case I_INS_1:   val = f_i1();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;

            case J_INS_1:   val = f_j1();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;

            case SYSCALL:   val = f_syscall();fwrite(&val,sizeof(int32_t),1,executable); PC+=4; break;


            /*** Pseudo-instructions ***/
            case INST_NOP:  f_nop(); break;
            case INST_LI:   f_li(); break;
            case INST_LA:   f_la(); break;
            case INST_MOVE: f_move(); break;
            // case ...


            /*** Directives ***/
            case T_ASCIIZ_DIRECTIVE:    if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != STRING ) yyerror("Asciiz directive should be followed by a string.");
                                        fwrite(value1,1,strlen(value1) + 1,executable);
                                        printf("Write \'%s\' to data (DC: %#010x)\n", value1, DC); DC += strlen(value1) + 1;
                                        break;

            case T_WORD_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Word directive should be followed by an integer.");
                                        rformatnum(&val);
                                        fwrite(&val,sizeof(int32_t),1,executable); DC += 4;
                                        break;

            case T_BYTE_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Byte directive should be followed by an integer or a char.");
                                        rformatnum(&val);
                                        fwrite(&val,1,1,executable); DC+=1;
                                        break;

            case T_HALF_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Half directive should be followed by an integer.");
                                        rformatnum(&val);
                                        fwrite(&val,2,1,executable); DC+=2;
                                        break;

            case T_GLOBL_DIRECTIVE:     if((tok1=yylex()) != T_ID) yyerror("Global directive should be followed by an ID.");
                                        if(strcmp(value1, "main") == 0) IS_GLOBAL_MAIN = TRUE;
                                        // do something ?
                                        break;

            case T_END_DIRECTIVE:       if((tok1=yylex()) != T_ID) yyerror("End directive should be followed by an ID.");
                                        if(LABEL_ADDRESS.count(value1) == 0) yyerror("End's ID not found.");
                                        // do something ?
                                        break;

            case T_DATA_DIRECTIVE:      IS_DATA_SEGMENT = TRUE;  break;
            case T_TEXT_DIRECTIVE:      IS_DATA_SEGMENT = FALSE; break;
            /*** Bad syntax ***/
            case LPAR:
            case RPAR:
            case COLON:
            case T_ID:
            case T_HEX_NUM:
            case T_REG:
            case STRAY:
                            yyerror("Syntax error or not supported instructions."); break;


            /* Do nothing */
            case T_COMMENT: break;
        }

        /* Data padding */
        if( DC % PADDING_BYTES_SIZE ){
            int offset = PADDING_BYTES_SIZE - DC % PADDING_BYTES_SIZE;
            for(int i = 0; i < offset; i++)
                fputc('\0', executable);
            DC += offset;
        }
    }
}

void reset_counters(){
    DC = START_DATA_SEGMENT;
    PC = START_TEXT_SEGMENT;
    yylineno = 1;
}

int main(int argv,char *arg[]){

    if(argv != 2){
        exit(-1);
    }
    yyin = fopen(arg[1],"r");
    executable = fopen("file.mips","w+");
    switch_1();

    /* Print the header of the exeutable */
    DATA_SEGMENT_SIZE = DC - START_DATA_SEGMENT;
    TEXT_SEGMENT_SIZE = PC - START_TEXT_SEGMENT;
    START_MAIN_ADDRESS = LABEL_ADDRESS["main"];
    fwrite(&DATA_SEGMENT_SIZE,sizeof DATA_SEGMENT_SIZE,1,executable);
    fwrite(&TEXT_SEGMENT_SIZE,sizeof TEXT_SEGMENT_SIZE,1,executable);
    fwrite(&START_MAIN_ADDRESS,sizeof START_MAIN_ADDRESS,1,executable);
    /* restarting execution over the file*/
    fclose(yyin);
    yyin = fopen(arg[1],"r");
    yyrestart(yyin);
    reset_counters();
    switch_2();
    fclose(executable);

    return 0;
}
