#ifndef __SL_MATHS_MATH_H__
#define __SL_MATHS_MATH_H__

#include "../structures.h"
#include "constants.h"
#include <math.h>


/// @brief Minimum of two doubles
/// @param a First value
/// @param b Second value
/// @return The smallest of the two
static inline double SL_min(double a, double b) {
    return a < b ? a : b;
}
/// @brief Maximum of two doubles
/// @param a First value
/// @param b Second value
/// @return The largest of the two
static inline double SL_max(double a, double b) {
    return a > b ? a : b;
}
/// @brief Restrict a double to the [l, h] range
/// @param f The value to restrict
/// @param l The lower bound
/// @param h The higher bound
/// @return The restricted value
static inline double SL_clamp(double f, double l, double h) {
    return SL_min(h, SL_max(f, l));
}
/// @brief Linear interpolation of two doubles
/// @param a First value
/// @param b Second value
/// @param t Interpolation factor
/// @return The interpolated value
static inline double SL_lerp(double a, double b, float t) {
    return a + (b - a) * t;
}
/// @brief Convert linear easing to quadratic for an interpolation
/// @param t The linear interpolation factor
/// @return The converted factor
static inline double SL_interpolateQuadratic(double t) {
    return t * t * (3.0 - 2.0 * t);
}
/// @brief Get fractionnal part
/// @param f The value
/// @return The fractionnal part
static inline double SL_fract(double f) {
    return f - floor(f);
}

/// @brief Hash a uint32 using PCG Hash
/// @param i The value to hash
/// @return The hashed value
uint32 SL_PCGHash(uint32 i);

/// @brief Set the seed for further random operations
/// @param i The new seed
void SL_randSeed(uint32 i);
/// @brief Produce a random uint32 with uniform distribution between 0 and INT_MAX
/// @note Using PCG Hash
/// @return A random uint32
uint32 SL_randU32();
/// @brief Produce a random float between 0.0f and 1.0f
/// @note Using PCG Hash
/// @return A random float
float SL_randFloat();
/// @brief Find the smallest power of two greater than an int
/// @param n The int to inspect
/// @return The closest power of two above n
uint SL_closestPow2(uint n);



#include "vector.h"

float hash21(vec2 p);
float hash31(vec3 p);

vec2 hash22(vec2 v);
vec3 hash33(vec3 v);

vec2 hash2Unit2(vec2 v);
vec3 hash3Unit3(vec3 v);

float SL_noise2D_voronoi(vec2 p);
float SL_noise3D_voronoi(vec3 p);

float SL_noise2D_perlin(vec2 p);
float SL_noise3D_perlin(vec3 p);


#endif