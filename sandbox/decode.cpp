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
#define ADDR(x)     ((x)&0x3FFFFFF)


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

void pb(uint64_t n){
    if( !n ) return;
    pb(n>>1);
    putchar("01"[n&1]);
}

#define TO_NBIT(x, n) (x & ((1<<n)-1))
#define TO_INT(x, n) ((x & ((1<<(n-1))-1)) - (x&(1<<(n-1))))



int main(){

    FILE *out = fopen("test1.mips", "wb");

    // HARDCODED, MAY NOT BE EQUAL TO PARSER GENERATED CODE
    uint32_t header[] = { 20, 88 };
    char* data_s[] = { "Enter a number\0\0", "i=\0\0" }; // notice 4-alignment
    int data_ss[] = { 16, 4 };
    uint32_t text_s[] = { 0x34020004, 0x3C041000, 0xC, 0x34020005, 0xC,
        0x28025, 0x34080000, 0x1110000D, 0x34020004, 0x3C011000, 0x34240010, 0xC,
        0x34020001, 0x82025, 0xC, 0x3402000B, 0x3404000A, 0xC, 0x21080001,
        0x8000008, 0x3402000A, 0xC };

    fwrite(header, sizeof(header), 1, out);
    for(int i = 0; i < 2; ++i)
        fwrite(data_s[i], data_ss[i], 1, out);
    fwrite(text_s, sizeof(text_s), 1, out);

    fclose(out);
    
    //decode(0x014B4820);
    //decode(0x02531022);

    //~ char s[10];
    //~ int maxl = 5;

    //~ char format[16];
    //~ char *arg = s;
    //~ int32_t maxlen = maxl;
    //~ snprintf(format, sizeof(format), "%%%u[^\\n]", maxlen);
    //~ printf("-%s-", format);
    //~ scanf(format, arg);
    //~ puts(arg);


    int x = -9;
    uint32_t y = TO_NBIT(x, 6);
    int z = TO_INT(y, 6);

    printf("%d %d %d\n", x, y, z);

    pb(y);

    #define pepe1(x) (((x)&0x1F0000)>>16)
    #define pepe2(x) ((x)&0x1F0000>>16)

    printf("%d %d\n", pepe1(0x02531022), pepe2(0x02531022));
     
    pb(4294966972); puts("");
    pb( 2400620139 );

    return 0;
}
