#ifndef __SL_MATHS_VECTOR_H__
#define __SL_MATHS_VECTOR_H__

#include <math.h>
#include "constants.h"
#include "../structures.h"

#define XPD_VEC2(v) v.x, v.y
#define XPD_VEC3(v) v.x, v.y, v.z
#define XPD_VEC4(v) v.x, v.y, v.z, v.w

#define FMT_VEC2 "(%f, %f)"
#define FMT_VEC3 "(%f, %f, %f)"
#define FMT_VEC4 "(%f, %f, %f, %f)"

#define FMT_DVEC2 "(%lf, %lf)"
#define FMT_DVEC3 "(%lf, %lf, %lf)"
#define FMT_DVEC4 "(%lf, %lf, %lf, %lf)"

#define FMT_IVEC2 "(%d, %d)"
#define FMT_IVEC3 "(%d, %d, %d)"
#define FMT_IVEC4 "(%d, %d, %d, %d)"

#define FMT_LIVEC2 "(%ld, %ld)"
#define FMT_LIVEC3 "(%ld, %ld, %ld)"
#define FMT_LIVEC4 "(%ld, %ld, %ld, %ld)"

#define FMT_UVEC2 "(%u, %u)"
#define FMT_UVEC3 "(%u, %u, %u)"
#define FMT_UVEC4 "(%u, %u, %u, %u)"

#define FMT_LUVEC2 "(%lu, %lu)"
#define FMT_LUVEC3 "(%lu, %lu, %lu)"
#define FMT_LUVEC4 "(%lu, %lu, %lu, %lu)"

#define FMT_BVEC2 "(%u, %u)"
#define FMT_BVEC3 "(%u, %u, %u)"
#define FMT_BVEC4 "(%u, %u, %u, %u)"

#define func2(v, f) Vec2(f(v.x), f(v.y))
#define func3(v, f) Vec3(f(v.x), f(v.y), f(v.z))
#define func4(v, f) Vec4(f(v.x), f(v.y), f(v.z), f(v.w))

#define func2_(v, f, c) (c ? set2_(c, f(v.x), f(v.y)) : Vec2_(f(v.x), f(v.y)))
#define func3_(v, f, c) (c ? set3_(c, f(v.x), f(v.y), f(v.z)) : Vec3_(f(v.x), f(v.y), f(v.z)))
#define func4_(v, f, c) (c ? set4_(c, f(v.x), f(v.y), f(v.z), f(v.w)) : Vec4_(f(v.x), f(v.y), f(v.z), f(v.w)))

#define func2_s(v, f) (set2_(v, f(v.x), f(v.y)))
#define func3_s(v, f) (set3_(v, f(v.x), f(v.y), f(v.z)))
#define func4_s(v, f) (set4_(v, f(v.x), f(v.y), f(v.z), f(v.w)))



#include <stdlib.h>
#define __SL_GEN_argsList2(type) type x, type y
#define __SL_GEN_argsList3(type) type x, type y, type z
#define __SL_GEN_argsList4(type) type x, type y, type z, type w

#define __SL_GEN_compList2() x, y
#define __SL_GEN_compList3() x, y, z
#define __SL_GEN_compList4() x, y, z, w

#define __SL_GEN_Args_AB(type) type a, type b

#define __SL_GEN_func2(a, func, access) func(a access x), func(a access y)
#define __SL_GEN_func3(a, func, access) func(a access x), func(a access y), func(a access z)
#define __SL_GEN_func4(a, func, access) func(a access x), func(a access y), func(a access z), func(a access w)

#define __SL_GEN_binOp2(a, b, s, operator, access) a access x operator b access x s , a access y operator b access y s
#define __SL_GEN_binOp3(a, b, s, operator, access) a access x operator b access x s , a access y operator b access y s , a access z operator b access z s
#define __SL_GEN_binOp4(a, b, s, operator, access) a access x operator b access x s , a access y operator b access y s , a access z operator b access z s , a access w operator b access w s

