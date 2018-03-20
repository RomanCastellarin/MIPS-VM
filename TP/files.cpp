#include "files.h"
#include "memory.h"
#include "simulator.h"

#include <cstdio>

// loads a compiled program and initializes the VM
int load_program(const char *filename){

    // reset virtual machine
    reset();

    // open file
    FILE *f = fopen(filename, "rb");

    if( !f ){
        fprintf(stderr, "Cannot open file\n");
        return -1; // TODO: standarize
    }

    // read program header
    fread(&DATA_SIZE, sizeof(DATA_SIZE), 1, f);
    fread(&TEXT_SIZE, sizeof(TEXT_SIZE), 1, f);

    // TODO: check size limits

    // allocate space for text and data segments
    DATA_POINTER = new uint8_t[DATA_SIZE]; // TODO: is this safe ? probably not
    TEXT_POINTER = new uint8_t[TEXT_SIZE];

    if( !DATA_POINTER or !TEXT_POINTER ){
        fprintf(stderr, "Cannot handle this size\n");
        return -1; // TODO: standarize
    }

    // load segments into memory
    fread(DATA_POINTER, DATA_SIZE, 1, f);
    fread(TEXT_POINTER, TEXT_SIZE, 1, f);

    // close file
    fclose(f);

    // initialize machine
    initialize();

    return 0;
    
}

