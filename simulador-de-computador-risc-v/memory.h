#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstring>

class Memory
{
public:
    static constexpr uint32_t RAM_SIZE = 0xA0000; // 640 KB

    // Mapeamento de memória
    static constexpr uint32_t MAIN_START = 0x00000;
    static constexpr uint32_t MAIN_END = 0x7FFFF;
    static constexpr uint32_t VRAM_START = 0x80000;
    static constexpr uint32_t VRAM_END = 0x8FFFF;
    static constexpr uint32_t RSV_START = 0x90000;
    static constexpr uint32_t RSV_END = 0x9FBFF;
    static constexpr uint32_t IO_START = 0x9FC00;
    static constexpr uint32_t IO_END = 0x9FFFF;

    uint8_t data[RAM_SIZE];

    Memory()
    {
        reset();
    }

    void reset()
    {
        std::memset(data, 0, sizeof(data));
    }

    // Acesso a memória
    uint8_t read8(uint32_t addr) const;
    uint16_t read16(uint32_t addr) const;
    uint32_t read32(uint32_t addr) const;

    void write8(uint32_t addr, uint8_t value);
    void write16(uint32_t addr, uint16_t value);
    void write32(uint32_t addr, uint32_t value);

private:
    static bool isValidAddress(uint32_t addr);
    static bool inRange(uint32_t addr, uint32_t start, uint32_t end);
};

#endif