#ifndef __SL_MATHS_MATRIX_H__
#define __SL_MATHS_MATRIX_H__

#include "vector.h"
#include "quaternion.h"
#include "../structures.h"
#include "../utils/inout.h"

// Undefine this macro to remove bounds checks
#define SL_MATHS_MATRIX_SAFE



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// * * *  GENERIC MATRICES  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/// @brief Structure for holding arbitrarily sized matrix
/// @note Stored in Row-Major order
typedef struct Matrix {
    uint r, c;
    float m[];
} mat;

/// @brief Get the (i, j)th element of a matrix
/// @param x The matrix
/// @param i The row
/// @param j The column
#define val(x, i, j) (x)->m[(i) + (x)->r*(j)]

/// @brief Get the (i, j)th element of a transposed matrix
/// @param x The matrix
/// @param i The row
/// @param j The column
#define valT(x, i, j) (x)->m[(j) + (x)->r*(i)]

/// @brief Create a matrix of size n * m
/// @param rows The number of rows
/// @param columns The number of columns
/// @param values The initial values in column-major order
/// @note Set values to NULL for zero matrix
/// @return The newly created matrix
mat* newMatrix(uint rows, uint columns, const float* values);

/// @brief Free a matrix
/// @param toFree The matrix to free
void freeMatrix(mat* toFree);

/// @brief Copy a matrix into another
/// @param source The matrix to copy
/// @param destination The matrix to which the data is copied
/// @return The destination matrix
mat* copyMat_(const mat* source, mat* restrict destination);

/// @brief Add two matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* addMat_(const mat* a, const mat* b, mat* restrict destination);
/// @brief Add two matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat* addMat_s(mat* restrict a, const mat* b);

/// @brief Subtract two matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* subMat_(const mat* a, const mat* b, mat* restrict destination);
/// @brief Subtract two matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat* subMat_s(mat* restrict a, const mat* b);

/// @brief Multiply two matrices
/// @param a Left matrix
/// @param ta If a is transposed
/// @param b Right matrix
/// @param tb If b is transposed 
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* mulMat_(const mat* a, bool ta, const mat* b, bool tb, mat* restrict destination);

/// @brief Scale a matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* scaleMat_(const mat* m, float s, mat* restrict destination);
/// @brief Scale a matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @return The input matrix
/// @note This opperation overrides the current value
mat* scaleMat_s(mat* restrict m, float s);

/// @brief Scale a matrice's rows
/// @param m The matrix
/// @param s The scalar vector
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* scaleMat_Row_(const mat* m, const float* s, mat* restrict destination);
/// @brief Scale a matrice's rows
/// @param m The matrix
/// @param s The scalar vector
/// @return The input matrix
/// @note This opperation overrides the current value
mat* scaleMat_Row_s(mat* restrict m, const float* s);
/// @brief Scale a matrice's columns
/// @param m The matrix
/// @param s The scalar vector
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* scaleMat_Col_(const mat* m, const float* s, mat* restrict destination);
/// @brief Scale a matrice's columns
/// @param m The matrix
/// @param s The scalar vector
/// @return The input matrix
/// @note This opperation overrides the current value
mat* scaleMat_Col_s(mat* restrict m, const float* s);

/// @brief Set an entire matrix to a value
/// @param m The matrix to replace
/// @param f The value to be set
void setMat_(mat* restrict m, float f);
/// @brief Set a matrices values
/// @param m The matrix to replace
/// @param values The values to be set
void setMat_V_(mat* restrict m, const float* values);

/// @brief Print a matrix to stdout
/// @param m The matrix to print
void printMat_(const mat* m);

/// @brief Check wether a matrix is filled with only zeros
/// @param m The matrix to check
/// @return If it is filled with only zeros
bool matIsZero(const mat* m);

/// @brief Multiply a matrix by a vector
/// @param m The matrix
/// @param v The vector
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
float* mulMat_LVec_(const mat* m, const float* v, float* restrict destination);
/// @brief Multiply transpose of a matrix by a vector
/// @param m The matrix
/// @param v The vector
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
float* mulMatT_LVec_(const mat* m, const float* v, float* restrict c);

