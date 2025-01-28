#include "quaternion.h"

#include <stdlib.h>
#include <math.h>
#include "constants.h"

const quat quat_identity = (quat){1, 0, 0, 0};

quat* newQuat(float w, float x, float y, float z) {
    quat* new = (quat*) malloc(sizeof(quat));
    *new = (quat) {w, x, y, z};
    return new;
}
quat* Quat_(float w, float x, float y, float z) { return newQuat(w, x, y, z); }

quat* setQ_(quat* restrict q, float w, float x, float y, float z) {
    *q = (quat) {w, x, y, z};
    return q;
}
quat* setQ_V(quat* restrict q, float r, vec3 i) {
    *q = (quat) {r, XPD_VEC3(i)};
    return q;
}
quat* setQ_Euler_(quat* restrict q, float yaw, float pitch, float roll) {
    *q = Quat_Euler(yaw, pitch, roll);
    return q;
}
quat* setQ_EulerVector_(quat* restrict q, const vec3* v) {
    return setQ_Euler_(q, XPD_VEC3_(v));
}
quat* setQ_AngleAxis_(quat* restrict q, float angle, const vec3* axis) {
    float sinA = sin(angle *= 0.5);
    return setQ_(q, cos(angle), sinA * axis->x, sinA * axis->y, sinA * axis->z);
}
quat* setQ_RotVec_(quat* restrict q, const vec3* v) {
    float length = len3_(v);
    if (length == 0.0) return setQ_(q, 1, 0, 0, 0);

    float halfAngle = length * 0.5;
    float coeff = sin(halfAngle) / length;

    return setQ_(q, cos(halfAngle), v->x * coeff, v->y * coeff, v->z * coeff);
}
quat* setQ_FromToVec_(quat* restrict q, const vec3* from, const vec3* to) {
    vec3 axis; cross3_(from, to, &axis);
    if (axis.x || axis.y || axis.z) {
        axis = norm3(axis);
        float angle = acos(dot3_(from, to));
        return setQ_AngleAxis_(q, angle, &axis);
    }
    return copyQ_(&quat_identity, q);
}

quat* newQuat_V(float r, const vec3 i) {
    quat* new = (quat*) malloc(sizeof(quat));
    return setQ_V(new, r, i);
}
quat* newQuat_Euler(float l, float m, float n) {
    quat* new = (quat*) malloc(sizeof(quat));
    return setQ_Euler_(new, l, m, n);
}
quat* newQuat_EulerVector(const vec3* v) {
    return newQuat_Euler(v->x, v->y, v->z);
}
quat* newQuat_AngleAxis(float angle, const vec3* axis) {
    quat* new = (quat*) malloc(sizeof(quat));
    return setQ_AngleAxis_(new, angle, axis);
}
quat* newQuat_RotVec(const vec3* v) {
    quat* new = (quat*) malloc(sizeof(quat));
    return setQ_RotVec_(new, v);
}
quat* newQuat_FromToVec(const vec3* from, const vec3* to) {
    quat* new = (quat*) malloc(sizeof(quat));
    return setQ_FromToVec_(new, from, to);
}

void freeQuat(quat* q) {
    free(q);
}

quat* identityQ_() {
    return newQuat(1, 0, 0, 0);
}

quat* copyQ_(const quat* q, quat* c) {
    if (c == NULL) c = malloc(sizeof(quat));
    *c = *q;
    return c;
}



#define quatTestC if (c == NULL) c = malloc(sizeof(quat));
quat* addQ_(const quat* a, const quat* b, quat* c) { 
    quatTestC return setQ_(c, a->w + b->w, a->x + b->x, a->y + b->y, a->z + b->z);
}
quat* addQ_s(quat* a, const quat* b) {
    return setQ_(a, a->w + b->w, a->x + b->x, a->y + b->y, a->z + b->z);
}

quat* subQ_(const quat* a, const quat* b, quat* c) {
    quatTestC return setQ_(c, a->w - b->w, a->x - b->x, a->y - b->y, a->z - b->z);
}
quat* subQ_s(quat* a, const quat* b) {
    return setQ_(a, a->w - b->w, a->x - b->x, a->y - b->y, a->z - b->z);
}

