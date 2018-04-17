
extern int32_t  yylex();
extern void     yyerror(const char*);
extern void     rformatnum(int32_t*);
extern void     updateinstr(int32_t*);

#define fitsInShort(x) !(((((x) & 0xffff8000) >> 15) + 1) & 0x1fffe)

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

/* Utility Functions */

void yyerror(const char *err){ fprintf(stderr, "ERROR: %s\nLINE: %d.\n", err, yylineno); exit(-1); }/* Handle the 'err' error and exit the program */

void rformatnum(int32_t *num){ sscanf(value1, "%i", num); }                                         /* Reads a T_HEX_NUM as 0x... or integer */
void updateinstr(int32_t *instr){ fwrite(instr,sizeof(int32_t),1,executable); PC+=4; }              /* Prints the instruction to the executable and updates PC */



/* R-type Instructions */

int32_t f_r4(){
    bool using_rd = ( (strcmp(yytext,"mfhi") == 0) or (strcmp(yytext,"mflo") == 0) );
    int32_t tok1;
    int32_t reg;

    tok1 = yylex(); reg = value;

    if( tok1 == T_REG )
        return OP(opcode) + (using_rd ? RD(reg) : RS(reg) ) + (funct);

    yyerror("Wrong R-Type instruction formatting, make sure to be using INST $REG.");
}

int32_t f_r3(){
    int32_t tok1, tok2;
    int32_t rs, rt;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + (funct));

    yyerror("Wrong R-Type instruction formatting, make sure to be using INST $REG, $REG.");
}

int32_t f_r2(){
    int32_t tok1, tok2, tok3;
    int32_t rs, rd, rt;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); rt = value;

    if( tok1 == T_REG && tok2 == T_REG && tok3 == T_REG )
        return (OP(opcode) + RS(rs) + RT(rt) + RD(rd) + (funct));

    yyerror("Wrong R-Type instruction formatting, make sure to be using INST $REG, $REG, $REG.");
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

    yyerror("Wrong R-Type instruction formatting, make sure to be using INST $REG, $REG, SHAMT.");
}

int32_t f_syscall(){ return 0xC; }

/* I-Type Instructions */

int32_t f_i3(){
    int32_t instruction;
    int32_t tok1,tok2,tok3,tok4,tok5;
    int32_t rt, rs;
    int32_t imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex();

    if( tok1 != T_REG ) yyerror("First argument of a Type-I Instruction should be a register.");

    if( tok2 == T_ID or tok2 == T_ID_HEX ){ // INST T_REG, T_ID/T_ID+IMM, pseudoinstruction
        if(tok2 == T_ID){
            if(LABEL_ADDRESS.count(value1) == 0) yyerror("ID not found.");
            imm = LABEL_ADDRESS[value1];
        }
        if(tok2 == T_ID_HEX){
            if(LABEL_ADDRESS.count(value2) == 0) yyerror("ID not found.");
            rformatnum(&imm);
            imm += LABEL_ADDRESS[value2];
        }

        // Now LW $REG, DATA is gonna be split into two instructions (where DATA is T_ID+IMM):
        // lui $1, DATA_hi
        // lw $REG, DATA_lo($1)

        opcode = 15; // LUI opcode
        instruction = OP(opcode) + RT(1) + IMM(imm>>16);
        updateinstr(&instruction);

        opcode = 33; // LW opcode
        instruction = OP(opcode) + RS(1) + IMM(imm);

        return instruction;
    }

    if( tok2 == T_HEX_NUM ){ // INST T_REG, IMM(T_REG)
        rformatnum(&imm);

        tok3 = yylex();
        tok4 = yylex(); rs = value;
        tok5 = yylex();

        if( not (tok3 == LPAR && tok4 == T_REG && tok5 == RPAR) ) yyerror("Syntax error.");

        instruction = OP(opcode) + RT(rt) + RS(rs) + IMM(imm);
        return instruction;
    }

    yyerror("Wrong I-Type instruction formatting, make sure to be using INST $REG, IMM($REG) or INST $REG, ID+IMM.");
}

int32_t f_i2(){
    bool using_rt = (strcmp(yytext,"lui") == 0);

    int32_t tok1,tok2;
    int32_t reg;
    int32_t imm;

    tok1 = yylex(); reg = value;
    tok2 = yylex();

    if( tok1 == T_REG ){
        if(tok2 == T_HEX_NUM){
            rformatnum(&imm);
        }

        if(tok2 == T_ID){
            if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");
            imm = (LABEL_ADDRESS[value1] - PC)/4;
        }
        return OP(opcode) + ( using_rt ? RT(reg) : RS(reg) ) + IMM(imm);
    }

    yyerror("Wrong I-Type instruction formatting, make sure to be using INST $REG, IMM/ID.");
}

