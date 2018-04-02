#include "simulator.h"
#include "regs.h"
#include "instructions.h"
#include "memory.h"
#include "files.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <map>

// Breakpoints
std::set<addr_t> Breakpoints;

// Ignore counter
unsigned int IC;

// Shell function type
typedef int (*sf_t)(char *);

#define DECL_SF(x) \
int x(char *args)

// initialize simulator (current program stays in memory)
int initialize(){
    // TODO: complete

    // clear registers
    clear_registers();

    // set PC
    PC = MAIN_START;

    // set stack pointer
    R[REG_SP] = STACK_START;
    
    return 0;
}

int reset(){
    // TODO: complete
    delete[] DATA_POINTER;  DATA_POINTER  = nullptr;
    delete[] TEXT_POINTER;  TEXT_POINTER  = nullptr;
    delete[] STACK_POINTER; STACK_POINTER = nullptr;
    
    DATA_SIZE = 0;
    TEXT_SIZE = 0;
    STACK_SIZE = 0;

    Heap.clear();
    
    return 0;
}

int step(){
    instruction *p = (instruction*) resolve_addr(PC);
    if( !p )
        return -1; // TODO: standarize

    //printf("INSTRUCTION 0x%X\n", *p);
    int status = decode(*p);
    
    //printf("PC 0x%X    t0 0x%X    a0 0x%X    v0 0x%X    s0 0x%X    status %i\n", PC, R[REG_T0], R[REG_A0], R[REG_V0], R[REG_S0], status);fflush(stdout);
    
    if( status ){
        // TODO: ?
        //printf("status %d\n", status);
        if( status == -1 ) return -1;
    }else
        PC += 4; // TODO: standarize, advance(4);

    /// TODO: ENSURE
        /// ZERO IS ZERO
        /// STACK SIZE
        // what else?

    R[REG_ZERO] = 0;
    if( R[REG_SP] > STACK_START ) return -2; // TODO: standarize: what to do?
    ensure_stack_size( STACK_START - R[REG_SP] );
    
    return 0;
}

DECL_SF(s_help){
    puts(
    "COMMANDS shortcut - fullname\n"
    "h help:                shows this help box\n"
    "l load <filename>:     loads indicated file\n"
    "n step:                performs one step of the simulation\n"
    "n step <n>:            performs n steps of the simulation\n"
    "r run:                 continues execution until break\n"
    "i init:                initializes VM for a second run\n"
    "w where:               print current PC and instruction\n"
    "b break <addr list>:   set breaks at given addresses (separated by spaces)\n"
    "b break:               shows all breaks\n"
    "c clear <addr list>:   clears given breaks (separated by spaces)\n"
    "c clear:               clears all breaks\n"
    "g ignore <n>:          ignore n consecutive breaks\n"
    "j jump <addr>:         set PC to given address\n"
    "s set <reg> <%i>:      set register to value (base 8, 10 and 16 allowed)\n"
    "f fset <reg> <%f>:     set register to float value\n"
    "d dset <reg> <%lf>:    set register reg and reg+1 to double value\n"
    "p print:               prints all registers\n"
    "q quit:                exits the machine\n"
    );
    return 0;
}

DECL_SF(s_load){
    if( !args ){
        puts("Please specify a filename or type h for help");
        return 0;
    }
    return load_program(args);
}

DECL_SF(s_step){
    int n = 1;
    if( args )
        sscanf(args, "%i", &n);
    for(int i = 0; i < n; ++i){
        int s = step();
        if( s ) return -1;
    }
    return 0;
}

DECL_SF(s_run){
    if( Breakpoints.count(PC) )
        IC++;
    while( true ){
        if( Breakpoints.count( PC ) ){
            if( IC > 0 ) IC--;
            else         return 0;
        }
        int s = step();
        if( s ) return -1;
    }
    return 0;
}

DECL_SF(s_init){
    return initialize();
}

DECL_SF(s_where){
    printf("PC = 0x%08X\tInstruction = ", PC);
    instruction *i = (instruction *) resolve_addr(PC);
    if( i ) printf("0x%08X\n", *i); // TODO: show in assembly
    else    puts("<invalid PC addr>");
    return 0;
}

DECL_SF(s_break){
    if( args ){
        do{
            addr_t brk;
            if( sscanf(args, "%i", &brk) == 1 ){
                Breakpoints.insert(brk);
                printf("Added break at position 0x%08X\n", brk);
            }else
                printf("%s is not a valid address\n", args);
        }while( (args = strtok(NULL, " \t\n")) );
    }else{
        puts("Current breaks: ");
        if( not Breakpoints.size() )
            puts("<none>");
        else
            for(auto &w: Breakpoints)
                printf("Break at 0x%08X\n", w);
    }
    return 0;
}