/// @brief Transpose a matrix
/// @param m The matrix to transpose
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat* transpMat_(const mat* m, mat* restrict destination);

// Define a matrix of specific size: mat(r)x(c)
#define defineMat(rows, columns) typedef struct Matrix##rows##x##columns {uint r, c; float m[rows*columns];} mat##rows##x##columns;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// * * *  SQUARE MATRICES  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  n x n //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


/// @brief Create a square matrix
/// @param n The size
/// @return The newly created matrix
mat* newMatrix_Square(uint n);

/// @brief Create a zero square matrix
/// @param n The size
/// @return The newly created zero matrix
mat* nullMat_Square_(uint n);

/// @brief Create a identity matrix
/// @param n The size
/// @return The newly created identity matrix
mat* identityMat_(uint n);

/// @brief Trace of a square matrix
/// @param m The matrix
/// @return The trace of the matrix
float traceMat_(const mat* m);

// /!\ NOT IMPLEMENTED
// Determinant of mat[n]x[n] m
// float detMat_(const mat* m);

// /!\ NOT IMPLEMENTED
// Inverse of mat[n]x[n] m
// mat* invMat_(const mat* m, mat* c);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  2 x 2 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


defineMat(2, 2)

/// @brief Create a 2x2 matrix
/// @return The newly created matrix
static inline mat2x2 createMat2x2() { return (mat2x2) {2, 2, .m = {0, 0, 0, 0}}; }
/// @brief Create a filled 2x2 matrix
/// @param v The initial values
/// @return The newly created matrix
static inline mat2x2 createMat2x2_V(float v[4]) { return (mat2x2) {2, 2, .m = {v[0], v[1], v[2], v[3]}}; }
static inline mat2x2 identity2x2() { return createMat2x2_V((float[4]){1, 0, 0, 1}); }
static inline mat2x2 add2x2(mat2x2 a, mat2x2 b) { return createMat2x2_V((float[4]){a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2], a.m[3] + b.m[3]}); }
static inline mat2x2 sub2x2(mat2x2 a, mat2x2 b) { return createMat2x2_V((float[4]){a.m[0] - b.m[0], a.m[1] - b.m[1], a.m[2] - b.m[2], a.m[3] - b.m[3]}); }
static inline mat2x2 mul2x2(mat2x2 a, mat2x2 b) { return createMat2x2_V((float[4]){ a.m[0] * b.m[0] + a.m[2] * b.m[1], a.m[1] * b.m[0] + a.m[3] * b.m[1], a.m[0] * b.m[2] + a.m[2] * b.m[3], a.m[1] * b.m[2] + a.m[3] * b.m[3]}); }
static inline mat2x2 mul2x2T(mat2x2 a, mat2x2 b) { return createMat2x2_V((float[4]){ a.m[0] * b.m[0] + a.m[2] * b.m[2], a.m[1] * b.m[0] + a.m[3] * b.m[2], a.m[0] * b.m[1] + a.m[2] * b.m[3], a.m[1] * b.m[1] + a.m[3] * b.m[3]}); }
static inline mat2x2 scale2x2(mat2x2 a, float s) { return createMat2x2_V((float[4]){a.m[0] * s, a.m[1] * s, a.m[2] * s, a.m[3] * s}); }
static inline float trace2x2(mat2x2 m) { return m.m[0] + m.m[3]; }
static inline float det2x2(mat2x2 m) { return m.m[0] * m.m[3] - m.m[1] * m.m[2]; }
static inline mat2x2 inv2x2(mat2x2 m) { float invDet = 1.0 / det2x2(m); return createMat2x2_V((float[4]){m.m[3] * invDet, -m.m[2] * invDet, -m.m[1] * invDet, m.m[0] * invDet}); }



/// @brief Create a square matrix of size 2
/// @return The newly created matrix
mat2x2* newMat2x2();
/// @brief Create a square matrix of size 3 with values
/// @param values Initial values for this matrix
/// @note Set value to NULL for null matrix
/// @return The newly created matrix
mat2x2* newMat2x2_V(const float values[2][2]);