#define __SL_GEN_ternOp2(a, b, m, s, operator1, operator2, access) a access x operator1 b access x operator2 m access x s , a access y operator1 b access y operator2 m access y s
#define __SL_GEN_ternOp3(a, b, m, s, operator1, operator2, access) a access x operator1 b access x operator2 m access x s , a access y operator1 b access y operator2 m access y s , a access z operator1 b access z operator2 m access z s
#define __SL_GEN_ternOp4(a, b, m, s, operator1, operator2, access) a access x operator1 b access x operator2 m access x s , a access y operator1 b access y operator2 m access y s , a access z operator1 b access z operator2 m access z s, a access w operator1 b access w operator2 m access w s

#define __SL_GEN_dot2(a, b, access) a access x * b access x + a access y * b access y
#define __SL_GEN_dot3(a, b, access) a access x * b access x + a access y * b access y + a access z * b access z
#define __SL_GEN_dot4(a, b, access) a access x * b access x + a access y * b access y + a access z * b access z + a access w * b access w

#define __SL_GEN_len2_manh(a, access) a access x + a access y
#define __SL_GEN_len3_manh(a, access) a access x + a access y + a access z
#define __SL_GEN_len4_manh(a, access) a access x + a access y + a access z + a access w

#define __SL_GEN_len2_max(a, access) a access x > a access y ? a access x : a access y
#define __SL_GEN_len3_max(a, access) a access x > a access z ? (__SL_GEN_len2_max(a, access)) : (a access y > a access z ? a access y : a access z)
#define __SL_GEN_len4_max(a, access) a access x > a access w ? (__SL_GEN_len3_max(a, access)) : (a access y > a access w ? (a access y > a access z ? a access y : a access z) : (a access z > a access w ? a access z : a access w))

#define __SL_GEN_scale2(a, s, access) a access x * s , a access y * s
#define __SL_GEN_scale3(a, s, access) a access x * s , a access y * s , a access z * s
#define __SL_GEN_scale4(a, s, access) a access x * s , a access y * s , a access z * s , a access w * s

#define __SL_GEN_minmax2(a, b, operator, access) a access x operator b access x ? a access x : b access x, a access y operator b access y ? a access y : b access y
#define __SL_GEN_minmax3(a, b, operator, access) a access x operator b access x ? a access x : b access x, a access y operator b access y ? a access y : b access y, a access z operator b access z ? a access z : b access z
#define __SL_GEN_minmax4(a, b, operator, access) a access x operator b access x ? a access x : b access x, a access y operator b access y ? a access y : b access y, a access z operator b access z ? a access z : b access z, a access w operator b access w ? a access w : b access w

#define __SL_GEN_generateVector_Generic_STRUCT(type, Type, aMin, aMaj, size) \
    static inline aMin##vec##size create##aMaj##ec##size (__SL_GEN_argsList##size(type))                                        { return (aMin##vec##size) {__SL_GEN_argsList##size()}; } \
    static inline aMin##vec##size aMaj##ec##size (__SL_GEN_argsList##size(type))                                                { return (aMin##vec##size) {__SL_GEN_argsList##size()}; } \
    static inline aMin##vec##size add##aMin####size (__SL_GEN_Args_AB(const aMin##vec##size))                                   { return (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, +, .)}; } \
    static inline aMin##vec##size sub##aMin####size (__SL_GEN_Args_AB(const aMin##vec##size))                                   { return (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, -, .)}; } \
    static inline aMin##vec##size mul##aMin####size (__SL_GEN_Args_AB(const aMin##vec##size))                                   { return (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, *, .)}; } \
    static inline aMin##vec##size div##aMin####size (__SL_GEN_Args_AB(const aMin##vec##size))                                   { return (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, /, .)}; } \
    static inline type dot##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size))                                                { return __SL_GEN_dot##size(a, b, .); } \
    static inline type len##aMin##size##_Max (const aMin##vec##size v)                                                          { return __SL_GEN_len##size##_max(v, .); } \
    static inline type len##aMin##size##_Manh (const aMin##vec##size v)                                                         { return __SL_GEN_len##size##_manh(v, .); } \
    static inline type len##aMin##size##_Sqrd (const aMin##vec##size v)                                                         { return __SL_GEN_dot##size(v, v, .); } \
    static inline type len##aMin##size##_Eucl (const aMin##vec##size v)                                                         { return sqrt(len##aMin##size##_Sqrd(v)); } \
    static inline type len##aMin##size (const aMin##vec##size v)                                                                { return len##aMin##size##_Eucl(v); } \
    static inline aMin##vec##size scale##aMin##size (const aMin##vec##size v, type s)                                           { return (aMin##vec##size) {__SL_GEN_scale##size(v, s, .)}; } \
    static inline aMin##vec##size norm##aMin##size (const aMin##vec##size v)                                                    { return scale##aMin##size(v, 1 / len##aMin##size(v)); } \
    static inline aMin##vec##size addS##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size), type s)                            { return (aMin##vec##size) {__SL_GEN_binOp##size(a, b, * s, +, .)}; } \
    static inline aMin##vec##size addM##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size), aMin##vec##size m)                 { return (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m,, +, *, .)}; } \
    static inline aMin##vec##size addSM##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size), type s, aMin##vec##size m)        { return (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m, * s, +, *, .)}; } \
    static inline aMin##vec##size min##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size))                                     { return (aMin##vec##size) {__SL_GEN_minmax##size(a, b, <, .)}; } \
    static inline aMin##vec##size max##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size))                                     { return (aMin##vec##size) {__SL_GEN_minmax##size(a, b, >, .)}; } \
    static inline aMin##vec##size refl##aMin##size (__SL_GEN_Args_AB(const aMin##vec##size))                                    { return addS##aMin##size(a, b, -2.0 * dot##aMin##size(a, b) / len##aMin##size##_Sqrd(a)); }