quat* mulQ_(const quat* a, const quat* b, quat* c) {
    quatTestC return setQ_(c, 
        (b->w * a->w) - (b->x * a->x) - (b->y * a->y) - (b->z * a->z),
        (b->w * a->x) + (b->x * a->w) - (b->y * a->z) + (b->z * a->y),
        (b->w * a->y) + (b->x * a->z) + (b->y * a->w) - (b->z * a->x),
        (b->w * a->z) - (b->x * a->y) + (b->y * a->x) + (b->z * a->w)
    );
}
quat* mulQ_s(quat* a, const quat* b) {
    return setQ_(a, 
        (b->w * a->w) - (b->x * a->x) - (b->y * a->y) - (b->z * a->z),
        (b->w * a->x) + (b->x * a->w) - (b->y * a->z) + (b->z * a->y),
        (b->w * a->y) + (b->x * a->z) + (b->y * a->w) - (b->z * a->x),
        (b->w * a->z) - (b->x * a->y) + (b->y * a->x) + (b->z * a->w)
    );
}

quat* scaleQ_(const quat* q, float s, quat* c) { 
    quatTestC return setQ_(c, q->w * s, q->x * s, q->y * s, q->z * s);
}
quat* scaleQ_s(quat* restrict q, float s) {
    return setQ_(q, q->w * s, q->x * s, q->y * s, q->z * s);
}

float lenQ_(const quat* q) {
    return q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
}
quat* normQ_(const quat* q, quat* restrict c) {
    quatTestC
    float length = lenQ_(q);
    float invLength = length == 0.0 ? 0.0 : 1.0 / length;
    c->w = q->w * invLength;
    c->x = q->x * invLength;
    c->y = q->y * invLength;
    c->z = q->z * invLength;
    return c;
}
quat* normQ_s(quat* restrict q) {
    float length = lenQ_(q);
    float invLength = length == 0.0 ? 0.0 : 1.0 / length;
    q->w *= invLength;
    q->x *= invLength;
    q->y *= invLength;
    q->z *= invLength;
    return q;
}

quat* transpQ_(const quat* q, quat* c) {
    quatTestC return setQ_(c, q->w, -q->x, -q->y, -q->z);
}
quat* transpQ_s(quat* restrict q) {
    return setQ_(q, q->w, -q->x, -q->y, -q->z);
}

quat* invQ_(const quat* q, quat* c) {
    float div = 1.0 / lenQ_(q);
    quatTestC return setQ_(c, q->w * div, -q->x * div, -q->y * div, -q->z * div);
}
quat* invQ_s(quat* restrict q) {
    float div = 1.0 / lenQ_(q);
    return setQ_(q, q->w * div, -q->x * div, -q->y * div, -q->z * div);
}

quat* divQ_(const quat* a, const quat* b, quat* c) {
    // TODO: Implement quaternion division
    return NULL;
}
quat* divQ_s(quat* restrict a, const quat* b) {
    // TODO: Implement quaternion division
    return NULL;
}

// Rotate vec3 by quaternion
// vec3* rotateQ(const vec3* v, const quat* q, vec3* c) {
//     if (c == NULL) c = nullVec3p();
//     quat* inv = invQ(q, NULL);
//     quat* repr = newQuaternion(0, v->x, v->y, v->z);
//     repr = multQ(q, repr, repr);
//     repr = multQ(repr, inv, repr);
//     c->x = repr->y;
//     c->y = repr->z;
//     c->z = repr->w;
//     freeQuat(inv);
//     freeQuat(repr);
//     return c;
// }

vec3* rot3Q_(const vec3* v, const quat* q, vec3* c) {
    float a2 = q->w * q->w, b2 = q->x * q->x, c2 = q->y * q->y, d2 = q->z * q->z; 
    float ab = q->w*q->x, ac = q->w*q->y, ad = q->w*q->z, bc = q->x*q->y, cd = q->y*q->z, bd = q->x*q->z;

    if (c == NULL) c = null3_();
    return set3_(c,
        v->x * (a2 + b2 - c2 - d2) + 2 * ((bc+ad)*v->y + (bd-ac)*v->z),
        v->y * (a2 - b2 + c2 - d2) + 2 * ((bc-ad)*v->x + (cd+ab)*v->z),
        v->z * (a2 - b2 - c2 + d2) + 2 * ((bd+ac)*v->x + (cd-ab)*v->y)
    );
}
vec3* rot3Q_s(vec3* v, const quat* q) {
    float a2 = q->w * q->w, b2 = q->x * q->x, c2 = q->y * q->y, d2 = q->z * q->z; 
    float ab = q->w*q->x, ac = q->w*q->y, ad = q->w*q->z, bc = q->x*q->y, cd = q->y*q->z, bd = q->x*q->z;

    return set3_(v,
        v->x * (a2 + b2 - c2 - d2) + 2 * ((bc+ad)*v->y + (bd-ac)*v->z),
        v->y * (a2 - b2 + c2 - d2) + 2 * ((bc-ad)*v->x + (cd+ab)*v->z),
        v->z * (a2 - b2 - c2 + d2) + 2 * ((bd+ac)*v->x + (cd-ab)*v->y)
    );
}