int32_t f_i1(){
    bool using_rs_rt = ( (strcmp(yytext,"beq") == 0) or (strcmp(yytext,"bne") == 0) );

    int32_t tok1,tok2,tok3;
    int32_t rs,rt,regs;
    int32_t imm, instruction;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex();


    if( tok1 == T_REG && tok2 == T_REG ){
        if( tok3 == T_HEX_NUM ){
            rformatnum(&imm);
        }

        if( tok3 == T_ID ){
            if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");
            imm = (LABEL_ADDRESS[value1] - PC)/4;
        }
        regs = using_rs_rt ? (RS(rt) + RT(rs)) : (RS(rs) + RT(rt));
        instruction = OP(opcode) + regs + IMM(imm);

        return instruction;
    }

    yyerror("Wrong I-Type instruction formatting, make sure to be using INST $REG, $REG, IMM/ID.");
}

/* J-Type Instructions */

int32_t f_j1(){
    int32_t tok1;
    int32_t addr, instruction;

    tok1 = yylex();
    if(tok1 == T_ID){ // INST T_ID
        if( LABEL_ADDRESS.count(value1) == 0 ) yyerror("ID not found.");
        addr = LABEL_ADDRESS[value1] & 0x0FFFFFFF;  // To fit 26 bits we need to get rid of the first 4 bits
        addr >>= 2;  // Since every address is padded to 4 bytes, we don't need to store the last 2 bits

        instruction = OP(opcode) + addr;
        updateinstr(&instruction);

        instruction = 0x0; //Print a NOP after
        return instruction;
    }

    yyerror("Wrong J-Type instruction formatting, make sure to be using INST ID.");
}


/* Pseudo-Instructions */

void f_nop(){ int32_t val = 0; updateinstr(&val); }
void f_li(){
    int32_t instruction;
    int32_t tok1, tok2;
    int32_t rt, rs, imm;

    tok1 = yylex(); rt = value;
    tok2 = yylex(); rformatnum(&imm);

    if( tok1 != T_REG or tok2 != T_HEX_NUM ) yyerror("Wrong LI instruction formatting, make sure to be using LI $REG, IMM.");

    if( !fitsInShort(imm) ){
        // IMM is a 32-bit value
        // LI T_REG, T_HEX_NUM is gonna be split into two instructions:
        // LUI T_REG, T_HEX_NUM_hi
        // ORI T_REG, T_REG, T_HEX_NUM_lo

        opcode = 15; // LUI opcode
        instruction = OP(opcode) + RT(rt) + IMM(imm>>16);
        updateinstr(&instruction);

        opcode = 13; // ORI opcode
        rs = rt;
        instruction = OP(opcode) + RT(rt) + RS(rs) + IMM(imm);
        updateinstr(&instruction);

    } else {
        // IMM is a 16-bit value
        // We can directly use: ADDIU T_REG, $0, T_HEX_NUM

        opcode = 9; // ADDIU opcode
        rs = 0x0;
        instruction = OP(opcode) + RS(rs) + RT(rt) + IMM(imm);

        updateinstr(&instruction);
    }
}

void f_la(){
    int32_t instruction;
    int32_t tok1, tok2;
    int32_t rt, rs;

    tok1 = yylex(); rt = value;
    tok2 = yylex();

    if( tok1 != T_REG or tok2 != T_ID ) yyerror("Wrong LA instruction formatting, make sure to be using LA $REG, ID.");

    // LA T_REG, T_ID is gonna be split into two instructions:
    // LUI T_REG, T_ID_hi
    // ORI T_REG, T_REG, T_ID_lo

    opcode = 15; // LUI opcode
    instruction = OP(opcode) + RT(rt) + IMM(LABEL_ADDRESS[value1]>>16);
    updateinstr(&instruction);

    opcode = 13; // ORI opcode
    rs = rt;
    instruction = OP(opcode) + RT(rt) + RS(rs) + IMM(LABEL_ADDRESS[value1]);
    updateinstr(&instruction);
}

