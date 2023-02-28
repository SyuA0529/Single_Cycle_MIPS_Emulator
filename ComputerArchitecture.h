#pragma once
#include <stdlib.h>
#include <string.h>

#define RA 31
#define SP 29
#define MAX_MEMORY 0x400000

typedef unsigned int REG;
typedef unsigned int DATA;

typedef struct CPU_ {
    unsigned PC;
    REG inst_reg;
    REG registers[32];    
}CPU;

typedef DATA* MEMORY;

typedef struct CA_ {
    CPU cpu;
    MEMORY memory;
}CA;

CA computerArch;

//initialize computer architecture
void InitCA() {
    memset(&computerArch, 0x00, sizeof(computerArch));
    computerArch.cpu.registers[RA] = 0xFFFFFFFF;
    computerArch.cpu.registers[SP] = 0x1000000;
    computerArch.memory = (MEMORY) malloc(sizeof(DATA) * MAX_MEMORY);
}