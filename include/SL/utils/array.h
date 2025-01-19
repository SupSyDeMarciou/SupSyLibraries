#ifndef __SL_UTILS_ARRAY_H__
#define __SL_UTILS_ARRAY_H__

#include "../structures.h"

/// @brief To allow for generic use of these arrays
// typedef struct ArrayVoid { uint count; size_t capa; void* data; } array_void;
// #define array(type) union { struct { uint count; size_t capa; type* data; }; array_void v; }
typedef struct ArrayVoid { uint count; size_t capa; void* data; } array_void;
#define array(type) union { struct { uint count; size_t capa; type* data; }; array_void v; }

/// @brief Type of an array element
/// @param array_ The array
#define __elemType(array_) typeof((array_).data[0])
/// @brief Size of an array element
/// @param array_ The array
#define __elemSize(array_) sizeof(typeof((array_).data[0]))

/// @brief Create an array ON STACK
/// @param type The type of the array elements
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define createArray(type, initialCapcity) {.data = calloc(initialCapcity, sizeof(type)), .count = 0, .capa = initialCapcity * sizeof(type)}
/// @brief Create an array ON STACK on existing memory
/// @param type The type of the array elements
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define createArray_2(array_, initialCapcity) array_ = ((typeof(array_)){.data = calloc(initialCapcity, __elemSize(array_)), .count = 0, .capa = initialCapcity * __elemSize(array_)})
/// @brief Destroy an array
/// @param toFree The array to destroy
#define destroyArray(array) (free((array).data))
/// @brief Create a new array ON HEAP
/// @param elemSize The size of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
/// @note Consider using "newArray" macro instead
array_void* __SL_newArray(size_t elemSize, uint capacity);
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
void __SL_arrayCheckResize(array_void* array, uint newCount, size_t elemSize);
/// @brief Add a C-array to the end of an array
/// @param array The array to expand
/// @param data The C-array to add
/// @param dataCount The size of the C-array
/// @param elemSize The size of an array element
/// @note Consider using "arrayAdd", "arrayAdd_T" or "arrayAdd_M" macros instead
void __SL_arrayAddMultiple(array_void* array, void* data, uint dataCount, size_t elemSize);

/// @brief Add an element to an array
/// @param array_ The array to modify
/// @param value The value to add
#define arrayAdd(array_, value)  do { __SL_arrayCheckResize((void*)&(array_), (array_).count + 1,  __elemSize(array_));  (array_).data[(array_).count++]   = value; } while (0)
/// @brief Add an element to an array
/// @param array_ The array to modify
/// @param value The value to add
#define arrayAdd_(array_, value) do { __SL_arrayCheckResize((void*) (array_), (array_)->count + 1, __elemSize(*array_)); (array_)->data[(array_)->count++] = value; } while (0)

/// @brief Add multiple elements to an array (C-array version)
/// @param a The array to modify
/// @param elements The elements to add
/// @param count The number of elements to add
#define arrayAdd_T(array_, elements, count)  __SL_arrayAddMultiple(&(array_).v, elements, count, __elemSize(array_));
/// @brief Add multiple elements to an array (C-array version)
/// @param a The array to modify
/// @param elements The elements to add
/// @param count The number of elements to add
#define arrayAdd_T_(array_, elements, count) __SL_arrayAddMultiple(&(array_)->v, elements, count, __elemSize(*array_));

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

#define __SL_array_foreach(varname, array_) for (typeof((array_).data[0]) *varname = (array_).data, *varname##Max = (void*)(((size_t)(array_).data) + (array_).count * __elemSize(array_)); (size_t)varname < (size_t)varname##Max; ++varname)
/// @brief Iterate over every element in array
/// @param VARNAME_in_ARRAY Should litteraly the name of the variable used to iterate followed by "in" then the name of the array
/// @note Use like a while-loop or a for-loop (as it is one)
#define array_foreach(VARNAME_in_ARRAY) __SL_array_foreach(VARNAME_in_ARRAY)

#endif