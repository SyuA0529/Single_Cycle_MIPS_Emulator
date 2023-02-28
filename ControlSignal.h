#pragma once
#include <windows.h>

#include "Instruction.h"

typedef enum ALUOP_ {
    NON, ADD, SUB, AND, OR, NOR, SLT, SLL, SRL
}ALUOP;

typedef enum ALUSRC_ {
    reg, imm
}ALUSRC;

typedef struct CONTROLSIG_ {
    //about alu
    BOOL RegDst;
    ALUOP ALUOp;
    ALUSRC ALUSrc;

    //about jump
    BOOL Jump;
    BOOL JumpReg;

    //about branch
    BOOL Branch;
    BOOL BranchN;

    //about memory
    BOOL MemRead;
    BOOL MemtoReg;
    BOOL MemWrite;

    //about register
    BOOL RegWrite;
    BOOL JumpAndLink;

    BOOL imm_8;
    BOOL imm_16;

    //about sign extend
    BOOL sign;

    //about LUI
    BOOL NotReadReg;
}CONTROLSIG;

CONTROLSIG signal;

//initialize control signal
void InitSignal() {
    memset(&signal, 0x00, sizeof(CONTROLSIG));
}

//Set Control Signal (only used by SetControlSig function)
void SetSig(BOOL RegDst, ALUOP ALUOp, ALUSRC ALUSrc, BOOL Jump, BOOL JumpReg, BOOL Branch, BOOL BranchN,
            BOOL MemRead, BOOL MemtoReg, BOOL MemWrite, BOOL RegWrite, BOOL JumpAndLink, 
            BOOL imm_8, BOOL imm_16, BOOL sign, BOOL cond, BOOL NotReadReg) {
    signal.RegDst = RegDst;
    signal.ALUOp = ALUOp;
    signal.ALUSrc = ALUSrc;
    signal.Jump = Jump;
    signal.JumpReg = JumpReg;
    signal.Branch = Branch;
    signal.BranchN = BranchN;
    signal.MemRead = MemRead;
    signal.MemtoReg = MemtoReg;
    signal.MemWrite = MemWrite;
    signal.RegWrite = RegWrite;
    signal.JumpAndLink = JumpAndLink;
    signal.imm_8 = imm_8;
    signal.imm_16 = imm_16;
    signal.sign = sign;
    signal.NotReadReg = NotReadReg;
}

void SetControlSig() {
    //Generate Control Signals by instruction name(ADD, ADDI, ...)
    switch(inst.name) {
        case Add:   SetSig(1, ADD, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Addi:  SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Addiu: SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Addu:  SetSig(1, ADD, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case And:   SetSig(1, AND, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; 
        case Andi:  SetSig(0, AND, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0); break;
        case Beq:   SetSig(0, SUB, reg, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0); break; //finish check
        case Bne:   SetSig(0, SUB, reg, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0); break; //finish check
        case Jmp:   SetSig(0, NON, reg, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0); break; //finish check
        case Jal:   SetSig(0, NON, imm, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0); break; //finish check
        case Jr:    SetSig(0, NON, reg, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0); break; //finish check
        case Lbu:   SetSig(0, ADD, imm, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0); break; 
        case Lhu:   SetSig(0, ADD, imm, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0); break; 
        case Ll:    SetSig(0, ADD, imm, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Lui:   SetSig(0, NON, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1); break; //finish check
        case Lw:    SetSig(0, ADD, imm, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Nor:   SetSig(1, NOR, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; 
        case Or:    SetSig(1, OR, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0);  break; 
        case Ori:   SetSig(0, OR, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);  break; //finish check
        case Slt:   SetSig(1, SLT, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Slti:  SetSig(0, SLT, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Sltiu: SetSig(0, SLT, imm, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Sltu:  SetSig(1, SLT, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; 
        case Sll:   SetSig(1, SLL, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Srl:   SetSig(1, SRL, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Sb:    SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0); break;
        case Sc:    SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0); break;
        case Sh:    SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0); break; 
        case Sw:    SetSig(0, ADD, imm, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0); break; //finish check
        case Sub:   SetSig(1, SUB, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
        case Subu:  SetSig(1, SUB, reg, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0); break; //finish check
    }
}