void f_ble(){
    int32_t instruction;
    int32_t tok1, tok2, tok3;
    int32_t rt, rs, offset;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;
    tok3 = yylex();

    if( tok1 != T_REG or tok2 != T_REG or tok3 != T_ID ) yyerror("Wrong BLE instruction formatting, make sure to be using BLE $REG, $REG, ID.");

    // BLE $S, $T, T_ID is gonna be split into two instructions:
    // SLT $1, $T, $S
    // BEQ $1, $0, T_ID

    opcode = 0; // SLT opcode
    funct = 42; // SLT funct

    instruction = OP(opcode) + RS(rt) + RT(rs) + RD(1) + (funct);
    updateinstr(&instruction);

    opcode = 4; // BEQ opcode
    offset = (LABEL_ADDRESS[value1]-PC) / 4;
    instruction = OP(opcode) + RS(1) + IMM( offset );
    updateinstr(&instruction);
}

void f_bge(){
    int32_t instruction;
    int32_t tok1, tok2, tok3;
    int32_t rt, rs, offset;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;
    tok3 = yylex();

    if( tok1 != T_REG or tok2 != T_REG or tok3 != T_ID ) yyerror("Wrong BLE instruction formatting, make sure to be using BLE $REG, $REG, ID.");

    // BGE $S, $T, T_ID is gonna be split into two instructions:
    // SLT $1, $S, $T
    // BEQ $1, $0, T_ID

    opcode = 0; // SLT opcode
    funct = 42; // SLT funct

    instruction = OP(opcode) + RD(1) + RS(rs) + RT(rt) + (funct);
    updateinstr(&instruction);

    opcode = 4; // BEQ opcode
    offset = (LABEL_ADDRESS[value1]-PC) / 4;
    instruction = OP(opcode) + RS(1) + IMM( offset );
    updateinstr(&instruction);
}

void f_bgt(){
    int32_t instruction;
    int32_t tok1, tok2, tok3;
    int32_t rt, rs, offset;

    tok1 = yylex(); rs = value;
    tok2 = yylex(); rt = value;
    tok3 = yylex();

    if( tok1 != T_REG or tok2 != T_REG or tok3 != T_ID ) yyerror("Wrong BGT instruction formatting, make sure to be using BGT $REG, $REG, ID.");

    // BGT $S, $T, T_ID is gonna be split into two instructions:
    // SLT $1, $T, $S
    // BNE $1, $0, T_ID

    opcode = 0; // SLT opcode
    funct = 42; // SLT funct

    instruction = OP(opcode) + RD(1) + RT(rs) + RS(rt) + (funct);
    updateinstr(&instruction);

    opcode = 5; // BNE opcode
    offset = (LABEL_ADDRESS[value1]-PC) / 4;
    instruction = OP(opcode) + RS(1) + IMM( offset );
    updateinstr(&instruction);
}

void f_move(){
    int32_t instruction;
    int32_t tok1, tok2, tok3;
    int32_t rt, rs, rd;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;

    if( tok1 != T_REG or tok2 != T_REG ) yyerror("Wrong MOVE instruction formatting, make sure to be using MOVE $REG, $REG.");

    // MOVE $T, $S translates to:
    // ADD $T, $S, $0

    opcode = 0; // ADD opcode
    funct = 32; // ADD funct
    instruction = OP(opcode) + RD(rd) + RS(rs) + (funct);
    updateinstr(&instruction);
}

void f_mul(){
    int32_t instruction;
    int32_t tok1, tok2, tok3;
    int32_t rt, rs, rd;

    tok1 = yylex(); rd = value;
    tok2 = yylex(); rs = value;
    tok3 = yylex(); rt = value;

    if( tok1 != T_REG or tok2 != T_REG or tok3 != T_REG ) yyerror("Wrong MUL instruction formatting, make sure to be using MUL $REG, $REG, $REG.");

    // MUL $D, $S, $T translates to:
    // MULT $S, $T
    // MFLO $D

    opcode = 0; // MULT opcode
    funct = 24; // MULT funct
    instruction = OP(opcode) + RS(rs) + RT(rt) + (funct);
    updateinstr(&instruction);

    opcode = 0; // MFLO opcode
    funct = 18; // MFLO funct
    instruction = OP(opcode) + RD(rd) + (funct);
    updateinstr(&instruction);
}

// ...


/***  ***/

