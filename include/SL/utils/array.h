#ifndef __SL_UTILS_ARRAY_H__
#define __SL_UTILS_ARRAY_H__

#include "../structures.h"

#define __SL_DEFINE_ARRAY_0P(type, type_with_p_instead_of_stars_a) typedef struct type_with_p_instead_of_stars_a   { type* data;    uint count; size_t capa; }    type_with_p_instead_of_stars_a
#define __SL_DEFINE_ARRAY_1P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_a { type** data;   uint count; size_t capa; }   type_with_p_instead_of_stars##_a
#define __SL_DEFINE_ARRAY_2P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_a { type*** data;  uint count; size_t capa; }  type_with_p_instead_of_stars##_a
#define __SL_DEFINE_ARRAY_3P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_a { type**** data; uint count; size_t capa; } type_with_p_instead_of_stars##_a
#define SL_DEFINE_ARRAY(type) \
    __SL_DEFINE_ARRAY_0P (type, type##_a ); \
    __SL_DEFINE_ARRAY_1P (type, type##p  ); \
    __SL_DEFINE_ARRAY_2P (type, type##pp ); \
    __SL_DEFINE_ARRAY_3P (type, type##ppp)

#define array(type, ...) type##__VA_ARGS__##_a

SL_DEFINE_ARRAY(void);

/// @brief Type of an array element
/// @param array_ The array
#define __elemType(array_) typeof((array_).data[0])
/// @brief Size of an array element
/// @param array_ The array
#define __elemSize(array_) sizeof(typeof((array_).data[0]))

/// @brief Create an array ON STACK
/// @param array_type The type of the array (Use "array(type)" with the type of the element to store)
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define createArray(array_type, initialCapcity) ((array_type){.data = calloc(initialCapcity, sizeof(((array_type){0}).data[0])), .count = 0, .capa = initialCapcity * sizeof(((array_type){0}).data[0])})
/// @brief Destroy an array
/// @param toFree The array to destroy
#define destroyArray(array) (free((array).data))
/// @brief Create a new array ON HEAP
/// @param elemSize The size of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
/// @note Consider using "newArray" macro instead
void_a* __SL_newArray(size_t elemSize, uint capacity);
/// @brief Create a new array ON HEAP
/// @param elemSize The size of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define newArray(type, initialCapcity) ((array(type)*)__SL_newArray(sizeof(type), initialCapacity))
/// @brief Free an array
/// @param toFree The array to free
#define freeArray(array) do { free((array)->data); free(array); }

/// @brief Check and resize an array to be able to fit newCount elements
/// @param array The array to check
/// @param newCount The number of elements to accomodate
/// @param elemSize The size of an array element
/// @note Every function provided by the SL already calls this function when adding elements
void __SL_arrayCheckResize(array(void)* array, uint newCount, size_t elemSize);
/// @brief Add a C-array to the end of an array
/// @param array The array to expand
/// @param data The C-array to add
/// @param dataCount The size of the C-array
/// @param elemSize The size of an array element
/// @note Consider using "arrayAdd", "arrayAdd_T" or "arrayAdd_M" macros instead
void __SL_arrayAddMultiple(array(void)* array, void* data, uint dataCount, size_t elemSize);

/// @brief Add an element to an array
/// @param array_ The array to modify
/// @param ... The value to add
#define arrayAdd(array_, ...)  do { __SL_arrayCheckResize((void*)&(array_), (array_).count + 1,  __elemSize(array_));  (array_).data[(array_).count++]   = __VA_ARGS__; } while (0)
/// @brief Add an element to an array
/// @param array_ The array to modify
/// @param ... The value to add
#define arrayAdd_(array_, ...) do { __SL_arrayCheckResize((void*) (array_), (array_)->count + 1, __elemSize(*array_)); (array_)->data[(array_)->count++] = __VA_ARGS__; } while (0)

/// @brief Add multiple elements to an array (C-array version)
/// @param a The array to modify
/// @param elements The elements to add
/// @param count The number of elements to add
#define arrayAdd_T(array_, elements, count)  __SL_arrayAddMultiple((void*)&(array_), elements, count, __elemSize(array_));
/// @brief Add multiple elements to an array (C-array version)
/// @param a The array to modify
/// @param elements The elements to add
/// @param count The number of elements to add
#define arrayAdd_T_(array_, elements, count) __SL_arrayAddMultiple((void*)&(array_), elements, count, __elemSize(*array_));

/// @brief Add multiple elements to an array (Variadic version)
/// @param a The array to modify
/// @param val0 The first element to add
/// @param ... The rest of the elements to add
#define arrayAdd_M(array_, val0, ...) do {  \
    typeof((array_).data[0]) __SL_TEMP_ARRAY__[] = {val0, ##__VA_ARGS__}; \
    const uint __SL_TEMP__ = sizeof(__SL_TEMP_ARRAY__) / __elemSize(array_); \
    arrayAdd_T(array_, __SL_TEMP_ARRAY__, __SL_TEMP__); \
} while(0)
/// @brief Add multiple elements to an array (Variadic version)
/// @param a The array to modify
/// @param val0 The first element to add
/// @param ... The rest of the elements to add
#define arrayAdd_M_(array_, val0, ...) do {  \
    typeof((array_)->data[0]) __SL_TEMP_ARRAY__[] = {val0, ##__VA_ARGS__}; \
    const uint __SL_TEMP__ = sizeof(__SL_TEMP_ARRAY__) / __elemSize(*array_); \
    arrayAdd_T_(array_, __SL_TEMP_ARRAY__, __SL_TEMP__); \
} while(0)

/// @brief Concatenate two arrays
/// @param a The array to expand
/// @param b The array to copy
#define arrayCat(a, b)  arrayAdd_T(a, (b).data, (b).count)
/// @brief Concatenate two arrays
/// @param a The array to expand
/// @param b The array to copy
#define arrayCat_(a, b) arrayAdd_T_(a, (b)->data, (b)->count)

/// @brief Pop last element of an array
/// @param array The array
#define arrayPop(array)  ((array).data[--(array).count])
/// @brief Pop last element of an array
/// @param array The array
#define arrayPop_(array) ((array)->data[--(array)->count])

/// @brief Get last element of an array
/// @param array The array
#define arrayLast(array)  ((array).data[(array).count - 1])
/// @brief Get last element of an array
/// @param array The array
#define arrayLast_(array) ((array)->data[(array)->count - 1])

/// @brief Get first element of an array
/// @param array The array
#define arrayFirst(array)  (*(array).data)
/// @brief Get first element of an array
/// @param array The array
#define arrayFirst_(array) (*(array)->data)

#define arrayWrap(carray, count) ((array(typeof(carray[0]))){.data = carray, .capa = count, .count = count})
#define arrayWrap_Var(v0, ...) {.data = (typeof(v0)[]){v0, ##__VA_ARGS__}, .capa = sizeof((typeof(v0)[]){v0, ##__VA_ARGS__}) / sizeof(v0), .count = sizeof((typeof(v0)[]){v0, ##__VA_ARGS__}) / sizeof(v0)}

#define arraySort(array, varA, varB, conditionOnAAndB) do { \
    uvec2 __SL_ARRAY_STACK__[1024] = {0}; \
    uint __SL_ARRAY_STACK_IDX__ = 1; \
    __SL_ARRAY_STACK__[0] = Uvec2(0, (array).count - 1); \
    do { \
        uvec2 __SL_ARRAY_A_B__ = __SL_ARRAY_STACK__[--__SL_ARRAY_STACK_IDX__]; \
        if (__SL_ARRAY_A_B__.x >= __SL_ARRAY_A_B__.y) continue; \
        __elemType(array) varA = (array).data[__SL_ARRAY_A_B__.y]; \
        uint __SL_ARRAY_J__ = __SL_ARRAY_A_B__.x; \
        for (uint __SL_ARRAY_I__ = __SL_ARRAY_A_B__.x; __SL_ARRAY_I__ < __SL_ARRAY_A_B__.y; ++__SL_ARRAY_I__) { \
            __elemType(array) varB = (array).data[__SL_ARRAY_I__]; \
            if (conditionOnAAndB) { \
                (array).data[__SL_ARRAY_I__] = (array).data[__SL_ARRAY_J__]; \
                (array).data[__SL_ARRAY_J__] = varB; \
                ++__SL_ARRAY_J__; \
            } \
        } \
        (array).data[__SL_ARRAY_A_B__.y] = (array).data[__SL_ARRAY_J__]; \
        (array).data[__SL_ARRAY_J__] = varA; \
        \
        if (__SL_ARRAY_J__ > 0) __SL_ARRAY_STACK__[__SL_ARRAY_STACK_IDX__++] = Uvec2(__SL_ARRAY_A_B__.x, __SL_ARRAY_J__ - 1); \
        __SL_ARRAY_STACK__[__SL_ARRAY_STACK_IDX__++] = Uvec2(__SL_ARRAY_J__ + 1, __SL_ARRAY_A_B__.y); \
    } while (__SL_ARRAY_STACK_IDX__); \
} while (0);

#define __SL_array_foreach(varname, array_) for (typeof((array_).data[0]) *varname = (array_).data, *varname##Max = (void*)(((size_t)(array_).data) + (array_).count * __elemSize(array_)); (size_t)varname < (size_t)varname##Max; ++varname)
/// @brief Iterate over every element in array
/// @param VARNAME_in_ARRAY Should litteraly the name of the variable used to iterate followed by "in" then the name of the array
/// @note Use like a while-loop or a for-loop (as it is one)
#define array_foreach(VARNAME_in_ARRAY) __SL_array_foreach(VARNAME_in_ARRAY)

#endif