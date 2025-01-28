#include "matrix.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// * * *  GENERIC MATRICES  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



mat* newMatrix(uint rows, uint columns, const float* values) {

    mat* new = (mat*)calloc(1, sizeof(int) * 2 + sizeof(float) * columns * rows);
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = rows;
    new->c = columns;

    if (!values) return new;

    float* newm = new->m;
    for (uint i = 0, size = rows * columns; i < size; i++) newm[i] = values[i];

    return new;
}

void freeMatrix(mat* toFree) {
    free(toFree);
}

#define matFailDiffSize(a, b, message) { if ((a)->r != (b)->r || (a)->c != (b)->c) SL_throwError(message); }

mat* copyMat_(const mat* m, mat* restrict c) {
    if (!c) return newMatrix(m->r, m->c, m->m);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else matFailDiffSize(m, c, "Cannot copy matrix to other with different size.");
    #endif
    const float* mm = m->m;
    float* cm = c->m;
    memcpy(cm, mm, c->r * c->c * sizeof(float));
    // for (size_t size = (size_t)(cm + c->r * c->c); (size_t)cm < size; cm++, mm++) *cm = *mm;
    return c;
}

mat* addMat_(const mat* a, const mat* b, mat* restrict c) {
    #ifdef __SL_MATHS_MATRIX_SAFE
    matFailDiffSize(a, b, "Cannot add matrices with different sizes.");
    #endif
    if (!c) c = newMatrix(a->r, a->c, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else matFailDiffSize(a, c, "Destination matrix is not correctly sized.");
    #endif
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;
    for (size_t size = (size_t)(cm + c->r * c->c); (size_t)cm < size; am++, bm++, cm++) *cm = *am + *bm;
    return c;
}
mat* addMat_s(mat* restrict a, const mat* b) {
    #ifdef __SL_MATHS_MATRIX_SAFE
    matFailDiffSize(a, b, "Cannot add matrices with different sizes.");
    #endif
    float* am = a->m;
    const float* bm = b->m;
    for (size_t size = (size_t)(am + a->r * a->c); (size_t)am < size; am++, bm++) *am += *bm;
    return a;
}

mat* subMat_(const mat* a, const mat* b, mat* restrict c) {
    #ifdef __SL_MATHS_MATRIX_SAFE
    matFailDiffSize(a, b, "Cannot add matrices with different sizes.");
    #endif
    if (!c) c = newMatrix(a->r, a->c, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else matFailDiffSize(a, c, "Destination matrix is not correctly sized.");
    #endif
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;
    for (size_t size = (size_t)(cm + c->r * c->c); (size_t)cm < size; am++, bm++, cm++) *cm = *am - *bm;
    return c;
}
mat* subMat_s(mat* restrict a, const mat* b) {
    #ifdef __SL_MATHS_MATRIX_SAFE
    matFailDiffSize(a, b, "Cannot add matrices with different sizes.");
    #endif
    float* am = a->m;
    const float* bm = b->m;
    for (size_t size = (size_t)(am + a->r * a->c); (size_t)am < size; am++, bm++) *am -= *bm;
    return a;
}

static mat* MmultMat(const mat* a, const mat* b, mat* restrict c) {
    uint R = a->r, C = b->c, D = a->c;
    #ifdef __SL_MATHS_MATRIX_SAFE
    if (D != b->r) SL_throwError("Cannot multiply matrices with first's width (a.c) different from second's height (b.r).");
    #endif
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (c->r != R || c->c != C) SL_throwError("Cannot store result of multiplication in matrix with height (c.r) different from first's height (a.r) and width (c.c) different from second's width (b.c).");
    #endif

    const float* am = a->m; uint ar = a->r;
    const float* bm = b->m; uint br = b->r;
    float* cm = c->m;

    for (uint i = 0, DR = D * ar, CR = C * br; i < R; i++, DR++) {
        for (uint ck = i, bj = 0; bj < CR; ck += R, bj += br) {
            float sum = 0;
            for (uint ak = i, bk = bj; ak < DR; ak += ar, bk++) sum += am[ak] * bm[bk];
            cm[ck] = sum;
        }
    }

    return c;
}
static mat* MmultMatT(const mat* a, const mat* b, mat* restrict c) {
    uint R = a->r, C = b->r, D = a->c;
    #ifdef __SL_MATHS_MATRIX_SAFE
    if (D != b->c) SL_throwError("Cannot multiply matrices with first's width (a.c) different from second's height (b.c).");
    #endif
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (c->r != R || c->c != C) SL_throwError("Cannot store result of multiplication in matrix with height (c.r) different from first's height (a.r) and width (c.c) different from second's width (b.c).");
    #endif

    const float* am = a->m; uint ar = a->r;
    const float* bm = b->m; uint br = b->r;
    float* cm = c->m;

    for (uint i = 0, DR = D * ar; i < R; i++, DR++) {
        for (uint j = 0, ck = i; j < C; j++, ck += R) {
            float sum = 0;
            for (uint ak = i, bk = j; ak < DR; ak += ar, bk += br) sum += am[ak] * bm[bk];
            cm[ck] = sum;
        }
    }

    return c;
}
static mat* MmultTMat(const mat* a, const mat* b, mat* restrict c) {
    uint R = a->c, C = b->c, D = a->r;
    #ifdef __SL_MATHS_MATRIX_SAFE
    if (D != b->r) SL_throwError("Cannot multiply matrices with first's width (a.c) different from second's height (b.c).");
    #endif
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (c->r != R || c->c != C) SL_throwError("Cannot store result of multiplication in matrix with height (c.r) different from first's height (a.r) and width (c.c) different from second's width (b.c).");
    #endif

    const float* am = a->m; uint ar = a->r;
    const float* bm = b->m; uint br = b->r;
    float* cm = c->m;

    // for (uint i = 0; i < R; i++)
    // for (uint j = 0; j < C; j++) {
    //     uint ai = ar * i, bj = br * j;
    //     float sum = am[ai] * bm[bj];
    //     for (uint k = 1; k < D; k++) sum += am[k + ai] * bm[k + bj];
    //     cm[i + R * j] = sum;
    // }

    for (uint i = 0; i < R; i++)
    for (uint j = 0; j < C; j++) {
        float sum = valT(a, i, 0) * val(b, 0, j);
        for (uint k = 1; k < D; k++) sum += valT(a, i, k) * val(b, k, j);
        val(c, i, j) = sum;
    }

    return c;
}
static mat* MmultTMatT(const mat* a, const mat* b, mat* restrict c) {
    uint R = a->c, C = b->r, D = a->r;
    #ifdef __SL_MATHS_MATRIX_SAFE
    if (D != b->c) SL_throwError("Cannot multiply matrices with first's width (a.r) different from second's height (b.c).");
    #endif
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (c->r != R || c->c != C) SL_throwError("Cannot store result of multiplication in matrix with height (c.r) different from first's height (a.r) and width (c.c) different from second's width (b.c).");;
    #endif

    const float* am = a->m; uint ar = a->r;
    const float* bm = b->m; uint br = b->r;
    float* cm = c->m;

    // for (uint i = 0; i < R; i++)
    // for (uint j = 0; j < C; j++) {
    //     uint ai = ar * i;
    //     float sum = am[ai] * bm[j];
    //     for (uint k = 1, bk = br; k < D; k++, bk += br) sum += am[k + ai] * bm[j + bk];
    //     cm[i + R * j] = sum;
    // }

    for (uint i = 0; i < R; i++)
    for (uint j = 0; j < C; j++) {
        float sum = valT(a, i, 0) * valT(b, 0, j);
        for (uint k = 1; k < D; k++) sum += valT(a, i, k) * valT(b, k, j);
        val(c, i, j) = sum;
    }

    return c;
}
mat* mulMat_(const mat* a, bool ta, const mat* b, bool tb, mat* restrict c) {
    if (ta) {
        if (tb) return MmultTMatT(a, b, c);
        return MmultTMat(a, b, c);
    }
    if (tb) return MmultMatT(a, b, c);
    return MmultMat(a, b, c);
}

mat* scaleMat_(const mat* m, float s, mat* restrict c) {
    uint R = m->r, C = m->c;
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (C != c->c || R != c->r) SL_throwError("Destination matrix is not correctly sized.");
    #endif
    const float* mm = m->m;
    float* cm = c->m;
    for (size_t size = (size_t)(cm + R * C); (size_t)cm < size; cm++, mm++) *cm = *mm * s;
    return c;
}
mat* scaleMat_s(mat* restrict m, float s) {
    float* mm = m->m;
    for (size_t size = (size_t)(mm + m->r * m->c); (size_t)mm < size; mm++) *mm *= s;
    return m;
}

mat* scaleMat_Row_(const mat* m, const float* s, mat* restrict c) {
    uint R = m->r, C = m->c;
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (C != c->c || R != c->r) SL_throwError("Destination matrix is not correctly sized.");
    #endif
    const float* mm = m->m;
    float* cm = c->m;
    uint CR = C*R;
    for (uint i = 0; i < R; i++, CR++) {
        float v = s[i];
        for (uint j = i; j < CR; j+=R) cm[j] = mm[j] * v;
    }
    return c;
}
mat* scaleMat_Row_s(mat* restrict m, const float* s) {
    uint R = m->r, CR = m->c * R;
    float* mm = m->m;
    for (uint i = 0; i < R; i++, CR++) {
        float v = s[i];
        for (uint j = i; j < CR; j+=R) mm[j] *= v;
    }
    return m;
}
mat* scaleMat_Col_(const mat* m, const float* s, mat* restrict c) {
    uint R = m->r, C = m->c;
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (C != c->c || R != c->r) SL_throwError("Destination matrix is not correctly sized.");
    #endif
    const float* mm = m->m;
    float* cm = c->m;
    uint CR = C*R;
    for (uint j = 0; j < C; j++, CR++) {
        float v = s[j];
        for (uint i = j; i < CR; i+=C) cm[j] = mm[j] * v;
    }
    return c;
}
mat* scaleMat_Col_s(mat* restrict m, const float* s) {
    uint C = m->c, CR = m->r * C;
    float* mm = m->m;
    for (uint j = 0; j < C; j++, CR++) {
        float v = s[j];
        for (uint i = j; i < CR; i+=C) mm[j] *= v;
    }
    return m;
}

void setMat_(mat* restrict m, float f) {
    float* mm = m->m;
    for (size_t size = (size_t)(mm + m->r * m->c); (size_t)mm < size; mm++) *mm = f;
}
void setMat_V_(mat* restrict m, const float* values) {
    float* mm = m->m;
    for (size_t size = (size_t)(mm + m->r * m->c); (size_t)mm < size; mm++, values++) *mm = *values;
}

#include <stdio.h>

void printMat_(const mat* m) {
    for (int j = 0; j < m->c * (9 + 1) + 3; j++) putchar('-');
    putchar('\n');
    for (int i = 0; i < m->r; i++) {
        putchar('[');
        for (int j = 0; j < m->c; j++) {
            if (__signbitf(val(m, i, j))) printf(" %f", val(m, i, j));
            else printf("  %f", val(m, i, j));
        }
        puts(" ]");
    }
    for (int j = 0; j < m->c * (9 + 1) + 3; j++) putchar('-');
    putchar('\n');
}

bool matIsZero(const mat* m) {
    const float* mm = m->m;
    for (size_t size = (size_t)(mm + m->r * m->c); (size_t)mm < size; mm++) if (!*mm) return false;
    return true;
}

float* mulMat_LVec_(const mat* m, const float* v, float* restrict c) {
    if (!c) c = (float*)malloc(sizeof(float) * m->r);
    for (uint i = 0; i < m->r; i++) {
        float sum = val(m, i, 0) * v[0];
        for (uint k = 1; k < m->c; k++) c[i] += val(m, i, k) * v[k];
        c[i] = sum;
    }
    return c;
}
float* mulMatT_LVec_(const mat* m, const float* v, float* restrict c) {
    if (!c) c = (float*)malloc(sizeof(float) * m->c);
    for (uint i = 0; i < m->c; i++) {
        float sum = valT(m, i, 0) * v[0];
        for (uint k = 1; k < m->r; k++) c[i] += valT(m, i, k) * v[k];
        c[i] = sum;
    }
    return c;
}

mat* transpMat_(const mat* m, mat* restrict c) {
    uint R = m->r, C = m->c;
    if (!c) c = newMatrix(R, C, NULL);
    #ifdef __SL_MATHS_MATRIX_SAFE
    else if (C != c->r || R != c->c) SL_throwError("Destination matrix is not correctly sized.");
    #endif
    const float* mm = m->m;
    float* cm = c->m;

    for (uint i = 0, ti = 0; i < R; i++, ti += R)
    for (uint j = 0, tj = i; j < C; j++, tj += R) {
        cm[j + ti] = mm[tj];
    }

    return c;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// * * *  SQUARE MATRICES  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  n x n //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////



#define mallocSq(n) malloc(sizeof(int) * 2 + sizeof(float) * n * n)
#define callocSq(n) calloc(1, sizeof(int) * 2 + sizeof(float) * n * n)
mat* newMatrix_Square(uint n) {
    mat* new = (mat*)mallocSq(n);
    new->c = new->r = n;

    return new;
}

mat* nullMat_Square_(uint n) {
    mat* new = (mat*)callocSq(n);
    new->c = new->r = n;

    return new;
}

mat* identityMat_(uint n) {
    mat* I = nullMat_Square_(n);
    for (int i = 0; i < n; i++) {
        val(I, i, i) = 1;
    }
    return I;
}

float traceMat_(const mat* m) {
    float tr = 0.0;
    for (int i = 0; i < m->c; i++) {
        tr += val(m, i, i);
    }
    return tr;
}

float detMat_(const mat* m) {
    SL_throwError("NOT IMPLEMENTED");
    // TODO: implement this shit
}

mat* invMat_(const mat* m, mat* c) {
    SL_throwError("NOT IMPLEMENTED");
    // TODO: implement this shit
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  2 x 2 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


mat2x2* newMat2x2() {
    mat2x2* new = (mat2x2*)malloc(sizeof(mat2x2));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = new->c = 2;
    return new;
}
mat2x2* mat2x2_() { return newMat2x2(); }
mat2x2* newMat2x2_V(const float values[2][2]) {
    mat2x2* new = (mat2x2*)calloc(1, sizeof(mat2x2));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = new->c = 2;

    if (!values) return new;

    new->m[0] = values[0][0];
    new->m[1] = values[1][0];
    new->m[2] = values[0][1];
    new->m[3] = values[1][1];

    return new;
}

mat2x2* set2x2_V(mat2x2* restrict m, const float values[4]) {
    m->m[0] = values[0];
    m->m[1] = values[1];
    m->m[2] = values[2];
    m->m[3] = values[3];
    return m;
}
mat2x2* set2x2_Rotation_(mat2x2* restrict m, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    m->m[0] = cosA;
    m->m[1] = sinA;
    m->m[2] = -sinA;
    m->m[3] = cosA;
    return m;
}

mat2x2* newMat2x2_Rotation(float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return newMat2x2_V((float[2][2]){{cosA, sinA}, {-sinA, cosA}});
}


mat2x2* identity2x2_() {
    mat2x2* new = malloc(sizeof(mat2x2));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = new->c = 2;

    new->m[0] = new->m[3] = 1;
    new->m[1] = new->m[2] = 0;

    return new;
}

mat2x2* add2x2_(const mat2x2* a, const mat2x2* b, mat2x2* c) {
    if (!c) c = newMat2x2();
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;
    cm[0] = am[0] + bm[0];
    cm[1] = am[1] + bm[1];
    cm[2] = am[2] + bm[2];
    cm[3] = am[3] + bm[3];
    return c;
}
mat2x2* add2x2_s(mat2x2* restrict a, const mat2x2* b) {
    float* am = a->m;
    const float* bm = b->m;
    am[0] += bm[0];
    am[1] += bm[1];
    am[2] += bm[2];
    am[3] += bm[3];
    return a;
}

mat2x2* sub2x2_(const mat2x2* a, const mat2x2* b, mat2x2* c) {
    if (!c) c = newMat2x2();
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;
    cm[0] = am[0] - bm[0];
    cm[1] = am[1] - bm[1];
    cm[2] = am[2] - bm[2];
    cm[3] = am[3] - bm[3];
    return c;
}
mat2x2* sub2x2_s(mat2x2* restrict a, const mat2x2* b) {
    float* am = a->m;
    const float* bm = b->m;
    am[0] -= bm[0];
    am[1] -= bm[1];
    am[2] -= bm[2];
    am[3] -= bm[3];
    return a;
}

mat2x2* mul2x2_(const mat2x2* a, const mat2x2* b, mat2x2* c) {
    if (!c) c = newMat2x2();
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;
    cm[0] = am[0] * bm[0] + am[2] * bm[1];
    cm[1] = am[1] * bm[0] + am[3] * bm[1];
    cm[2] = am[0] * bm[2] + am[2] * bm[3];
    cm[3] = am[1] * bm[2] + am[3] * bm[3];
    return c;
}
mat2x2* mul2x2T_(const mat2x2* a, const mat2x2* b, mat2x2* c) {
    if (!c) c = newMat2x2();
    val(c, 0, 0) = val(a, 0, 0)*val(b, 0, 0) + val(a, 0, 1)*val(b, 0, 1);
    val(c, 0, 1) = val(a, 0, 0)*val(b, 1, 0) + val(a, 0, 1)*val(b, 1, 1);
    val(c, 1, 0) = val(a, 1, 0)*val(b, 0, 0) + val(a, 1, 1)*val(b, 0, 1);
    val(c, 1, 1) = val(a, 1, 0)*val(b, 1, 0) + val(a, 1, 1)*val(b, 1, 1);
    return c;
}

mat2x2* scale2x2_(const mat2x2* m, float s, mat2x2* c) {
    if (!c) c = newMat2x2();
    const float* mm = m->m;
    float* cm = c->m;
    cm[0] = mm[0] * s;
    cm[1] = mm[1] * s;
    cm[2] = mm[2] * s;
    cm[3] = mm[3] * s;
    return c;
}
mat2x2* scale2x2_s(mat2x2* restrict m, float s) {
    float* mm = m->m;
    mm[0] *= s;
    mm[1] *= s;
    mm[2] *= s;
    mm[3] *= s;
    return m;
}

float trace2x2_(const mat2x2* m) {
    const float* mm = m->m;
    return mm[0] + mm[3];
}
float det2x2_v(float m11, float m12, float m21, float m22) {
    return m11*m22 - m12*m21;
}
float det2x2_(const mat2x2* m) {
    const float* mm = m->m;
    return mm[0] * mm[3] - mm[1] * mm[2];
}

mat2x2* inv2x2_(const mat2x2* m, mat2x2* c) {
    float det = det2x2_(m);
    if (det == 0.0) SL_throwError("Matrix is not inversible.");

    if (!c) c = newMat2x2();

    const float* mm = m->m;
    float invDet = 1.0 / det;
    float m11 = mm[3] * invDet;
    float m12 = -mm[2] * invDet;
    float m21 = -mm[1] * invDet;
    float m22 = mm[0] * invDet;

    float* cm = c->m;
    cm[0] = m11;
    cm[1] = m21;
    cm[2] = m12;
    cm[3] = m22;

    return c;
}

vec2 mul2x2_2(const mat2x2* m, const vec2* v) {
    const float* mm = m->m;
    return Vec2(mm[0] * v->x + mm[2] * v->y, mm[1] * v->x + mm[3] * v->y);
}
vec2* mul2x2_2_(const mat2x2* m, const vec2* v, vec2* c) {
    const float* mm = m->m;
    if (!c) return newVec2(mm[0] * v->x + mm[2] * v->y, mm[1] * v->x + mm[3] * v->y);
    return set2_(c, mm[0] * v->x + mm[2] * v->y, mm[1] * v->x + mm[3] * v->y);
}
vec2* mul2x2_2_s(const mat2x2* m, vec2* v) {
    const float* mm = m->m;
    return set2_(v, mm[0] * v->x + mm[2] * v->y, mm[1] * v->x + mm[3] * v->y);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  3 x 3 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


static void quatTo3x3_values(const quat* q, float* m) {
    float wx = q->w*q->x, wy = q->w*q->y, wz = q->w*q->z, xy = q->x*q->y, yz = q->y*q->z, xz = q->x*q->z;
    float w2 = q->w*q->w, x2 = q->x*q->x, y2 = q->y*q->y, z2 = q->z*q->z;

    m[0] = w2 + x2 - y2 - z2; m[1] = 2*(xy - wz);       m[2] = 2*(xz + wy);
    m[3] = 2*(xy + wz);       m[4] = w2 - x2 + y2 - z2; m[5] = 2*(yz - wx);
    m[6] = 2*(xz - wy);       m[7] = 2*(yz + wx);       m[8] = w2 - x2 - y2 + z2;
}
mat3x3* newMat3x3() {
    mat3x3* new = (mat3x3*)malloc(sizeof(mat3x3));
    if (!new) { SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!"); return NULL; }
    new->r = new->c = 3;

    return new;
}
mat3x3* newMat3x3_v(const float values[3][3]) {
    mat3x3* new = (mat3x3*)calloc(1, sizeof(mat3x3));
    if (!new) { SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!"); return NULL; }
    new->r = new->c = 3;

    if (!values) return new;

    float* m = new->m;
    m[0] = values[0][0];
    m[1] = values[1][0];
    m[2] = values[2][0];
    m[3] = values[0][1];
    m[4] = values[1][1];
    m[5] = values[2][1];
    m[6] = values[0][2];
    m[7] = values[1][2];
    m[8] = values[2][2];

    return new;
}

mat3x3* set3x3_Euler_(mat3x3* restrict m, float yaw, float pitch, float roll) {
    float cosY = cos(yaw);
    float sinY = sin(yaw);
    float cosP = cos(pitch);
    float sinP = sin(pitch);
    float cosR = cos(roll);
    float sinR = sin(roll);

    val(m, 0, 0) = cosY*cosP;   val(m, 0, 1) = cosY*sinP*sinR - sinY*cosR;  val(m, 0, 2) = cosY*sinP*cosR + sinY*sinR;
    val(m, 1, 0) = sinY*cosP;   val(m, 1, 1) = sinY*sinP*sinR + cosY*cosR;  val(m, 1, 2) = sinY*sinP*cosR - cosY*sinR;
    val(m, 2, 0) = -sinP;       val(m, 2, 1) = cosP*sinR;                   val(m, 2, 2) = cosP*cosR;
    return m;
}
mat3x3* set3x3_EulerVector_(mat3x3* restrict m, const vec3* v) {
    return set3x3_Euler_(m, v->x, v->y, v->z);
}
mat3x3* set3x3_RotVec_(mat3x3* restrict m, const vec3* v) {
    float angle = len3_(v);
    vec3 n = angle == 0.0 ? vec3_forw : scale3(*v, 1.0 / angle);

    float c = cos(angle * 0.5), s = sin(angle * 0.5);
    float nc = 1.0 - c;

    float nx = nc * v->x, ny = nc * v->y, nz = nc * v->z;
    float sx = s * v->x, sy = s * v->y, sz = s * v->z;
    float nxy = v->y * nx, nyz = v->y * nz, nzx = v->z * nx;

    m->m00 = c + v->x * nx;   m->m01 = nxy - sz;        m->m02 = nzx + sy;   
    m->m10 = nxy + sz;        m->m11 = c + v->y * ny;   m->m12 = nyz - sx;
    m->m20 = nzx - sy;        m->m21 = nyz + sx;        m->m22 = c + v->z * nz;
    return m;
}
mat3x3* set3x3_Cross_(mat3x3* restrict m, const vec3* v) {
    m->m00 =  0.0 ; m->m10 = -v->z; m->m20 =  v->y;
    m->m01 =  v->z; m->m11 =  0.0 ; m->m21 = -v->x;
    m->m02 = -v->y; m->m12 =  v->x; m->m22 =  0.0 ;
    return m;
}
mat3x3* set3x3_TransformNormal_(mat3x3* m, const quat* q, const vec3* s) {

    float qm[9]; quatTo3x3_values(q, qm);
    val(m, 0, 0) = qm[0] * s->x; val(m, 0, 1) = qm[3] * s->y; val(m, 0, 2) = qm[6] * s->z;
    val(m, 1, 0) = qm[1] * s->x; val(m, 1, 1) = qm[4] * s->y; val(m, 1, 2) = qm[7] * s->z;
    val(m, 2, 0) = qm[2] * s->x; val(m, 2, 1) = qm[5] * s->y; val(m, 2, 2) = qm[8] * s->z;
    mat3x3 temp = inv3x3(createMat3x3_v(qm));

    return (mat3x3*)transpMat_(&temp.v, &m->v);
}

mat3x3* newMat3x3_Euler(float yaw, float pitch, float roll) {
    mat3x3* m = newMat3x3();
    return set3x3_Euler_(m, yaw, pitch, roll);
}
mat3x3* newMat3x3_EulerVector(const vec3* v) {
    mat3x3* m = newMat3x3();
    return set3x3_EulerVector_(m, v);
}
mat3x3* newMat3x3_RotVec(const vec3* v) {
    mat3x3* m = newMat3x3();
    return set3x3_RotVec_(m, v);
}
mat3x3* newMat3x3_Cross(const vec3* v) {
    mat3x3* m = newMat3x3();
    return set3x3_Cross_(m, v);
}



mat3x3* identity3x3_() {
    return newMat3x3_v((float[3][3]){{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
}

mat3x3* add3x3_(const mat3x3* a, const mat3x3* b, mat3x3* c) {
    if (!c) c = newMat3x3();

    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;

    cm[0] = am[0] + bm[0];
    cm[1] = am[1] + bm[1];
    cm[2] = am[2] + bm[2];
    cm[3] = am[3] + bm[3];
    cm[4] = am[4] + bm[4];
    cm[5] = am[5] + bm[5];
    cm[6] = am[6] + bm[6];
    cm[7] = am[7] + bm[7];
    cm[8] = am[8] + bm[8];

    return c;
}
mat3x3* add3x3_s(mat3x3* restrict a, const mat3x3* b) {
    float* am = a->m;
    const float* bm = b->m;

    am[0] += bm[0];
    am[1] += bm[1];
    am[2] += bm[2];
    am[3] += bm[3];
    am[4] += bm[4];
    am[5] += bm[5];
    am[6] += bm[6];
    am[7] += bm[7];
    am[8] += bm[8];

    return a;
}

mat3x3* sub3x3_(const mat3x3* a, const mat3x3* b, mat3x3* c) {
    if (!c) c = newMat3x3();

    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;

    cm[0] = am[0] - bm[0];
    cm[1] = am[1] - bm[1];
    cm[2] = am[2] - bm[2];
    cm[3] = am[3] - bm[3];
    cm[4] = am[4] - bm[4];
    cm[5] = am[5] - bm[5];
    cm[6] = am[6] - bm[6];
    cm[7] = am[7] - bm[7];
    cm[8] = am[8] - bm[8];

    return c;
}
mat3x3* sub3x3_s(mat3x3* restrict a, const mat3x3* b) {
    float* am = a->m;
    const float* bm = b->m;

    am[0] -= bm[0];
    am[1] -= bm[1];
    am[2] -= bm[2];
    am[3] -= bm[3];
    am[4] -= bm[4];
    am[5] -= bm[5];
    am[6] -= bm[6];
    am[7] -= bm[7];
    am[8] -= bm[8];

    return a;
}

mat3x3* mul3x3_(const mat3x3* a, const mat3x3* b, mat3x3* c) {
    if (!c) c = newMat3x3();
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
        val(c, i, j) = val(a, i, 0) * val(b, 0, j) + val(a, i, 1) * val(b, 1, j) + val(a, i, 2) * val(b, 2, j);
    }
    return c;
}
mat3x3* mul3x3T_(const mat3x3* a, const mat3x3* b, mat3x3* c) {
    if (!c) c = newMat3x3();
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
        val(c, i, j) = val(a, i, 0) * valT(b, 0, j) + val(a, i, 1) * valT(b, 1, j) + val(a, i, 2) * valT(b, 2, j);
    }
    return c;
}

mat3x3* scale3x3_(const mat3x3* m, float s, mat3x3* c) {
    if (!c) c = newMat3x3();

    const float* mm = m->m;
    float* cm = c->m;

    cm[0] = mm[0] * s;
    cm[1] = mm[1] * s;
    cm[2] = mm[2] * s;
    cm[3] = mm[3] * s;
    cm[4] = mm[4] * s;
    cm[5] = mm[5] * s;
    cm[6] = mm[6] * s;
    cm[7] = mm[7] * s;
    cm[8] = mm[8] * s;

    return c;
}
mat3x3* scale3x3_s(mat3x3* restrict m, float s) {
    float* mm = m->m;

    mm[0] *= s;
    mm[1] *= s;
    mm[2] *= s;
    mm[3] *= s;
    mm[4] *= s;
    mm[5] *= s;
    mm[6] *= s;
    mm[7] *= s;
    mm[8] *= s;

    return m;
}

float trace3x3_(const mat3x3* m) {
    return val(m, 0, 0) + val(m, 1, 1) + val(m, 2, 2);
}
float det3x3_v(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) {
    return m11*m22*m33 + m21*m32*m13 + m31*m12*m23 - m13*m22*m31 - m23*m32*m11 - m33*m12*m21;
}
float det3x3_(const mat3x3* m) {
    return det3x3_v(XPD_MAT3X3_(m));
}

mat3x3* inv3x3_(const mat3x3* m, mat3x3* c) {
    if (!c) c = newMat3x3();

    float minors[3][3];
    for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
        minors[i][j] = 
            val(m, (i == 0), (j == 0)) * val(m, 1 + (i < 2), 1 + (j < 2))
            - val(m, (i == 0), 1 + (j < 2)) * val(m, 1 + (i < 2), (j == 0));
    }

    float det = val(m, 0, 0) * minors[0][0] - val(m, 1, 0) * minors[1][0] + val(m, 2, 0) * minors[2][0];
    if (det == 0.0) SL_throwError("Matrix is not inversible.");

    float invDet = 1.0 / det;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) val(c, i, j) = minors[j][i] * invDet * ((i + j) % 2 ? -1 : 1);

    return c;
}
mat3x3* inv3x3_s(mat3x3* restrict m) { return inv3x3_(m, m); }

mat3x3* transp3x3_(const mat3x3* m, mat3x3* c) {
    c->m00 = m->m00;
    c->m01 = m->m10;
    c->m02 = m->m20;
    c->m10 = m->m01;
    c->m11 = m->m11;
    c->m12 = m->m21;
    c->m20 = m->m02;
    c->m21 = m->m12;
    c->m22 = m->m22;
    return c;
}
mat3x3* transp3x3_s(mat3x3* restrict m) {
    float m01 = m->m01;
    m->m01 = m->m10;
    m->m10 = m01;

    float m02 = m->m02;
    m->m02 = m->m20;
    m->m20 = m02;

    float m12 = m->m12;
    m->m12 = m->m21;
    m->m21 = m12;

    return m;
}

vec3 mul3x3_3(const mat3x3* m, const vec3* v) {
    return Vec3(
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z
    );
}
vec3* mul3x3_3_(const mat3x3* m, vec3* v, vec3* c) {
    if (!c) return newVec3(
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z
    );
    return set3_(c,
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z
    );
}
vec3* mul3x3_3_s(const mat3x3* m, vec3* v) {
    return set3_(v,
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z
    );
}



mat3x3* quatTo3x3_(const quat* q, mat3x3* m) {
    if (!m) m = newMat3x3();

    float qm[9]; quatTo3x3_values(q, qm);
    
    m->m00 = qm[0];
    m->m10 = qm[1];
    m->m20 = qm[2];
    m->m01 = qm[3];
    m->m11 = qm[4];
    m->m21 = qm[5];
    m->m02 = qm[6];
    m->m12 = qm[7];
    m->m22 = qm[8];

    return m;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// MATRIX  4 x 4 //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


mat4x4* newMat4x4() {
    mat4x4* new = (mat4x4*)malloc(sizeof(mat4x4));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = new->c = 4;

    return new;
}
mat4x4* newMat4x4_v(const float values[4][4]) {
    mat4x4* new = (mat4x4*)malloc(sizeof(mat4x4));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate matrix!");
    new->r = new->c = 4;

    if (values) for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) val(new, i, j) = values[i][j];
    else for (int i = 0; i < 16; new->m[i++] = 0)

    return new;
}

mat4x4* set4x4_Transform_(mat4x4* m, const vec3* p, const quat* q, const vec3* s) {

    float qm[9]; quatTo3x3_values(q, qm);

    val(m, 0, 0) = qm[0] * s->x; val(m, 0, 1) = qm[3] * s->y; val(m, 0, 2) = qm[6] * s->z; val(m, 0, 3) = p->x;
    val(m, 1, 0) = qm[1] * s->x; val(m, 1, 1) = qm[4] * s->y; val(m, 1, 2) = qm[7] * s->z; val(m, 1, 3) = p->y;
    val(m, 2, 0) = qm[2] * s->x; val(m, 2, 1) = qm[5] * s->y; val(m, 2, 2) = qm[8] * s->z; val(m, 2, 3) = p->z;
    val(m, 3, 0) = 0.0;          val(m, 3, 1) = 0.0;          val(m, 3, 2) = 0.0;          val(m, 3, 3) = 1.0;

    return m;
}
mat4x4* set4x4_ProjectionAsym_(mat4x4* m, float nearPlane, float farPlane, float left, float right, float top, float bottom) {

    float* mm = m->m;
    for (size_t s = (size_t)(mm + 16); (size_t)mm < s; mm++) *mm = 0.0;

    val(m, 0, 0) = 2*nearPlane / (right - left);                    val(m, 0, 2) = (right + left) / (right - left);
    val(m, 1, 1) = 2*nearPlane / (top - bottom);                    val(m, 1, 2) = (top + bottom) / (top - bottom);
    val(m, 2, 2) = (farPlane + nearPlane) / (farPlane - nearPlane); val(m, 2, 3) = -2.0*(farPlane * nearPlane) / (farPlane - nearPlane);
    val(m, 3, 2) = 1.0;

    return m;
}
mat4x4* set4x4_ProjectionSym_(mat4x4* m, float nearPlane, float farPlane, float width, float height) {
    
    float* mm = m->m;
    for (size_t s = (size_t)(mm + 16); (size_t)mm < s; mm++) *mm = 0.0;

    val(m, 0, 0) = nearPlane / width;
    val(m, 1, 1) = nearPlane / height;
    val(m, 2, 2) = (farPlane + nearPlane) / (farPlane - nearPlane); val(m, 2, 3) = -2*(farPlane * nearPlane) / (farPlane - nearPlane);
    val(m, 3, 2) = 1.0;
    
    return m;
}



mat4x4* identity4x4_() {
    return newMat4x4_v((float[4][4]){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
}

mat4x4* add4x4_(const mat4x4* a, const mat4x4* b, mat4x4* c) {
    if (!c) c = newMat4x4();
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;

    for (size_t s = (size_t)(am + 16); (size_t)am < s; am++, bm++, cm++) *cm = *am + *bm;
    return c;
}
mat4x4* add4x4_s(mat4x4* restrict a, const mat4x4* b) {
    float* am = a->m;
    const float* bm = b->m;

    for (size_t s = (size_t)(am + 16); (size_t)am < s; am++, bm++) *am += *bm;
    return a;
}

mat4x4* sub4x4_(const mat4x4* a, const mat4x4* b, mat4x4* c) {
    if (!c) c = newMat4x4();
    const float* am = a->m;
    const float* bm = b->m;
    float* cm = c->m;

    for (size_t s = (size_t)(am + 16); (size_t)am < s; am++, bm++, cm++) *cm = *am - *bm;
    return c;
}
mat4x4* sub4x4_s(mat4x4* restrict a, const mat4x4* b) {
    float* am = a->m;
    const float* bm = b->m;

    for (size_t s = (size_t)(am + 16); (size_t)am < s; am++, bm++) *am -= *bm;
    return a;
}

mat4x4* mul4x4_(const mat4x4* a, const mat4x4* b, mat4x4* c) {
    if (!c) c = newMat4x4();
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        val(c, i, j) = 0;
        for (int k = 0; k < 4; k++) val(c, i, j) += val(a, i, k) * val(b, k, j);
    }
    return c;
}
mat4x4* mul4x4T_(const mat4x4* a, const mat4x4* b, mat4x4* c) {
    if (!c) c = newMat4x4();
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        val(c, i, j) = 0;
        for (int k = 0; k < 4; k++) val(c, i, j) += val(a, i, k) * valT(b, k, j);
    }
    return c;
}

mat4x4* scale4x4_(const mat4x4* m, float s, mat4x4* c) {
    if (!c) c = newMat4x4();
    const float* mm = m->m;
    float* cm = c->m;

    for (size_t s = (size_t)(mm + 16); (size_t)mm < s; mm++, cm++) *cm = *mm * s;
    return c;
}
mat4x4* scale4x4_s(mat4x4* restrict m, float s) {
    float* mm = m->m;

    for (size_t s = (size_t)(mm + 16); (size_t)mm < s; mm++) *mm *= s;
    return m;
}

float trace4x4_(const mat4x4* m) {
    const float* mm = m->m;
    return mm[0] + mm[5] + mm[10] + mm[15];
}
float det4x4_(const mat4x4* m) {
    float min1 = val(m, 0, 0) * det3x3_v(val(m, 1, 1), val(m, 1, 2), val(m, 1, 3), val(m, 2, 1), val(m, 2, 2), val(m, 2, 3), val(m, 3, 1), val(m, 3, 2), val(m, 3, 3));
    float min2 = val(m, 0, 1) * det3x3_v(val(m, 1, 0), val(m, 1, 2), val(m, 1, 3), val(m, 2, 0), val(m, 2, 2), val(m, 2, 3), val(m, 3, 0), val(m, 3, 2), val(m, 3, 3));
    float min3 = val(m, 0, 2) * det3x3_v(val(m, 1, 0), val(m, 1, 1), val(m, 1, 3), val(m, 2, 0), val(m, 2, 1), val(m, 2, 3), val(m, 3, 0), val(m, 3, 1), val(m, 3, 3));
    float min4 = val(m, 0, 3) * det3x3_v(val(m, 1, 0), val(m, 1, 1), val(m, 1, 2), val(m, 2, 0), val(m, 2, 1), val(m, 2, 2), val(m, 3, 0), val(m, 3, 1), val(m, 3, 2));

    return min1 - min2 + min3 - min4;
}
mat4x4* inv4x4_(const mat4x4* m, mat4x4* c) {
    SL_throwError("NOT IMPLEMENTED");
    // if (!c) c = newMatrix4x4();

    // float minors[4][4];
    // for (int i = 0; i < 4; i++)
    // for (int j = 0; j < 4; j++) {
    //     minors[i][j] = det3x3val(
    //         m->values[(i + 1) % 4][(j + 1) % 4], m->values[(i + 2) % 4][(j + 1) % 4], m->values[(i + 3) % 4][(j + 1) % 4],
    //         m->values[(i + 1) % 4][(j + 2) % 4], m->values[(i + 2) % 4][(j + 2) % 4], m->values[(i + 3) % 4][(j + 2) % 4],
    //         m->values[(i + 1) % 4][(j + 3) % 4], m->values[(i + 2) % 4][(j + 3) % 4], m->values[(i + 3) % 4][(j + 3) % 4]
    //     );
    // }

    // float det = m->values[0][0] * minors[0][0] - m->values[1][0] * minors[1][0] + m->values[2][0] * minors[2][0] - m->values[3][0] * minors[3][0];
    // if (det == 0.0) failWithError("Matrix is not inversible.");

    // float invDet = 1.0 / det;
    // for (int i = 0; i < 4; i++)
    // for (int j = 0; j < 4; j++) {
    //     c->values[i][j] = minors[i][j] * invDet;
    // }

    // return c;
}



vec4 mul4x4_4(const mat4x4* m, const vec4* v) {
    return Vec4(
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z + val(m, 0, 3) * v->w,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z + val(m, 1, 3) * v->w,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z + val(m, 2, 3) * v->w,
        val(m, 3, 0) * v->x + val(m, 3, 1) * v->y + val(m, 3, 2) * v->z + val(m, 3, 3) * v->w
    );
}
vec4* mul4x4_4_(const mat4x4* m, const vec4* v, vec4* c) {
    if (!c) return newVec4(
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z + val(m, 0, 3) * v->w,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z + val(m, 1, 3) * v->w,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z + val(m, 2, 3) * v->w,
        val(m, 3, 0) * v->x + val(m, 3, 1) * v->y + val(m, 3, 2) * v->z + val(m, 3, 3) * v->w
    );
    return set4_(c,
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z + val(m, 0, 3) * v->w,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z + val(m, 1, 3) * v->w,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z + val(m, 2, 3) * v->w,
        val(m, 3, 0) * v->x + val(m, 3, 1) * v->y + val(m, 3, 2) * v->z + val(m, 3, 3) * v->w
    );
}
vec4* mul4x4_4_s(const mat4x4* m, vec4* v) {
    return set4_(v,
        val(m, 0, 0) * v->x + val(m, 0, 1) * v->y + val(m, 0, 2) * v->z + val(m, 0, 3) * v->w,
        val(m, 1, 0) * v->x + val(m, 1, 1) * v->y + val(m, 1, 2) * v->z + val(m, 1, 3) * v->w,
        val(m, 2, 0) * v->x + val(m, 2, 1) * v->y + val(m, 2, 2) * v->z + val(m, 2, 3) * v->w,
        val(m, 3, 0) * v->x + val(m, 3, 1) * v->y + val(m, 3, 2) * v->z + val(m, 3, 3) * v->w
    );
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// * * *  LINEAR EQUATIONS  * * * ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



mat* addSMatRow(mat* restrict m, uint row, uint toAdd, float s) {
    for (int i = 0; i < m->c; i++) val(m, row, i) += val(m, toAdd, i) * s;
    return m;
}

mat* addSMatCol(mat* restrict m, uint column, uint toAdd, float s) {
    for (int j = 0; j < m->r; j++) val(m, j, column) += val(m, j, toAdd) * s;
    return m;
}


// Row reduction algorithm (I think)
// /!\ O(n^3), really slow for big systems (n = systemMatrix.r)
void solveSystemGaussPivot(mat* systemMatrix, float* rightSide) {

    uint r = systemMatrix->r;
    uint c = systemMatrix->c;
    #ifdef __SL_MATHS_MATRIX_SAFE
    if (r > c) SL_throwError("System is not solvable.");
    if (r < c) SL_throwError("System is not uniquely solvable."); // Should it be an error?
    #endif

    // Make diagonal 1 and triangular
    for (uint t = 0; t < c; t++) {

        float vtt = val(systemMatrix, t, t);
        if (vtt == 0.0) { // We have to find another factor for term t
            uint nt = 0;
            for (; nt < r && val(systemMatrix, nt, t) == 0.0; nt++);

            // The entire column is 0
            #ifdef __SL_MATHS_MATRIX_SAFE
            if (nt >= r) SL_throwError("System is not uniquely solvable."); // Should it be an error?
            #endif

            // Set (t, t) to one using this new-found row
            float l = 1.0 / val(systemMatrix, nt, t);
            for (uint j = t; j < c; j++) val(systemMatrix, t, j) += l * val(systemMatrix, nt, j);
            rightSide[t] += rightSide[nt] * l;
        }
        else { // Just divide the entire row
            float l = 1.0 / vtt;
            for (uint j = t; j < c; j++) val(systemMatrix, t, j) *= l;
            rightSide[t] *= l;
        }

        // Set all value under (t, t) to zero
        for (uint i = t + 1; i < r; i++) {
            float l = val(systemMatrix, i, t); // Scalar for entire row
            if (l == 0.0) continue; // There is nothing to remove here

            for (uint j = t; j < r; j++) val(systemMatrix, i, j) -= val(systemMatrix, t, j) * l;
            rightSide[i] -= rightSide[t] * l;
        }
    }

    // Make diagonal
    for (uint j = c - 1; j > 0; j--) {
        for (uint i = 0; i < j; i++) {
            rightSide[i] -= val(systemMatrix, i, j) * rightSide[j];
            val(systemMatrix, i, j) = 0.0;
        }
    }

    // Tada!!!
}

float* solveSystemGaussSeidel(mat* leftMember, float* rightMember, float maxError, float* x, uint maxIter) {

    int size = leftMember->r;
    if (leftMember->c != size) SL_throwError("System is not solvable (Not square matrix)");
    size = leftMember->r;

    if (!x) x = (float*)malloc(sizeof(float) * size);

    for (uint k = 0; k < maxIter; k++) {
        // Calc next generation
        for (uint i = 0; i < size; i++) {
            float new = rightMember[i];

            uint j = 0;
            for (; j < i; j++) new -= val(leftMember, i, j) * x[j]; // x[j] k+1
            for (j++; j < size; j++) new -= val(leftMember, i, j) * x[j]; // x[j] k
            
            float vii = val(leftMember, i, i);
            if (vii == 0.0) SL_throwError("System is not solvable (Zeros in diagonal)");
            x[i] = new / vii;
        }

        // Calc error
        float e = 0.0;
        for (uint i = 0; i < size; i++) {
            float dist = -rightMember[i];
            for (uint j = 0; j < size; j++) dist += val(leftMember, i, j) * x[j];
            float newError = fabs(dist);
            e = __max(e, newError);
        }
        if (e <= maxError) return x;
    }

    return x;
}