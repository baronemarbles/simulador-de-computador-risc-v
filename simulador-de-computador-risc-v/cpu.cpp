#include "cpu.h"
#include <iostream>

CPU::CPU(Memory &mem_ref) 
    : mem(mem_ref) // inicializa referência
{
    reset();
}

void CPU::reset()
{
    std::memset(reg, 0, sizeof(reg));
    std::memset(saved_reg, 0, sizeof(saved_reg));
    pc = 0;
    saved_pc = 0;
    running = true;
    // Se quiser, pode resetar a memória aqui também:
    // mem.reset();
}

void CPU::fetch_execute() {
    // Buscar instrução da memória
    //uint32_t instruction = read32(pc);
    //NÃO UTILIZADO, PROGRAMA FUNCIONA PELO BARRAMENTO
}

void CPU::handle_interrupt() {
    // Salvar contexto atual
    save_context();
    
    // Pular para handler de interrupção
    pc = 0x8000; // Endereço do handler
}

void CPU::save_context() {
    saved_pc = pc;
    for (int i = 0; i < 32; i++) {
        saved_reg[i] = reg[i];
    }
}

void CPU::restore_context() {
    pc = saved_pc;
    for (int i = 0; i < 32; i++) {
        reg[i] = saved_reg[i];
    }
}