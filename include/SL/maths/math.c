#include "math.h"

static uint32 SEED = 0;
void SL_randSeed(uint32 i) {
    SEED = i;
}

uint32 SL_PCGHash(uint32 i) {
    uint32 state = i * 747796405u + 2891336453u;
    uint32 word = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
    return (word >> 22) ^ word;
}

uint32 SL_randU32() {
    return SEED = SL_PCGHash(SEED);
}

float SL_randFloat() {
    union { uint32 i; float f; } res = {SL_randU32() & ((1 << 23) - 1) | (((1 << 7) - 1) << 23)};
    return res.f - 1.0;
}

uint SL_closestPow2(uint n) {
    uint i = 1;
    while (n > i) i <<= 1;
    return i;
}

///// NOISE

#include "vector.h"

float hash21(vec2 p) {
    return SL_fract(43757.5453*sin(dot2(p, Vec2(12.9898,78.233))));
}
float hash31(vec3 p) {
    return SL_fract(43757.5453*sin(dot3(p, Vec3(12.9898,78.233, 9.8192))));
}

vec2 hash22(vec2 v) {
    return Vec2(hash21(v), hash31(cross3(Vec3(v.x, v.y, v.x), Vec3(5.82, 13.0182, 8.61))));
}
vec3 hash33(vec3 v) {
    return Vec3(hash31(v), hash31(cross3(Vec3(v.y, v.z, v.x), Vec3(5.82, 13.0182, 8.61))), hash31(cross3(Vec3(v.z, v.x, v.y), Vec3(13.0182, 5.82, 8.61))));
}

vec2 hash2Unit2(vec2 v) {
    float theta = hash21(v)*TAU;
    return Vec2(cos(theta), sin(theta));
}
vec3 hash3Unit3(vec3 v) {
    float a = hash31(v) * TAU;
    float b = (hash31(cross3(Vec3(v.y, v.z, v.x), Vec3(1892.82, 73.0182, 162.5229))) - 0.5) * PI;

    float cosA = cos(a);
    return Vec3(cosA * cos(b), cosA * sin(b), sin(a));
}

float SL_noise2D_voronoi(vec2 p) {
    vec2 ip = floor2(p);
    vec2 fp = sub2(p, ip);
    
    float minDist = 9.0;
    for(int x = -1; x <= 1; x++)
    for(int y = -1; y <= 1; y++) {
        vec2 offset = Vec2(x, y);
        vec2 randomPoint = add2(hash22(add2(ip, offset)), offset);
        vec2 mid = scale2(add2(randomPoint, fp), 0.5);
        
        vec2 diff = sub2(mid, fp);
        float md = len2_Sqrd(diff);
        
        minDist = SL_min(minDist, md);
    }
    
    return sqrt(minDist);
}
float SL_noise3D_voronoi(vec3 p) {
    vec3 ip = floor3(p);
    vec3 fp = sub3(p, ip);
    
    float minDist = 27.0;
    for(int x = -1; x <= 1; x++)
    for(int y = -1; y <= 1; y++)
    for(int z = -1; z <= 1; z++) {
        vec3 offset = Vec3(x, y, z);
        vec3 randomPoint = add3(hash33(add3(ip, offset)), offset);
        vec3 mid = scale3(add3(randomPoint, fp), 0.5);
        
        vec3 diff = sub3(mid, fp);
        float md = len3_Sqrd(diff);
        
        minDist = SL_min(minDist, md);
    }
    
    return sqrt(minDist);
}

float SL_noise2D_perlin(vec2 p) {

    vec2 ip = floor2(p);
    vec2 fp = sub2(p, ip);
    vec2 sfp = Vec2(fp.x*fp.x*(3.0-2.0*fp.x), fp.y*fp.y*(3.0-2.0*fp.y));

    // corner random vectors
    vec2 cr1 = hash2Unit2(add2(ip, Vec2(0, 0)));
    vec2 cr2 = hash2Unit2(add2(ip, Vec2(1, 0)));
    vec2 cr3 = hash2Unit2(add2(ip, Vec2(0, 1)));
    vec2 cr4 = hash2Unit2(add2(ip, Vec2(1, 1)));
    
    // dot products lerped in x
    float d1 = SL_lerp(dot2(cr1, sub2(fp, Vec2(0, 0))), dot2(cr2, sub2(fp, Vec2(1, 0))), sfp.x);
    float d2 = SL_lerp(dot2(cr3, sub2(fp, Vec2(0, 1))), dot2(cr4, sub2(fp, Vec2(1, 1))), sfp.x);

    // dot products lerped in y
    return SL_lerp(d1, d2, sfp.y);
}
float SL_noise3D_perlin(vec3 p) {

    vec3 ip = floor3(p);
    vec3 fp = sub3(p, ip);
    vec3 sfp = Vec3(fp.x*fp.x*(3.0-2.0*fp.x), fp.y*fp.y*(3.0-2.0*fp.y), fp.z*fp.z*(3.0-2.0*fp.z));

    // corner random vectors
    vec3 cr1 = hash3Unit3(add3(ip, Vec3(0, 0, 0)));
    vec3 cr2 = hash3Unit3(add3(ip, Vec3(1, 0, 0)));
    vec3 cr3 = hash3Unit3(add3(ip, Vec3(0, 1, 0)));
    vec3 cr4 = hash3Unit3(add3(ip, Vec3(1, 1, 0)));

    vec3 cr5 = hash3Unit3(add3(ip, Vec3(0, 0, 1)));
    vec3 cr6 = hash3Unit3(add3(ip, Vec3(1, 0, 1)));
    vec3 cr7 = hash3Unit3(add3(ip, Vec3(0, 1, 1)));
    vec3 cr8 = hash3Unit3(add3(ip, Vec3(1, 1, 1)));
    
    // dot products lerped in x
    float d1 = SL_lerp(dot3(cr1, sub3(fp, Vec3(0, 0, 0))), dot3(cr2, sub3(fp, Vec3(1, 0, 0))), sfp.x);
    float d2 = SL_lerp(dot3(cr3, sub3(fp, Vec3(0, 1, 0))), dot3(cr4, sub3(fp, Vec3(1, 1, 0))), sfp.x);

    float d3 = SL_lerp(dot3(cr5, sub3(fp, Vec3(0, 0, 1))), dot3(cr6, sub3(fp, Vec3(1, 0, 1))), sfp.x);
    float d4 = SL_lerp(dot3(cr7, sub3(fp, Vec3(0, 1, 1))), dot3(cr8, sub3(fp, Vec3(1, 1, 1))), sfp.x);

    // dot products lerped in y
    float d5 = SL_lerp(d1, d3, sfp.y);
    float d6 = SL_lerp(d2, d4, sfp.y);

    // dot products lerped in z
    return SL_lerp(d1, d2, sfp.z);
}