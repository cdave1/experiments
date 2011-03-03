#include <stdio.h>

typedef unsigned long long uint64;

unsigned int d(uint64 a, uint64 b)
{
  const unsigned int mask = 0xFFFEFEFEUL;
  uint64 tt = (a & mask) + (b & mask);
  return tt >> 1;
}


int main (int argc, char ** argv)
{
  unsigned int a = 0xFF749EAB;
  unsigned int mask = 0xFFFEFEFE;

  printf("%X\n", d(0xFF749EAB, 0xFF709CA9));

  unsigned int aa = 0xFF749EAB;
  unsigned int bb = 0xFF709CA9;

  printf("%X\n", d(aa, bb));

  unsigned int result = d(aa, bb);

  printf("%X\n", result);

  return 0;
}
