#ifndef __SL_MATH_QUATERNION
#define __SL_MATH_QUATERNION

#include "vector.h"

/// @brief Quaternion (x, y, z, w) = x + iy + jz + kw
/// @note where i² = j² = k² = ijk = -1
typedef struct Quaternion {
    float x, y, z, w;
} quat;

/// @brief The identity quaternion representing no rotation: (1, 0, 0, 0)
extern const quat quat_identity;

#define FMT_QUAT "%f + %fi + %fj + %fk"
#define XPD_QUAT(q) q.x, q.y, q.z, q.w


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// STRUCT QUATERNIONS ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////




/// @brief Create a quaternion
/// @param x x component
/// @param y y component
/// @param z z component
/// @param w w component
/// @return The newly created quaternion
static inline quat Quat(float x, float y, float z, float w) { 
    return (quat) {x, y, z, w};
}
/// @brief Create a quaternion from real and imaginary parts
/// @param r real part
/// @param i imaginary part
/// @return The newly created quaternion
static inline quat Quat_V(float w, const vec3 v) {
    return (quat) {w, v.x, v.y, v.z};
}
/// @brief Create a quaternion from euler angles
/// @param pitch The angle around X
/// @param yaw The angle around Y
/// @param roll The angle around Z
/// @return The newly created quaternion
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
static inline quat Quat_Euler(float l, float m, float n) {
    float cr = cos(l * 0.5);
    float sr = sin(l * 0.5);
    float cp = cos(m * 0.5);
    float sp = sin(m * 0.5);
    float cy = cos(n * 0.5);
    float sy = sin(n * 0.5);

    return (quat) {
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy
    };
}
/// @brief Create a quaternion from euler angles vector
/// @param v The vector of euler angles
/// @return The newly created quaternion
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
static inline quat Quat_EulerVector(const vec3 v) {
    return Quat_Euler(v.x, v.y, v.z);
}
/// @brief Create a quaternion from angle axis
/// @param angle The angle of rotation
/// @param axis The axis of rotation
/// @return The newly created quaternion
static inline quat Quat_AngleAxis(float angle, const vec3 v) {
    angle *= 0.5;
    float sinA = sin(angle);
    return (quat) {cos(angle), sinA * v.x, sinA * v.y, sinA * v.z};
}