#define __SL_GEN_generateVector_Generic_POINTER_EXT(type, Type, aMin, aMaj, size) \
    static inline aMin##vec##size* new##aMaj##ec##size (__SL_GEN_argsList##size(type))                                          { aMin##vec##size* v = malloc(sizeof(aMin##vec##size)); *v = (aMin##vec##size) {__SL_GEN_argsList##size()}; return v; } \
    static inline aMin##vec##size* aMaj##ec##size##_ (__SL_GEN_argsList##size(type))                                            { return new##aMaj##ec##size(__SL_GEN_argsList##size()); } \
    static inline void free##aMaj##ec##size(aMin##vec##size* toFree)                                                            { free(toFree); }; \
    static inline aMin##vec##size* copy##aMin##size##_(const aMin##vec##size* source, aMin##vec##size* c)                       { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = *source; return c; } \
    static inline aMin##vec##size* set##aMin##size##_(aMin##vec##size* v, __SL_GEN_argsList##size(type))                        { *v = (aMin##vec##size) {__SL_GEN_argsList##size()}; return v; } \
    static inline aMin##vec##size* null##aMin##size##_()                                                                        { return calloc(1, sizeof(aMin##vec##size)); } \
    static inline aMin##vec##size* add##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, +, ->)}; return c; } \
    static inline aMin##vec##size* sub##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, -, ->)}; return c; } \
    static inline aMin##vec##size* mul##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, *, ->)}; return c; } \
    static inline aMin##vec##size* div##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, /, ->)}; return c; } \
    static inline type dot##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*))                                            { return __SL_GEN_dot##size(a, b, ->); } \
    static inline type len##aMin##size##_Max_ (const aMin##vec##size* v)                                                        { return __SL_GEN_len##size##_max(v, ->); } \
    static inline type len##aMin##size##_Manh_ (const aMin##vec##size* v)                                                       { return __SL_GEN_len##size##_manh(v, ->); } \
    static inline type len##aMin##size##_Sqrd_ (const aMin##vec##size* v)                                                       { return __SL_GEN_dot##size(v, v, ->); } \
    static inline type len##aMin##size##_Eucl_ (const aMin##vec##size* v)                                                       { return sqrt(len##aMin##size##_Sqrd_(v)); } \
    static inline type len##aMin##size##_ (const aMin##vec##size* v)                                                            { return len##aMin##size##_Eucl_(v); } \
    static inline aMin##vec##size* scale##aMin##size##_ (const aMin##vec##size* v, type s, aMin##vec##size* c)                  { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_scale##size(v, s, ->)}; return c; } \
    static inline aMin##vec##size* norm##aMin##size##_ (const aMin##vec##size* v, aMin##vec##size* c)                           { return scale##aMin##size##_(v, 1 / len##aMin##size##_(v), c); } \
    static inline aMin##vec##size* addS##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), type s, aMin##vec##size* c)                               { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_binOp##size(a, b, * s, +, ->)}; return c; } \
    static inline aMin##vec##size* addM##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), const aMin##vec##size* m, aMin##vec##size* c)             { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m,, +, *, ->)}; return c; } \
    static inline aMin##vec##size* addSM##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), type s, const aMin##vec##size* m, aMin##vec##size* c)    { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m, * s, +, *, ->)}; return c; } \
    static inline aMin##vec##size* min##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_minmax##size(a, b, <, ->)}; return c; } \
    static inline aMin##vec##size* max##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)            { if(!c) c = malloc(sizeof(aMin##vec##size)); *c = (aMin##vec##size) {__SL_GEN_minmax##size(a, b, >, ->)}; return c; } \
    static inline aMin##vec##size* refl##aMin##size##_ (__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c)           { return addS##aMin##size##_(a, b, -2.0 * dot##aMin##size##_(a, b) / len##aMin##size##_Sqrd_(a), c); }