/// @brief Set a mat2x2
/// @param m The matrix to set
/// @param values The values to assign
/// @return The input matrix
/// @note This opperation overrides the current value
mat2x2* set2x2_V(mat2x2* restrict m, const float values[4]);
/// @brief Set a mat2x2 2D rotation matrix
/// @param m The matrix to set
/// @param angle The rotation in radians
/// @return The input matrix
/// @note This opperation overrides the current value
mat2x2* set2x2_Rotation_(mat2x2* restrict m, float angle);

/// @brief Create a 2D rotation matrix
/// @param angle The rotation in radians
/// @return The newly created matrix
mat2x2* newMat2x2_Rotation(float angle);



///@brief Create an identity mat2x2
/// @return The newly created identity mat2x2
mat2x2* identity2x2_();

#define XPD_MAT2X2(x) (x).m[0], (x).m[1], (x).m[2], (x).m[3]
#define XPD_MAT2X2_(x) (x)->m[0], (x)->m[1], (x)->m[2], (x)->m[3]

/// @brief Add two 2x2 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* add2x2_(const mat2x2* a, const mat2x2* b, mat2x2* destination);
/// @brief Add two 2x2 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat2x2* add2x2_s(mat2x2* restrict a, const mat2x2* b);

/// @brief Subtract two 2x2 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* sub2x2_(const mat2x2* a, const mat2x2* b, mat2x2* destination);
/// @brief Subtract two 2x2 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat2x2* sub2x2_s(mat2x2* restrict a, const mat2x2* b);

/// @brief Multiply two 2x2 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* mul2x2_(const mat2x2* a, const mat2x2* b, mat2x2* destination);
/// @brief Multiply a 2x2 matrix with a transposed 2x2 matrix
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* mul2x2T_(const mat2x2* a, const mat2x2* b, mat2x2* destination);

/// @brief Scale a 2x2 matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* scale2x2_(const mat2x2* m, float s, mat2x2* destination);
/// @brief Scale a matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @return The input matrix
/// @note This opperation overrides the current value
mat2x2* scale2x2_s(mat2x2* restrict m, float s);

/// @brief Trace of a 2x2 matrix
/// @param m The matrix
/// @return The trace of the matrix
float trace2x2_(const mat2x2* m);
/// @brief Discriminant of a 2x2 matrix by values
/// @param m11 Coeff (1, 1)
/// @param m12 Coeff (1, 2)
/// @param m21 Coeff (2, 1)
/// @param m22 Coeff (2, 2)
/// @return The discriminant of the matrix
float det2x2_v(float m11, float m12, float m21, float m22);
/// @brief Discriminant of a 2x2 matrix
/// @param m The matrix
/// @return The discriminant of the matrix
float det2x2_(const mat2x2* m);

/// @brief Invert a 2x2 matrix
/// @param m The matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat2x2* inv2x2_(const mat2x2* m, mat2x2* destination);

vec2 mul2x2_2(const mat2x2* m, const vec2* v);
vec2* mul2x2_2_(const mat2x2* m, const vec2* v, vec2* c);
vec2* mul2x2_2_s(const mat2x2* m, vec2* v);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  3 x 3 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


defineMat(3, 3)


