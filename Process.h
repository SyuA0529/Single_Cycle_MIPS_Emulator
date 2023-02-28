#pragma once
#include "ComputerArchitecture.h"
#include "ControlSignal.h"
#include "Instruction.h"

extern INST inst;
extern BOOL check_state;

//Instruction Fetch Stage
void InstFetch() {
    computerArch.cpu.inst_reg = computerArch.memory[computerArch.cpu.PC/4];
}

//Instruction Decode Stage
void InstDecode() {
    //decode
    inst.opcode = (computerArch.cpu.inst_reg >> 26) & 0x3F;
    inst.rs = (computerArch.cpu.inst_reg >> 21) & 0x1F;
    inst.rt = (computerArch.cpu.inst_reg >> 16) & 0x1F;
    inst.rd = (computerArch.cpu.inst_reg >> 11) & 0x1F;
    inst.shmat = (computerArch.cpu.inst_reg >> 6) & 0x1F;
    inst.funct = computerArch.cpu.inst_reg & 0x3F;
    inst.imm = (short)(computerArch.cpu.inst_reg & 0xFFFF);
    inst.addr = computerArch.cpu.inst_reg & 0x3FFFFFF;
    
    //get inst format and name
    SetInstInfo();

    //generate control signal
    SetControlSig();
}

//Instruction Execute Stage
DATA Execute(DATA operand1, DATA operand2) {
    //Do ALU operation
    switch(signal.ALUOp) {
        case NON: return 0x00;
        case ADD: return operand1 + operand2;
        case SUB: return operand1 - operand2;
        case AND: return operand1 & operand2;
        case OR: return operand1 | operand2;
        case NOR: return !(operand1 | operand2);
        case SLT: return (operand1 < operand2) ? 1 : 0;
        case SLL: return operand1 << operand2;
        case SRL: return operand1 >> operand2;
    }
}

//Instruction MEM Stage
DATA MEM(DATA address, DATA write_data) {
    //if read memory
    if(signal.MemRead) {
        if(signal.imm_8) return computerArch.memory[address/4] & 0x000000FF; //lbu
        else if(signal.imm_16) return computerArch.memory[address/4] & 0x0000FFFF; //lhu

        //print processing state
        if(check_state) printf("M[0x%x] is 0x%x(%d) ", address, computerArch.memory[address/4], write_data);
        return computerArch.memory[address/4];
    }
    
    //if write memory
    else if(signal.MemWrite) {
        DATA temp = computerArch.memory[address/4];
        if(signal.imm_8) write_data = (temp & 0xFFFFFF00) | (write_data & 0x000000FF); //sb
        else if(signal.imm_16) write_data = (temp & 0xFFFF0000) | (write_data & 0x0000FFFF); //sh
        computerArch.memory[address/4] = write_data;

        //print processing state
        if(check_state) printf("=> M[0x%x] = 0x%x(R[%d], %d) ", address, write_data, inst.rt, write_data);
        return TRUE;
    }

    //Error condition
    return FALSE;
}

//Instruction Write Back Stage
void WriteBack(DATA write_register, DATA write_data) {
    if(signal.RegWrite) {
        computerArch.cpu.registers[write_register] = write_data;
        
        //print processing state
        if(check_state) printf("=> R[%d] = 0x%x(%d) ", write_register, write_data, write_data);
    }
}

//Print Current Instruction
void PrintInst() {
    char *name = NULL;
    switch(inst.name) {
        case Add:   name = "ADD";   break;
        case Addi:  name = "ADDI";  break;
        case Addiu: name = "ADDIU"; break;
        case Addu:  name = "ADDU";  break;
        case And:   name = "AND";   break;
        case Andi:  name = "ANDI";  break;
        case Beq:   name = "BEQ";   break;
        case Bne:   name = "BNE";   break; 
        case Jmp:   name = "JMP";   break;
        case Jal:   name = "JAL";   break;
        case Jr:    name = "JR";    break;
        case Lbu:   name = "LBU";   break; 
        case Lhu:   name = "LHU";   break; 
        case Ll:    name = "LL";    break;
        case Lui:   name = "LUI";   break;
        case Lw:    name = "LW";    break;
        case Nor:   name = "NOR";   break;
        case Or:    name = "OR";    break;
        case Ori:   name = "ORI";   break;
        case Slt:   name = "SLT";   break; 
        case Slti:  name = "SLTI";  break;
        case Sltiu: name = "SLTIU"; break;
        case Sltu:  name = "SLTU";  break;
        case Sll:   name = "SLL";   break;
        case Srl:   name = "SRL";   break;
        case Sb:    name = "SB";    break;
        case Sc:    name = "SC";    break;
        case Sh:    name = "SH";    break;
        case Sw:    name = "SW";    break;
        case Sub:   name = "SUB";   break;
        case Subu:  name = "SUBU";  break; 
    }

    //get imm value (if sign signal is on, imm be sign value, else imm be unsign value)
    int temp = 0;
    if(signal.sign) {
        short temp2 = inst.imm;
        temp = temp2;
    }

    else temp = (unsigned)inst.imm;

    //print instruction by format
    switch(inst.format) {
        case R:
            printf("[PC = 0x%x] %s rs = %d(0x%x), rt = %d(0x%x), rd = %d(0x%x), shmat = %d(0x%x) ", 
                computerArch.cpu.PC, name, inst.rs, computerArch.cpu.registers[inst.rs], inst.rt, computerArch.cpu.registers[inst.rt], 
                inst.rd, computerArch.cpu.registers[inst.rd], inst.shmat, inst.shmat);
            break;

        case I:
            printf("[PC = 0x%x] %s rs = %d(0x%x), rt = %d(0x%x), imm = 0x%x(%d) ", 
                computerArch.cpu.PC, name, inst.rs, computerArch.cpu.registers[inst.rs], inst.rt, computerArch.cpu.registers[inst.rt], inst.imm, temp);
            break;

        case J:
            printf("[PC = 0x%x] %s addr = 0x%x ", computerArch.cpu.PC, name, inst.addr);
            break;
    }

    printf("\n");
}