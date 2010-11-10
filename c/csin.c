/*
csin.c by David Petrie <www.davidpetrie.com>

Just a quick comparison of native c sine functions against pre-calculated sine tables.

Uses <time.h> so will probably not compile on windows.

Some code borrowed from a post on gamedev.net (find link).

Build:
$ g++ -lrt -o csin csin.c

Run:
$ ./csin
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SIN_TABLE_SIZE 2048
#define SIN_TABLE_BIT_MASK 2047
#define pi 3.141952f

float step;
float invstep = 162.97466;
float sinvals[SIN_TABLE_SIZE];

float fast_sin(float x);
float sint(double a);

void setupsintable();
void dosintabletest();
void dosinftest();
void dofastsintest();


int main()
{
  step = pi * 2.0f / (float)SIN_TABLE_SIZE;
  invstep = 1.0f / step;

  printf("%3.6f\n", step);

  setupsintable();

  dosintabletest();
  dosinftest();
  dofastsintest();

  printf("%3.6f, %3.6f\n", sint(0), sinf(0));
  printf("%3.6f, %3.6f\n", sint(0.25 * pi), sinf(0.25 * pi));
  printf("%3.6f, %3.6f\n", sint(0.5 * pi), sinf(0.5 * pi));
  printf("%3.6f, %3.6f\n", sint(0.75 * pi), sinf(0.75 * pi));
  printf("%3.6f, %3.6f\n", sint(1.0 * pi), sinf(1.0 * pi));

  printf("%3.6f, %3.6f\n", sint(1.25 * pi), sinf(1.25 * pi));
  printf("%3.6f, %3.6f\n", sint(1.5 * pi), sinf(1.5 * pi));
  printf("%3.6f, %3.6f\n", sint(1.75 * pi), sinf(1.75 * pi));
  printf("%3.6f, %3.6f\n", sint(2.0 * pi), sinf(2.0 * pi));

  printf("%3.6f, %3.6f\n", sint(2.25 * pi), sinf(2.25 * pi));
  printf("%3.6f, %3.6f\n", sint(2.5 * pi), sinf(2.5 * pi));
  printf("%3.6f, %3.6f\n", sint(2.75 * pi), sinf(2.75 * pi));
  printf("%3.6f, %3.6f\n", sint(3.0 * pi), sinf(3.0 * pi));

  printf("%3.6f, %3.6f\n", sint(3.25 * pi), sinf(3.25 * pi));
  printf("%3.6f, %3.6f\n", sint(3.5 * pi), sinf(3.5 * pi));
  printf("%3.6f, %3.6f\n", sint(3.75 * pi), sinf(3.75 * pi));
  printf("%3.6f, %3.6f\n", sint(4.0 * pi), sinf(4.0 * pi));

  return 0;
}


void dosintabletest()
{
  unsigned i, j;
  int n = 1000;
  timespec time3, time4;

  clock_gettime(CLOCK_REALTIME, &time3);

  float sum = 3.1;
  for (i = 0; i < n; i++) {
    for (j = 0; j < 1000; j++) {
      sum += sint(sum);
      sum += sint(sum);
      sum += sint(sum);
      sum += sint(sum);
    }
  }
  clock_gettime(CLOCK_REALTIME, &time4);

  double dt3 = (time4.tv_nsec - time3.tv_nsec) / (n * 4000.0);
  printf("Sin table: %.3f ns / sin; (check:%.9f)\n", dt3, sum);

}


void dosinftest()
{
  unsigned i, j;
  int n = 1000;

  timespec time3, time4;

  clock_gettime(CLOCK_REALTIME, &time3);

   float sum = 3.1;
  for (i = 0; i < n; i++) {
    for (j = 0; j < 1000; j++) {
      sum += sinf(sum);
      sum += sinf(sum);
      sum += sinf(sum);
      sum += sinf(sum);
    }
  }

  clock_gettime(CLOCK_REALTIME, &time4);

  double dt3 = (time4.tv_nsec - time3.tv_nsec) / (n * 4000.0);

  printf("sinf: %.3f ns / sin; (check:%.9f)\n", dt3, sum);

}


void dofastsintest()
{
  unsigned i, j;
  int n = 1000;

  timespec time3, time4;

  clock_gettime(CLOCK_REALTIME, &time3);

   float sum = 3.1;
  for (i = 0; i < n; i++) {
    for (j = 0; j < 1000; j++) {
      sum += fast_sin(sum);
      sum += fast_sin(sum);
      sum += fast_sin(sum);
      sum += fast_sin(sum);
    }
  }

  clock_gettime(CLOCK_REALTIME, &time4);

  double dt3 = (time4.tv_nsec - time3.tv_nsec) / (n * 4000.0);

  printf("fast_sin: %.3f ns / sin; (check:%.9f)\n", dt3, sum);

}


void setupsintable()
{
  unsigned i;

  float f = 0;

  for (i = 0; i <= SIN_TABLE_SIZE; i++)
    {
      sinvals[i] = sinf(f);
      f += step;
    }
}


float fast_sin(float x)
{
    const float B = 4/pi;
    const float C = -4/(pi*pi);

    float y = B * x + C * x * abs(x);

#ifdef EXTRA_PRECISION
    const float Q = 0.775;
    const float P = 0.225;

    y = P * (y * abs(y) - y) + y; // Q * y + P * y * abs(y)
#endif
        return y;
}


float sint(double a)
{
  return sinvals[((int)(a * invstep)) & SIN_TABLE_BIT_MASK];
}
