// SYSCALLS.H
#ifndef SYSCALLS_H
#define SYSCALLS_H

// handle syscalls
int syscall();

// syscalls
enum {
    SC_PRINT_INTEGER    = 1,
    SC_PRINT_FLOAT      = 2,
    SC_PRINT_DOUBLE     = 3,
    SC_PRINT_STRING     = 4,
    SC_READ_INTEGER     = 5,
    SC_READ_FLOAT       = 6,
    SC_READ_DOUBLE      = 7,
    SC_READ_STRING      = 8,
    SC_MALLOC           = 9,
    SC_EXIT             = 10,
    SC_PRINT_CHAR       = 11
    // maybe add more?
};


#endif