#define __SL_GEN_generateVector_Generic_POINTER_SELF(type, Type, aMin, aMaj, size) \
    static inline aMin##vec##size* add##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, +, ->)}; return a; } \
    static inline aMin##vec##size* sub##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, -, ->)}; return a; } \
    static inline aMin##vec##size* mul##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, *, ->)}; return a; } \
    static inline aMin##vec##size* div##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_binOp##size(a, b,, /, ->)}; return a; } \
    static inline aMin##vec##size* scale##aMin##size##_s (aMin##vec##size* v, type s)                                           { *v = (aMin##vec##size) {__SL_GEN_scale##size(v, s, ->)}; return v; } \
    static inline aMin##vec##size* norm##aMin##size##_s (aMin##vec##size* v)                                                    { return scale##aMin##size##_s(v, 1 / len##aMin##size##_(v)); } \
    static inline aMin##vec##size* addS##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b, type s)                  { *a = (aMin##vec##size) {__SL_GEN_binOp##size(a, b, * s, +, ->)}; return a; } \
    static inline aMin##vec##size* addM##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b, const aMin##vec##size* m)                            { *a = (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m,, +, *, ->)}; return a; } \
    static inline aMin##vec##size* addSM##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b, type s, const aMin##vec##size* m)                   { *a = (aMin##vec##size) {__SL_GEN_ternOp##size(a, b, m, * s, +, *, ->)}; return a; } \
    static inline aMin##vec##size* min##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_minmax##size(a, b, <, ->)}; return a; } \
    static inline aMin##vec##size* max##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                           { *a = (aMin##vec##size) {__SL_GEN_minmax##size(a, b, >, ->)}; return a; } \
    static inline aMin##vec##size* refl##aMin##size##_s (aMin##vec##size* a, const aMin##vec##size* b)                          { return addS##aMin##size##_s(a, b, -2.0 * dot##aMin##size##_(a, b) / len##aMin##size##_Sqrd_(a)); }

