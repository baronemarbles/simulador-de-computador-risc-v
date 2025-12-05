#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <cstring>

class CPU {
public:
    static constexpr uint32_t RAM_SIZE = 0xA0000; // 640 KB
    
    // Mapeamento de memória
    static constexpr uint32_t MAIN_START = 0x00000;
    static constexpr uint32_t MAIN_END   = 0x7FFFF;
    static constexpr uint32_t VRAM_START = 0x80000;
    static constexpr uint32_t VRAM_END   = 0x8FFFF;
    static constexpr uint32_t RSV_START  = 0x90000;
    static constexpr uint32_t RSV_END    = 0x9FBFF;
    static constexpr uint32_t IO_START   = 0x9FC00;
    static constexpr uint32_t IO_END     = 0x9FFFF;

    // Registradores
    uint32_t reg[32];
    uint32_t pc;
    uint8_t memory[RAM_SIZE];
    
    // Contexto para interrupções
    uint32_t saved_pc;
    uint32_t saved_reg[32];
    
    CPU();
    void reset();
    void fetch_execute();
    void handle_interrupt();
    void save_context();
    void restore_context();
    
    // Métodos de acesso à memória
    uint8_t read8(uint32_t addr);
    uint16_t read16(uint32_t addr);
    uint32_t read32(uint32_t addr);
    void write8(uint32_t addr, uint8_t value);
    void write16(uint32_t addr, uint16_t value);
    void write32(uint32_t addr, uint32_t value);
    bool running = true;

private:
    bool isValidAddress(uint32_t addr) const;
    bool inRange(uint32_t addr, uint32_t start, uint32_t end) const;
    uint8_t readIO8(uint32_t addr);
    void writeIO8(uint32_t addr, uint8_t value);
};

#endif