void assing_label_addresses(){
    int32_t tok, tok1;
    int32_t val;

    while( tok = yylex() ){
        switch(tok){
            /*** Regular instructions ***/
            case R_INS_4:   PC+= 4; break;
            case R_INS_3:   PC+= 4; break;
            case R_INS_2:   PC+= 4; break;
            case R_INS_1:   PC+= 4; break;

            case I_INS_3:   yylex(); tok1 = yylex();
                            if(tok1 == T_ID or tok1 == T_ID_HEX) PC+=8;
                            else PC+=4;
                            break;
            case I_INS_2:   PC+= 4; break;
            case I_INS_1:   PC+= 4; break;
            case J_INS_1:   PC+= 8; break;
            case SYSCALL:   PC+= 4; break;


            /*** Pseudo-instructions ***/
            case INST_NOP:  PC+= 4; break;
            case INST_LI:   yylex(); yylex(); rformatnum(&val); /* LI is 1 instruction if val is a 16-bit number, 2 instructions otherwise */
                            if( !fitsInShort(val) ) PC+= 8; else PC+= 4;
                            break;
            case INST_LA:   PC+= 8;     break;
            case INST_BLE:  PC+= 8;     break;
            case INST_BGT:  PC+= 8;     break;
            case INST_BGE:  PC+= 8;     break;
            case INST_MOVE: PC+= 4;     break;
            case INST_MUL:  PC+= 8;     break;
            // case ...

            /*** Directives ***/
            case T_ASCIIZ_DIRECTIVE:    yylex(); DC += strlen(value1) + 1; break;
            case T_WORD_DIRECTIVE:      DC+=4;  break;
            case T_BYTE_DIRECTIVE:      DC+=1;  break;
            case T_HALF_DIRECTIVE:      DC+=2;  break;
            case T_COMMENT:             break;

            case T_DATA_DIRECTIVE:      IS_DATA_SEGMENT = TRUE;  break;
            case T_TEXT_DIRECTIVE:      IS_DATA_SEGMENT = FALSE; break;

            case LABEL:     if( LABEL_ADDRESS.count(value1)) yyerror("Label already exists");
                            LABEL_ADDRESS[value1] = (IS_DATA_SEGMENT ? DC : PC);
                            break;
        }
        /* Data padding */
        if( DC % PADDING_BYTES_SIZE )
            DC += PADDING_BYTES_SIZE - DC % PADDING_BYTES_SIZE;
    }
}

void process_instructions(){
    int32_t tok, tok1;
    int32_t val;
    while( tok = yylex() ){
        switch(tok){
            /*** Regular instructions ***/
            case R_INS_4:   val = f_r4(); updateinstr(&val); break;
            case R_INS_3:   val = f_r3(); updateinstr(&val); break;
            case R_INS_2:   val = f_r2(); updateinstr(&val); break;
            case R_INS_1:   val = f_r1(); updateinstr(&val); break;

            case I_INS_3:   val = f_i3(); updateinstr(&val); break;
            case I_INS_2:   val = f_i2(); updateinstr(&val); break;
            case I_INS_1:   val = f_i1(); updateinstr(&val); break;

            case J_INS_1:   val = f_j1(); updateinstr(&val); break;

            case SYSCALL:   val = f_syscall(); updateinstr(&val); break;


            /*** Pseudo-instructions ***/
            case INST_NOP:  f_nop();    break;
            case INST_LI:   f_li();     break;
            case INST_LA:   f_la();     break;
            case INST_BLE:  f_ble();    break;
            case INST_BGT:  f_bgt();    break;
            case INST_BGE:  f_bge();    break;
            case INST_MOVE: f_move();   break;
            case INST_MUL:  f_mul();    break;
            // case ...


            /*** Directives ***/
            case T_ASCIIZ_DIRECTIVE:    if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != STRING ) yyerror("Asciiz directive should be followed by a string.");
                                        fwrite(value1,1,strlen(value1) + 1,executable);
                                        DC += strlen(value1) + 1;   /* Also copies the '\0' char */
                                        break;

            case T_WORD_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Word directive should be followed by an integer.");
                                        rformatnum(&val);
                                        fwrite(&val,sizeof(int32_t),1,executable); DC += 4;
                                        break;

            case T_BYTE_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Byte directive should be followed by an integer or a char.");
                                        rformatnum(&val);
                                        fwrite(&val,1,1,executable); DC += 1;
                                        break;

            case T_HALF_DIRECTIVE:      if( IS_DATA_SEGMENT == FALSE ) yyerror("Text segment is read-only.");
                                        if( (tok1=yylex()) != T_HEX_NUM ) yyerror("Half directive should be followed by an integer.");
                                        rformatnum(&val);
                                        fwrite(&val,2,1,executable); DC += 2;
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

int main(int argv, char *arg[]){

    if(argv != 2){
        exit(-1);
    }
    yyin = fopen(arg[1],"r");

    // TODO: add -o output file support
    executable = fopen("file.mips","wb");
    assing_label_addresses();

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
    process_instructions();
    fclose(executable);

    return 0;
}