vec3* rot3TQ_(const vec3* v, const quat* q, vec3* c) {
    float a2 = q->w * q->w, b2 = q->x * q->x, c2 = q->y * q->y, d2 = q->z * q->z; 
    float ab = q->w*q->x, ac = q->w*q->y, ad = q->w*q->z, bc = q->x*q->y, cd = q->y*q->z, bd = q->x*q->z;

    if (c == NULL) c = null3_();
    return set3_(c,
        v->x * (a2 + b2 - c2 - d2) + 2 * ((bc-ad)*v->y + (bd+ac)*v->z),
        v->y * (a2 - b2 + c2 - d2) + 2 * ((bc+ad)*v->x + (cd-ab)*v->z),
        v->z * (a2 - b2 - c2 + d2) + 2 * ((bd-ac)*v->x + (cd+ab)*v->y)
    );
}
vec3* rot3TQ_s(vec3* v, const quat* q) {
    float a2 = q->w * q->w, b2 = q->x * q->x, c2 = q->y * q->y, d2 = q->z * q->z; 
    float ab = q->w*q->x, ac = q->w*q->y, ad = q->w*q->z, bc = q->x*q->y, cd = q->y*q->z, bd = q->x*q->z;

    return set3_(v,
        v->x * (a2 + b2 - c2 - d2) + 2 * ((bc-ad)*v->y + (bd+ac)*v->z),
        v->y * (a2 - b2 + c2 - d2) + 2 * ((bc+ad)*v->x + (cd-ab)*v->z),
        v->z * (a2 - b2 - c2 + d2) + 2 * ((bd-ac)*v->x + (cd+ab)*v->y)
    );
}

quat* expQ_(const quat* q, quat* restrict c) {

    float ex = exp(q->w);
    float a = sqrt(q->x*q->x + q->y*q->y + q->z*q->z);
    float s = a == 0.0 ? 0.0 : ex * sin(a) / a;

    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    return setQ_(c, ex * cos(a), q->x * s, q->y * s, q->z * s);
}
quat* expQ_s(quat* restrict q) {
    float ex = exp(q->w);
    float a = sqrt(q->x*q->x + q->y*q->y + q->z*q->z);
    float s = a == 0.0 ? 0.0 : ex * sin(a) / a;

    return setQ_(q, ex * cos(a), q->x * s, q->y * s, q->z * s);
}

quat* lnQ_(const quat* q, quat* restrict c) {
    float l = lenQ_(q);
    float s = l == 0.0 ? 0.0 : acos(q->w / l) / sqrt(q->x * q->x + q->y * q->y + q->z * q->z);

    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    return setQ_(c, log(l), q->x * s, q->y * s, q->z * s);
}
quat* lnQ_s(quat* restrict q) {
    float l = lenQ_(q);
    float s = l == 0.0 ? 0.0 : acos(q->w / l) / sqrt(q->x * q->x + q->y * q->y + q->z * q->z);

    return setQ_(q, log(l), q->x * s, q->y * s, q->z * s);
}
quat* lnQ_Unit_(const quat* q, quat* restrict c) {
    float s = acos(q->w);

    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    return setQ_(c, 0, q->x * s, q->y * s, q->z * s);
}
quat* lnQ_Unit_s(quat* restrict q) {
    float s = acos(q->w);

    return setQ_(q, 0, q->x * s, q->y * s, q->z * s);
}

quat* powQ_(const quat* q, const float t, quat* restrict c) {
    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    float angle = acos(q->w) * t;
    return setQ_V(c, cos(angle), scale3(norm3(*(vec3*)&q->x), sin(angle)));
}
quat* powQ_s(quat* restrict q, const float t) {
    float angle = acos(q->w) * t;
    return setQ_V(q, cos(angle), scale3(norm3(*(vec3*)&q->x), sin(angle)));
}