#define __SL_GEN_generateVector_Type(type, Type, aMin, aMaj) \
    typedef struct Type##Vector2 { type x, y; } aMin##vec2; /* @brief 2D Integer Vector (x, y) */ \
    extern const aMin##vec2 aMin##vec2_right; \
    extern const aMin##vec2 aMin##vec2_left; \
    extern const aMin##vec2 aMin##vec2_up; \
    extern const aMin##vec2 aMin##vec2_down; \
    extern const aMin##vec2 aMin##vec2_zero; \
    extern const aMin##vec2 aMin##vec2_one; \
    __SL_GEN_generateVector_Generic_STRUCT(type, Type, aMin, aMaj, 2) \
    __SL_GEN_generateVector_Generic_POINTER_EXT(type, Type, aMin, aMaj, 2) \
    __SL_GEN_generateVector_Generic_POINTER_SELF(type, Type, aMin, aMaj, 2) \
    static inline aMin##vec2 aMin##vec2One(type s) { return aMaj##ec2(s, s); } \
    static inline aMin##vec2* aMin##vec2One_(type s) { return new##aMaj##ec2(s, s); } \
    \
    typedef struct Type##Vector3 { type x, y, z; } aMin##vec3; \
    extern const aMin##vec3 aMin##vec3_right; \
    extern const aMin##vec3 aMin##vec3_left; \
    extern const aMin##vec3 aMin##vec3_up; \
    extern const aMin##vec3 aMin##vec3_down; \
    extern const aMin##vec3 aMin##vec3_forward; \
    extern const aMin##vec3 aMin##vec3_back; \
    extern const aMin##vec3 aMin##vec3_zero; \
    extern const aMin##vec3 aMin##vec3_one; \
    __SL_GEN_generateVector_Generic_STRUCT(type, Type, aMin, aMaj, 3) \
    __SL_GEN_generateVector_Generic_POINTER_EXT(type, Type, aMin, aMaj, 3) \
    __SL_GEN_generateVector_Generic_POINTER_SELF(type, Type, aMin, aMaj, 3) \
    static inline aMin##vec3 aMin##vec3One(type s) { return aMaj##ec3(s, s, s); } \
    static inline aMin##vec3* aMin##vec3One_(type s) { return new##aMaj##ec3(s, s, s); } \
    \
    typedef struct Type##Vector4 { type x, y, z, w; } aMin##vec4; \
    extern const aMin##vec4 aMin##vec4_zero; \
    extern const aMin##vec4 aMin##vec4_one; \
    __SL_GEN_generateVector_Generic_STRUCT(type, Type, aMin, aMaj, 4) \
    __SL_GEN_generateVector_Generic_POINTER_EXT(type, Type, aMin, aMaj, 4) \
    __SL_GEN_generateVector_Generic_POINTER_SELF(type, Type, aMin, aMaj, 4) \
    static inline aMin##vec4 aMin##vec4One(type s) { return aMaj##ec4(s, s, s, s); } \
    static inline aMin##vec4* aMin##vec4One_(type s) { return new##aMaj##ec4(s, s, s, s); } \
    \
    static inline aMin##vec3 aMin##vec2To3 (const aMin##vec2 v) { return (aMin##vec3){v.x, v.y, 0}; }; \
    static inline aMin##vec3 aMin##vec2To3_z(const aMin##vec2 v, type z) { return (aMin##vec3){v.x, v.y, z}; }; \
    static inline aMin##vec4 aMin##vec2To4(const aMin##vec2 v) { return (aMin##vec4){v.x, v.y, 0, 0}; }; \
    static inline aMin##vec4 aMin##vec2To4_zw(const aMin##vec2 v, type z, type w) { return (aMin##vec4){v.x, v.y, z, w}; }; \
    static inline aMin##vec4 aMin##vec3To4(const aMin##vec3 v) { return (aMin##vec4){v.x, v.y, v.z, 0}; }; \
    static inline aMin##vec4 aMin##vec3To4_w(const aMin##vec3 v, type w) { return (aMin##vec4){v.x, v.y, v.z, w}; }; \
    static inline aMin##vec2 aMin##vec3To2(const aMin##vec3 v) { return (aMin##vec2){v.x, v.y}; }; \
    static inline aMin##vec3 aMin##vec4To3(const aMin##vec4 v) { return (aMin##vec3){v.x, v.y, v.z}; }; \
    static inline aMin##vec2 aMin##vec4To2(const aMin##vec4 v) { return (aMin##vec2){v.x, v.y}; }; \

__SL_GEN_generateVector_Type(float, Float, , V)
__SL_GEN_generateVector_Type(double, Double, d, Dv)
__SL_GEN_generateVector_Type(int, Int, i, Iv)
__SL_GEN_generateVector_Type(int64, LongInt, li, Liv)
__SL_GEN_generateVector_Type(uint, Uint, u, Uv)
__SL_GEN_generateVector_Type(uint64, LongUint, lu, Luv)
__SL_GEN_generateVector_Type(bool, Boolean, b, Bv)

#define __SL_GEN_generateConvert_Simple(aMinS, aMajS, aMinD, aMajD, size) \
    static inline aMinD##vec##size aMinS##vec##size##To##aMajD(const aMinS##vec##size v) { return aMajD##ec##size(XPD_VEC##size(v)); }

#define __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD, aMajD) \
    __SL_GEN_generateConvert_Simple(aMinS, aMajS, aMinD, aMajD, 2) \
    __SL_GEN_generateConvert_Simple(aMinS, aMajS, aMinD, aMajD, 3) \
    __SL_GEN_generateConvert_Simple(aMinS, aMajS, aMinD, aMajD, 4)

