#include "cpu.h"
#include "cpu_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  if (file == NULL)
  {
    cpu_load_default(cpu);
    return;
  }
  // TODO: Replace this with something less hard-coded
  cpu_load_file(cpu, file);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // remove warnings
  (void)cpu;
  (void)regA;
  (void)regB;

  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  default:
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  printf("Executing instructions from RAM on CPU...\n");
  printf("--------------------------------------------------------\n");
  int running = 1, num_operands; // True until we get a HLT instruction

  unsigned char IR;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    /*
      It needs to read the memory address that's stored in register `PC`, and store
      that result in `IR`, the _Instruction Register_.
    */
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    num_operands = ((IR >> 6) & 0b11);
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operands[sizeof(num_operands)];
    for (int i = 0, c = 1; i < num_operands; i++, c++)
    {
      operands[i] = cpu_ram_read(cpu, cpu->PC + c);
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    // case *:
    //    5. Do whatever the instruction should do according to the spec.
    //    6. Move the PC to the next instruction.
    case LDI:
      ldi(cpu, IR, num_operands, operands);
      break;
    case MUL:
      mul(cpu, IR, num_operands, operands);
      break;
    case PRN:
      prn(cpu, IR, num_operands, operands);
      break;
    case HLT:
      // Finished Instructions
      printf("\nInstructions executed successfully, exiting program...\n\n");
      running = 0;
      break;
    default:
      printf("Instruction not recognized. Exiting program...\n\n");
      exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  /*
    At first, the PC, registers, and RAM should be cleared to zero.
    (`memset()` might help you clear registers and RAM.)
  */
  printf("Initializing CPU...\n");
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers) - 1); // R7 is skipped, because it will be initialized as 0xF4
  memset(cpu->ram, 0, sizeof(cpu->ram));

  /*
    When the LS-8 is booted, the following steps occur:

    - `R0`-`R6` are cleared to `0`.
    - >>>>>>>>>>>>>>>>>>>`R7` is set to `0xF4`.<<<<<<<<<<<<<<<<<<<
    - `PC` and `FL` registers are cleared to `0`.
    - RAM is cleared to `0`.
  */
  cpu->registers[7] = 0xF4;
  printf("Complete.\n\n");

  /*
    Later on, you might do further initialization here, e.g. setting the initial
    value of the stack pointer.
  */
}
