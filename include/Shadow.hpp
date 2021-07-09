#pragma once 

#include <iostream>
#include "Utils.hpp"

typedef shadow128_t* v2shadow128_t[2];
typedef shadow128_t* v4shadow128_t[4];
typedef shadow128_t* v8shadow128_t[8];

typedef shadow256_t* v2shadow256_t[2];
typedef shadow256_t* v4shadow256_t[4];
typedef shadow256_t* v8shadow256_t[8];


template<typename>
struct shadow_type {
    
};

template<>
struct shadow_type<float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t VectorSize = 1;
};

template<>
struct shadow_type<v2float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t VectorSize = 2;
};

template<>
struct shadow_type<v4float> {
    using type = shadow128_t;
    using scalar_type = float;
    static constexpr size_t VectorSize = 4;
};

template<>
struct shadow_type<double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t VectorSize = 1;
};

template<>
struct shadow_type<v2double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t VectorSize = 2;
};

template<>
struct shadow_type<v4double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t VectorSize = 4;
};

template<>
struct shadow_type<v8double> {
    using type = shadow256_t;
    using scalar_type = double;
    static constexpr size_t VectorSize = 8;
};