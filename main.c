#include <stdio.h>
#include <stdlib.h>

#include "integrals.h"

int main(int argc, char ** argv){

  double (*func_ptr)(double) = &linear;
  double begin = 0, end = 100;
  double result = integrate(func_ptr, 0,100, 5);
  printf("begin is %g end is %g \n", begin , end);
  printf("f(begin) is %g f(end) is %g \n", (*func_ptr)(begin) , (*func_ptr)(end));
  printf("Integral is %g\n", result);

  return 0;
}