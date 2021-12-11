#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <omp.h>

using namespace std;

double time_taken;

int strike(bool *composite, int i, int stride, int limit) {
  for ( ; i <= limit; i += stride) {
    composite[i] = true;
  }
  return i;
}

int cache_unfriendly_sieve(int n) {
  int count = 0;
  int m = (int) sqrt(n);
  bool *composite = new bool[n + 1];
  memset(composite, 0, n + 1);
  composite[0] = composite[1] = true;
  time_taken = omp_get_wtime();
  for (int i = 2; i <= m; ++i) {
    if (!composite[i]) {
      ++count;
      strike(composite, 2 * i, i, n);
    }
  }
  for (int i = m + 1; i <= n; ++i) {
    if (!composite[i]) {
      ++count;
    }
  }
  time_taken = omp_get_wtime() - time_taken;
  delete[] composite;
  return count;
}

int cache_friendly_sieve(int n) {
  int count = 0;
  int m = (int) sqrt(n);
  bool *composite = new bool[n + 1];
  int *primes = new int[m + 1];
  int *striker = new int[m + 1];
  int p_idx = 0;
  memset(composite, 0, n + 1);
  time_taken = omp_get_wtime();
  for (int i = 2; i <= m; ++i) {
    if (!composite[i]) {
      ++count;
      striker[p_idx] = strike(composite, 2 * i, i, m);
      primes[p_idx] = i;
      ++p_idx;
    }
  }
  for (int window = m + 1; window <= n; window += m) {
    int limit = min(window + m - 1, n);
    for (int i = 0; i < p_idx; ++i) {
      striker[i] = strike(composite, striker[i], primes[i], limit);
    }
    for (int i = window; i <= limit; ++i) {
      if (!composite[i]) {
        ++count;
      }
    }
  }
  time_taken = omp_get_wtime() - time_taken;
  delete[] composite;
  delete[] primes;
  delete[] striker;
  return count;
}

int parallel_sieve(int n) {
  int count = 0;
  int m = (int) sqrt(n);
  int *primes = new int[m + 1];
  int p_idx = 0;
  time_taken = omp_get_wtime();
#pragma omp parallel
  {
    bool *composite = new bool[m + 1];
    int *striker = new int[m + 1];
#pragma omp single
    {
      memset(composite, 0, m + 1);
      for (int i = 2; i <= m; ++i) {
        if (!composite[i]) {
          ++count;
          strike(composite, 2 * i, i, m);
          primes[p_idx] = i;
          ++p_idx;
        }
      }
    }
    int base = -1;
#pragma omp for reduction(+ : count)
    for (int window = m + 1; window <= n; window += m) {
      memset(composite, 0, m + 1);
      if (base == -1) {
        base = window;
        for (int i = 0; i < p_idx; ++i) {
          striker[i] = (base + primes[i] - 1) / primes[i] * primes[i] - base;
        }
      }
      int limit = min(window + m - 1, n) - base;
      for (int i = 0; i < p_idx; ++i) {
        striker[i] = strike(composite, striker[i], primes[i], limit) - m;
      }
      for (int i = 0; i <= limit; ++i) {
        if (!composite[i]) {
          ++count;
        }
      }
      base += m;
    }
    delete[] composite;
    delete[] striker;
  }
  time_taken = omp_get_wtime() - time_taken;
  delete[] primes;
  return count;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./3 <input_size>\n");
    exit(0);
  }
  int n = atoi(argv[1]);
  int count = cache_unfriendly_sieve(n);
  printf("\nCache unfriendly sieve\nPrimes = %d\n", count);
  printf("Time taken = %lf\n", time_taken);
  count = cache_friendly_sieve(n);
  printf("\nCache friendly sieve\nPrimes = %d\n", count);
  printf("Time taken = %lf\n", time_taken);
  count = parallel_sieve(n);
  printf("\nParallel sieve\nPrimes = %d\n", count);
  printf("Time taken = %lf\n", time_taken);
  return 0;
}

