#include <iostream>
#include <limits>
#include <stdint.h>

typedef struct
{
    double val;
    uint64_t padding[1];
} shadow128_t;

typedef struct
{
    __float128 val;
    uint64_t padding[2];
} shadow256_t;

namespace
{

    constexpr int Tolerance = 1;
    constexpr bool Warn = true;

    template <typename FTy>
    struct IEEE
    {
    };

    template <>
    struct
#ifdef __MINGW32__
        /* On mingw targets the ms-bitfields option is active by default.
         Therefore enforce gnu-bitfield style.  */
        __attribute__((gcc_struct))
#endif
        IEEE<float>
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        uint32_t sign : 1;
        uint32_t exponent : 8;
        uint32_t mantissa : 23;
#else
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
#endif
    };

    template <>
    struct
#ifdef __MINGW32__
        /* On mingw targets the ms-bitfields option is active by default.
         Therefore enforce gnu-bitfield style.  */
        __attribute__((gcc_struct))
#endif
        IEEE<double>
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        uint64_t sign : 1;
        uint64_t exponent : 11;
        uint64_t mantissa : 52;
#else
        uint64_t mantissa : 52;
        uint64_t exponent : 11;
        uint64_t sign : 1;
#endif
    };

    template <>
    struct
#ifdef __MINGW32__
        /* On mingw targets the ms-bitfields option is active by default.
         Therefore enforce gnu-bitfield style.  */
        __attribute__((gcc_struct))
#endif
        IEEE<__float128>
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        unsigned sign : 1;
        unsigned exponent : 15;
        uint64_t mant_high : 48;
        uint64_t mant_low : 64
#else
        uint64_t mant_low : 64;
        uint64_t mant_high : 48;
        unsigned exponent : 15;
        unsigned sign : 1;
#endif
    };

    template <typename FTy>
    class Binary
    {
    public:
        Binary(FTy val) : val(val)
        {
        }

        IEEE<FTy> &getIEEE()
        {
            return Ieee;
        }

        FTy &getVal()
        {
            return val;
        }

    private:
        union
        {
            FTy val;
            IEEE<FTy> Ieee;
        };
    };

    template <typename FTy>
    struct FLTConfig
    {
    };

    template <>
    struct FLTConfig<float>
    {
        static const uint64_t ExpComp = 127;
    };

    template <>
    struct FLTConfig<double>
    {
        static const uint64_t ExpComp = 1023;
    };

    template <>
    struct FLTConfig<__float128>
    {
        static const uint64_t ExpComp = 16383;
    };

    template <typename FTy>
    int getExpFlt(FTy const val)
    {
        return Binary<FTy>{val}.getIEEE().exponent - FLTConfig<FTy>::ExpComp;
    }
    double fast_pow2_binary64(int const exp)
    {
        Binary<double> b64{0.0};
        b64.getIEEE().exponent = exp + FLTConfig<double>::ExpComp;
        return b64.getVal();
    }

    double noiseBinary64(int const exp)
    {
        const double DRand = ((static_cast<double>(rand()) / std::numeric_limits<double>::max()) - 0.5);
        return fast_pow2_binary64(exp) * DRand;
    }

    template <typename STy>
    void cancell(STy const &a, STy const &b, STy &res)
    {
        const int32_t E_z = getExpFlt(res);
        /* computes the difference between the max of both operands and the       
        * exponent of the result to find the size of the cancellation */
        int Cancellation = std::max(getExpFlt(a), getExpFlt(b)) - E_z;
        if (Cancellation >= Tolerance)
        {
            if (Warn)
                std::cout << "Cancellation of size " << Cancellation << " detected\n";
            /* Add an MCA noise of the magnitude of cancelled bits.                  
            * This particular version in the case of cancellations does not use     
            * extended quad types */
            const int32_t E_n = E_z - (Cancellation - 1);
            //res += noiseBinary64(E_n);
        }
    }

}

extern "C" float __interflop_float_add(float a, shadow128_t const *sa, float b, shadow128_t const *sb, shadow128_t *res)
{
    res->val = sa->val + sb->val;
    float ret = a + b;
    cancell(a, b, ret);
    return ret;
}

