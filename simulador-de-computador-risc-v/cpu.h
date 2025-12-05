#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <cstring>
#include "memory.h"

class CPU {
public:
    // Registradores
    uint32_t reg[32];
    uint32_t pc;
    
    // Contexto para interrupções
    uint32_t saved_pc;
    uint32_t saved_reg[32];

    bool running = true;

    Memory &mem;

    CPU(Memory &mem_ref);
    void reset();
    void fetch_execute();
    void handle_interrupt();
    void save_context();
    void restore_context();
    
    // Métodos de acesso à memória
    uint8_t read8(uint32_t addr)   {return mem.read8(addr);}
    uint16_t read16(uint32_t addr) {return mem.read16(addr);}
    uint32_t read32(uint32_t addr) {return mem.read32(addr);}
    void write8(uint32_t addr, uint8_t value)   {mem.write8(addr, value);} 
    void write16(uint32_t addr, uint16_t value) {mem.write16(addr, value);}
    void write32(uint32_t addr, uint32_t value) {mem.write32(addr, value);}

};

#endif