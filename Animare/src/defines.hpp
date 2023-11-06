#pragma once

typedef float real;

// comparing floating point numbers can be difficult
// instead we'll use an epsilon to compare such numbers
#define FLOAT_ESPISON 0.0000000001f
#define VEC2_EPSILON  0.000001f
#define VEC3_EPSILON  0.000001f
#define VEC4_EPSILON  0.000001f
#define MAT4_EPSILON  0.000001f
#define QUAT_EPSILON  0.000001f


#define PI 3.14159265359f