static inline mat3x3 createMat3x3() { return (mat3x3) {3, 3, .m = {0, 0, 0, 0}}; }
static inline mat3x3 createMat3x3_v(float v[9]) { return (mat3x3) {3, 3, .m = {v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]}}; }
static inline mat3x3 identity3x3() { return createMat3x3_v((float[9]){1, 0, 0, 1, 0, 0, 1}); }
static inline mat3x3 add3x3(mat3x3 a, mat3x3 b) { return createMat3x3_v((float[9]){a.m[0] + b.m[0], a.m[1] + b.m[1], a.m[2] + b.m[2], a.m[3] + b.m[3], a.m[4] + b.m[4], a.m[5] + b.m[5], a.m[6] + b.m[6], a.m[7] + b.m[7], a.m[8] + b.m[8]}); }
static inline mat3x3 sub3x3(mat3x3 a, mat3x3 b) { return createMat3x3_v((float[9]){a.m[0] - b.m[0], a.m[1] - b.m[1], a.m[2] - b.m[2], a.m[3] - b.m[3], a.m[4] - b.m[4], a.m[5] - b.m[5], a.m[6] - b.m[6], a.m[7] - b.m[7], a.m[8] - b.m[8]}); }
static inline mat3x3 mul3x3(mat3x3 a, mat3x3 b) { float c[9] = {0}; for (uint i = 0; i < 3; i++) for (uint j = 0; j < 9; j+=3) c[i + j] = a.m[i + 0] * b.m[0 + j] + a.m[i + 3] * b.m[1 + j] + a.m[i + 6] * b.m[2 + j]; return createMat3x3_v(c); }
static inline mat3x3 mul3x3T(mat3x3 a, mat3x3 b) { float c[9] = {0}; for (uint i = 0; i < 3; i++) for (uint j = 0; j < 3; j++) c[i + j*3] = a.m[i + 0] * b.m[j + 0] + a.m[i + 3] * b.m[j + 3] + a.m[i + 6] * b.m[j + 6]; return createMat3x3_v(c); }
static inline mat3x3 scale3x3(mat3x3 a, float s) { return createMat3x3_v((float[9]){a.m[0] * s, a.m[1] * s, a.m[2] * s, a.m[3] * s, a.m[4] * s, a.m[5] * s, a.m[6] * s, a.m[7] * s, a.m[8] * s}); }
static inline float trace3x3(mat3x3 m) { return m.m[0] + m.m[3] + m.m[6]; }
static inline float det3x3(mat3x3 m) { return m.m[0] * m.m[4] * m.m[8] + m.m[1] * m.m[5] * m.m[6] + m.m[2] * m.m[3] * m.m[7] - m.m[6] * m.m[4] * m.m[2] - m.m[7] * m.m[5] * m.m[0] - m.m[8] * m.m[3] * m.m[1]; }
static inline mat3x3 inv3x3(mat3x3 m) { failWithError("NOT IMPLEMENTED"); }



/// @brief Create a square matrix of size 3
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3();
/// @brief Create a square matrix of size 3 with values
/// @param values Initial values for this matrix
/// @note Set value to NULL for null matrix
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3_v(const float values[3][3]);

/// @brief Set a mat3x3 from Euler angles
/// @param m The matrix to set
/// @param pitch The angle around X
/// @param yaw The angle around Y
/// @param roll The angle around Z
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* set3x3_Euler_(mat3x3* restrict m, float yaw, float pitch, float roll);
/// @brief Set a mat3x3 from Euler vector
/// @param m The matrix to set
/// @param v The Euler vector
/// @note Rotations are applied in this order: (yaw, pitch, roll)
/// @note Thank you wikipedia
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* set3x3_EulerVector_(mat3x3* restrict m, const vec3* v);
/// @brief Set a mat3x3 from rotation vector
/// @param m The matrix to set
/// @param v The rotation vector
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* set3x3_RotVec_(mat3x3* restrict m, const vec3* v);
/// @brief Set a mat3x3 from cross product
/// @param m The matrix to set
/// @param v The vector to cross
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* set3x3_Cross_(mat3x3* restrict m, const vec3* v);
/// @brief Set a mat3x3 from normal transform
/// @param m The matrix to set
/// @param q The rotation
/// @param s The scale
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* set3x3_TransformNormal_(mat3x3* m, const quat* q, const vec3* s);

/// @brief Create a new mat3x3 from Euler angles
/// @param m The matrix to set
/// @param pitch The angle around X
/// @param yaw The angle around Y
/// @param roll The angle around Z
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3_Euler(float yaw, float pitch, float roll);
/// @brief Create a new mat3x3 from Euler vector
/// @param m The matrix to set
/// @param v The Euler vector
/// @note Rotations are applied in this order: (pitch, yaw, roll)
/// @note Thank you wikipedia
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3_EulerVector(const vec3* v);
/// @brief Create a new mat3x3 from rotation vector
/// @param m The matrix to set
/// @param v The rotation vector
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3_RotVec(const vec3* v);
/// @brief Create a new mat3x3 from cross product
/// @param m The matrix to set
/// @param v The vector to cross
/// @return The newly created 3x3 matrix
mat3x3* newMat3x3_Cross(const vec3* v);



