#include <stdio.h>
#include <stdlib.h>

#define PRINT_BEEJ 1
#define HALT 2
#define PRINT_NUM 3
#define SAVE_REG 4
#define PRINT_REG 5

unsigned char reg[8];

unsigned char memory[128];

void load_program(char *filename)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "comp: error opening file\n");
    exit(2);
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;

    unsigned int val = strtoul(line, &endptr, 10);

    if (endptr == line)
    {
      //printf("Found no digits\n");
      continue;
    }

    //printf("%u\n", val);

    memory[address] = val;
    address++;
  }

  fclose(fp);
}

int main(int argc, char **argv)
{
  int halted = 0;
  int pc = 0; // Program Counter, index of the current instruction

  if (argc != 2)
  {
    fprintf(stderr, "usage: comp filename\n");
    return 1;
  }

  load_program(argv[1]);

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