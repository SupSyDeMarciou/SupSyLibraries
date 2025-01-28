#ifndef __SL_UTILS_ARRAY_DEF_H__
#define __SL_UTILS_ARRAY_DEF_H__

#include "array.h"
#include "../structures.h"

SL_DEFINE_ARRAY(bool);
SL_DEFINE_ARRAY(char);  SL_DEFINE_ARRAY(char8);  SL_DEFINE_ARRAY(char16); SL_DEFINE_ARRAY(char32);
SL_DEFINE_ARRAY(int);   SL_DEFINE_ARRAY(int8);   SL_DEFINE_ARRAY(int16);  SL_DEFINE_ARRAY(int32);  SL_DEFINE_ARRAY(int64);
SL_DEFINE_ARRAY(uint);  SL_DEFINE_ARRAY(uint8);  SL_DEFINE_ARRAY(uint16); SL_DEFINE_ARRAY(uint32); SL_DEFINE_ARRAY(uint64);
SL_DEFINE_ARRAY(float); SL_DEFINE_ARRAY(double);

#include "../maths/vector.h"

#define __SL_DefineVectorArrays(prefix) SL_DEFINE_ARRAY(prefix##vec2); SL_DEFINE_ARRAY(prefix##vec3); SL_DEFINE_ARRAY(prefix##vec4)

__SL_DefineVectorArrays(); __SL_DefineVectorArrays(d); __SL_DefineVectorArrays(i); __SL_DefineVectorArrays(u); __SL_DefineVectorArrays(li); __SL_DefineVectorArrays(lu); __SL_DefineVectorArrays(b);

#include "../maths/quaternion.h"

SL_DEFINE_ARRAY(quat);

#include "../maths/matrix.h"

SL_DEFINE_ARRAY(mat);
SL_DEFINE_ARRAY(mat2x2); SL_DEFINE_ARRAY(mat3x3); SL_DEFINE_ARRAY(mat4x4);

#endif