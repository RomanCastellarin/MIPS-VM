#include "syscalls.h"
#include "regs.h"
#include "memory.h"

#include <cstdio>

// TODO: should we perform an fflush(stdout) after every print?

// handle syscalls
int syscall(){
    
    switch( R[REG_V0] ){
        case SC_PRINT_INTEGER:{
            int32_t arg = R[REG_A0];
            printf("%d", arg); // TODO: standarize output (virtualize console)
            break;
        }
        case SC_PRINT_FLOAT:{
            float32_t arg = AS_FLOAT(F[12]);
            printf("%f", arg); // TODO: standarize output (virtualize console)
            break;
        }
        case SC_PRINT_DOUBLE:{
            float64_t arg = AS_DOUBLE(F[12]);
            printf("%f", arg); // TODO: standarize output (virtualize console)
            break;
        }
        case SC_PRINT_STRING:{
            char *arg = (char *) resolve_addr(R[REG_A0]);
            printf("%s", arg); // TODO: standarize output (virtualize console)
            break;
        }
        case SC_READ_INTEGER:{
            int32_t &val = reinterpret_cast<int32_t&> (R[REG_V0]);
            scanf("%i", &val); // TODO: standarize input (virtualize console)
            break;
        }
        case SC_READ_FLOAT:{
            float32_t &val = AS_FLOAT(F[0]);
            scanf("%f", &val); // TODO: standarize input (virtualize console)
            break;
        }
        case SC_READ_DOUBLE:{
            float64_t &val = AS_DOUBLE(F[0]);
            scanf("%lf", &val); // TODO: standarize input (virtualize console)
            break;
        }
        case SC_READ_STRING:{
            char format[16];
            char *arg = (char *) resolve_addr(R[REG_A0]);
            int32_t maxlen = R[REG_A1];
            snprintf(format, sizeof(format), "%%%u[^\\n]", maxlen);
            scanf(format, arg); // TODO: standarize input (virtualize console)
            break;
        }
        case SC_MALLOC:{
            // TODO: Complete (depends on the memory module)
            break;
        }
        case SC_EXIT:{
            // TODO: Investigate how to return nicely
            break;
        }
        case SC_PRINT_CHAR:{
            int32_t arg = R[REG_A0]; // int for wide characters
            printf("%c", arg); // TODO: standarize output (virtualize console)
            break;
        }
        default:
            fprintf(stderr, "syscall not supported yet.\n");
            // TODO: implement error handling (read MIPS manual)
            return -1;
            break;
    }

    return 0; // TODO: define standard codes for SUCCESS-FAILURE in a header file
}
