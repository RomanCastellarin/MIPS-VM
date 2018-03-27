#include "simulator.h"
#include "regs.h"
#include "instructions.h"
#include "memory.h"
#include <cstdio>

// initialize simulator (current program stays in memory)
int initialize(){
    // TODO: complete
    // 
    PC = MAIN_START;

    
    R[REG_SP] = STACK_START;
    return 0;
}

int reset(){
    // TODO: complete
    delete[] DATA_POINTER;  DATA_POINTER  = nullptr;
    delete[] TEXT_POINTER;  TEXT_POINTER  = nullptr;
    delete[] STACK_POINTER; STACK_POINTER = nullptr;
    
    DATA_SIZE = 0;
    TEXT_SIZE = 0;
    STACK_SIZE = 0;

    Heap.clear();
    
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
        //printf("status %d\n", status);
        if( status == -1 ) return -1;
    }else
        PC += 4; // TODO: standarize, advance(4);

    /// TODO: ENSURE
        /// ZERO IS ZERO
        /// STACK SIZE
        // what else?

    R[REG_ZERO] = 0;
    if( R[REG_SP] > STACK_START ) return -2; // TODO: standarize: what to do?
    ensure_stack_size( STACK_START - R[REG_SP] );
    
    return 0;
}
