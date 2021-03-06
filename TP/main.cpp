#include "simulator.h"
#include "regs.h"
#include "syscalls.h"
#include "instructions.h"
#include "files.h"
#include "memory.h"
#include <cstdio>
using namespace std;

#include <cstring>

int main(){

    // test shell
    initialize();
    shell();

    // test loads and stores

    initialize();

    R[REG_SP] -= 4;
    ensure_stack_size(STACK_START - R[REG_SP]);
    R[REG_T0] = 0x12345678;
    decode(0xAFA80000); // sw t0, 0($sp)
    decode(0x83A80000); // lb t0, 0($sp)
    decode(0x83A90001); // lb t1, 0($sp)
    decode(0x83AA0002); // lb t2, 0($sp)
    decode(0x83AB0003); // lb t3, 0($sp)
    printf("%X%X%X%X\n", R[REG_T0], R[REG_T1], R[REG_T2], R[REG_T3]); //78563412

    R[REG_T0] = 0xFF; // +255
    decode(0xAFA80000); // sw t0, 0($sp)
    decode(0x83A80000); // lb t0, 0($sp)
    printf("%d\n", R[REG_T0]); // -1
    decode(0x93A80000); // lbu t0, 0($sp)
    printf("%d\n", R[REG_T0]); // 255

    R[REG_T0] = 0xAB;
    decode(0xA3A80001); // sb t0, 1($sp)
    decode(0x8FA80000); // lw t0, 0($sp)
    printf("%X\n", R[REG_T0]); // 255

    R[REG_SP] += 4;

    return 0;

    // test program

    initialize();

    load_program("file.mips");

    while(true){
        int s = step();
        if( s == -1 ){
            puts("oh no");
            return -1;
        }
        
        // getchar(); // step by step
    }
    
    return 0;

    // test malloc
    
    initialize();

    R[REG_V0] = 9;
    R[REG_A0] = 5; // allocate 5 bytes
    syscall();
    char *addr1 = (char *) resolve_addr(R[REG_V0]);
    memcpy(addr1, "HELLO", 5);
    printf("addr1 = 0x%X\n", addr1);

    R[REG_V0] = 9;
    R[REG_A0] = 5; // allocate 6 bytes
    syscall();
    char *addr2 = (char *) resolve_addr(R[REG_V0]);
    memcpy(addr2, " WORLD\0", 7);
    printf("addr2 = 0x%X\n", addr2);

    addr1 = (char *) resolve_addr(HEAP_START);
    printf("addr1 = 0x%X %s\n", addr1, addr1);

    return 0;

    // test stack
    
    initialize();

    void *p = resolve_addr(R[REG_SP]);
    printf("0x%X\n", p);

    R[REG_SP] -= 12; // space for 3 words;
    ensure_stack_size( STACK_START - R[REG_SP] );
    printf("stack size %u\n", STACK_SIZE);
    
    p = resolve_addr(R[REG_SP]);
    printf("0x%X\n", p);
    strcpy((char*)p,"HELLO WORLD");

    ensure_stack_size( 20 );
    printf("stack size %u\n", STACK_SIZE);
    p = resolve_addr(R[REG_SP]);
    printf("0x%X %s\n", p, p);


    return 0;

    

    // pseudo testing syscalls and registers

    R[REG_V0] = 1;
    R[REG_A0] = 65;
    syscall();
    puts("");

    R[REG_V0] = 11;
    R[REG_A0] = 65;
    syscall();
    puts("");

    R[REG_V0] = 2;
    AS_FLOAT(F[12]) = -7.3;
    syscall();
    printf("\n$f12 = %x\n", F[12]); 
    printf("$f13 = %x\n", F[13]); 
    puts("");

    R[REG_V0] = 3;
    AS_DOUBLE(F[12]) = -7.3;
    syscall();
    printf("\n$f12 = %x\n", F[12]); 
    printf("$f13 = %x\n", F[13]); 
    printf("as double = 0x%x%x\n", F[13], F[12]); 
    puts("");

    R[REG_V0] = SC_READ_DOUBLE;
    printf("Enter a double "); fflush(stdout);
    syscall();
    printf("\n$f0 = %x\n", F[0]); 
    printf("$f1 = %x\n", F[1]); 
    printf("as double = 0x%x%x\n", F[1], F[0]); 
    puts("");

    R[REG_V0] = SC_READ_INTEGER;
    printf("Enter an int "); fflush(stdout);
    syscall();
    printf("\n$v0 = %d = %x\n", R[REG_V0], R[REG_V0]); 
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

    R[REG_V0] = 7;
    R[REG_V1] = 3;
    status = decode(0x0043001A); // div $v0,$v1
    printf("%d %d (status %d)\n", HI, LO, status );

    PC = 0xB0000000;
    status = decode(0x0C000011); // jal 17 (i.e. to address 0xB0000044)
    printf("%x %x (status %d)\n", PC, R[REG_RA], status );

    PC = 0xB0000000;
    status = decode(0x10840008); // beq $a0, $a0, 8 (as offset) 
    printf("%x (status %d)\n", PC, status );
    
    return 0;
}
