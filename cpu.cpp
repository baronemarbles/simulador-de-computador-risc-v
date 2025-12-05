#include "cpu.h"
#include <iostream>

CPU::CPU() {
    reset();
}

void CPU::reset() {
    std::memset(reg, 0, sizeof(reg));
    std::memset(memory, 0, sizeof(memory));
    std::memset(saved_reg, 0, sizeof(saved_reg));
    pc = 0;
    saved_pc = 0;
}

void CPU::fetch_execute() {
    // Buscar instrução da memória
    //uint32_t instruction = read32(pc);
    
    // Decodificar e executar (será implementado em instructions.cpp)
    //execute(instruction); // Será chamado pelo barramento
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

// TEM QUE ESTAR DENTRO DA AREA DE ENDEREÇAMENTO DA RAM
bool CPU::isValidAddress(uint32_t addr) const {
    return addr <= IO_END; // 0x00000 - 0x9FFFF são válidos
}

// LEITURA / ESCRITA DE 8 bits com mapeamento
bool CPU::inRange(uint32_t addr, uint32_t start, uint32_t end) const {
    return addr >= start && addr <= end;
}

uint8_t CPU::readIO8(uint32_t addr) { //IMPLEMENTAR
    (void)addr;

    // if (addr == 0x9FC00) return teclado;
    return 0;
}

void CPU::writeIO8(uint32_t addr, uint8_t value) { //IMPLEMENTAR
    // Exemplo: escrever em registrador de vídeo, timer etc.
    // if (addr == 0x9FC10) video_reg = value;
    (void)addr;
    (void)value;
}

uint8_t CPU::read8(uint32_t addr) {
    // VERIFICAÇÃO
    if (!isValidAddress(addr)) {
        return 0; // Endereço inválido
    }

    if (inRange(addr, MAIN_START, MAIN_END)) {
        // RAM Principal
        return memory[addr];
    }

    if (inRange(addr, VRAM_START, VRAM_END)) {
        // VRAM (mesmo array, região separada)
        return memory[addr];
    }

    if (inRange(addr, RSV_START, RSV_END)) {
        // Reservado – pode retornar 0 ou lançar erro
        return 0;
    }

    if (inRange(addr, IO_START, IO_END)) {
        // Periféricos mapeados em memória
        return readIO8(addr);
    }

    // Fora de qualquer região conhecida
    return 0;
}

void CPU::write8(uint32_t addr, uint8_t value) {
    // VERIFICAÇÃO
    if (!isValidAddress(addr)) {
        return; //Endereço inválido
    }

    if (addr >= RAM_SIZE) {
        // Ignora ou loga erro
        return;
    }

    if (inRange(addr, MAIN_START, MAIN_END) ||
        inRange(addr, VRAM_START, VRAM_END)) {
        // RAM e VRAM gravam no mesmo array
        memory[addr] = value;
        return;
    }

    if (inRange(addr, RSV_START, RSV_END)) {
        // Reservado – ignora
        return;
    }

    if (inRange(addr, IO_START, IO_END)) {
        writeIO8(addr, value);
        return;
    }
}

// LEITURA / ESCRITA DE 16 E 32 bits
uint16_t CPU::read16(uint32_t addr) {
    uint16_t b0 = read8(addr);
    uint16_t b1 = read8(addr + 1);
    return b0 | (b1 << 8); // little endian
}

uint32_t CPU::read32(uint32_t addr) {

    if (addr + 3 >= RAM_SIZE){
        std::cerr << "Endereço fora dos limites de memória: " << addr << std::endl;
        return 0;
    }

    uint32_t b0 = read8(addr);
    uint32_t b1 = read8(addr + 1);
    uint32_t b2 = read8(addr + 2);
    uint32_t b3 = read8(addr + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void CPU::write16(uint32_t addr, uint16_t value) {
    write8(addr, value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
}

void CPU::write32(uint32_t addr, uint32_t value) {
    write8(addr, value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
    write8(addr + 2, (value >> 16) & 0xFF);
    write8(addr + 3, (value >> 24) & 0xFF);
}