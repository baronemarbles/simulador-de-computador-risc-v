#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

class Instructions {
public:
    static void execute(CPU& cpu, uint32_t instruction);
    
    static inline int32_t sext12(uint32_t x)
    {
        if (x & 0x800){
            x |= 0xFFFFF000;
        }
        return (int32_t)x;
    }

    // Instruções aritméticas
    static void ADD(CPU& cpu, uint32_t i);
    static void ADDI(CPU& cpu, uint32_t i);
    static void SUB(CPU& cpu, uint32_t i);
    static void LUI(CPU& cpu, uint32_t i);
    static void AUIPC(CPU& cpu, uint32_t i);
    
    // Instruções lógicas
    static void XOR(CPU& cpu, uint32_t i);
    static void XORI(CPU& cpu, uint32_t i);
    static void OR(CPU& cpu, uint32_t i);
    static void ORI(CPU& cpu, uint32_t i);
    static void AND(CPU& cpu, uint32_t i);
    static void ANDI(CPU& cpu, uint32_t i);
    
    // Instruções de shift
    static void SLL(CPU& cpu, uint32_t i);
    static void SLLI(CPU& cpu, uint32_t i);
    static void SRL(CPU& cpu, uint32_t i);
    static void SRLI(CPU& cpu, uint32_t i);
    static void SRA(CPU& cpu, uint32_t i);
    static void SRAI(CPU& cpu, uint32_t i);
    
    // Instruções de branch
    static void BEQ(CPU& cpu, uint32_t i);
    static void BNE(CPU& cpu, uint32_t i);
    static void BLT(CPU& cpu, uint32_t i);
    static void BGE(CPU& cpu, uint32_t i);
    static void BLTU(CPU& cpu, uint32_t i);
    static void BGEU(CPU& cpu, uint32_t i);
    
    // Instruções de jump
    static void JAL(CPU& cpu, uint32_t i);
    static void JALR(CPU& cpu, uint32_t i);
    
    // Instruções de load/store
    static void LW(CPU& cpu, uint32_t instr);
    static void SW(CPU& cpu, uint32_t instr);
    
    // Instruções de ambiente
    static void ECALL(CPU& cpu, uint32_t i);
    static void EBREAK(CPU& cpu, uint32_t i);
};

#endif