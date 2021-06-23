#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* typedef uint64_t vec2i64 __attribute__((ext_vector_type(2)));
typedef uint64_t vec4i64 __attribute__((ext_vector_type(4)));
 */

typedef struct {
    double val;
    uint64_t padding[1];
} shadow128_t;

typedef struct {
    __float128 val;
    uint64_t padding[2];
} shadow256_t;

/* 
shadow256_t compactShadow256(vec2i64 shadowBeg, vec2i64 shadowEnd); */