#include <cstdio>
#include <cmath>

typedef float v2float __attribute__((vector_size (8)));


int main() {
  float a = 100.0f;
  float b = 1000.0f;
  float* sum = (float*) malloc(sizeof(float) * 50);
  for (int i = 0, e = 1000000000; i < e; i++)
  {
    sum[i % 50] = a + b + i;
  }
  printf("sum %f\n", sum[0]);
  float res = sum[0];
  free(sum);
  return res;
}