# Simulador de Computador RISC-V - Organização de Computadores

![imagemDiagramaSimulador](./assets/diagrama-do-simulador.png)

Este repositório contém a solução para o projeto final da disciplina <b>Organização de Computadores</b> desenvolvida durante o período letivo de 2025 por mim(<a href="https://github.com/baronemarbles">Marcio Paiva Barone Borges</a>) e pelo <a href="https://github.com/MaiA221">Rafael do Nascimento Maia</a>. Nós utilizamos a linguagem C++ para desenvolver o projeto.

## 📖 Sobre o Projeto

O <a href="https://riscv.org">RISC-V</a> é uma arquitetura
de conjunto de instruções (ISA - Instruction Set Architecture) de padrão aberto, baseada nos princípios
de computadores com conjunto de instruções reduzido (RISC - Reduced Instruction Set Computer ).
Diferente da maioria das outras arquiteturas (como x86 e ARM), o RISC-V é um padrão livre e de
código aberto, o que significa que qualquer pessoa pode usá-lo, projetar, fabricar e vender processadores
RISC-V sem pagar taxas de licenciamento.
Deste modo, será implementado simulador de um computador com as seguintes características:
(a)
(b)
(c)

## Características
<ul>
  <li> CPU que implemente a ISA RV32I</li>
  <li> Memória RAM/VRAM</li>
  <li> Periféricos de entrada e saída</li>
</ul>

### ⚙️ Como Executar

Para executar a aplicação localmente, siga os passos abaixo:

1. Clone este repositório:

```bash
  git clone https://github.com/baronemarbles/simulador-de-computador-risc-v.git
  cd simulador-de-computador-risc-v

```

2. Instale o compilador <b>gcc</b> ou <b>Clang</b> para compilar o código

<b>GCC</b>
```bash
  sudo dnf install gcc
```

<b>Clang</b>
```bash
  sudo dnf install clang
```

3. Inicie a aplicação

```bash
  g++ main.cpp cpu.cpp bus.cpp memory.cpp instructions.cpp -o simulador
  ./simulador
```

<i>Ou</i>

```bash
  clang++ main.cpp cpu.cpp bus.cpp memory.cpp instructions.cpp -o simulador
  ./simulador
```

# Sobre o Simulador de CPU RISC-V (RV32I)

Este projeto implementa um simulador simplificado de uma CPU RISC-V 32 bits (RV32I), com suporte a:

- Conjunto básico de instruções aritméticas, lógicas, de desvio e de memória;
- Memória principal (RAM) e memória de vídeo (VRAM) mapeadas em um único espaço de endereços;
- Barramento (`Bus`) que coordena a busca e execução de instruções;
- Um programa de teste simples que escreve `"HELLO"` na VRAM.

---

## Arquitetura geral

A arquitetura simulada é composta pelos seguintes módulos principais:

- **CPU (`cpu.h` / `cpu.cpp`)**  
  Implementa os 32 registradores de propósito geral (`x0`–`x31`), o contador de programa (`pc`) e funções de contexto para interrupções. Também encapsula o acesso à memória por meio de métodos como `read32` e `write32`. :contentReference[oaicite:0]{index=0}  

- **Memory (`memory.h` / `memory.cpp`)**  
  Modela uma RAM de 640 KB com regiões distintas para RAM principal, VRAM, área reservada e I/O mapeado em memória. Toda leitura/escrita da CPU passa por este módulo. :contentReference[oaicite:1]{index=1}  

- **Bus (`bus.h` / `bus.cpp`)**  
  É o “cérebro do ciclo”: a cada `clock()` ele busca a instrução na memória usando o `pc` da CPU, chama o decodificador de instruções e contabiliza o número de instruções executadas. Também oferece suporte a interrupções e exibição da VRAM em modo ASCII. :contentReference[oaicite:2]{index=2}  

