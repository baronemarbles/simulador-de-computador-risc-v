#include "bus.h"

Bus::Bus(CPU &cpu_ref, int interval, bool verb)
    : cpu(cpu_ref),
      interrupt_flag(false),
      instructions_executed(0),
      vram_interval(interval),
      verbose(verb)
{
}

void Bus::clock() {
    // 1. Verificar interrupção externa
    if (interrupt_flag) {
        handle_interrupt();
        interrupt_flag = false;
        return;
    }
    
    // 2. Buscar instrução
    uint32_t instruction = cpu.read32(cpu.pc);

    // // DEBUG: Mostrar instrução sendo executada
    // std::cout << "Instrucao em PC=" << std::hex << cpu.pc
    // << ": 0x" << instruction << std::dec << std::endl;

    // 3. Executar instrução
    Instructions::execute(cpu, instruction);
    
    // 4. Incrementar contador
    instructions_executed++;
    
    // 5. Verificar se é hora de mostrar VRAM
    if (instructions_executed % vram_interval == 0) {
        show_vram_ascii();
    }
}

void Bus::trigger_interrupt() {
    interrupt_flag = true;
}

void Bus::handle_interrupt() {
    cpu.save_context();
    
    // Salvar endereço de retorno
    cpu.saved_pc = cpu.pc;
    
    // Pular para handler de interrupção (exemplo: 0x8000)
    cpu.pc = 0x8000;
    
    std::cout << "[INTERRUPÇÃO] Handler chamado em 0x8000" << std::endl;
}

void Bus::show_vram_ascii() {
    std::cout << "\n=== VRAM (ASCII) - Instrucao " << instructions_executed << " ===" << std::endl;
    
    int chars_per_line = 64;
    int lines_to_show = 10; // Mostrar apenas 10 linhas
    int char_count = 0;
    int lines_count = 0;

    for (uint32_t addr = CPU::VRAM_START; addr <= CPU::VRAM_END 
        && lines_count < lines_to_show; addr++) {
        uint8_t value = cpu.read8(addr);
        
        if (value >= 32 && value <= 126) {  // Caracteres imprimíveis
            std::cout << static_cast<char>(value);
        } else if (value == 0) {
            std::cout << ' ';
        } else {
            std::cout << '.';
        }
        
        char_count++;
        if (char_count % chars_per_line == 0) {
            std::cout << std::endl;
            lines_count++;
        }
    }

    if (char_count % chars_per_line != 0){
        std::cout << std::endl;
    }

    std::cout << "\n==================" << std::endl;
}

uint32_t Bus::read(uint32_t addr) {
    // Se for I/O mapeado, tratar aqui
    if (addr >= CPU::IO_START && addr <= CPU::IO_END) {
        return read_io(addr);
    }
    return cpu.read32(addr);
}

void Bus::write(uint32_t addr, uint32_t value) {
    // Se for I/O mapeado, tratar aqui
    if (addr >= CPU::IO_START && addr <= CPU::IO_END) {
        write_io(addr, value & 0xFF);
        return;
    }
    cpu.write32(addr, value);
}

uint8_t Bus::read_io(uint32_t addr) {
    // Exemplo: leitura de teclado
    if (addr == 0x9FC00) {
        return get_keyboard_input();
    }
    
    // Timer ou outros dispositivos
    if (addr == 0x9FC10) {
        return 0; // Timer não implementado
    }
    
    return 0;
}

void Bus::write_io(uint32_t addr, uint8_t value) {
    // Exemplo: escrita em porta serial
    if (addr == 0x9FC20) {
        std::cout << "[PORTA SERIAL] " << static_cast<char>(value);
    }
    
    // Exemplo: escrita em registrador de controle
    if (addr == 0x9FC30) {
        std::cout << "[CONTROLE] Valor: 0x" << std::hex << (int)value << std::dec << std::endl;
    }
}

uint8_t Bus::get_keyboard_input() {
    // Simulação simples de entrada de teclado
    static char keys[] = {'A', 'B', 'C', 'D', 'E'};
    static int key_index = 0;
    
    uint8_t key = static_cast<uint8_t>(keys[key_index]);
    key_index = (key_index + 1) % 5;
    
    return key;
}