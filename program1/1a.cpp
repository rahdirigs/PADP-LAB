#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./1a <rounds> <num_threads>\n");
    exit(0);
  }
  int rounds = atoi(argv[1]);
  int num_threads = atoi(argv[2]);
  int i, j, count = 0;
  double x, y, z;
  srand(time(nullptr));
  double start_time = omp_get_wtime();
  omp_set_num_threads(num_threads);
#pragma omp parallel for private(j, x, y, z) reduction(+ : count)
  for (i = 0; i < rounds; ++i) {
    for (j = 0; j < rounds; ++j) {
      x = (double) rand() / RAND_MAX;
      y = (double) rand() / RAND_MAX;
      z = x * x + y * y;
      if (z <= 1.0) {
        ++count;
      }
    }
  }
  double PI = 4.0 * count / (rounds * rounds);
  double end_time = omp_get_wtime();
  cout << "Estimated value of PI = " << PI << '\n';
  cout << "Actual value of PI = " << M_PI << '\n';
  cout << "Absolute error = " << fabs(PI - M_PI) << '\n';
  cout << "Time taken = " << end_time - start_time << "s.\n";
  return 0;
}