///@brief Create an identity mat3x3
/// @return The newly created identity mat3x3
mat3x3* identity3x3_();

#define XPD_MAT3X3(x) (x).m[0], (x).m[1], (x).m[2], (x).m[3], (x).m[4], (x).m[5], (x).m[6], (x).m[7], (x).m[8]
#define XPD_MAT3X3_(x) (x)->m[0], (x)->m[1], (x)->m[2], (x)->m[3], (x)->m[4], (x)->m[5], (x)->m[6], (x)->m[7], (x)->m[8]

/// @brief Add two 3x3 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* add3x3_(const mat3x3* a, const mat3x3* b, mat3x3* destination);
/// @brief Add two 3x3 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat3x3* add3x3_s(mat3x3* restrict a, const mat3x3* b);

/// @brief Subtract two 3x3 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* sub3x3_(const mat3x3* a, const mat3x3* b, mat3x3* destination);
/// @brief Subtract two 3x3 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat3x3* sub3x3_s(mat3x3* restrict a, const mat3x3* b);

/// @brief Multiply two 3x3 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* mul3x3_(const mat3x3* a, const mat3x3* b, mat3x3* destination);
/// @brief Multiply a 3x3 matrix with a transposed 3x3 matrix
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* mul3x3T_(const mat3x3* a, const mat3x3* b, mat3x3* destination);

/// @brief Scale a 3x3 matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* scale3x3_(const mat3x3* m, float s, mat3x3* destination);
/// @brief Scale a matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @return The input matrix
/// @note This opperation overrides the current value
mat3x3* scale3x3_s(mat3x3* restrict m, float s);

