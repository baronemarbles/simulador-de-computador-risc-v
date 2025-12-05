#include "memory.h"
#include <iostream>

bool Memory::isValidAddress(uint32_t addr)
{
    // Qualquer coisa dentro do espaço mapeado é válida
    return addr <= IO_END;
}

bool Memory::inRange(uint32_t addr, uint32_t start, uint32_t end)
{
    return addr >= start && addr <= end;
}

uint8_t Memory::read8(uint32_t addr) const
{
    if (!isValidAddress(addr))
    {
        return 0;
    }

    if (inRange(addr, MAIN_START, MAIN_END))
    {
        return data[addr];
    }

    if (inRange(addr, VRAM_START, VRAM_END))
    {
        return data[addr];
    }

    // Reservado ou IO: por enquanto, CPU lê como 0
    return 0;
}

void Memory::write8(uint32_t addr, uint8_t value)
{
    if (!isValidAddress(addr))
    {
        return;
    }

    if (addr >= RAM_SIZE)
    {
        return;
    }

    if (inRange(addr, MAIN_START, MAIN_END) ||
        inRange(addr, VRAM_START, VRAM_END))
    {
        data[addr] = value;
        return;
    }

    // Reservado / IO: ignorar
}

uint16_t Memory::read16(uint32_t addr) const
{
    uint16_t b0 = read8(addr);
    uint16_t b1 = read8(addr + 1);
    return b0 | (b1 << 8);
}

uint32_t Memory::read32(uint32_t addr) const
{
    if (addr + 3 >= RAM_SIZE)
    {
        std::cerr << "Endereço fora dos limites de memória: " << addr << std::endl;
        return 0;
    }

    uint32_t b0 = read8(addr);
    uint32_t b1 = read8(addr + 1);
    uint32_t b2 = read8(addr + 2);
    uint32_t b3 = read8(addr + 3);
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void Memory::write16(uint32_t addr, uint16_t value)
{
    write8(addr, value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
}

void Memory::write32(uint32_t addr, uint32_t value)
{
    write8(addr, value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
    write8(addr + 2, (value >> 16) & 0xFF);
    write8(addr + 3, (value >> 24) & 0xFF);
}