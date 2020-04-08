#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double *begin_end_array;
  int *number_of_points_array;
  if (rank == MASTER_ID)
  {

    if (argc != 4)
    {
      printf("Not sufficient arguments");
      exit(EXIT_FAILURE);
    }
    double begin = atof(argv[1]);
    double end = atof(argv[2]);
    int number_of_points = atoi(argv[3]);

    double step;
    int calculate_world = world_size;
    begin_end_array = malloc(2 * world_size * sizeof(*begin_end_array));
    number_of_points_array = malloc(world_size * sizeof(*number_of_points_array));

    if (number_of_points == world_size)
    {
      step = (end - begin) / (world_size - 1);
    }
    else if (number_of_points > world_size)
    {
      step = (end - begin) / world_size;
    }
    else if (number_of_points < world_size)
    {
      calculate_world = number_of_points;
      step = (end - begin) / (calculate_world - 1);
    }
    int slave_batch = number_of_points / calculate_world;
    int last_point_number = 1;

    for (int i = 1; i < calculate_world; i++)
    {
      // send to i their begin and end and num points
      last_point_number += slave_batch;
      begin_end_array[2*i] = begin + (i - 1) * step;
      begin_end_array[2*i + 1] = begin + i * step;
      number_of_points_array[i] = slave_batch + 1;
    }
    memset(number_of_points_array + calculate_world, 0, world_size - calculate_world);
    for (int i = calculate_world; i < world_size; i++)
    {
      begin_end_array[2*i] = 0.0;
      begin_end_array[2*i + 1] = 0.0;
    }
    begin_end_array[0] = begin + (calculate_world - 1) * step;
    begin_end_array[1] = end;
    number_of_points_array[0] = (number_of_points - last_point_number + 1);

  }
  double *begin_end = malloc(2 * sizeof(begin_end));
  int number_points;
  MPI_Scatter(begin_end_array, 2, MPI_DOUBLE, begin_end, 2, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
  MPI_Scatter(number_of_points_array, 1, MPI_INT, &number_points, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

  double partial_integral = integrate(func_ptr, begin_end[0], begin_end[1], number_points);
  double integral;
  MPI_Reduce(&partial_integral,&integral, 1, MPI_DOUBLE, MPI_SUM, MASTER_ID, MPI_COMM_WORLD);
  if(rank == MASTER_ID){
    printf("Integral is %g\n", integral);
  }
  MPI_Finalize();
  return 0;
}