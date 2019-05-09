/**
 * Alternate solution that properly counts out seconds.
 */
#include <stdio.h>
#include <sys/time.h>

/**
 * Helper function that returns the current second as a float
 */
double getcurtime(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(void)
{
  double last_sec = getcurtime();
  int running = 1;

  // Like in the main cpu loop:

  while (running)
  {
    double cur_sec = getcurtime();

    if (cur_sec - last_sec >= 1.0)
    {
      printf("Bing!\n");
      last_sec = cur_sec;
    }
  }

  return 0;
}