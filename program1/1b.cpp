#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char ** argv) {
  MPI_Init(&argc, &argv);
  int rank, num_procs;
  int rounds = 100000;
  int count = 0, accumulate = 0, total = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  double x, y, z;
  double start_time, end_time;
  if (rank == 0) {
    start_time = MPI_Wtime();
  }
  srand(time(nullptr) ^ rank);
  for (int i = 0; i < rounds; ++i) {
    x = (double) rand() / RAND_MAX;
    y = (double) rand() / RAND_MAX;
    z = x * x + y * y;
    if (z <= 1.0) {
      ++count;
    }
  }
  MPI_Reduce(&count, &accumulate, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&rounds, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    double PI = 4.0 * accumulate / total;
    end_time = MPI_Wtime();
    cout << "Estimated value of PI = " << PI << '\n';
    cout << "Actual value of PI = " << M_PI << '\n';
    cout << "Absolute error = " << fabs(PI - M_PI) << '\n';
    cout << "Time taken = " << end_time - start_time << "s.\n";
  }
  MPI_Finalize();
  return 0;
}

