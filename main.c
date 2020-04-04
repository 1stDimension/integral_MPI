#include <stdio.h>
#include <stdlib.h>

#include "integrals.h"

int main(int argc, char ** argv){

  
  double result = integrate(linear, 0,100, 5);

  printf("Integral is %g\n", result);

  return 0;
}