/// @brief Trace of a 3x3 matrix
/// @param m The matrix
/// @return The trace of the matrix
float trace3x3_(const mat3x3* m);
/// @brief Discriminant of a 3x3 matrix by values
/// @param m11 Coeff (1, 1)
/// @param m12 Coeff (1, 2)
/// @param m21 Coeff (2, 1)
/// @param m22 Coeff (2, 2)
/// @return The discriminant of the matrix
float det3x3_v(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
/// @brief Discriminant of a 3x3 matrix
/// @param m The matrix
/// @return The discriminant of the matrix
float det3x3_(const mat3x3* m);
/// @brief Invert a 3x3 matrix
/// @param m The matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* inv3x3_(const mat3x3* m, mat3x3* destination);

vec3 mul3x3_3(const mat3x3* m, const vec3* v);
vec3* mul3x3_3_(const mat3x3* m, vec3* v, vec3* c);
vec3* mul3x3_3_s(const mat3x3* m, vec3* v);

/// @brief Convert a quaternion to a 3D rotation matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat3x3* quatTo3x3_(const quat* q, mat3x3* destination);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  4 x 4 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


defineMat(4, 4)


/// @brief Create a square matrix of size 4
/// @return The newly created 4x4 matrix
mat4x4* newMat4x4();
/// @brief Create a square matrix of size 4 with values
/// @param values Initial values for this matrix
/// @note Set value to NULL for null matrix
/// @return The newly created 4x4 matrix
mat4x4* newMat4x4_v(const float values[4][4]);

// Convert a position, rotation and scale transform into a transform matrix
// (i) Set destination to NULL for new matrix
mat4x4* set4x4_Transform_(mat4x4* m, const vec3* p, const quat* q, const vec3* s);
// Create projection matrix for asymetric frustum
// (i) Set destination to NULL for new matrix
mat4x4* set4x4_ProjectionAsym_(mat4x4* m, float nearPlane, float farPlane, float left, float right, float top, float bottom);
// Create projection matrix for symetric frustum
// (i) Set destination to NULL for new matrix
mat4x4* set4x4_ProjectionSym_(mat4x4* m, float nearPlane, float farPlane, float width, float height);



/// @brief Create an identity matrix of size 4
/// @return The newly created 4x4 identity matrix
mat4x4* identity4x4_();
/// @brief Create a 4x4 matrix ON STACK
/// @return The newly created 4x4 matrix
#define STATIC_MAT4X4() ((mat4x4){4, 4, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
/// @brief Create a 4x4 matrix ON STACK
/// @param values The initial values in the matrix (row major)
/// @return The newly created 4x4 matrix
#define STATIC_MAT4X4V(values) ((mat4x4){.r = 4, .c = 4, .m = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15]}})

#define XPD_MAT4X4(x) (x).m[0], (x).m[1], (x).m[2], (x).m[3], (x).m[4], (x).m[5], (x).m[6], (x).m[7], (x).m[8], (x).m[9], (x).m[10], (x).m[11], (x).m[12], (x).m[13], (x).m[14], (x).m[15]
#define XPD_MAT4X4_(x) (x)->m[0], (x)->m[1], (x)->m[2], (x)->m[3], (x)->m[4], (x)->m[5], (x)->m[6], (x)->m[7], (x)->m[8], (x)->m[9], (x)->m[10], (x)->m[11], (x)->m[12], (x)->m[13], (x)->m[14], (x)->m[15]

/// @brief Add two 4x4 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* add4x4_(const mat4x4* a, const mat4x4* b, mat4x4* destination);
/// @brief Add two 4x4 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat4x4* add4x4_s(mat4x4* restrict a, const mat4x4* b);

/// @brief Subtract two 4x4 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* sub4x4_(const mat4x4* a, const mat4x4* b, mat4x4* destination);
/// @brief Subtract two 4x4 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @return The input matrix (left)
/// @note This opperation overrides the current value
mat4x4* sub4x4_s(mat4x4* restrict a, const mat4x4* b);

/// @brief Multiply two 4x4 matrices
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* mul4x4_(const mat4x4* a, const mat4x4* b, mat4x4* destination);
/// @brief Multiply a 4x4 matrix with a transposed 4x4 matrix
/// @param a Left matrix
/// @param b Right matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* mul4x4T_(const mat4x4* a, const mat4x4* b, mat4x4* destination);

/// @brief Scale a 4x4 matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* scale4x4_(const mat4x4* m, float s, mat4x4* destination);
/// @brief Scale a matrix by factor
/// @param m The matrix to scale
/// @param s The scalar
/// @return The input matrix
/// @note This opperation overrides the current value
mat4x4* scale4x4_s(mat4x4* restrict m, float s);

/// @brief Trace of a 4x4 matrix
/// @param m The matrix
/// @return The trace of the matrix
float trace4x4_(const mat4x4* m);
/// @brief Discriminant of a 4x4 matrix
/// @param m The matrix
/// @return The discriminant of the matrix
float det4x4_(const mat4x4* m);
/// @brief Invert a 4x4 matrix
/// @param m The matrix
/// @param destination Where the result is stored
/// @note Set destination to NULL for new value
/// @return The destination value
mat4x4* inv4x4_(const mat4x4* m, mat4x4* destination);



vec4 mul4x4_4(const mat4x4* m, const vec4* v);
vec4* mul4x4_4_(const mat4x4* m, const vec4* v, vec4* c);
vec4* mul4x4_4_s(const mat4x4* m, vec4* v);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// * * *  LINEAR EQUATIONS  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Add row a scaled by s to row b
mat* addSMatRow(mat* restrict m, uint a, uint b, float s);
// Add column a scaled by s to column b
mat* addSMatCol(mat* restrict m, uint a, uint b, float s);

// Row reduction algorithm (I think)
// (i) Solves the system of linear equations represented by systemMatrix and rightSide
// /!\ rightSide must be of length systemMatrix.r
// /!\ O(n^2 * m), really slow for big systems (n = systemMatrix.r, m = systemMatrix.c)
void solveSystemGaussPivot(mat* systemMatrix, float* rightSide);
float* solveSystemGaussSeidel(mat* leftMember, float* rightMember, float maxError, float* x, uint maxIter);

#endif