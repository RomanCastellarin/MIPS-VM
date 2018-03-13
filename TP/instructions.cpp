#include "instructions.h"
#include "regs.h"
#include <map>
#include <cstdio>
using namespace std;

// MIPS instruction parts
#define OPCODE(x)   (((x)&0xFC000000)>>26)
#define RS(x)       (((x)&0x3E00000)>>21)
#define RT(x)       (((x)&0x1F0000)>>16)
#define RD(x)       (((x)&0xF800)>>11)
#define SHAMT(x)    (((x)&0x7C0)>>6)
#define FUNCT(x)    ((x)&0x3F)
#define IMM(x)      ((x)&0xFFFF)
#define ADDR(x)     ((x)&0x3FFFFFF)

// HELPERS

// interpret immediate as a signed 16-bit integer (default: unsigned)
#define SIGN_IMM(x) ((int32_t(x)&0x7FFF) - (int32_t(x)&0x8000))

// INTRUCTOR DECLARATORS
#define DECL_R_INSTR(x) \
int x(int32_t rs, int32_t rt, int32_t rd, int32_t shamt)

#define DECL_I_INSTR(x) \
int x(int32_t rs, int32_t rt, int32_t imm)

// INSTRUCTIONS
DECL_R_INSTR(add){
    int32_t a = R[rs];
    int32_t b = R[rt];
    R[rd] = a + b;
    return 0;
}

DECL_R_INSTR(sub){
    int32_t a = R[rs];
    int32_t b = R[rt];
    R[rd] = a - b;
    return 0;
}

DECL_R_INSTR(mult){
    int64_t a = (int32_t) R[rs];
    int64_t b = (int32_t) R[rt];
    int64_t c = a * b;
    HI = c >> 32;
    LO = c & 0xFFFFFFFF;
    return 0;
}

DECL_R_INSTR(multu){
    uint64_t a = R[rs];
    uint64_t b = R[rt];
    uint64_t c = a * b;
    HI = c >> 32;
    LO = c & 0xFFFFFFFF;
    return 0;
}

DECL_R_INSTR(div){
    int32_t a = R[rs];
    int32_t b = R[rt];
    try{
        HI = a % b;
        LO = a / b;
        return 0;
    }catch( ... ){
        return -1;  // TODO: standarize: DISGUSTING !
    }

}

DECL_R_INSTR(divu){
    uint32_t a = R[rs];
    uint32_t b = R[rt];
    try{
        HI = a % b;
        LO = a / b;
        return 0;
    }catch( ... ){
        return -1;  // TODO: standarize: DISGUSTING !
    }

}

DECL_R_INSTR(slt){
    int32_t a = R[rs];
    int32_t b = R[rt];
    R[rd] = (a < b);
    return 0;
}

DECL_R_INSTR(sltu){
    uint32_t a = R[rs];
    uint32_t b = R[rt];
    R[rd] = (a < b);
    return 0;
}

DECL_R_INSTR(sll){
    R[rd] =  R[rt] << shamt;
    return 0;
}

DECL_R_INSTR(srl){
    R[rd] =  R[rt] >> shamt;
    return 0;
}

DECL_R_INSTR(sra){
    int32_t a = R[rt];
    R[rd] = a >> shamt;
    return 0;
}

DECL_R_INSTR(jr){ // TODO: FIND OUT HOW TO HANDLE MEMORY ADDRESSES
    return -1;
}

DECL_R_INSTR(and_){ // and is a reserved C++ keyword
    R[rd] = R[rs] & R[rt];
    return 0;
}

DECL_R_INSTR(or_){ // or is a reserved C++ keyword
    R[rd] = R[rs] | R[rt];
    return 0;
}

DECL_R_INSTR(xor_){ // xor is a reserved C++ keyword
    R[rd] = R[rs] ^ R[rt];
    return 0;
}

DECL_R_INSTR(nor){
    R[rd] = ~(R[rs] | R[rt]);
    return 0;
}

DECL_R_INSTR(mfhi){
    R[rd] = HI;
    return 0;
}

DECL_R_INSTR(mflo){
    R[rd] = LO;
    return 0;
}

DECL_I_INSTR(addi){
    int32_t a = R[rs];
    int32_t b = SIGN_IMM(imm);
    R[rt] = a + b;
    return 0;
}

DECL_I_INSTR(addiu){
    int32_t a = R[rs];
    R[rt] = a + imm;
    return 0;
}

DECL_I_INSTR(slti){
    int32_t a = R[rs];
    int32_t b = SIGN_IMM(imm);
    R[rt] = (a < b);
    return 0;
}

DECL_I_INSTR(sltiu){
    uint32_t a = R[rs];
    uint32_t b = imm;
    R[rt] = (a < b);
    return 0;
}

DECL_I_INSTR(andi){ // zero-extending (MIPS convention)
    R[rt] = R[rs] & imm;
    return 0;
}

DECL_I_INSTR(ori){ // zero-extending (MIPS convention)
    R[rt] = R[rs] | imm;
    return 0;
}


// R-INSTRUCTION MAP
map<int, inst_r_t> R_funct = {
    {0x00, sll},
    {0x02, srl},
    {0x03, sra},
    {0x08, jr},
    {0x10, mfhi},
    {0x12, mflo},
    {0x18, mult},
    {0x19, multu},
    {0x1A, div},
    {0x1B, divu},
    {0x20, add},
    {0x21, add}, // ADDU TODO: HANDLE OVERFLOW TRAP
    {0x22, sub},
    {0x23, sub}, // SUBU TODO: HANDLE OVERFLOW TRAP
    {0x24, and_},
    {0x25, or_},
    {0x26, xor_},
    {0x27, nor},
    {0x2A, slt},
    {0x2B, sltu}
};

// I-INSTRUCTION MAP
map<int, inst_i_t> I_opcodes = {
    {0x08, addi},
    {0x09, addiu},
    {0x0A, slti},
    {0x0B, sltiu},
    {0x0C, andi},
    {0x0D, ori}
};


// DECODE FUNCTION
int decode(instruction i){
    unsigned oc = OPCODE(i);
    if( oc == 0x00 ){
        // r type instruction
        int32_t rs = RS(i);
        int32_t rt = RT(i);
        int32_t rd = RD(i);
        int32_t shamt = SHAMT(i);
        int32_t funct = FUNCT(i);
        auto operation = R_funct[funct];
        if( operation )
            return operation(rs, rt, rd, shamt);
        else{
            // not implement or invalid TODO: STANDARIZE THIS S·H·I·T
            printf("funct %u not found\n", funct);
            return -2;
        }
    }
    auto operation = I_opcodes[oc];
    if( operation ){
        int32_t rs = RS(i);
        int32_t rt = RT(i);
        int32_t imm = IMM(i);
        return operation(rs, rt, imm);
    }else
        // not implemented
        printf("opcode %u not found\n", oc);
        return -1;
}
