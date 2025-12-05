# Simulador de Computador RISC-V - Organização de Computadores

![imagemDiagramaSimulador](./assets/diagrama-do-simulador.png)

Este repositório contém a solução para o projeto final da disciplina <b>Organização de Computadores</b> desenvolvida durante o período letivo de 2025 por mim(<a href="https://github.com/baronemarbles">Marcio Paiva Barone Borges</a>) e pelo <a href="https://github.com/MaiA221">Rafael Maia</a>. Nós utilizamos a linguagem C++ para desenvolver o projeto.

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
  gcc main.cpp -o main
  ./main.o

```

<i>Ou</i>

```bash
  clang++ main.cpp -o main
  ./main.o
```

## Licença
Este projeto está sob a licença MIT. Consulte o arquivo [LICENSE](./LICENSE) para obter mais detalhes.

Este arquivo README.md foi inicado com o template do README.md da Fernanda Kipper em seu repositório do [desafio da SmartFit](https://github.com/Fernanda-Kipper/smartfit-frontend-challenge/tree/main?tab=MIT-1-ov-file#readme)
