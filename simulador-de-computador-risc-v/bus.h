#ifndef BUS_H
#define BUS_H

#include <iostream>
#include "cpu.h"
#include "instructions.h"

class Bus {
private:
    CPU& cpu;
    bool interrupt_flag = false;
    uint32_t instructions_executed;
    int vram_interval;
    bool verbose;
    bool interrupt_fired = false;

public:
    Bus(CPU& cpu_ref, int interval = 100, bool verbose = false);

    void clock();
    void trigger_interrupt();
    void show_vram_ascii();
    
    uint32_t read(uint32_t addr);
    void write(uint32_t addr, uint32_t value);
    
    uint32_t getInstructionsExecuted() const{
        return instructions_executed;
    }

private:
    void handle_interrupt();
    uint8_t read_io(uint32_t addr);
    void write_io(uint32_t addr, uint8_t value);
    uint8_t get_keyboard_input();
};

#endif