#include <cstdio>
#include <cmath>

typedef float v2float __attribute__((vector_size (8)));

__attribute_noinline__ 
v2float f(v2float a, v2float b) {
  return a * b;
}

/* int main() {

  v2float a = {5, 10};
  v2float b = {2, 2};

  for (int i = 0; i < 500000; i++)
  {
    a = f(a, b);
  }


  return a[0];
}
 */
int main() {
  float a = 100;
  float b = 1000;
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

/* int main() {
  float a = 5;
  float b = 5;
  return a
}
 */
/* #define xstr(s) str(s)
#define str(s) #s
#define FN Square

static float Square(float x) {
  return x * x;
}

static float Cube(float x) {
  return x * x * x;
}

static float Inverse(float x) {
  return 1.0f / x;
}

__attribute__((noinline))  // To check call stack reporting.
float ComputeDerivative(float(*fn)(float), float a, float da) {
  return (fn(a + da) - fn(a)) / da;
  // CHECK: WARNING: NumericalStabilitySanitizer: inconsistent shadow results while checking return
  // CHECK: float {{ *}}precision (native):
  // CHECK: float{{ *}}precision (shadow):
  // CHECK: {{#0 .*in ComputeDerivative}}
}

int main() {
  for (int i = 7; i < 31; ++i) {
    float step = 1.0f / (1ull << i);
    ComputeDerivative(&FN, 0.1f, step);
    //printf("%s derivative: %.8f\n", xstr(FN), ComputeDerivative(&FN, 0.1f, step));
  }
  return 0;
} */

/* 
#include <cmath>
#include <cstdio>
#define IMPL Naive

// This never loses more than 1/2 of the digits.
static double Naive(const double X) __attribute__((noinline)) {
  double Y, Z;
  Y = X - 1.0;
  Z = exp(Y);
  if (Z != 1.0)
    Z = Y / (Z - 1.0);
  return Z;
}

static double Better1(const double X) __attribute__((noinline)) {
  long double Y, Z;
  Y = X - 1.0;
  Z = exp(Y);
  if (Z != 1.0)
    Z = Y / (Z - 1.0);
  return Z;
}

// This is precise to a a few ulps.
static double Better2(const double X) __attribute__((noinline)) {
  double Y, Z;
  Y = X - 1.0;
  Z = exp(Y);
  if (Z != 1.0)
    Z = log(Z) / (Z - 1.0);
  return Z;
}

int main() {
  for (int i = 7; i < 31; ++i) {
    const double x = 1.0 + 1.0 / (1ull << i);
    printf("value at %.16f:\n", x);
    printf("    %.16f\n", IMPL(x));
  }
  return 0;
} */



/* float random1() {
  static int nrand = 23;
  nrand = (nrand * 5363 + 143) % 1387;
  return 2.0 * nrand / 1387.0 - 1.0;
}

int main() {
  constexpr const float kEpsilon = 1e-4;
  constexpr const int kNDims = 20;
  constexpr const int kNIters = 1000;

  float a[kNDims][kNDims];
  float b[kNDims];
  float x[kNDims];
  float y[kNDims];
  const float xsol[kNDims] = {
      1.7,    -4746.89, 50.23, -245.32,  4778.29,  -75.73,  3495.43,
      4.35,   452.98,   -2.76, 8239.24,  3.46,     1000.0,  -5.0,
      3642.4, 735.36,   1.7,   -2349.17, -8247.52, 9843.57,
  };

  for (int i = 0; i < kNDims; ++i) {
    for (int j = 0; j < kNDims; ++j) {
      a[i][j] = random1();
    }
    a[i][i] = a[i][i] + 4.9213648f;
  }

  for (int i = 0; i < kNDims; ++i) {
    float aux = 0.0f;
    for (int j = 0; j < kNDims; ++j) {
      aux = aux + a[i][j]*xsol[j];
    }
    b[i] = aux;
    y[i] = 10.0f;
  }

  int iter = 0;
  for (iter = 0; iter < kNIters; ++iter) {
    float anorm = 0.0f;
    for (int j = 0; j < kNDims; ++j) {
      x[j] = y[j];
    }
    for (int j = 0; j < kNDims; ++j) {
      float aux = b[j];
      for (int k = 0; k < kNDims; ++k) {
        if (k != j) {
          aux = aux - a[j][k]*x[k];
        }
      }
// REL: WARNING: NumericalStabilitySanitizer: inconsistent shadow
// Note: We are not checking the line because nsan detects the issue at the
// `y[j]=` store location in dbg mode, and at the `abs()` location in release
// because the store is optimized out.
      y[j] = aux / a[j][j];

// STOP: WARNING: NumericalStabilitySanitizer: floating-point comparison results depend on precision
// STOP: #0{{.*}}in main{{.*}}cadna_ex7.cc:[[@LINE+1]]
      if (fabsf(x[j]-y[j]) > anorm) {
        anorm = fabsf(x[j]-y[j]);
      }
    }
    printf("iter = %i\n", iter);
// STOP: WARNING: NumericalStabilitySanitizer: floating-point comparison results depend on precision
// STOP: #0{{.*}}in main{{.*}}cadna_ex7.cc:[[@LINE+1]]
    if (anorm < kEpsilon) break;
  }

  printf("niter = %i\n", iter);
  for (int i = 0; i < kNDims; ++i) {
    float aux = -b[i];
    for (int j = 0; j < kNDims; ++j) {
      aux = aux + a[i][j]*y[j];
    }
    printf("x_sol(%2i) = %15.7f (true value : %15.7f), residue(%2i) = %15.7f\n",
           i, y[i], xsol[i], i, aux);
  }

  return 0;

} */