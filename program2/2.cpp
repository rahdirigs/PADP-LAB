#include <cstdio>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./2 <input_size> <num_threads>\n");
    exit(0);
  }
  int size = atoi(argv[1]);
  int num_threads = atoi(argv[2]);
  int **a = (int **) malloc(size * sizeof(int *));
  int **b = (int **) malloc(size * sizeof(int *));
  int **c = (int **) malloc(size * sizeof(int *));
  int i, j, k, count = -1;
  for (i = 0; i < size; ++i) {
    a[i] = (int *) malloc(size * sizeof(int));
    b[i] = (int *) malloc(size * sizeof(int));
    c[i] = (int *) malloc(size * sizeof(int));
    for (j = 0; j < size; ++j) {
      a[i][j] = ++count;
      b[i][j] = ++count;
    }
  }
  double start_time = omp_get_wtime();
  omp_set_num_threads(num_threads);
#pragma omp parallel for private(j, k)
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      for (k = 0; k < size; ++k) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  double end_time = omp_get_wtime();
  cout << "Time taken = " << end_time - start_time << "s.\n";
  return 0;
}

