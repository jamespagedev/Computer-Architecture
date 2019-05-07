#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/*
  Helper Functions
  In `cpu.c`, add functions `cpu_ram_read()` and `cpu_ram_write()` that access the
  RAM inside the `struct cpu`.

  We'll make use of these helper function later.
  */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char pc)
{
  return cpu->ram[pc];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char binary)
{
  cpu->ram[index] = binary;
}

int get_num_of_operands(const unsigned char ir)
{
  switch (ir)
  {
  case LDI:
    return 2;
  case MUL:
    return 2;
  case PRN:
    return 1;
  default:
    printf("Instruction has no operands.\nExiting program...");
    exit(1);
  }
}

void print_binary(const unsigned char dec)
{
  char binary[] = {'0', '0', '0', '0', '0', '0', '0', '0'};

  int num = dec;
  for (int i = 0; i < sizeof(binary) && num != 0; i++)
  {
    if (num >= 128)
    {
      binary[0] = '1';
      num -= 128;
    }
    else if (num >= 64)
    {
      binary[1] = '1';
      num -= 64;
    }
    else if (num >= 32)
    {
      binary[2] = '1';
      num -= 32;
    }
    else if (num >= 16)
    {
      binary[3] = '1';
      num -= 16;
    }
    else if (num >= 8)
    {
      binary[4] = '1';
      num -= 8;
    }
    else if (num >= 4)
    {
      binary[5] = '1';
      num -= 4;
    }
    else if (num >= 2)
    {
      binary[6] = '1';
      num -= 2;
    }
    else if (num >= 1)
    {
      binary[7] = '1';
      num -= 1;
    }
  }
  printf("BIN = %s\n", binary);
}

void print_ir_bin_hex_dec(const unsigned char ir)
{
  printf("IR Values:\n");
  printf("DEC = %d\n", ir);
  printf("HEX = %X\n", ir);
  print_binary(ir);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  if (file == NULL)
  {
    printf("No file given, using example process...\n\n");
    char data[DATA_LEN] = {
        // From print8.ls8
        0b10000010, // LDI R0,8
        0b00000000,
        0b00001000,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001 // HLT
    };

    int address = 0;

    for (int i = 0; i < DATA_LEN; i++)
    {
      cpu->ram[address++] = data[i];
    }
    return;
  }
  // TODO: Replace this with something less hard-coded
  printf("Running file %s ...\n\n", file);
  FILE *fp;
  char line[512];
  int index = 0;
  int base_num = 2;

  fp = fopen(file, "r");

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    if (line[0] != '0' && line[0] != '1')
    {
      printf("Skipping line... %s\n", line);
      continue;
    }
    char *endptr;
    unsigned char binary_num = strtoul(line, &endptr, base_num);
    cpu_ram_write(cpu, index++, binary_num);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
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
    if (IR == HLT)
    {
      return;
    }
    print_ir_bin_hex_dec(IR);
    printf("\n");
    // 2. Figure out how many operands this next instruction requires
    num_operands = get_num_of_operands(IR);
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operands[sizeof(num_operands)];
    for (int i = 0, c = 1; i < num_operands; i++, c++)
    {
      operands[i] = cpu_ram_read(cpu, cpu->PC + c);
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    case LDI:
      // Set the value of a register to an integer.
      printf("LDI Operands:\n");
      printf("Num of operands = %d\n", num_operands);
      printf("Operand 1 = %d\n", operands[0]);
      printf("Operand 2 = %d\n", operands[1]);
      cpu->registers[operands[0]] = (int)operands[1];
      printf("register = %d\n", cpu->registers[operands[0]]);
      cpu->PC += 3;
      break;
    case MUL:
      // Multiply the values in two registers together and store the result in registerA.
      printf("MUL Operands:\n");
      printf("Num of operands = %d\n", num_operands);
      printf("Operand 1 = %d\n", operands[0]);
      printf("Operand 2 = %d\n", operands[1]);
      cpu->registers[operands[0]] = (operands[0] * operands[1]);
      printf("Product of registerA = %d\n", cpu->registers[operands[0]]);
      cpu->PC += 3;
      break;
    case PRN:
      // Print to the console the decimal integer value that is stored in the given register.
      printf("PRN: Operand = %d\n", operands[0]);
      cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("Instruction not recognized. Exiting program...");
      exit(1);
    }
    printf("--------------------------------------------------------\n");
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
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers) - 1);
  memset(cpu->ram, 0, sizeof(cpu->ram));

  /*
    When the LS-8 is booted, the following steps occur:

    - `R0`-`R6` are cleared to `0`.
    - >>>>>>>>>>>>>>>>>>>`R7` is set to `0xF4`.<<<<<<<<<<<<<<<<<<<
    - `PC` and `FL` registers are cleared to `0`.
    - RAM is cleared to `0`.
  */
  cpu->registers[7] = 0xF4;

  /*
    Later on, you might do further initialization here, e.g. setting the initial
    value of the stack pointer.
  */
}
