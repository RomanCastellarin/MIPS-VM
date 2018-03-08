#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <map>
using namespace std;


#define OPCODE(x)   (((x)&0xFC000000)>>26)
#define RS(x)       (((x)&0x3E00000)>>21)
#define RT(x)       (((x)&0x1F0000)>>16)
#define RD(x)       (((x)&0xF800)>>11)
#define SHIFT(x)    (((x)&0x7C0)>>6)
#define FUNCT(x)    ((x)&127)
#define IMM(x)      ((x)&0xFFFF)
#define ADDR(x)      ((x)&0x3FFFFFF)

typedef uint32_t instruction;

char* itoreg[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "t3", "t0", "t1",
                  "t2", "t3", "t4", "t5", "t6", "t8", "t9", "s0", "s1", "s2",
                  "s3", "s4", "s5", "s6", "s8", "k0", "k1", "gp", "sp", "ra"};
map<uint32_t, char*> itofunct = { {0x20, "add" },
                                  {0x1A, "div" },   // pasar estas definiciones
                                  {0x22, "sub" } }; // a un archivo

void decode(instruction s){
    unsigned oc = OPCODE(s);
    if( oc == 0x00 ){
        puts("TYPE R");
        printf("OPCODE %u\n", OPCODE(s));
        printf("RS %2u ($%s)\n", RS(s), itoreg[RS(s)]);
        printf("RT %2u ($%s)\n", RT(s), itoreg[RT(s)]);
        printf("RD %2u ($%s)\n", RD(s), itoreg[RD(s)]);
        printf("SHIFT %u\n", SHIFT(s));
        printf("FUNCT %u (%s)\n", FUNCT(s), itofunct[FUNCT(s)]);
    }else
    if( oc <= 0x03 ){
        puts("TYPE J");
        printf("OPCODE %u\n", OPCODE(s));
        printf("RS %u\n", RS(s));
        printf("RT %u\n", RT(s));
        printf("IMM %u\n", IMM(s));
    }else{
        puts("TYPE I");
        printf("OPCODE %u\n", OPCODE(s));
        printf("ADDR %u\n", ADDR(s));
    }
    puts("");
}

int main(){
    decode(0x014B4820);
    decode(0x02531022);


    return 0;
}
