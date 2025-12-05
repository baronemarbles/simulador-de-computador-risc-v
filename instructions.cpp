#include "instructions.h"
#include <iostream>

void Instructions::execute(CPU &cpu, uint32_t instruction)
{
    uint8_t opcode = instruction & 0x7F;
    uint8_t funct3 = (instruction >> 12) & 0x07;

    switch (opcode)
    {
    case 0x13: // I-type: ADDI, ANDI, ORI, XORI, SLTI, SLTIU, SLLI, SRLI, SRAI
        switch (funct3)
        {
        case 0x0:
            ADDI(cpu, instruction);
            break;
        case 0x1:
            SLLI(cpu, instruction);
            break;
        case 0x2: // SLTI
            // Implementar
            break;
        case 0x3: // SLTIU
            // Implementar
            break;
        case 0x4:
            XORI(cpu, instruction);
            break;
        case 0x5:
            if ((instruction >> 25) & 1)
                SRAI(cpu, instruction);
            else
                SRLI(cpu, instruction);
            break;
        case 0x6:
            ORI(cpu, instruction);
            break;
        case 0x7:
            ANDI(cpu, instruction);
            break;
        }
        break;

    case 0x33: // R-type: ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
        switch (funct3)
        {
        case 0x0:
            if ((instruction >> 25) & 0x20)
                SUB(cpu, instruction);
            else
                ADD(cpu, instruction);
            break;
        case 0x1:
            SLL(cpu, instruction);
            break;
        case 0x2: // SLT
            // Implementar
            break;
        case 0x3: // SLTU
            // Implementar
            break;
        case 0x4:
            XOR(cpu, instruction);
            break;
        case 0x5:
            if ((instruction >> 25) & 0x20)
                SRA(cpu, instruction);
            else
                SRL(cpu, instruction);
            break;
        case 0x6:
            OR(cpu, instruction);
            break;
        case 0x7:
            AND(cpu, instruction);
            break;
        }
        break;

    case 0x03: // LOAD
        if (funct3 == 0x2)
            LW(cpu, instruction);
        break;

    case 0x23: // STORE
        if (funct3 == 0x2)
            SW(cpu, instruction);
        break;

    case 0x63: // BRANCH
        switch (funct3)
        {
        case 0x0:
            BEQ(cpu, instruction);
            break;
        case 0x1:
            BNE(cpu, instruction);
            break;
            // Implementar outros branches
        }
        break;

    case 0x6F: // JAL
        JAL(cpu, instruction);
        break;

    case 0x67: // JALR
        JALR(cpu, instruction);
        break;

    case 0x37: // LUI
        LUI(cpu, instruction);
        break;

    case 0x17: // AUIPC
        AUIPC(cpu, instruction);
        break;

    case 0x73: // SYSTEM
    {
        uint32_t imm12 = instruction >> 20;

        if (funct3 == 0 && imm12 == 0)
        {
            // ECALL = 0x00000073
            ECALL(cpu, instruction);
        }
        else if (funct3 == 0 && imm12 == 1)
        {
            // EBREAK = 0x00100073
            EBREAK(cpu, instruction);
        }
        else
        {
            cpu.pc += 4;
        }
    }
    break;
    }
}

// Implementações das instruções
//========================================================
//-----------------------SHIFT----------------------------
//========================================================
void Instructions::SLL(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    uint8_t rs2 = (i >> 20) & 0x1F; // Source Register2

    uint32_t result = cpu.reg[rs1] << (cpu.reg[rs2] & 0x1F);

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}

void Instructions::SLLI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    uint8_t shamt = (i >> 20) & 0x1F;
    // Shift lógico à esquerda

    uint32_t result = cpu.reg[rs1] << shamt;

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}

void Instructions::SRL(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    uint8_t rs2 = (i >> 20) & 0x1F; // Source Register2

    uint32_t result = cpu.reg[rs1] >> (cpu.reg[rs2] & 0x1F);

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}

void Instructions::SRLI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t shamt = (i >> 20) & 0x1F;
    // Shift aritmético à direita

    uint32_t result = cpu.reg[rs1] >> shamt;

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}

void Instructions::SRA(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    uint8_t rs2 = (i >> 20) & 0x1F; // Source Register2
    uint8_t shamt = cpu.reg[rs2] & 0x1F;
    // Shift aritmético à direita (preserva sinal)

    int32_t s = (int32_t)cpu.reg[rs1];
    uint32_t result = s >> shamt;

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}

void Instructions::SRAI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;     // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;   // Source Register1
    uint8_t shamt = (i >> 20) & 0x1F; // Source Register2
    // Shift aritmético à direita (preserva sinal)

    int32_t s = (int32_t)cpu.reg[rs1];
    uint32_t result = s >> shamt;

    if (rd != 0)
        cpu.reg[rd] = result;
    cpu.pc += 4;
}
//--------------------------------------------------------

