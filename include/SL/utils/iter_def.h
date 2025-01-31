#ifndef __SL_UTILS_ARRAY_DEF_H__
#define __SL_UTILS_ARRAY_DEF_H__

#include "array.h"
#include "list.h"
#include "../structures.h"

SL_DEFINE_ARRAY(bool);
SL_DEFINE_ARRAY(char);  SL_DEFINE_ARRAY(char8);  SL_DEFINE_ARRAY(char16); SL_DEFINE_ARRAY(char32);
SL_DEFINE_ARRAY(int);   SL_DEFINE_ARRAY(int8);   SL_DEFINE_ARRAY(int16);  SL_DEFINE_ARRAY(int32);  SL_DEFINE_ARRAY(int64);
SL_DEFINE_ARRAY(uint);  SL_DEFINE_ARRAY(uint8);  SL_DEFINE_ARRAY(uint16); SL_DEFINE_ARRAY(uint32); SL_DEFINE_ARRAY(uint64);
SL_DEFINE_ARRAY(float); SL_DEFINE_ARRAY(double);

SL_DEFINE_LIST(bool);
SL_DEFINE_LIST(char);  SL_DEFINE_LIST(char8);  SL_DEFINE_LIST(char16); SL_DEFINE_LIST(char32);
SL_DEFINE_LIST(int);   SL_DEFINE_LIST(int8);   SL_DEFINE_LIST(int16);  SL_DEFINE_LIST(int32);  SL_DEFINE_LIST(int64);
SL_DEFINE_LIST(uint);  SL_DEFINE_LIST(uint8);  SL_DEFINE_LIST(uint16); SL_DEFINE_LIST(uint32); SL_DEFINE_LIST(uint64);
SL_DEFINE_LIST(float); SL_DEFINE_LIST(double);

#include "../maths/vector.h"

#define __SL_GEN_VectorArrays(prefix) SL_DEFINE_ARRAY(prefix##vec2); SL_DEFINE_ARRAY(prefix##vec3); SL_DEFINE_ARRAY(prefix##vec4)
#define __SL_GEN_VectorLists(prefix)  SL_DEFINE_LIST (prefix##vec2); SL_DEFINE_LIST (prefix##vec3); SL_DEFINE_LIST (prefix##vec4)

__SL_GEN_VectorArrays(); __SL_GEN_VectorArrays(d); __SL_GEN_VectorArrays(i); __SL_GEN_VectorArrays(u); __SL_GEN_VectorArrays(li); __SL_GEN_VectorArrays(lu); __SL_GEN_VectorArrays(b);
__SL_GEN_VectorLists();  __SL_GEN_VectorLists(d);  __SL_GEN_VectorLists(i);  __SL_GEN_VectorLists(u);  __SL_GEN_VectorLists(li);  __SL_GEN_VectorLists(lu);  __SL_GEN_VectorLists(b);

#include "../maths/quaternion.h"

SL_DEFINE_ARRAY(quat);
SL_DEFINE_LIST (quat);

#include "../maths/matrix.h"

SL_DEFINE_ARRAY(mat);
SL_DEFINE_LIST(mat);
SL_DEFINE_ARRAY(mat2x2); SL_DEFINE_ARRAY(mat3x3); SL_DEFINE_ARRAY(mat4x4);
SL_DEFINE_LIST(mat2x2);  SL_DEFINE_LIST(mat3x3);  SL_DEFINE_LIST(mat4x4);

#endif