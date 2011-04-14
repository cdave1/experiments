#include <stdio.h>

// time ./pow 

unsigned int pow1(unsigned int in)
{
  unsigned int i = 1;
  while (i < in) i = i << 1;
  return i;
}


unsigned int pow2(unsigned int in)
{
     in -= 1;

     in |= in >> 16;
     in |= in >> 8;
     in |= in >> 4;
     in |= in >> 2;
     in |= in >> 1;

     return in + 1;
}


int main (int argc, char ** argv)
{
  unsigned i,j, p;

  for (j = 0; j < 100; ++j) {
    for (i = 0; i < 1000000; ++i) {
      p = pow1(i);
      //printf("%u: %u\n", i, pow1(i));
    }
  }
  return 0;
}
