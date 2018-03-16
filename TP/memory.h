// MEMORY.H
#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

// address type
typedef uint32_t addr_t;

// memory space
const uint32_t STACK_START = 0x7FFFFFFC;
const uint32_t HEAP_START  = 0x10010000; 
const uint32_t DATA_START  = 0x10000000; 
const uint32_t TEXT_START  = 0x00400000; 
 
// change program's break (sbrk)
addr_t data_break(int32_t);

// resolve virtual address to "real"
void* resolve_addr(addr_t addr);

// read from main memory
//int memory_read(addr_t);

// write to main memory
//int memory_write(addr_t);


#endif
