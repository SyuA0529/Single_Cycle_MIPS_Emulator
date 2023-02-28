#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ComputerArchitecture.h"
#include "Instruction.h"
#include "LoadProgram.h"
#include "ControlSignal.h"
#include "Process.h"
#include "MyString.h"

extern CA computerArch;
extern INST inst;
extern CONTROLSIG signal;
BOOL check_state = FALSE;

int main() {
    //initializing global variable
    InitCA();
    InitInst();
    InitSignal();

    //ask print processing state
    printf("do you wan't check processing state? (y/n) : ");
    char input = getchar();
    if(input == 'y') check_state = TRUE;
    else if(input == 'n') check_state = FALSE;
    else check_state = FALSE;

    while(getchar() != '\n');

    //Load Program to Memory
    printf("Input Path : ");
    char *PATH = GetString();
    
    LoadProgram(PATH);

    while(computerArch.cpu.PC != 0xFFFFFFFF) {
        //Instruction Fetch
        InstFetch();

        //Instruction Decode
        InstDecode();

        if(check_state) PrintInst();

        /*
         * Execute Stage
         */
        //get operands
        int operand1 = computerArch.cpu.registers[inst.rs]; //without SLL, SRL, operand1 is inst.rs
        int operand2 = 0;

        //get operand2
        if(signal.ALUSrc) { //using imm
            if(signal.sign) {
                short temp = inst.imm;
                operand2 = temp;
            }

            else operand2 = (unsigned)inst.imm;
        }

        //if instruction is SLL or SRL, operand1 is inst.rt, operand2 is inst.shmat
        else if(signal.ALUOp == SLL || signal.ALUOp == SRL) {
            operand1 = computerArch.cpu.registers[inst.rt];
            operand2 = inst.shmat;
        }
        
        else operand2 = computerArch.cpu.registers[inst.rt]; //using inst.rt

        //EXECUTE
        int exe_result = 0;
        if(signal.ALUOp) exe_result = Execute(operand1, operand2);

        /*
         * MEM Stage
         */
        int address = 0;
        int write_data = 0;
        int mem_result = 0;

        //set addr and write_data
        address = exe_result;
        write_data = computerArch.cpu.registers[inst.rt];

        //MEM
        if(signal.MemRead) mem_result = MEM(address, 0);
        if(signal.MemWrite) mem_result = MEM(address, write_data);

        /*
         * Write Back Stage
         */
        //set write data
        write_data = 0;
        if(signal.MemtoReg) write_data = mem_result;
        else write_data = exe_result;

        //WB only execute when RegWrite signal is on
        if(signal.RegWrite) {
            //set dest reg
            int dest_reg = 0;
            
            if(signal.JumpAndLink) {
                dest_reg = RA;
                write_data = computerArch.cpu.PC + 8; //if instruction is jal, write data is PC + 8
            }

            else {
                if(signal.RegDst) dest_reg = inst.rd;

                else {
                    dest_reg = inst.rt;
                    //when instruction is LUI
                    if(signal.NotReadReg) write_data = inst.imm << 16;
                }
            }

            //Do WRITEBACK
            WriteBack(dest_reg, write_data);
        }

        /*
         * Change PC
         * BEQ, BNE, JUMP, JAL
         */

        //get branch addr
        short temp = inst.imm; 
        int baddr = temp; //sign extend

        if((signal.Branch && !exe_result) || (signal.BranchN && exe_result)) computerArch.cpu.PC += (baddr << 2) + 4;//beq, bne        
        else if(signal.Jump) computerArch.cpu.PC = ((inst.addr << 2) & 0x0FFFFFFF) | (computerArch.cpu.PC && 0xF0000000) ; //jump
        else if(signal.JumpReg) computerArch.cpu.PC = computerArch.cpu.registers[inst.rs]; //jr
        else computerArch.cpu.PC += 4; //normal

        //print devide each instruction
        if(check_state) printf("\n-----------------------------------------------\n");
    }

    //print result(return value)
    printf("Result : v0 = 0x%x(%d), v1 = 0x%x(%d)\n", computerArch.cpu.registers[2], computerArch.cpu.registers[2], 
        computerArch.cpu.registers[3], computerArch.cpu.registers[3]);

    free(computerArch.memory);
    return 0;
}