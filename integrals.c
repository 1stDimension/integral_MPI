#include "integrals.h"
#include <stdlib.h>
#include <stdio.h>

double integrate(double (*func)(double), double begin, double end, int num_points){
  printf("Not implemented");
  return 0;
}

double linear(double x){
  return x;
}

double parabole(double x){
  return x*x;
}