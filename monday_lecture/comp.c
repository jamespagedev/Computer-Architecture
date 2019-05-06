#include <stdio.h>
#include <stdlib.h>

#define PRINT_BEEJ 1
#define HALT 2
#define PRINT_NUM 3
#define SAVE_REG 4
#define PRINT_REG 5

unsigned char reg[8];

unsigned char memory[128] = {
    PRINT_BEEJ,
    PRINT_NUM,
    99,
    SAVE_REG,
    2, // Register 2
    5, // Save 5 in register 2
    /*
    SAVE_REG,
    3,
    7,
    MULT, // future work
    2,
    3,
    */
    PRINT_REG,
    2, // Print value in register 2, "5"
    HALT};

int main(void)
{
  int halted = 0;
  int pc = 0; // Program Counter, index of the current instruction

  while (!halted)
  {
    int current_inst = memory[pc];
    int reg_num, val;

    switch (current_inst)
    {
    case PRINT_BEEJ:
      printf("Beej!\n");
      pc++; // move to next instruction
      break;

    case PRINT_NUM:
      printf("%d\n", memory[pc + 1]);
      pc += 2;
      break;

    case SAVE_REG:
      reg_num = memory[pc + 1];
      val = memory[pc + 2];
      reg[reg_num] = val;
      pc += 3; // next inst
      break;

    case PRINT_REG:
      reg_num = memory[pc + 1];
      printf("%d\n", reg[reg_num]);
      pc += 2; // next inst
      break;

    case HALT:
      halted = 1;
      pc++; // move to next instruction
      break;

    default:
      printf("Unknown instruction at %d: %d\n", pc, current_inst);
      exit(1);
    }
  }
  return 0;
}