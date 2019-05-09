#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <ctype.h> // for isprint()

struct termios oldattr;
int oldfcntl;

/**
 * Set the terminal to raw, non-blocking
 */
void set_terminal(void)
{
  struct termios newattr;

  // Non-canonical, no-echo mode
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  // Non-blocking mode
  oldfcntl = fcntl(STDIN_FILENO, F_GETFL, 0);
  int newfcntl = oldfcntl | O_NONBLOCK;

  fcntl(STDIN_FILENO, F_SETFL, newfcntl);
}

/**
 * Reset the terminal to what it was before
 */
void reset_terminal(void)
{
  fcntl(STDIN_FILENO, F_SETFL, oldfcntl);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
}

/**
 * Main
 */
int main(void)
{
  int done = 0;

  printf("Type 'q' or ESC to quit\n");

  // Raw, noecho, non-blocking mode
  set_terminal();

  while (!done)
  {
    int c = getchar();

    // getchar() now returns -1 (EWOULDBLOCK) if there are no
    // characters ready to read:
    if (c != -1)
    {
      if (isprint(c))
      {
        printf("%d: %c\n", c, c);
      }
      else
      {
        printf("%d: <unprintable>\n", c);
      }
    }

    // Quit if any of these are hit (27 == ESC)
    if (c == 'q' || c == 'Q' || c == 27)
    {
      done = 1;
    }
  }

  // Back to normal mode
  reset_terminal();

  return 0;
}