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
    
    printf("World size = %d\n", world_size);
    if (argc != 4){
      printf("Not sufficient arguments");
      exit(EXIT_FAILURE);
    }
    double begin = atof(argv[1]);
    double end = atof(argv[2]);
    int number_of_points = atoi(argv[3]);
    int number_of_regions = number_of_points - 1;
    int slave_batch = number_of_points/world_size;
    int master_batch = number_of_points % world_size;
    double step = (end - begin)/world_size; 
    printf("slave batch = %d\n", slave_batch);
    printf("master batch = %d\n", master_batch);
    printf("step = %g\n",step);
    for(int i = 1; i < world_size; i++){
      // send to i their begin and end and num points
      double b,e;
      int p;
      b = begin + (i -1) * step;
      e = begin + i * step;
      p = slave_batch + 1;
      printf("b = %g e = %g p = %d\n", b,e,p);
    }
    double m_b = begin + (world_size - 1) * step;
    double m_e = end;
    int m_p = master_batch + 1;
    printf("m_b = %g m_e = %g m_p = %d", m_b, m_e, m_p);
  } else {
    printf("I'm slave nr %d\n", rank);
  }

  MPI_Finalize();
  return 0;
}