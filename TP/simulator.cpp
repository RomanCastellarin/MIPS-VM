#include "simulator.h"
#include "regs.h"
#include "instructions.h"
#include "memory.h"
#include <cstdio>

int initialize(){
    // TODO: complete
    PC = TEXT_START;
    return 0;
}

int reset(){
    // TODO: complete
    if( DATA_POINTER ) delete[] DATA_POINTER;
    if( TEXT_POINTER ) delete[] TEXT_POINTER;
    DATA_SIZE = 0;
    TEXT_SIZE = 0;
    return 0;
}

int step(){
    instruction *p = (instruction*) resolve_addr(PC);
    if( !p )
        return -1; // TODO: standarize

    //printf("INSTRUCTION 0x%X\n", *p);
    int status = decode(*p);

    //printf("PC 0x%X    t0 0x%X    a0 0x%X    v0 0x%X    s0 0x%X    status %i\n", PC, R[REG_T0], R[REG_A0], R[REG_V0], R[REG_S0], status);fflush(stdout);
    
    if( status ){
        // TODO: ? 
    }else
        PC += 4; // TODO: standarize, advance(4);
    return 0;
}
