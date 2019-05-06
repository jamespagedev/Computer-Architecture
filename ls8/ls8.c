#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, const char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  if (argc == 1)
  {
    cpu_load(&cpu, NULL);
  }
  else
  {
    cpu_load(&cpu, (char *)argv[1]);
  }

  cpu_run(&cpu);

  return 0;
}