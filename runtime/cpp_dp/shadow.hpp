#pragma once 

#include <iostream>

typedef struct {
    double val;
    uint64_t padding[1];
} shadow128_t;

typedef struct {
    __float128 val;
    uint64_t padding[2];
} shadow256_t;

typedef struct {
    shadow128_t* elems[2];
} v2shadow128_t;

typedef struct {
    shadow128_t* elems[4];
} v4shadow128_t;

typedef struct {
    shadow128_t* elems[8];
} v8shadow128_t;

typedef struct {
    shadow256_t* elems[2];
} v2shadow256_t;

typedef struct {
    shadow256_t* elems[4];
} v4shadow256_t;

typedef struct {
    shadow256_t* elems[8];
} v8shadow256_t;

typedef float v2float __attribute__((vector_size (8)));
typedef float v4float __attribute__((vector_size (16)));
typedef float v8float __attribute__((vector_size (32)));

typedef double v2double __attribute__((vector_size (16)));
typedef double v4double __attribute__((vector_size (32)));
typedef double v8double __attribute__((vector_size (64)));

struct float_shadow {
    using type = shadow128_t;
};

template<typename>
struct shadow_type {
    
};

template<>
struct shadow_type<float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t vector_size = 1;
};

template<>
struct shadow_type<v2float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t vector_size = 2;
};

template<>
struct shadow_type<v4float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t vector_size = 4;
};

template<>
struct shadow_type<double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t vector_size = 1;
};

template<>
struct shadow_type<v2double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t vector_size = 2;
};

template<>
struct shadow_type<v4double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t vector_size = 4;
};

template<>
struct shadow_type<v8double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t vector_size = 8;
};

