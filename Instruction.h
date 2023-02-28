#pragma once
#include <string.h>

typedef enum INST_FORMAT_ {
    R, I, J, NONE
}INST_FORMAT;

typedef enum INST_NAME_{
    Add, Addi, Addiu, Addu, And, Andi, Beq, Bne, Jmp, Jal, Jr, Lbu,
    Lhu, Ll, Lui, Lw, Nor, Or, Ori, Slt, Slti, Sltiu, Sltu, Sll, Srl,
    Sb, Sc, Sh, Sw, Sub, Subu
}INST_NAME;

typedef struct INST_ {
    INST_FORMAT format;
    INST_NAME name;

    unsigned char opcode;
    unsigned char rs;
    unsigned char rt;
    unsigned char rd;
    unsigned char shmat;
    unsigned char funct;
    unsigned short imm;
    unsigned int addr;
}INST;

INST inst;

//initialize instruction
void InitInst() {
    memset(&inst, 0x00, sizeof(INST));
}

//set instruction's information
void SetInstInfo() {
    INST_FORMAT format = NONE;
    switch(inst.opcode) {
        case 0x00:
            switch(inst.funct) {
                case 0x00: inst.name = Sll; break;
                case 0x02: inst.name = Srl; break;
                case 0x08: inst.name = Jr; break;
                case 0x20: inst.name = Add; break;
                case 0x21: inst.name = Addu; break;
                case 0x22: inst.name = Sub; break;
                case 0x23: inst.name = Subu; break;
                case 0x24: inst.name = And; break;
                case 0x25: inst.name = Or; break;
                case 0x27: inst.name = Nor; break;
                case 0x2a: inst.name = Slt; break;
                case 0x2b: inst.name = Sltu; break;
            }
            inst.format = R;
            break;

        case 0x04:  inst.name = Beq; inst.format = I; break;
        case 0x05:  inst.name = Bne; inst.format = I; break;
        case 0x08:  inst.name = Addi; inst.format = I; break;
        case 0x09:  inst.name = Addiu; inst.format = I; break;
        case 0x0A:  inst.name = Slti; inst.format = I; break;
        case 0x0B:  inst.name = Sltiu; inst.format = I; break;
        case 0x0C:  inst.name = Andi; inst.format = I; break;
        case 0x0D:  inst.name = Ori; inst.format = I; break;
        case 0x0F:  inst.name = Lui; inst.format = I; break;
        case 0x23:  inst.name = Lw; inst.format = I; break;
        case 0x24:  inst.name = Lbu; inst.format = I; break;
        case 0x25:  inst.name = Or; inst.format = I; break;
        case 0x28:  inst.name = Sb; inst.format = I; break;
        case 0x29:  inst.name = Sh; inst.format = I; break;
        case 0x2b:  inst.name = Sw; inst.format = I; break;
        case 0x30:  inst.name = Ll; inst.format = I; break;
        case 0x38:  inst.name = Sc; inst.format = I; break;

        case 0x02: inst.name = Jmp; inst.format = J; break;
        case 0x03: inst.name = Jal; inst.format = J; break;

        default : inst.format = NONE;
    }
}