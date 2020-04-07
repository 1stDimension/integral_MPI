#include "integrals.h"
#include <stdlib.h>
#include <stdio.h>

double integrate(double (*func)(double), double begin, double end, int num_points){
  if (num_points < 2){
    // printf("Too few point to calculate\n");
    return 0.0;
  }
  double step = (end - begin) / (double) (num_points - 1);
  double integral  = 0.0;
  for(int i = 0 ; i < num_points -1 ; i++){
     double tmp = (func(begin + i * step) + func(begin + (i+1) * step))/2.0 * step;
     integral += tmp;
  }
  return integral;
}

double linear(double x){
  return x;
}

double parabole(double x){
  return x*x;
}