// c = a * (a^-1 * b)^t
quat* slerpQ_(const quat* a, const quat* b, float t, quat* restrict c) {
    quat temp;
    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    return mulQ_(a, powQ_s(mulQ_s(invQ_(a, &temp), b), t), c);
}
quat* slerpQ_s(quat* restrict a, const quat* b, float t) {
    quat temp;
    return mulQ_s(a, powQ_s(mulQ_s(invQ_(a, &temp), b), t));
}
quat* slerpQ_Unit_(const quat* a, const quat* b, float t, quat* restrict c) {
    quat temp = quat_identity;

    if (c == NULL) c = (quat*) malloc(sizeof(quat));
    return mulQ_(a, powQ_s(mulQ_s(transpQ_(a, &temp), b), t), c);
}
quat* slerpQ_Unit_s(quat* restrict a, const quat* b, float t) {
    quat temp = quat_identity;

    return mulQ_s(a, powQ_s(mulQ_s(transpQ_(a, &temp), b), t));
}

vec3* quatToVec3_Rot_(const quat* q, vec3* restrict c) {
    float coeff = sqrt(1 - q->w*q->w);
    if (coeff == 0.0) return copy3_(&vec3_zero, c);

    float angle = 2.0 * acos(q->w);
    coeff = angle / coeff;

    if (c == NULL) return newVec3(q->x * coeff, q->y * coeff, q->z * coeff);
    return set3_(c, q->x * coeff, q->y * coeff, q->z * coeff);
}

#include <stdio.h>
#include "../utils/inout.h"
// With the help of this paper: https://pmc.ncbi.nlm.nih.gov/articles/PMC9648712/
static vec3* quatToVec3_EulerANY_(const quat* q, ivec3 s, vec3* restrict o) {
    if (o == NULL) o = null3_();

    bool notProper = s.x != s.z;
    if (!notProper) s.z = 6 - s.x - s.y;

    float eps = (s.x - s.y) * (s.y - s.z) * (s.z - s.x) * 0.5;
    
    float a = q->m[0], b = q->m[s.x], c = q->m[s.y], d = q->m[s.z] * eps;
    if (notProper) {
        a -= q->m[s.y];
        b += q->m[s.z] * eps;
        c += q->m[0];
        d -= q->m[s.x];
    }

    float t1, t3, t1h = 0.0;
    float t2 = acos(2.0 * (a*a + b*b) / (a*a + b*b + c*c + d*d) - 1.0);
    float tp = atan2(b, a);
    float tm = atan2(d, c);

    if (fabs(t2) < 1e-4) {
        t1 = t1h;
        t3 = 2.0 * tp - t1h;
    }
    else if (fabs(t2 - PI) < 1e-4) {
        t1 = t1h;
        t3 = 2.0 * tm + t1h;
    }
    else {
        t1 = tp - tm;
        t3 = tp + tm;
    }

    if (notProper) {
        t3 *= eps;
        t2 -= PI/2;
    }

    return set3_(o, fmod(t1 + TAU, TAU), fmod(t2 + TAU, TAU), fmod(t3 + TAU, TAU));
}

// Corresponds to "quatToVec3_EulerANY_(q, Ivec3(2, 1, 3), o)"
vec3* quatToVec3_Euler_(const quat* q, vec3* restrict o) {
    if (o == NULL) o = null3_();

    const float eps = -1.0;
    
    float a = q->w - q->x;
    float b = q->y + q->z * eps;
    float c = q->x + q->w;
    float d = q->z * eps - q->y;

    float t1, t3, t1h = 0.0;
    float t2 = acos(2.0 * (a*a + b*b) / (a*a + b*b + c*c + d*d) - 1.0);
    float tp = atan2(b, a);
    float tm = atan2(d, c);

    if (t2 < 1e-8) {
        t1 = t1h;
        t3 = 2.0 * tp - t1h;
    }
    else if (PI - t2 < 1e-8) {
        t1 = t1h;
        t3 = 2.0 * tm + t1h;
    }
    else {
        t1 = tp - tm;
        t3 = tp + tm;
    }

    return set3_(o, fmod(t1 + TAU, TAU), fmod(t2 - PI/2 + TAU, TAU), fmod(t3 * eps + TAU, TAU));
}