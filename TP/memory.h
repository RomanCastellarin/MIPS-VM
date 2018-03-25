// MEMORY.H
#ifndef MEMORY_H
#define MEMORY_H

/*
 * This module implements the Main Memory of the virtual machine, comprising:
 * Text Segment, Data Segment (no distinction between ro/rw), Heap, and Stack
 */

#include <vector>
#include <cstdint>
#include <cstdlib>

// address type
typedef uint32_t addr_t;

// memory space
const uint32_t STACK_START = 0x7FFFFFFC;
const uint32_t HEAP_START  = 0x10010000; 
const uint32_t DATA_START  = 0x10000000; 
const uint32_t TEXT_START  = 0x00400000; 
extern uint32_t MAIN_START; 

// Parameters
extern uint32_t DATA_SIZE;
extern uint32_t TEXT_SIZE;
extern uint32_t STACK_SIZE;

// Memory Pointers
extern uint8_t *DATA_POINTER;
extern uint8_t *TEXT_POINTER;
extern uint8_t *STACK_POINTER;

// Heap Memory
extern std::vector<uint8_t> Heap;

// change program's break (sbrk)
addr_t data_break(int32_t);

// resolve virtual address to "real"
void* resolve_addr(addr_t addr);

// reserve stack size 
int ensure_stack_size(size_t cap);

// TODO: methods for reading and writing 8, 16, 32 and 64 bits from memory

// read from main memory
//int memory_read(addr_t);

// write to main memory
//int memory_write(addr_t);


#endif