/// @brief Add two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The resulting quaternion
static inline quat addQ(const quat a, const quat b) {
    return (quat) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
/// @brief Subtract two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The resulting quaternion
static inline quat subQ(const quat a, const quat b) {
    return (quat) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
/// @brief Multiply two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The resulting quaternion
static inline quat mulQ(const quat a, const quat b) {
    return (quat) {
        (a.x * b.x) - (a.y * b.y) - (a.z * b.z) - (a.w * b.w),
        (a.x * b.y) + (a.y * b.x) - (a.z * b.w) + (a.w * b.z),
        (a.x * b.z) + (a.y * b.w) + (a.z * b.x) - (a.w * b.y),
        (a.x * b.w) - (a.y * b.z) + (a.z * b.y) + (a.w * b.x)
    };
}

/// @brief Scale a quaternion by a factor
/// @param q The quaternion
/// @param s The factor
/// @return The resulting quaternion
static inline quat scaleQ(const quat q, float s) {
    return (quat) {q.x * s, q.y * s, q.z * s, q.w * s};
}
/// @brief Get length of a quaternion
/// @param q The quaternion
/// @return The length of the quaternion
static inline float lenQ(const quat q) {
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}
/// @brief Normalize a quaternion
/// @param q The quaternion
/// @return The resulting quaternion
static inline quat normQ(const quat q) {
    return scaleQ(q, 1.0 / lenQ(q));
}
/// @brief Transpose a quaternion
/// @param q The quaternion
/// @return The resulting quaternion
static inline quat transpQ(const quat q) {
    return (quat) {q.x, -q.y, -q.z, -q.w};
}
/// @brief Invert a quaternion
/// @param q The quaternion
/// @return The resulting quaternion
static inline quat invQ(const quat q) {
    return transpQ(normQ(q));
}
/// @brief Divide two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The resulting quaternion
static inline quat divQ(const quat a, const quat b) {
    return mulQ(a, invQ(b));
}

/// @brief Exponential of a quaternion
/// @param q The quaternion
/// @return The resulting quaternion
static inline quat expQ(const quat q) {
    float ex = exp(q.x);
    float a = sqrt(q.y * q.y + q.z * q.z + q.w * q.w);
    float s = a == 0.0 ? 0.0 : ex * sin(a) / a;

    return (quat) {ex * cos(a), q.y * s, q.z * s, q.w * s};
}
/// @brief Natural logarithm of a quaternion
/// @param q The quaternion
/// @return The resulting quaternion
static inline quat lnQ(const quat q) {
    float l = lenQ(q);
    float arg = l == 0.0 ? 0.0 : acos(q.x / l) / sqrt(q.y * q.y + q.z * q.z + q.w * q.w);

    return (quat) {log(l), q.y * arg, q.z * arg, q.w * arg};
}
/// @brief Exponential of a unit quaternion
/// @param q The quaternion
/// @return The resulting quaternion
/// @note The quaternion is supposed to be unit, a.k.a of length 1. This speeds up the computation.
static inline quat lnQ_Unit(const quat q) {
    float arg = acos(q.x);
    return (quat) {0, q.y * arg, q.z * arg, q.w * arg};
}
/// @brief Power of a quaternion
/// @param q The quaternion
/// @param t the power
/// @return The resulting quaternion
static inline quat powQ(const quat q, float t) {
    return expQ(scaleQ(lnQ(q), t));
}
/// @brief Power of a unit quaternion
/// @param q The quaternion
/// @param t the power
/// @return The resulting quaternion
/// @note The quaternion is supposed to be unit, a.k.a of length 1. This speeds up the computation.
static inline quat powQ_Unit(const quat q, float t) {
    return expQ(scaleQ(lnQ_Unit(q), t));
}

static inline quat slerpQ(const quat a, const quat b, float t) {
    // c = a * (a^-1 * b)^t
    return mulQ(a, powQ(mulQ(invQ(a), b), t));
}
static inline quat slerpQ_Unit(const quat a, const quat b, float t) {
    // c = a * (a^-1 * b)^t
    return mulQ(a, powQ_Unit(mulQ(invQ(a), b), t));
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// POINTER QUATERNIONS ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////



/// @brief Create a quaternion
/// @param x x component
/// @param y y component
/// @param z z component
/// @param w w component
/// @return The newly created quaternion
quat* newQuat(float x, float y, float z, float w);
/// @brief Create a quaternion
/// @param x x component
/// @param y y component
/// @param z z component
/// @param w w component
/// @return The newly created quaternion
quat* Quat_(float x, float y, float z, float w);

/// @brief Set a quaternion
/// @param q The quaternion to set
/// @param x The new x component
/// @param y The new y component
/// @param z The new z component
/// @param w The new w component
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_(quat* restrict q, float x, float y, float z, float w);
/// @brief Set a quaternion from Euler angles
/// @param q The quaternion to set
/// @param pitch The angle around X
/// @param yaw The angle around Y
/// @param roll The angle around Z
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_Euler_(quat* restrict q, float pitch, float yaw, float roll);
/// @brief Set a quaternion from Euler angles
/// @param q The quaternion to set
/// @param v The Euler angles vector
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_EulerVector_(quat* restrict q, const vec3* v);
/// @brief Set a quaternion from angle-axis
/// @param q The quaternion to set
/// @param angle The angle around axis
/// @param axis The axis
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_AngleAxis_(quat* restrict q, float angle, const vec3* axis);
/// @brief Set a quaternion from rotation vector
/// @param q The quaternion to set
/// @param v The rotation vector
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_RotVec_(quat* restrict q, const vec3* v);
/// @brief Set a quaternion from a vector to another
/// @param q The quaternion to set
/// @param from The source
/// @param to The destination
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* setQ_FromToVec_(quat* restrict q, const vec3* from, const vec3* to);

/// @brief Create a quaternion from real and imaginary parts
/// @param r real part
/// @param i imaginary part
/// @return The newly created quaternion
quat* newQuat_V(float r, const vec3* i);
/// @brief Create a quaternion from euler angles
/// @param pitch The angle around X
/// @param yaw The angle around Y
/// @param roll The angle around Z
/// @return The newly created quaternion
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
quat* newQuat_Euler(float pitch, float yaw, float roll);
/// @brief Create a quaternion from euler angles vector
/// @param v The vector of euler angles
/// @return The newly created quaternion
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
quat* newQuat_EulerVector(const vec3* v);
/// @brief Create a quaternion from angle axis
/// @param angle The angle of rotation
/// @param axis The axis of rotation
/// @return The newly created quaternion
quat* newQuat_AngleAxis(float angle, const vec3* axis);
/// @brief Create a quaternion from rotation vector
/// @param v The rotation vector
/// @return The newly created quaternion
quat* newQuat_RotVec(const vec3* v);
/// @brief Create a quaternion from a vector to another
/// @param from The source
/// @param to The destination
/// @return The newly created quaternion
quat* newQuat_FromToVec(const vec3* from, const vec3* to);

/// @brief Free quaternion
/// @param toFree The quaternion to free
void freeQuat(quat* toFree);

/// @brief Create an identity quaternion: (1, 0, 0, 0)
/// @return The newly created identity quaternion
quat* identityQ_(void);

/// @brief Copy value of a quaternion
/// @param q The quaternion to copy
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* copyQ_(const quat* q, quat* destination);



/// @brief Add two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* addQ_(const quat* a, const quat* b, quat* destination);
/// @brief Add two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The input quaternion (left)
/// @note This opperation overrides the current value
quat* addQ_s(quat* a, const quat* b);

/// @brief Substract two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* subQ_(const quat* a, const quat* b, quat* destination);
/// @brief Substract two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The input quaternion (left)
/// @note This opperation overrides the current value
quat* subQ_s(quat* a, const quat* b);

/// @brief Multiply two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* mulQ_(const quat* a, const quat* b, quat* destination);
/// @brief Multiply two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @return The input quaternion (left)
/// @note This opperation overrides the current value
quat* mulQ_s(quat* a, const quat* b);

/// @brief Scale a quaternion be a scalar
/// @param q The quaternion to scale
/// @param s The amount
/// @param destination Where the result is stored
/// @return The destination value
/// @note Set destination to NULL for new value
quat* scaleQ_(const quat* q, float s, quat* destination);
/// @brief Scale a quaternion be a scalar
/// @param q The quaternion to scale
/// @param s The amount
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* scaleQ_s(quat* restrict q, float s);

/// @brief Length of a quaternion
/// @param q The input quaternion
/// @return The length of the quaternion (norm 2)
float lenQ_(const quat* q);
/// @brief Set quaternion length to 1
/// @param q The quaternion to normalize
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* normQ_(const quat* q, quat* restrict destination);
/// @brief Set quaternion length to 1
/// @param q The quaternion to normalize
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* normQ_s(quat* restrict q);

/// @brief Transpose a quaternion
/// @param q The quaternion to transpose
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* transpQ_(const quat* q, quat* destination);
/// @brief Transpose a quaternion
/// @param q The quaternion to transpose
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* transpQ_s(quat* restrict q);
/// @brief Invert quaternion
/// @param q The quaternion to invert
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @note If the quaternion is known to ne unit, then the inverse is equal to the transpose. Therefore it is best to use "transposeQ" for performance
/// @return The destination value
quat* invQ_(const quat* q, quat* destination);
/// @brief Invert quaternion
/// @param q The quaternion to invert
/// @note If the quaternion is known to ne unit, then the inverse is equal to the transpose. Therefore it is best to use "transposeQ" for performance
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* invQ_s(quat* restrict q);

/// @brief Divide two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* divQ_(const quat* a, const quat* b, quat* c);
/// @brief Divide two quaternions
/// @param a The left quaternion
/// @param b The right quaternion
/// @note This opperation overrides the current value
/// @return The input quaternion (left)
quat* divQ_s(quat* restrict a, const quat* b);

/// @brief Rotate a vec3 by quaternion
/// @param v The vector to rotate
/// @param q The rotation quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @note The input quaternion is assumed to be unit
/// @return The destination value
vec3* rot3Q_(const vec3* v, const quat* q, vec3* destination);
/// @brief Rotate a vec3 by quaternion
/// @param v The vector to rotate
/// @param q The rotation quaternion
/// @note This opperation overrides the current value
/// @note The input quaternion is assumed to be unit
/// @return The input vector
vec3* rot3Q_s(vec3* v, const quat* q);
/// @brief Rotate a vec3 by transposed quaternion
/// @param v The vector to rotate
/// @param q The rotation quaternion
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @note The input quaternion is assumed to be unit
/// @return The destination value
vec3* rot3TQ_(const vec3* v, const quat* q, vec3* destination);
/// @brief Rotate a vec3 by transposed quaternion
/// @param v The vector to rotate
/// @param q The rotation quaternion
/// @note This opperation overrides the current value
/// @note The input quaternion is assumed to be unit
/// @return The input vector
vec3* rot3TQ_s(vec3* v, const quat* q);

/// @brief Exponential of a quaternion
/// @param q The quaternion as argument
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* expQ_(const quat* q, quat* restrict destination);
/// @brief Exponential of a quaternion
/// @param q The quaternion as argument
/// @return The input quaternion
/// @note This opperation overrides the current value
quat* expQ_s(quat* restrict q);

/// @brief Logarithm of a quaternion
/// @param q The quaternion as argument
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* lnQ_(const quat* q, quat* restrict destination);
/// @brief Logarithm of a quaternion
/// @param q The quaternion as argument
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* lnQ_s(quat* restrict q);
/// @brief Logarithm of a unit quaternion
/// @param q The quaternion as argument
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* lnQ_Unit_(const quat* q, quat* restrict destination);
/// @brief Logarithm of a unit quaternion
/// @param q The quaternion as argument
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* lnQ_Unit_s(quat* restrict q);

/// @brief Power of a quaternion
/// @param q The quaternion in the base
/// @param t The exponent
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* powQ_(const quat* q, float t, quat* restrict destination);
/// @brief Exponential of a quaternion
/// @param q The quaternion in the base
/// @param t The exponent
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* powQ_s(quat* restrict q, float t);
/// @brief Power of a unit quaternion
/// @param q The quaternion in the base
/// @param t The exponent
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
quat* powQ_Unit_(const quat* q, float t, quat* restrict destination);
/// @brief Power of a unit quaternion
/// @param q The quaternion in the base
/// @param t The exponent
/// @note This opperation overrides the current value
/// @return The input quaternion
quat* powQ_Unit_s(quat* restrict q, float t);

/// @brief Spherical interpolation between quat a and quat b with factor t
/// @param a The start position
/// @param b The end position
/// @param t The factor (Supposed in the range [0, 1])
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @note Represents shortest path on unit hypersphere between a and b with factor t
/// @return The destination value
quat* slerpQ_(const quat* a, const quat* b, float t, quat* restrict destination);
/// @brief Spherical interpolation between quat a and quat b with factor t
/// @param a The start position
/// @param b The end position
/// @param t The factor (Supposed in the range [0, 1])
/// @note This opperation overrides the current value
/// @note Represents shortest path on unit hypersphere between a and b with factor t
/// @return The input quaternion
quat* slerpQ_s(quat* restrict a, const quat* b, float t);
/// @brief Spherical interpolation between unit quat a and unit quat b with factor t
/// @param a The start position
/// @param b The end position
/// @param t The factor (Supposed in the range [0, 1])
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @note Represents shortest path on unit hypersphere between a and b with factor t
/// @return The destination value
quat* slerpQ_Unit_(const quat* a, const quat* b, float t, quat* restrict destination);
/// @brief Spherical interpolation between unit quat a and unit quat b with factor t
/// @param a The start position
/// @param b The end position
/// @param t The factor (Supposed in the range [0, 1])
/// @note This opperation overrides the current value
/// @note Represents shortest path on unit hypersphere between a and b with factor t
/// @return The input quaternion
quat* slerpQ_Unit_s(quat* restrict a, const quat* b, float t);

/// @brief Convert quaternion to rotation vector
/// @param q The quaternion to convert
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
vec3* quatToVec3_Rot_(const quat* q, vec3* restrict destination);

/// @brief Convert quaterion to euler angles
/// @param q The quaternion to convert
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
vec3* quatToVec3_Euler_(const quat* q, vec3* restrict destination);

#endif