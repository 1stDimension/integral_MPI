#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "integrals.h"

int main(int argc, char ** argv){

  // double (*func_ptr)(double) = &linear;
  // double begin = 0, end = 100;
  // double result = integrate(func_ptr, 0,100, 5);
  // printf("begin is %g end is %g \n", begin , end);
  // printf("f(begin) is %g f(end) is %g \n", (*func_ptr)(begin) , (*func_ptr)(end));
  // printf("Integral is %g\n", result);

  MPI_Init(&argc, &argv);

  int world_size, rank;
  //How many processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //Position inside processes
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0){
    printf("I'm master\n");
    if (argc != 4){
      printf("Not sufficient arguments");
      exit(EXIT_FAILURE);
    }
    double begin = atof(argv[1]);
    double end = atof(argv[2]);
    int number_of_points = atoi(argv[3]);
    // int slave_batch = ;
  } else {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("I'm slave nr %d\n", my_rank);
  }

  MPI_Finalize();
  printf("MPI Environment end\n");
  return 0;
}