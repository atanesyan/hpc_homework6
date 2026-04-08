#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <time.h>

#define N 100000000

int main() {
  srand(time(NULL));
  double* A = (double*)malloc(N * sizeof(double));
  if (A == NULL) {
    perror("Couldn't allocate memory for the array\n");
    exit(1);
  }

  for (int i = 0; i < N; ++i) {
    A[i] = (double)rand() / RAND_MAX;
  }

  double max_val = -DBL_MAX;

  #pragma omp parallel for reduction(max: max_val)
  for (size_t i = 0; i < N; ++i) {
    max_val = A[i] > max_val ? A[i] : max_val;
  }

  double T = 0.8 * max_val;
  double sum = 0.0;

  #pragma omp parallel for reduction(+: sum)
  for (int i = 0; i < N; ++i) {
    if (A[i] > T) {
      sum += A[i];
    }
  }

  printf("Max: %f, Threshold: %f, Filtered Sum: %f\n", max_val, T, sum);
  free(A);
  return 0;
}