#define __SL_GEN_generateConvert_Full(aMinS, aMajS, aMinD1, aMajD1, aMinD2, aMajD2, aMinD3, aMajD3, aMinD4, aMajD4, aMinD5, aMajD5, aMinD6, aMajD6) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD1, aMajD1) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD2, aMajD2) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD3, aMajD3) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD4, aMajD4) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD5, aMajD5) \
    __SL_GEN_generateConvert_Sizes(aMinS, aMajS, aMinD6, aMajD6)

__SL_GEN_generateConvert_Full(, V, d, Dv, i, Iv, li, Liv, u, Uv, lu, Luv, b, Bv)
__SL_GEN_generateConvert_Full(d, Dv, , V, i, Iv, li, Liv, u, Uv, lu, Luv, b, Bv)
__SL_GEN_generateConvert_Full(i, Iv, d, Dv, , V, li, Liv, u, Uv, lu, Luv, b, Bv)
__SL_GEN_generateConvert_Full(li, Liv, d, Dv, i, Iv, , V, u, Uv, lu, Luv, b, Bv)
__SL_GEN_generateConvert_Full(u, Uv, d, Dv, i, Iv, li, Liv, , V, lu, Luv, b, Bv)
__SL_GEN_generateConvert_Full(lu, Luv, d, Dv, i, Iv, li, Liv, u, Uv, , V, b, Bv)
__SL_GEN_generateConvert_Full(b, Bv, d, Dv, i, Iv, li, Liv, u, Uv, lu, Luv, , V)

#define vec2At(v, i) (*((void*)&(v) + i * (sizeof(v) / 2)))
#define vec3At(v, i) (*((void*)&(v) + i * (sizeof(v) / 3)))
#define vec4At(v, i) (*((void*)&(v) + i * (sizeof(v) / 4)))



///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// STRUCT  VECTORS /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////



