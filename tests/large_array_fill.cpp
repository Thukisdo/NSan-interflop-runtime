#include <cstdio>
#include <cmath>

int main() {
  long double a = 100.0f;
  long double b = 1000.0f;
  long double* sum = (long double*) malloc(sizeof(long double) * 50);
  for (int i = 0, e = 1000000000; i < e; i++)
  {
    sum[i % 50] = a + b + i;
  }
  printf("sum %f\n", sum[0]);
  long double res = sum[0];
  free(sum);
  return res;
}