//========================================================
//----------------------ARITMÉTICA------------------------
//========================================================
void Instructions::ADD(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t rs2 = (i >> 20) & 0x1F; // Source Register2

    int32_t result = (int32_t)cpu.reg[rs1] + (int32_t)cpu.reg[rs2];

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::ADDI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;            // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;          // Source Register1
    int32_t imm = sext12((i >> 20) & 0xFFF); // Immediate

    // // DEBUG ADICIONADO
    // std::cout << "[DEBUG ADDI] rd=x" << (int)rd
    //           << ", rs1=x" << (int)rs1 << " (valor=" << cpu.reg[rs1] << ")"
    //           << ", imm=0x" << std::hex << imm << std::dec
    //           << " (" << imm << ")" << std::endl;

    int32_t result = (int32_t)cpu.reg[rs1] + imm;

    // std::cout << "[DEBUG ADDI] Resultado: " << cpu.reg[rs1] << " + " << imm
    //           << " = " << result << std::endl;

    if (rd != 0)
    {
        cpu.reg[rd] = (uint32_t)result;

        // std::cout << "[DEBUG ADDI] x" << (int)rd << " = " << cpu.reg[rd] << std::endl;
    }

    cpu.pc += 4;
}

void Instructions::SUB(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t rs2 = (i >> 20) & 0x1F; // Source Register2

    int32_t result = (int32_t)cpu.reg[rs1] - (int32_t)cpu.reg[rs2];

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::LUI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;
    // Pega bits [31:12] = 20 bits
    int32_t imm = (i >> 12) & 0xFFFFF;

    // Extensão de sinal para 20 bits
    if (imm & 0x80000)
    { // Bit 19 (sinal para 20 bits)
        imm |= 0xFFF00000;
    }

    // Desloca 12 bits para esquerda
    imm = imm << 12;

    if (rd != 0)
    {
        cpu.reg[rd] = imm;
    }
    cpu.pc += 4;
}

// Sobe o valor do imediato para o PC
void Instructions::AUIPC(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;
    int32_t imm = (i & 0xFFFFF000); // Immediate

    if (rd != 0)
    {
        cpu.reg[rd] = cpu.pc + imm;
    }

    cpu.pc += 4;
}
//--------------------------------------------------------

