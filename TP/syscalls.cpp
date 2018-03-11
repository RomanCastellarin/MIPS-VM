#include "syscalls.h"
#include "regs.h"

#include <cstdio>

// handle syscalls
int syscall(){
    switch( R[REG_V0] ){
        int arg;
        case SC_PRINT_INTEGER:
            arg = R[REG_A0];
            printf("%d", arg); // TODO: standarize output (virtualize console)
            break;

        case SC_PRINT_CHAR:
            arg = R[REG_A0];
            printf("%c", arg); // TODO: standarize output (virtualize console)
            break;

        default:
            fprintf(stderr, "syscall not supported yet.\n");
            // TODO: implement error handling (read MIPS manual)
            return -1;
            break;
    }

    return 0; // TODO: define standard codes for SUCCESS-FAILURE in a header file
}
