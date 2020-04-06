#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "integrals.h"

#define MASTER_ID 0
#define BEGIN 0
#define END 1
#define NUM_POINTS 2
#define RETURN_RESULTS 3

int main(int argc, char **argv)
{

  double (*func_ptr)(double) = &linear;

  MPI_Init(&argc, &argv);

  int world_size, rank;
  //How many processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  //Position inside processes
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == MASTER_ID)
  {
    printf("I'm master\n");

    printf("World size = %d\n", world_size);
    if (argc != 4)
    {
      printf("Not sufficient arguments");
      exit(EXIT_FAILURE);
    }
    double begin = atof(argv[1]);
    double end = atof(argv[2]);
    int number_of_points = atoi(argv[3]);

    int slave_batch = number_of_points / world_size;
    double step = (end - begin) / world_size;

    if (number_of_points == world_size)
    {
      step = (end - begin) / (world_size - 1);
    }
#ifdef
    printf("slave batch = %d\n", slave_batch);
    printf("step = %g\n", step);
#endif
    int last_point_number = 1;
    for (int i = 1; i < world_size; i++)
    {
      // send to i their begin and end and num points
      double b, e;
      int p;
      b = begin + (i - 1) * step;
      e = begin + i * step;
      p = slave_batch + 1;
      last_point_number += slave_batch;
#ifdef DEBUG
      printf("b = %g e = %g p = %d\n", b, e, p);
#endif
      MPI_Send(&b, 1, MPI_DOUBLE, i, BEGIN, MPI_COMM_WORLD);
      MPI_Send(&e, 1, MPI_DOUBLE, i, END, MPI_COMM_WORLD);
      MPI_Send(&p, 1, MPI_INT, i, NUM_POINTS, MPI_COMM_WORLD);
    }
    double m_b = begin + (world_size - 1) * step;
    double m_e = end;
    int m_p = (number_of_points - last_point_number + 1);
    double partial_integral = 0.0;
    if (m_p > 1)
    {
      partial_integral = integrate(func_ptr, m_b, m_e, m_p);
    }
#ifdef DEBUG
    printf("m_b = %g m_e = %g m_p = %d\n", m_b, m_e, m_p);
#endif
    for (int i = 1; i < world_size; i++)
    {
      double tmp;
      MPI_Recv(&tmp, 1, MPI_DOUBLE, i, RETURN_RESULTS, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      partial_integral += tmp;
    }
    printf("Integral is equal to %g\n", partial_integral);
  }
  else
  {
    double begin, end;
    int number_points;
    MPI_Recv(&begin, 1, MPI_DOUBLE, MASTER_ID, BEGIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&end, 1, MPI_DOUBLE, MASTER_ID, END, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&number_points, 1, MPI_INT, MASTER_ID, NUM_POINTS, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
    printf("I'm slave nr %d received begin = %g, end = %g, number_points = %d\n", rank, begin, end, number_points);
#endif
    double partial_Integral = integrate(func_ptr, begin, end, number_points);
    MPI_Send(&partial_Integral, 1, MPI_DOUBLE, MASTER_ID, RETURN_RESULTS, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}