extern "C" float __interflop_float_sub(float a, shadow128_t const *sa, float b, shadow128_t const *sb, shadow128_t *res)
{
    res->val = sa->val - sb->val;
    float ret = a - b;
    cancell(a, b, ret);
    return ret;
}

extern "C" float __interflop_float_mul(float a, shadow128_t const *sa, float b, shadow128_t const *sb, shadow128_t *res)
{
    res->val = sa->val * sb->val;
    return a * b;
}

extern "C" float __interflop_float_div(float a, shadow128_t *sa, float b, shadow128_t *sb, shadow128_t *res)
{
    res->val = sa->val / sb->val;
    return a / b;
}

extern "C" double __interflop_double_add(double a, shadow256_t const *sa, double b, shadow256_t const *sb, shadow256_t *res)
{
    res->val = sa->val + sb->val;
    double ret = a + b;
    cancell(a, b, ret);
    return ret;
}

extern "C" double __interflop_double_sub(double a, shadow256_t const *sa, double b, shadow256_t const *sb, shadow256_t *res)
{
    res->val = sa->val - sb->val;
    double ret = a - b;
    cancell(a, b, ret);
    return ret;
}

extern "C" double __interflop_double_mul(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val * sb->val;
    return a * b;
}
extern "C" double __interflop_double_div(double a, shadow256_t *sa, double b, shadow256_t *sb, shadow256_t *res)
{
    res->val = sa->val / sb->val;
    return a / b;
}

template <typename V, typename S>
int value_check(V a, S *sa)
{
    double AbsoluteErrror = a - sa->val;
    double RelativeError = AbsoluteErrror / sa->val * 100;
    if (AbsoluteErrror < 0)
        AbsoluteErrror *= -1;
    if (RelativeError < 0)
        RelativeError *= -1;

    if (RelativeError >= 0.0001)
    {
        printf("Inconsistent shadow result : expected %.20lf, value is %.20f\n", (double) sa->val, a);
        printf("\tValue (%f)\n", a);
        printf("\tSHADOW Value (%lf)\n", (double) sa->val);
        printf("\tRelative error %.20f%%\n", RelativeError);
        printf("\tAbsolute error %.20f\n", AbsoluteErrror);
        exit(1);
        return 1;
    }
    return 0;
}

extern "C" int __interflop_float_check(float a, shadow128_t *sa)
{
    return value_check(a, sa);
}

extern "C" int __interflop_double_check(double a, shadow256_t *sa)
{
    return value_check(a, sa);
}

extern "C" void __interflop_float_make_shadow(float a, shadow128_t *res)
{
    res->val = a;
}

extern "C" void __interflop_double_make_shadow(double a, shadow256_t *res)
{
    res->val = a;
}

extern "C" void __interflop_float_double_cast(float a, shadow128_t *sa, shadow256_t *res)
{
    res->val = sa->val;
}

extern "C" void __interflop_double_float_cast(double a, shadow256_t *sa, shadow128_t *res)
{
    res->val = sa->val;
}

extern "C" float __interflop_float_neg(float a, shadow128_t *sa, shadow128_t *res)
{
    res->val = -sa->val;
    return -a;
}

extern "C" double __interflop_double_neg(double a, shadow256_t *sa, shadow256_t *res)
{
    res->val = -sa->val;
    return -a;
}


extern "C" void __interflop_float_fcmp_gt(double a, shadow128_t *sa, double b, shadow128_t *sb)
{
    if ((a > b) != (sa->val > sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%f)\n", a);
        printf("\tSHADOW Value (%lf)\n", (double) sa->val);
        exit(1);
    }
}

extern "C" void __interflop_float_fcmp_lt(double a, shadow128_t *sa, double b, shadow128_t *sb)
{
    if ((a < b) != (sa->val < sb->val))
    {
        printf("floating-point comparison results depend on precision\n");
        printf("\tValue (%f)\n", a);
        printf("\tSHADOW Value (%lf)\n", (double) sa->val);
        exit(1);
    }
}
