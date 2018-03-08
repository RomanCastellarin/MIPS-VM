#include "regs.h"
#include "syscalls.h"
#include <cstdio>
using namespace std;


int main(){
    // pseudo testing syscalls and registers
    
    R[REG_V0] = 1;
    R[REG_A0] = 65;
    syscall();

    R[REG_V0] = 11;
    R[REG_A0] = 65;
    syscall();
    
    return 0;
}
