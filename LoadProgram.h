#pragma once
#include <stdio.h>
#include <winsock2.h>

#include "ComputerArchitecture.h"

//store instructions to memory
void LoadProgram(char *path) {
    FILE *fp = NULL;
    
    //read bin file
    if((fp = fopen(path, "rb")) == NULL) {
        printf("Can't Open File : %s\n", path);
        exit(1); //if can't open, end program
    }

    //read and store instructions to memory
    int memory_index = 0;
    while(!feof(fp)) {
        int binary_data = 0;

        //if overflow
        if(memory_index > MAX_MEMORY - 1) {
            printf("Can't Load Instruction, Overflowed\n");
            exit(1);
        }

        fread(&binary_data, sizeof(int), 1, fp);
        binary_data = ntohl(binary_data);
        computerArch.memory[memory_index] = binary_data;
        memory_index++;
    }
}