#include "regs.h"
#include "syscalls.h"
#include "instructions.h"
#include <cstdio>
using namespace std;


int main(){
    // pseudo testing syscalls and registers
    
    R[REG_V0] = 1;
    R[REG_A0] = 65;
    syscall();
    puts("");

    R[REG_V0] = 11;
    R[REG_A0] = 65;
    syscall();
    puts("");

    // pseudo testing R instructions

    R[REG_A3] = -6;
    R[REG_S7] = 10;
    int status = decode(0x02E74820); // add $t1, $s7, a3
    printf("%d (status %d)\n", R[REG_T1], status);
    
    R[REG_T1] = 6989001;
    R[REG_S7] = 565432;
    status = decode(0x01370018); // mult $t1, $s7
    printf("%u %u (status %d)\n", HI, LO, status);
    // prod = 3951804813432 (base 10) =
    //111001100000011001111011000001000001111000

    R[REG_T1] = 6989001;
    R[REG_S7] = -198765;
    status = decode(0x01370018); // mult $t1, $s7
    printf("%u %u (status %d)\n", HI, LO, status);
    // prod = -1389168783765 (base 10) =
    //1111111111111111111111101011110010001111000101101000111001101011
    //1111111111111111111111101011110010001111000101101000111001101011

    R[REG_V0] = 6;
    status = decode(0x000230C0); // sll $a2, $v0, 3
    printf("%u (status %d)\n", R[REG_A2], status);
    
    R[REG_T2] = -5;
    status = decode(0x2149FFFF); // addi $t1, $t2, -1
    printf("%d (status %d)\n", R[REG_T1], status);
       
    return 0;
}