- **Instructions (`instructions.h` / `instructions.cpp`)**  
  Implementa o decodificador e a lógica de execução das instruções RV32I suportadas. A função estática `Instructions::execute` recebe a instrução bruta (32 bits), decodifica opcode/funct3/funct7 e despacha para a função correta (ADDI, LW, BEQ, JAL, etc.). :contentReference[oaicite:3]{index=3}  

- **Programa de exemplo (`main.cpp`)**  
  Carrega um pequeno programa em memória que escreve os caracteres `HELLO` na VRAM usando instruções RISC-V e finaliza com `EBREAK`. :contentReference[oaicite:4]{index=4}  

---

## Fluxo de execução

O ciclo de execução de instruções segue aproximadamente este fluxo:

1. **Inicialização**  
   O `main` instancia a memória, a CPU e o barramento:

   ```cpp
   Memory mem;
   CPU cpu(mem);
   Bus bus(cpu, 1, true);

Em seguida, grava um programa (array de uint32_t com opcodes RV32I) na memória usando cpu.write32(endereco, instrução) e zera o pc.

2. **Loop principal**
  Enquanto cpu.running for true, o main chama:
  while (cpu.running) {
      bus.clock();
  }

3. **Ciclo do barramento (Bus::clock)**

  Dentro de clock():
  * O barramento lê a instrução na posição atual do pc:
  uint32_t instruction = cpu.read32(cpu.pc);

  * Em seguida chama o decodificador:
  Instructions::execute(cpu, instruction);

  * Incrementa o contador de instruções executadas.
  * Opcionalmente, imprime o conteúdo da VRAM em ASCII a cada vram_interval instruções.

4. **Decodificação e execução de instruções**
  A função:
  void Instructions::execute(CPU& cpu, uint32_t instruction);

  faz:
  Extrai o opcode (bits [6:0]) e funct3 (bits [14:12]);
  * Usa um switch em opcode e funct3 para decidir qual grupo de instruções executar (I-type, R-type, LOAD, STORE, BRANCH, JUMP, LUI/AUIPC, SYSTEM);
  * Chama funções específicas como ADDI, LW, BEQ, JALR, etc.;
  * Cada função de instrução:
  * Lê registradores da CPU (cpu.reg[...]);
  * Calcula o resultado ou endereço efetivo;
  * Lê/escreve memória via cpu.read* / cpu.write* quando necessário;
  * Atualiza o pc (tipicamente pc += 4 ou salto relativo);
  * Garante que x0 nunca seja modificado (checando rd != 0).

5. **Término da execução**
  A instrução EBREAK é usada como “parada” do simulador:
  void Instructions::EBREAK(CPU &cpu, uint32_t i) {
    (void)i;
    cpu.running = false;
    cpu.pc += 4;
  }
  Quando ela é executada, cpu.running se torna false e o loop no main termina.

## Mapa de memória
O arquivo memory.h define a organização da memória simulada:
* Tamanho total da RAM: 0xA0000 bytes (640 KB)
Regiões principais:
Região	        Início	    Fim	        Descrição
RAM principal	  0x00000	    0x7FFFF	    Código e dados gerais
VRAM	          0x80000	    0x8FFFF	    Memória de vídeo (texto ASCII)
Reservado	      0x90000	    0x9FBFF	    Reservado para uso futuro
I/O mapeado	    0x9FC00	    0x9FFFF	    Dispositivos de entrada/saída

A classe Memory expõe os métodos:
uint8_t  read8(uint32_t addr)   const;
uint16_t read16(uint32_t addr)  const;
uint32_t read32(uint32_t addr)  const;

void write8(uint32_t addr, uint8_t value);
void write16(uint32_t addr, uint16_t value);
void write32(uint32_t addr, uint32_t value);

* A leitura de 32 bits (read32) reconstrói o valor a partir de quatro bytes sucessivos, em formato little-endian.
* A escrita de 32 bits (write32) quebra o valor em quatro bytes e os grava byte a byte.
* Leituras/escritas fora das regiões permitidas retornam 0 ou são ignoradas, evitando acessos ilegais.

## CPU
A classe CPU contém:
* Registradores de propósito geral:
  uint32_t reg[32];
* Contador de programa:
  uint32_t pc;
* Contexto para interrupções: saved_pc e saved_reg[32]
* Flag de execução: bool running

Ela mantém uma referência à memória:
* 'Memory &mem;'

E expõe métodos auxiliares de acesso a memória:
uint8_t read8(uint32_t addr)    { return mem.read8(addr); }
uint16_t read16(uint32_t addr)  { return mem.read16(addr); }
uint32_t read32(uint32_t addr)  { return mem.read32(addr); }

void write8(uint32_t addr, uint8_t value)   { mem.write8(addr, value); }
void write16(uint32_t addr, uint16_t value) { mem.write16(addr, value); }
void write32(uint32_t addr, uint32_t value) { mem.write32(addr, value); }
Ela também implementa funções de contexto para interrupções (salvar/restaurar registradores) e um reset() que zera os registradores, pc e o contexto salvo.

## Barramento (Bus)

A classe Bus conecta a CPU à memória e ao decodificador de instruções:
* clock()
    * Busca a instrução atual com cpu.read32(cpu.pc)
    * Chama Instructions::execute(cpu, instruction)
    * Incrementa o contador instructions_executed
    * Opcionalmente chama show_vram_ascii() para debugar a VRAM
* show_vram_ascii()
  Percorre a região de VRAM e imprime caracteres ASCII (ou espaços/pontos para bytes não imprimíveis), permitindo visualizar o conteúdo de “tela” no terminal.
* I/O mapeado
  Métodos read() e write() fazem roteamento de endereços entre RAM/VRAM e região de I/O, permitindo simular dispositivos como teclado, porta serial, etc.

## Conjunto de instruções suportado (RV32I parcial)
O decodificador (Instructions::execute) suporta um subconjunto relevante do RV32I:

* Aritméticas (inteiras)
    * ADD, ADDI, SUB, LUI, AUIPC

* Comparação
    * SLT, SLTU, SLTI, SLTIU

* Lógicas
    * AND, ANDI, OR, ORI, XOR, XORI

* Shift
    * SLL, SLLI, SRL, SRLI, SRA, SRAI

* Loads / Stores
    * LW, LB, LBU, LH, LHU
    * SW, SB, SH

* Branches
    * BEQ, BNE, BLT, BGE, BLTU, BGEU

* Desvios incondicionais
    * JAL, JALR

* Sistema
    * ECALL (no momento, apenas avança o pc)
    * EBREAK (encerra a execução do simulador)

## Programa de exemplo: H E L L O
O main.cpp inclui um programa em código de máquina que:
1. Carrega o endereço base da VRAM em um registrador (lui x1, 0x00080 → x1 = 0x00080000);
2. Escreve os caracteres 'H', 'E', 'L', 'L', 'O' em posições sucessivas da VRAM usando ADDI + SW;
3. Executa EBREAK para encerrar o simulador.
Ao final, o conteúdo da VRAM é exibido no terminal em modo texto.

## Possíveis extesões
Algumas ideias para evolução do projeto:
* Suporte a mais instruções do padrão RV32I ou extensões (M, C, etc.);
* Implementação completa de I/O mapeado (teclado, temporizador, porta serial);
* Suporte a interrupções com retorno (mret) e registradores de status;
* Carregamento de programas a partir de arquivos binários ou ELF;
* Interface gráfica simples para visualizar a VRAM como “tela” de vídeo.

## Licença
Este projeto está sob a licença MIT. Consulte o arquivo [LICENSE](./LICENSE) para obter mais detalhes.

Este arquivo README.md foi inicado com o template do README.md da Fernanda Kipper em seu repositório do [desafio da SmartFit](https://github.com/Fernanda-Kipper/smartfit-frontend-challenge/tree/main?tab=MIT-1-ov-file#readme)
