#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  unsigned int x = 123;

  // Printing a value as decimal and hex

  printf("%d\n", x); // print "123" (decimal)
  printf("%x\n", x); // print "7b" (hex)

  // There's no printf specifier for printing binary

  // Converting a decimal number in a string to a value

  char *s = "1234";         // 1234 is 0x4d2
  x = strtoul(s, NULL, 10); // Convert base-10 string to value

  // Printing a value as decimal and hex

  printf("%d\n", x); // prints "1234" (decimal)
  printf("%x\n", x); // prints "4d2" (hex)

  // Converting a binary number in a string to a value

  s = "100101";            // 0b100101 is 37 is 0x25
  x = strtoul(s, NULL, 2); // Convert base-2 string to value

  // Printing a value as decimal and hex

  printf("%d\n", x); // prints "37" (decimal)
  printf("%x\n", x); // prints "25" (hex)

  return 0;
}