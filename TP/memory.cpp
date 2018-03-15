#include "memory.h"
#include "regs.h"


// OS assigned page break;
const addr_t heap_top = HEAP_START;

// change program's break (sbrk)
addr_t data_break(int32_t delta){
    // convention: delta zero returns current heap_top
    if( delta == 0 ) return heap_top;

    auto old_top = heap_top;
    auto new_top = heap_top + delta;

    // TODO: implement
    /*if( new_top is valid ){
        heap_top = new_top;
        return old_top;
    }*/

    return -1;

}

// resolve virtual address to "real"
void* resolve_addr(addr_t addr){

    // TODO: implement memory parts, link to this function

    // reserved space
    if( addr < TEXT_START or addr >= STACK_START )
        return nullptr;

    // text segment
    if( addr < DATA_START ){
        // return TEXT_POINTER + (addr - TEXT_START);
    }

    // data segment
    if( addr < HEAP_START ){
        // return DATA_POINTER + (addr - DATA_START);
    }

    // stack
    if( addr >= R[REG_SP] ){
        // return STACK_POINTER + (addr - R[REG_SP]);
    }

    // heap
    if( addr >= R[REG_SP] ){
        // return HEAP_POINTER + (addr - HEAP_START);
    }

    // invalid
    return nullptr;
}
