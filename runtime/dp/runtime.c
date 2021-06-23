#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "shadow.h"

float __interflop_float_add(float a, shadow128_t *const sa, float b, shadow128_t *const sb, shadow128_t *res)
{
    res->val = sa->val + sb->val;
    return a + b;
}

float __interflop_float_sub(float a, shadow128_t *sa, float b, shadow128_t *sb, shadow128_t *res)
{
    res->val = sa->val - sb->val;
    return a - b;
}

float __interflop_float_mul(float a, shadow128_t *sa, float b, shadow128_t *sb, shadow128_t *res)
{
    res->val = sa->val * sb->val;
    return a * b;
}

float __interflop_float_div(float a, shadow128_t *sa, float b, shadow128_t *sb, shadow128_t *res)
{
    res->val = sa->val / sb->val;
    return a / b;
}

double __interflop_double_add(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val + sb->val;
    return a + b;
}

double __interflop_double_sub(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val - sb->val;
    return a - b;
}

double __interflop_double_mul(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val * sb->val;
    return a * b;
}

double __interflop_double_div(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val / sb->val;
    return a / b;
}

int __interflop_float_check(float a, shadow128_t *sa)
{
    double AbsoluteErrror = a - sa->val;
    double RelativeError = AbsoluteErrror / sa->val * 100;
    if (AbsoluteErrror < 0)
        AbsoluteErrror *= -1;
    if (RelativeError < 0)
        RelativeError *= -1;

    if (RelativeError >= 0.0001)
    {
        printf("Inconsistent shadow result : expected %.20lf, value is %.20f\n", sa->val, a);
        printf("\tFLOAT Value (%f)\n", a);
        printf("\tSHADOW DOUBLE Value (%lf)\n", sa->val);
        printf("\tRelative error %.20f%%\n", RelativeError);
        printf("\tAbsolute error %.20f\n", AbsoluteErrror);
        exit(1);
        return 1;
    }
    return 0;
}

int __interflop_double_check(double a, shadow256_t *sa)
{
    double AbsoluteErrror = a - sa->val;
    double RelativeError = (AbsoluteErrror / sa->val) * 100;
    if (AbsoluteErrror < 0)
        AbsoluteErrror *= -1;
    if (RelativeError < 0)
        RelativeError *= -1;

    if (RelativeError >= 0.0001)
    {
        printf("Inconsistent shadow result : expected %.20lf, value is %.20f\n", (double)sa->val, a);
        printf("\tDOUBLE Value (%f)\n", a);
        printf("\tSHADOW DOUBLE Value (%lf)\n", (double)sa->val);
        printf("\tAbsolute error %.20f%%\n", AbsoluteErrror);
        printf("\tRelative error %.20f\n", RelativeError);
        exit(1);
        return 1;
    }
    return 0;
}

void __interflop_float_make_shadow(float a, shadow128_t *res)
{
    res->val = a;
}

void __interflop_double_make_shadow(double a, shadow256_t *res)
{
    res->val = a;
}

void __interflop_float_double_cast(float a, shadow128_t *sa, shadow256_t *res)
{
    res->val = sa->val;
}

void __interflop_double_float_cast(double a, shadow256_t *sa, shadow128_t *res)
{
    res->val = sa->val;
}

float __interflop_float_neg(float a, shadow128_t *sa, shadow128_t *res)
{
    res->val = -sa->val;
    return -a;
}

double __interflop_double_neg(float a, shadow256_t *sa, shadow256_t *res)
{
    res->val = -sa->val;
    return -a;
}

void __interflop_float_fcmp_eq(float a, shadow128_t *sa, float b, shadow128_t *sb)
{
    if ((a == b) != (sa->val == sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_float_fcmp_neq(float a, shadow128_t *sa, float b, shadow128_t *sb)
{
    if ((a != b) != (sa->val != sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_float_fcmp_gt(float a, shadow128_t *sa, float b, shadow128_t *sb)
{
    if ((a > b) != (sa->val > sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_float_fcmp_lt(float a, shadow128_t *sa, float b, shadow128_t *sb)
{
    if ((a < b) != (sa->val < sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_double_fcmp_eq(double a, shadow256_t *sa, double b, shadow256_t *sb)
{
    if ((a == b) != (sa->val == sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_double_fcmp_gt(double a, shadow256_t *sa, double b, shadow256_t *sb)
{
    if ((a > b) != (sa->val > sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}

void __interflop_double_fcmp_lt(double a, shadow256_t *sa, double b, shadow256_t *sb)
{
    printf("double : %.20lf %.20lf\n", a, b);
    printf("__float128 : %.20lf %.20lf\n\n", (double) sa->val, (double) sb->val);
    if ((a < b) != (sa->val < sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%.20f)\n", a);
        printf("\tSHADOW Value (%.20lf)\n", (double) sa->val);
        exit(1);
    }
}