DECL_SF(s_clear){
    if( args ){
        do{
            addr_t brk;
            if( sscanf(args, "%i", &brk) == 1 ){
                Breakpoints.erase(brk);
                printf("Cleared break from position 0x%08X\n", brk);
            }else
                printf("%s is not a valid address\n", args);
        }while( (args = strtok(NULL, " \t\n")) );
    }else{
        puts("Clearing all breaks...");
        if( not Breakpoints.size() )
            puts("<nothing to clear>");
        else
            for(auto &w: Breakpoints)
                printf("Cleared break from position 0x%08X\n", w);
        Breakpoints.clear();
    }
    return 0;
}

DECL_SF(s_ignore){
    unsigned int n;
    if( !args or sscanf(args, "%u", &n) != 1 ){
        puts("Please specify a number.");
        return -1;
    }
    printf("Set number of ignores to %d.\n", n);
    IC = n;
    return 0;
}

DECL_SF(s_jump){
    addr_t a;
    if( !args or sscanf(args, "%i", &a) != 1 ){
        puts("Please specify a valid address.");
        return -1;
    }
    printf("PC set to 0x%X.\n", a);
    PC = a;
    return 0;
}

DECL_SF(s_set){
    int reg;
    int32_t val;
    if( !args or sscanf(args, "%i", &reg) != 1 or reg < 0 or reg >= R_SIZE ){
        printf("Please specify a valid register (0-%d).\n", R_SIZE);
        return -1;
    }
    args = strtok(NULL, " \t\n");
    if( !args or sscanf(args, "%i", &val) != 1 ){
        puts("Please specify an integral value for the register.");
        return -1;
    } 
    printf("Register n. %d set to 0x%X.\n", reg, val);
    R[reg] = val;
    return 0;
}

DECL_SF(s_fset){
    int reg;
    float32_t val;
    if( !args or sscanf(args, "%i", &reg) != 1 or reg < 0 or reg >= F_SIZE ){
        printf("Please specify a valid register (0-%d).\n", F_SIZE);
        return -1;
    }
    args = strtok(NULL, " \t\n");
    if( !args or sscanf(args, "%f", &val) != 1 ){
        puts("Please specify a floating-point value for the register.");
        return -1;
    }
    AS_FLOAT(F[reg]) = val;
    printf("Register $f%d set to 0x%X (%f).\n", reg, F[reg], val);
    return 0;
}

DECL_SF(s_dset){
    int reg;
    float64_t val;
    if( !args or sscanf(args, "%i", &reg) != 1 or reg < 0 or reg >= F_SIZE or reg & 1 ){
        printf("Please specify a valid register (0-%d) *even*.\n", F_SIZE);
        return -1;
    }
    args = strtok(NULL, " \t\n");
    if( !args or sscanf(args, "%lf", &val) != 1 ){
        puts("Please specify a double-precision floating-point value for the register.");
        return -1;
    } 
    AS_DOUBLE(F[reg]) = val;
    printf("Register $f%d set to 0x%X (lo %f).\n", reg, F[reg], val);
    printf("Register $f%d set to 0x%X (hi %f).\n", reg+1, F[reg+1], val);
    return 0;
}

DECL_SF(s_print){
    puts("====== GP REGISTERS ======");
    for(int i = 0; i < R_SIZE; ++i)
        printf("$%2d = 0x%08X%c", i, R[i], i % 4 == 3 ? '\n' : '\t');
    puts("====== FP REGISTERS ======");
    for(int i = 0; i < F_SIZE; ++i)
        printf("$%2d = 0x%08X%c", i, F[i], i % 4 == 3 ? '\n' : '\t');
    puts("======= OTHER REGS =======");
    printf("HI = %X\n", HI);
    printf("LO = %X\n", LO);
    printf("PC = %X\n", PC);
    return 0;
}

DECL_SF(s_quit){
    exit(0);
}

std::map<std::string, sf_t> shell_fun = {
    {"h", s_help},  {"help", s_help},
    {"l", s_load},  {"load", s_load},
    {"n", s_step},  {"step", s_step},
    {"r", s_run},   {"run", s_run},
    {"w", s_where}, {"where", s_where},
    {"b", s_break}, {"break", s_break},
    {"c", s_clear}, {"clear", s_clear},
    {"i", s_ignore},{"ignore", s_ignore},
    {"j", s_jump},  {"jump", s_jump},
    {"s", s_set},   {"set", s_set},
    {"f", s_fset},  {"fset", s_fset},
    {"d", s_dset},  {"dset", s_dset},
    {"p", s_print}, {"print", s_print},
    {"q", s_quit},  {"quit", s_quit},
};

int shell(){
    char line[128];
    while( true ){
        printf("$>"); fflush(stdout);
        fgets(line, 127, stdin);
        char *cmd = strtok(line, " \t\n");
        if( !cmd ) continue;
        auto f = shell_fun[cmd];
        if( f ){
            char *args = strtok(NULL, " \t\n");
            f(args);
        }else
            printf("Unknown command: %s.\n", cmd);
    }
     return 0;
}
