#include <stdio.h>
#include <string.h>


char *
mystrncpy(char * s1, const char * s2, size_t n)
{
  if (n == 0) return s1;
  strncpy(s1, s2, n-1);
  s1[n-1] = '\0';
  return s1;
}


int main (int argc, char ** argv)
{
  char dst[8];
  bzero(dst, sizeof(dst));
  const char * src = "12345678";

  strncpy(dst, src, 8);

  printf("%s\n", dst);
  
  mystrncpy(dst, src, 8);
  printf("%s\n", dst);

  return 0;
}