//========================================================
//-----------------------LÓGICA---------------------------
//========================================================
void Instructions::XOR(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t rs2 = (i >> 20) & 0x1F; // Source Register2

    int32_t result = (int32_t)cpu.reg[rs1] ^ (int32_t)cpu.reg[rs2];

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::XORI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;            // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;          // Source Register1
    int32_t imm = sext12((i >> 20) & 0xFFF); // Immediate

    int32_t result = (int32_t)cpu.reg[rs1] ^ imm;

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::OR(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t rs2 = (i >> 20) & 0x1F; // Source Register2

    int32_t result = (int32_t)cpu.reg[rs1] | (int32_t)cpu.reg[rs2];

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::ORI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;            // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;          // Source Register1
    int32_t imm = sext12((i >> 20) & 0xFFF); // Immediate

    int32_t result = (int32_t)cpu.reg[rs1] | imm;

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::AND(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;   // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F; // Source Register1
    int32_t rs2 = (i >> 20) & 0x1F; // Source Register2

    int32_t result = (int32_t)cpu.reg[rs1] & (int32_t)cpu.reg[rs2];

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}

void Instructions::ANDI(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;            // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;          // Source Register1
    int32_t imm = sext12((i >> 20) & 0xFFF); // Immediate

    int32_t result = (int32_t)cpu.reg[rs1] & imm;

    if (rd != 0)
        cpu.reg[rd] = (uint32_t)result;
    cpu.pc += 4;
}
//--------------------------------------------------------

//========================================================
//-------------------------BRANCH-------------------------
//========================================================
void Instructions::BEQ(CPU &cpu, uint32_t i)
{
    uint8_t rs1 = (i >> 15) & 0x1F; // Destination Register
    uint8_t rs2 = (i >> 20) & 0x1F; // Source Register1
    // Formato B: bits [31|30:25|24:21|20|19:12|11|10:5|4:1|0]
    int32_t imm = ((i >> 31) & 0x1) << 12 |
                  ((i >> 7) & 0x1) << 11 |
                  ((i >> 25) & 0x3F) << 5 |
                  ((i >> 8) & 0xF) << 1; // Immediate

    // Extensão de sinal para 13 bits
    if (imm & 0x1000)
    { // Bit 12
        imm |= 0xFFFFE000;
    }

    if (cpu.reg[rs1] == cpu.reg[rs2])
    {
        cpu.pc += imm;
    }
    else
    {
        cpu.pc += 4;
        // Se não for igual, o PC avança normalmente
    }
}

void Instructions::BNE(CPU &cpu, uint32_t i)
{
    uint8_t rs1 = (i >> 15) & 0x1F; // Destination Register
    uint8_t rs2 = (i >> 20) & 0x1F; // Source Register1
    int32_t imm = ((i >> 31) & 0x1) << 12 |
                  ((i >> 7) & 0x1) << 11 |
                  ((i >> 25) & 0x3F) << 5 |
                  ((i >> 8) & 0xF) << 1; // Immediate

    // Extensão de sinal para 13 bits
    if (imm & 0x1000)
    { // Bit 12
        imm |= 0xFFFFE000;
    }

    if (cpu.reg[rs1] != cpu.reg[rs2])
    {
        cpu.pc += imm;
    }
    else
    {
        cpu.pc += 4; // Se não for diferente, o PC avança normalmente
    }
}

void Instructions::BLT(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;    // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;  // Source Register1
    int32_t imm = (i >> 20) & 0xFFF; // Immediate
}
//--------------------------------------------------------

//========================================================
//-------------------------JUMPERS------------------------
//========================================================
void Instructions::JAL(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F; // Destination Register
    int32_t imm = ((i >> 31) & 0x1) << 20 |
                  ((i >> 12) & 0xFF) << 12 |
                  ((i >> 20) & 0x1) << 11 |
                  ((i >> 21) & 0x3FF) << 1; // Immediate

    // Extensão de sinal para 21 bits
    if (imm & 0x100000)
    { // Bit 20
        imm |= 0xFFE00000;
    }

    if (rd != 0)
    {
        cpu.reg[rd] = cpu.pc + 4;
    }

    cpu.pc += imm; // Executa o salto
}

void Instructions::JALR(CPU &cpu, uint32_t i)
{
    uint8_t rd = (i >> 7) & 0x1F;            // Destination Register
    uint8_t rs1 = (i >> 15) & 0x1F;          // Source Register1
    int32_t imm = sext12((i >> 20) & 0xFFF); // Immediate

    // std::cout << "[DEBUG JALR] rd=x" << (int)rd
    //           << ", rs1=x" << (int)rs1 << " (valor=0x"
    //           << std::hex << cpu.reg[rs1] << ")"
    //           << ", imm=" << std::dec << imm
    //           << " (0x" << std::hex << imm << ")" << std::dec << std::endl;

    // Garantir alinhamento do endereço
    uint32_t target = (cpu.reg[rs1] + imm) & ~1;

    // std::cout << "[DEBUG JALR] target = (0x" << std::hex << cpu.reg[rs1]
    //           << " + 0x" << imm << ") & ~1 = 0x" << target << std::dec << std::endl;

    if (rd != 0)
    {
        cpu.reg[rd] = cpu.pc + 4;
        // std::cout << "[DEBUG JALR] x" << (int)rd << " = PC+4 = 0x"
        //           << std::hex << cpu.reg[rd] << std::dec << std::endl;
    }

    cpu.pc = target;
    // std::cout << "[DEBUG JALR] Novo PC = 0x" << std::hex << cpu.pc << std::dec << std::endl;
}

//--------------------------------------------------------

// LOAD WORD
void Instructions::LW(CPU &cpu, uint32_t instr)
{
    uint8_t rd = (instr >> 7) & 0x1F;
    uint8_t rs1 = (instr >> 15) & 0x1F;
    int32_t imm = sext12((instr >> 20) & 0xFFF); // Immediate

    uint32_t addr = cpu.reg[rs1] + imm;
    if (rd != 0)
    {
        cpu.reg[rd] = cpu.read32(addr);
    }
    cpu.pc += 4;
}

// STORE WORD
void Instructions::SW(CPU &cpu, uint32_t instr)
{
    uint8_t rs1 = (instr >> 15) & 0x1F;
    uint8_t rs2 = (instr >> 20) & 0x1F;

    // std::cout << "[DEBUG SW] rs1=x" << (int)rs1 << " (valor=" << cpu.reg[rs1]
    //           << "), rs2=x" << (int)rs2 << " (valor=" << cpu.reg[rs2] << ")" << std::endl;

    int32_t imm = ((instr >> 7) & 0x1F) |
                  (((instr >> 25) & 0x7F) << 5);

    if (imm & 0x800)
        imm |= 0xFFFFF000;

    // std::cout << "[DEBUG SW] imm=" << imm << " (0x" << std::hex << imm << std::dec << ")" << std::endl;

    uint32_t addr = cpu.reg[rs1] + imm;

    // std::cout << "[DEBUG SW] Escrevendo valor " << cpu.reg[rs2]
    //           << " em addr=0x" << std::hex << addr << std::dec << std::endl;

    cpu.write32(addr, cpu.reg[rs2]);
    cpu.pc += 4;
}

void Instructions::ECALL(CPU &cpu, uint32_t i)
{
    (void)i;
    cpu.pc += 4;
}

void Instructions::EBREAK(CPU &cpu, uint32_t i)
{
    (void)i;
    cpu.running = false;
}