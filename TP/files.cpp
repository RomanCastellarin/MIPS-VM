#include "files.h"
#include "memory.h"
#include "instructions.h"
#include "simulator.h"
#include <cstdio>


int load_program(const char *filename){
    FILE *f = fopen(filename, "rb");

    if( !f ){
        fprintf(stderr, "Cannot open file\n");
        return -1; // TODO: standarize
    }

    fread(&DATA_SIZE, sizeof(DATA_SIZE), 1, f);
    fread(&TEXT_SIZE, sizeof(TEXT_SIZE), 1, f);

    // TODO: check size limits

    DATA_POINTER = new uint8_t[DATA_SIZE]; // TODO: is this safe ? probably not
    TEXT_POINTER = new uint8_t[TEXT_SIZE];

    if( !DATA_POINTER or !TEXT_POINTER ){
        fprintf(stderr, "Cannot handle this size\n");
        return -1; // TODO: standarize
    }

    fread(DATA_POINTER, DATA_SIZE, 1, f);
    fread(TEXT_POINTER, TEXT_SIZE, 1, f);

    fclose(f);

    initialize();

    return 0;
    
}

