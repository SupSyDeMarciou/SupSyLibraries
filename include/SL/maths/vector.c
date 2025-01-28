#include "vector.h"

#define __SL_GEN_generateVector_Constants(aMin) \
    const aMin##vec2 aMin##vec2_right = { 1,  0}; \
    const aMin##vec2 aMin##vec2_left  = {-1,  0}; \
    const aMin##vec2 aMin##vec2_up    = { 0,  1}; \
    const aMin##vec2 aMin##vec2_down  = { 0, -1}; \
    const aMin##vec2 aMin##vec2_zero  = { 0,  0}; \
    const aMin##vec2 aMin##vec2_one   = { 1,  1}; \
    \
    const aMin##vec3 aMin##vec3_right = { 1,  0,  0}; \
    const aMin##vec3 aMin##vec3_left  = {-1,  0,  0}; \
    const aMin##vec3 aMin##vec3_up    = { 0,  1,  0}; \
    const aMin##vec3 aMin##vec3_down  = { 0, -1,  0}; \
    const aMin##vec3 aMin##vec3_forw  = { 0,  0,  1}; \
    const aMin##vec3 aMin##vec3_back  = { 0,  0, -1}; \
    const aMin##vec3 aMin##vec3_zero  = { 0,  0,  0}; \
    const aMin##vec3 aMin##vec3_one   = { 1,  1,  1}; \
    \
    const aMin##vec4 aMin##vec4_zero = {0, 0, 0, 0}; \
    const aMin##vec4 aMin##vec4_one  = {1, 1, 1, 1};

__SL_GEN_generateVector_Constants()
__SL_GEN_generateVector_Constants(d)
__SL_GEN_generateVector_Constants(i)
__SL_GEN_generateVector_Constants(li)
__SL_GEN_generateVector_Constants(u)
__SL_GEN_generateVector_Constants(lu)
__SL_GEN_generateVector_Constants(b)



///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// STRUCT  VECTORS /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////



vec2 slerp2(vec2 a, vec2 b, float t) {
    float angle = acos(dot2(norm2(a), norm2(b)));
    float oneOverSinA = 1.0 / sin(angle);
    float coeff1 = sin(angle * (1.0 - t)) * oneOverSinA;
    float coeff2 = sin(angle * t) * oneOverSinA;
    return Vec2(b.x * coeff1 + a.x * coeff2, b.y * coeff1 + a.y * coeff2);
}
vec3 slerp3(vec3 a, vec3 b, float t) {
    float angle = acos(dot3(norm3(a), norm3(b)));
    float oneOverSinA = 1.0 / sin(angle);
    float coeff1 = sin(angle * (1.0 - t)) * oneOverSinA;
    float coeff2 = sin(angle * t) * oneOverSinA;
    return Vec3(b.x * coeff1 + a.x * coeff2, b.y * coeff1 + a.y * coeff2, b.z * coeff1 + a.z * coeff2);
}
vec4 slerp4(vec4 a, vec4 b, float t) {
    float angle = acos(dot4(norm4(a), norm4(b)));
    float oneOverSinA = 1.0 / sin(angle);
    float coeff1 = sin(angle * (1.0 - t)) * oneOverSinA;
    float coeff2 = sin(angle * t) * oneOverSinA;
    return Vec4(b.x * coeff1 + a.x * coeff2, b.y * coeff1 + a.y * coeff2, b.z * coeff1 + a.z * coeff2, b.w * coeff1 + a.w * coeff2);
}

#include "math.h"
vec2 rand2() {
    return Vec2(SL_randFloat(), SL_randFloat());
}
vec3 rand3() {
    return Vec3(SL_randFloat(), SL_randFloat(), SL_randFloat());
}
vec4 rand4() {
    return Vec4(SL_randFloat(), SL_randFloat(), SL_randFloat(), SL_randFloat());
}

vec2 rand2_Unit() {
    float theta = SL_randFloat() * TAU;
    return Vec2(cos(theta), sin(theta));
}
vec3 rand3_Unit() {
    float theta = SL_randFloat() * TAU;
    float z = SL_randFloat() * 2.0 - 1.0;
    float s = sqrt(1 - z * z);
    return Vec3(cos(theta) * s, sin(theta) * s, z);
}