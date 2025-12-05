#include "cpu.h"
#include "bus.h"
#include <iostream>

int main()
{
    Memory mem;
    CPU cpu(mem);
    Bus bus(cpu, 1, true);

    // Programa HELLO carregado na memória
    uint32_t program[] = {
        0x000800B7, // lui x1, 0x00080
        0x04800113, // addi x2, x0, 'H'
        0x0020A023, // sw x2, 0(x1)

        0x04500113, // addi x2, x0, 'E'
        0x0020A223, // sw x2, 4(x1)

        0x04C00113, // addi x2, x0, 'L'
        0x0020A423, // sw x2, 8(x1)

        0x04C00113, // addi x2, x0, 'L'
        0x0020A623, // sw x2, 12(x1)

        0x04F00113, // addi x2, x0, 'O'
        0x0020A823, // sw x2, 16(x1)

        0x00100073 // EBREAK (encerra execução)
    };

    // Carregar programa na RAM da CPU
    for (int i = 0; i < sizeof(program) / 4; i++)
    {
        cpu.write32(i * 4, program[i]);
    }

    cpu.pc = 0;

    std::cout << "Executando programa HELLO...\n";

    while (cpu.running)
    {
        bus.clock(); // executa uma instrução por ciclo
    }

    std::cout << "Programa terminou. Instrucoes executadas = "
              << bus.getInstructionsExecuted() << "\n";

    std::cout << "\n=== CONTEUDO FINAL DA VRAM ===\n";
    bus.show_vram_ascii();

    return 0;
}
