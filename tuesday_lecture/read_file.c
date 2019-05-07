#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fp;
  char line[1024];

  if (argc != 2)
  {
    fprintf(stderr, "usage: readfile filename\n");
    return 1;
  }

  fp = fopen(argv[1], "r");

  if (fp == NULL)
  {
    fprintf(stderr, "readfile: error opening file\n");
    return 2;
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    //printf("%s", line);
    char *endptr;

    unsigned int val = strtoul(line, &endptr, 10);

    if (endptr == line)
    {
      //printf("Found no digits\n");
      continue;
    }

    printf("%u\n", val);
  }

  fclose(fp);

  return 0;
}