// Cross product of two vec3
static inline vec3 cross3(const vec3 a, const vec3 b) { return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
// Cross product of two dvec3
static inline dvec3 crossd3(const dvec3 a, const dvec3 b) { return Dvec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
// Cross product of two ivec3
static inline ivec3 crossi3(const ivec3 a, const ivec3 b) { return Ivec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

// Cross product of two vec3
static inline vec3* cross3_(const vec3* a, const vec3* b, vec3* c) { if (!c) c = malloc(sizeof(vec3)); return set3_(c, a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x); }
// Cross product of two dvec3
static inline dvec3* crossd3_(const dvec3* a, const dvec3* b, dvec3* c) { if (!c) c = malloc(sizeof(dvec3)); return setd3_(c, a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x); }
// Cross product of two ivec3
static inline ivec3* crossi3_(const ivec3* a, const ivec3* b, ivec3* c) { if (!c) c = malloc(sizeof(ivec3)); return seti3_(c, a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x); }



#define __SL_GEN_intVector_Specific(type, Type, aMin, aMaj, size) \
    static inline aMin##vec##size mod##aMin##size(__SL_GEN_Args_AB(const aMin##vec##size)) { return aMaj##ec##size(__SL_GEN_binOp##size(a, b,, %, .)); } \
    static inline aMin##vec##size* mod##aMin##size##_(__SL_GEN_Args_AB(const aMin##vec##size*), aMin##vec##size* c) { if (!c) c = malloc(sizeof(aMin##vec##size)); return set##aMin##size##_(c, __SL_GEN_binOp##size(a, b,, %, ->)); } \
    static inline aMin##vec##size* mod##aMin##size##_s(aMin##vec##size* a, const aMin##vec##size* b) { return set##aMin##size##_(a, __SL_GEN_binOp##size(a, b,, %, ->)); }
#define __SL_GEN_intVector_Specifics(type, Type, aMin, aMaj) \
    __SL_GEN_intVector_Specific(type, Type, aMin, aMaj, 2) \
    __SL_GEN_intVector_Specific(type, Type, aMin, aMaj, 3) \
    __SL_GEN_intVector_Specific(type, Type, aMin, aMaj, 4)

__SL_GEN_intVector_Specifics(int, Int, i, Iv)
__SL_GEN_intVector_Specifics(int64, LongInt, li, Liv)
__SL_GEN_intVector_Specifics(uint, Uint, u, Uv)
__SL_GEN_intVector_Specifics(uint64, LongUint, lu, Luv)

#define __SL_GEN_floatVector_Specific(type, Type, aMin, aMaj, size) \
    static inline aMin##vec##size  floor##aMin##size(const aMin##vec##size v) { return aMaj##ec##size(__SL_GEN_func##size(v, floor, .)); } \
    static inline aMin##vec##size* floor##aMin##size##_(const aMin##vec##size* v, aMin##vec##size* c) { if (!c) c = malloc(sizeof(aMin##vec##size)); return set##aMin##size##_(c, __SL_GEN_func##size(v, floor, ->)); } \
    static inline aMin##vec##size* floor##aMin##size##_s(aMin##vec##size* restrict v) { return set##aMin##size##_(v, __SL_GEN_func##size(v, floor, ->)); } \
    \
    static inline aMin##vec##size  ceil##aMin##size(const aMin##vec##size v) { return aMaj##ec##size(__SL_GEN_func##size(v, ceil, .)); } \
    static inline aMin##vec##size* ceil##aMin##size##_(const aMin##vec##size* v, aMin##vec##size* c) { if (!c) c = malloc(sizeof(aMin##vec##size)); return set##aMin##size##_(c, __SL_GEN_func##size(v, ceil, ->)); } \
    static inline aMin##vec##size* ceil##aMin##size##_s(aMin##vec##size* restrict v) { return set##aMin##size##_(v, __SL_GEN_func##size(v, ceil, ->)); } \
    \
    static inline aMin##vec##size  frac##aMin##size(const aMin##vec##size v) { return sub##aMin##size(v, floor##aMin##size(v)); } \
    static inline aMin##vec##size* frac##aMin##size##_(const aMin##vec##size* v, aMin##vec##size* c) { if (!c) c = malloc(sizeof(aMin##vec##size)); *c = frac##aMin##size(*v); return c; } \
    static inline aMin##vec##size* frac##aMin##size##_s(aMin##vec##size* restrict v) { *v = frac##aMin##size(*v); return v; } \
    \
    static inline aMin##vec##size aMin##vec##size##Unit(__SL_GEN_argsList##size(type)) { return norm##aMin##size(aMaj##ec##size(__SL_GEN_compList##size())); } \
    static inline aMin##vec##size* aMin##vec##size##Unit_(__SL_GEN_argsList##size(type)) { return norm##aMin##size##_s(aMaj##ec##size##_(__SL_GEN_compList##size())); } \

#define __SL_GEN_floatVector_Specifics(type, Type, aMin, aMaj) \
    __SL_GEN_floatVector_Specific(type, Type, aMin, aMaj, 2) \
    __SL_GEN_floatVector_Specific(type, Type, aMin, aMaj, 3) \
    __SL_GEN_floatVector_Specific(type, Type, aMin, aMaj, 4) \
    \
    static inline aMin##vec##2 lerp##aMin##2(const aMin##vec##2 a, const aMin##vec##2 b, type t) { return aMaj##ec##2((b.x - a.x) * t + a.x, (b.y - a.y) * t + a.y); } \
    static inline aMin##vec##3 lerp##aMin##3(const aMin##vec##3 a, const aMin##vec##3 b, type t) { return aMaj##ec##3((b.x - a.x) * t + a.x, (b.y - a.y) * t + a.y, (b.z - a.z) * t + a.z); } \
    static inline aMin##vec##4 lerp##aMin##4(const aMin##vec##4 a, const aMin##vec##4 b, type t) { return aMaj##ec##4((b.x - a.x) * t + a.x, (b.y - a.y) * t + a.y, (b.z - a.z) * t + a.z, (b.w - a.w) * t + a.w); } \
    \
    static inline aMin##vec##2* lerp##aMin##2_(const aMin##vec##2* a, const aMin##vec##2* b, type t, aMin##vec##2* c) { if (!c) c = malloc(sizeof(aMin##vec##2)); return set##aMin##2_(c, (b->x - a->x) * t + a->x, (b->y - a->y) * t + a->y); } \
    static inline aMin##vec##3* lerp##aMin##3_(const aMin##vec##3* a, const aMin##vec##3* b, type t, aMin##vec##3* c) { if (!c) c = malloc(sizeof(aMin##vec##3)); return set##aMin##3_(c, (b->x - a->x) * t + a->x, (b->y - a->y) * t + a->y, (b->z - a->z) * t + a->z); } \
    static inline aMin##vec##4* lerp##aMin##4_(const aMin##vec##4* a, const aMin##vec##4* b, type t, aMin##vec##4* c) { if (!c) c = malloc(sizeof(aMin##vec##4)); return set##aMin##4_(c, (b->x - a->x) * t + a->x, (b->y - a->y) * t + a->y, (b->z - a->z) * t + a->z, (b->w - a->w) * t + a->w); } \

__SL_GEN_floatVector_Specifics(float, Float, , V)
__SL_GEN_floatVector_Specifics(double, Double, d, Dv)



/// @brief Spherical interpolation between two vec2
/// @param a The base vector
/// @param b The destination vector
/// @param t The parameter in [0, 1] range (can exceed but results might not work as expected)
/// @return The interpolated vector
vec2 slerp2(vec2 a, vec2 b, float t);
/// @brief Spherical interpolation between two vec3
/// @param a The base vector
/// @param b The destination vector
/// @param t The parameter in [0, 1] range (can exceed but results might not work as expected)
/// @return The interpolated vector
vec3 slerp3(vec3 a, vec3 b, float t);
/// @brief Spherical interpolation between two vec4
/// @param a The base vector
/// @param b The destination vector
/// @param t The parameter in [0, 1] range (can exceed but results might not work as expected)
/// @return The interpolated vector
vec4 slerp4(vec4 a, vec4 b, float t);



/// @brief Generate a uniformly distributed unit vec2
/// @return The generated vec2
vec2 rand2();
/// @brief Generate a uniformly distributed unit vec3
/// @return The generated vec3
vec3 rand3();

/// @brief Project a vec3 on a plane
/// @param a The vector to project
/// @param n The normal of the plane
/// @return The projected vector
static inline vec3 vec3ProjPlane(const vec3 a, const vec3 n) { return addS3(a, n, -dot3(a, n)); }
/// @brief Project a vec3 on a plane
/// @param a The vector to project
/// @param n The normal of the plane
/// @return The projected vector
static inline dvec3 dvec3ProjPlane(const dvec3 a, const dvec3 n) { return addSd3(a, n, -dotd3(a, n)); }

/// @brief Project a vec3 on a plane
/// @param a The vector to project
/// @param n The normal of the plane
/// @return The projected vector
static inline vec3* vec3ProjPlane_(const vec3* a, const vec3* n, vec3* c) { return addS3_(a, n, -dot3_(a, n), c); }
/// @brief Project a vec3 on a plane
/// @param a The vector to project
/// @param n The normal of the plane
/// @return The projected vector
static inline dvec3* dvec3ProjPlane_(const dvec3* a, const dvec3* n, dvec3* c) { return addSd3_(a, n, -dotd3_(a, n), c); }

static inline vec2 rot2(const vec2 v, float angle) { float c = cos(angle), s = sin(angle); return Vec2(c * v.x - s * v.y, s * v.y + c * v.x); }
static inline vec2* rot2_(const vec2* v, float angle, vec2* d) { float c = cos(angle), s = sin(angle); return c ? Vec2_(c * v->x - s * v->y, s * v->y + c * v->x) : set2_(d, c * v->x - s * v->y, s * v->y + c * v->x); }
static inline vec2* rot2_s(vec2* restrict v, float angle) { float c = cos(angle), s = sin(angle); return set2_(v, c * v->x - s * v->y, s * v->y + c * v->x); }

static inline dvec2 rotd2(const dvec2 v, double angle) { double c = cos(angle), s = sin(angle); return Dvec2(c * v.x - s * v.y, s * v.y + c * v.x); }
static inline dvec2* rotd2_(const dvec2* v, double angle, dvec2* d) { double c = cos(angle), s = sin(angle); return c ? Dvec2_(c * v->x - s * v->y, s * v->y + c * v->x) : setd2_(d, c * v->x - s * v->y, s * v->y + c * v->x); }
static inline dvec2* rotd2_s(dvec2* restrict v, double angle) { double c = cos(angle), s = sin(angle); return setd2_(v, c * v->x - s * v->y, s * v->y + c * v->x); }

#endif