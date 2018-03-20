#include "memory.h"
#include "regs.h"
#include <cstdio>
#include <cstring>

// Parameters
uint32_t DATA_SIZE = 0;
uint32_t TEXT_SIZE = 0;
uint32_t STACK_SIZE = 0; // TODO: move constants to header file

// Memory Pointers
uint8_t *DATA_POINTER;
uint8_t *TEXT_POINTER;
uint8_t *STACK_POINTER;

// Heap Memory
std::vector<uint8_t> Heap;

// change program's break (sbrk)
addr_t data_break(int32_t delta){

    // calculate heap top
    addr_t heap_top = HEAP_START + Heap.size();

    // convention: delta zero returns current heap_top
    if( delta == 0 ) return heap_top;

    auto old_top = heap_top;
    auto new_top = heap_top + delta;

    // check if new top is valid
    if( HEAP_START <= new_top and new_top <= R[REG_SP] ){
        Heap.resize(new_top - HEAP_START);
        return old_top;
    }

    return -1; // this is standard (sbrk) 

}

// resolve virtual address to "real"
void* resolve_addr(addr_t addr){

    // reserved space
    if( addr < TEXT_START or addr >= STACK_START )
        return nullptr;

    // text segment
    if( addr < DATA_START ){
        auto delta = addr - TEXT_START;
        return delta < TEXT_SIZE ? TEXT_POINTER + delta : nullptr;
    }

    // data segment
    if( addr < HEAP_START ){
        auto delta = addr - DATA_START;
        return delta < DATA_SIZE ? DATA_POINTER + delta : nullptr;
    }

    // heap
    if( addr < HEAP_START + Heap.size() )
        return Heap.data() + (addr - HEAP_START);

    // stack
    if( addr >= R[REG_SP] )
        return STACK_POINTER + (STACK_SIZE - (STACK_START - addr));

    // invalid
    return nullptr;
}

int ensure_stack_size(size_t cap){

    // constraint already satisfied
    if( STACK_SIZE >= cap ) return 0;
    
    // calculate new stack size
    size_t new_size = 1;
    while( new_size < cap ) new_size <<= 1;
    auto delta = new_size - STACK_SIZE;

    // try to reallocate
    try{
        auto *new_stack = new uint8_t[new_size];
        memmove(new_stack + delta, STACK_POINTER, STACK_SIZE);
        delete[] STACK_POINTER;
        STACK_POINTER = new_stack;
        STACK_SIZE = new_size;
    }catch(...){
        return -1; // TODO: standarize
    }

    return 0;    
}
