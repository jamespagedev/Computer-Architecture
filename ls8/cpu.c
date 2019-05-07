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
    printf("No file given, loading example instructions into RAM...\n");
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
    printf("Complete.\n\n");
    return;
  }
  // TODO: Replace this with something less hard-coded
  printf("Loading file %s into RAM...\n", file);
  FILE *fp;

  /*
    | Level |   Capacity   | Lookup Time (nanoseconds) |
    | :---: | :----------: | :-----------------------: |
    |  L1   |    2-8 KB    |           ~1 ns           |
    |  L2   |  256-512 KB  |           ~3 ns           |
    |  L3   | 1024-8192 KB |          ~12 ns           |
    |  RAM  | 8-32 **GB**  |          ~100 ns          |

    Using L2, since I don't see any files where more will be needed
  */
  char line[512];

  // used for keeping track of the current address in the ram
  int index = 0;

  // used for converting str value pulled from line into binary
  int base_num = 2;

  // will either be '\0' or the str(line)... after used by strtoul()
  //    used to either skip line or validate the value, and plug it into RAM
  char *endptr;

  fp = fopen(file, "r");

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    unsigned char binary_num = strtoul(line, &endptr, base_num);
    if (endptr == line)
    {
      printf("Skipping line... %s\n", line);
      continue; // skip to next line iteration in while loop
    }
    // else, endptr = '\0'
    cpu_ram_write(cpu, index++, binary_num);
  }

  fclose(fp);
  printf("File loaded into RAM successfully.\n\n");
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
      // Set the value of a register to an integer.
      print_ir_bin_hex_dec(IR);
      printf("\n");
      printf("LDI Operand(s):\n");
      printf("Num of operands = %d\n", num_operands);
      printf("Operand 1 = %d\n", operands[0]);
      printf("Operand 2 = %d\n", operands[1]);
      cpu->registers[operands[0]] = (int)operands[1];
      printf("value of %d stored in register[%d]\n", cpu->registers[operands[0]], operands[0]);
      printf("--------------------------------------------------------\n");
      cpu->PC += (num_operands + 1);
      break;
    case MUL:
      // Multiply the values in two registers together and store the result in registerA.
      print_ir_bin_hex_dec(IR);
      printf("\n");
      printf("MUL Operand(s):\n");
      printf("Num of operands = %d\n", num_operands);
      printf("Operand 1 = %d\n", operands[0]);
      printf("Operand 2 = %d\n", operands[1]);
      cpu->registers[operands[0]] = (cpu->registers[operands[0]] * cpu->registers[operands[1]]);
      printf("Product value of register[%d] = %d\n", operands[0], cpu->registers[operands[0]]);
      printf("stored product value of %d in register[%d]\n", cpu->registers[operands[0]], operands[0]);
      printf("--------------------------------------------------------\n");
      cpu->PC += (num_operands + 1);
      break;
    case PRN:
      // Print to the console the decimal integer value that is stored in the given register.
      print_ir_bin_hex_dec(IR);
      printf("\n");
      printf("PRN Operand(s):\n");
      printf("Num of operands = %d\n", num_operands);
      printf("Operand 1 = %d\n", operands[0]);
      printf("register[%d] = %d\n", operands[0], cpu->registers[operands[0]]);
      printf("--------------------------------------------------------\n");
      cpu->PC += (num_operands + 1);
      break;
    case HLT:
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
