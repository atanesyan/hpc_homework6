#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000
#define NUM_VALS 256

void print_vals(int* arr, const char* prefix) {
  printf("Printing %s { ", prefix);
  for (int i = 0; i < NUM_VALS; ++i) {
    printf("%d ", arr[i]);
  }
  printf("}\n");
}

int main() {
  int* A = (int*)malloc(N * sizeof(int));
  int hist_naive[NUM_VALS] = { 0 };
  int hist_critical[NUM_VALS] = { 0 };
  int hist_reduction[NUM_VALS] = { 0 };

  for (int i = 0; i < N; ++i) {
    A[i] = rand() % NUM_VALS;
  }

  // 1. Naive parallel version (Race conditions expected)
  #pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    ++hist_naive[A[i]];
  }

  // 2. Version with critical
  #pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    #pragma omp critical
    {
      ++hist_critical[A[i]];
    }
  }

  // 3. Version using reduction
  #pragma omp parallel for reduction(+:hist_reduction[:256])
  for (int i = 0; i < N; ++i) {
    ++hist_reduction[A[i]];
  }

  print_vals(hist_naive, "Naive");
  print_vals(hist_critical, "Critical");
  print_vals(hist_reduction, "Reduction");

  free(A);
  return 0;
}
