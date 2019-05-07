#include "cpu.h"
#include "cpu_helpers.h"
#include <stdio.h>

// ********************************************* helper functions **********************************************
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
  for (int i = 0; i < (int)sizeof(binary) && num != 0; i++)
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

// ***************************************** cpu_run support functions *****************************************
void ldi(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands)
{
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
}

void mul(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands)
{
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
}

void prn(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands)
{
  // Print to the console the decimal integer value that is stored in the given register.
  print_ir_bin_hex_dec(IR);
  printf("\n");
  printf("PRN Operand(s):\n");
  printf("Num of operands = %d\n", num_operands);
  printf("Operand 1 = %d\n", operands[0]);
  printf("register[%d] = %d\n", operands[0], cpu->registers[operands[0]]);
  printf("--------------------------------------------------------\n");
  cpu->PC += (num_operands + 1);
}