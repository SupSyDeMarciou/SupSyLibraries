#ifndef __SL_UTILS_ARRAY
#define __SL_UTILS_ARRAY

#include "../structures.h" 

typedef struct Array {
    void* data;         // The actual array
    uint capa, count;   // The allocated size and current number of elements
    size_t elemSize;    // The size of a single element
} array;
// Just to make arrays more explicit with their type
#define array(element) array

/// @brief Create a new empty array
/// @param element The type of the array
#define STATIC_ARRAY_EMPTY(element) ((array){.capa = 0, .count = 0, .elemSize = sizeof(element), .data = NULL})

/// @brief Create an array ON STACK
/// @param elementSize The size of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
array createArray_P(size_t elementSize, uint initialCapacity); 
/// @brief Create a new array ON HEAP
/// @param elementSize The size of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
array* newArray_P(size_t elementSize, uint initialCapacity);
/// @brief Destroy an array
/// @param toFree The array to destroy
void destroyArray(array toDestroy);
/// @brief Free an array
/// @param toFree The array to free
void freeArray(array* toFree);

/// @brief Get a pointer to an element of the array
/// @param a The array to modify
/// @param i The index of the element to look up (Positive is relative to 0, Negative is relative to last element)
/// @return The pointer to the element or NULL if index out of bounds
void* arrayGet_P(array* a, int i);
/// @brief Set the i-th element of the array to the value val
/// @param a The array to modify
/// @param i The index in the array (Positive is relative to 0, Negative is relative to last element)
/// @param val The pointer to the value to set (NULL sets the bits to 0)
/// @return Whether the element was successufuly added
bool arraySet_P(array* a, int i, void* val);
/// @brief Add an element to an array
/// @param a The array to modify
/// @param val The pointer to the value to add (NULL adds 0)
/// @return Wether the array has been reallocated
/// @note Setting "val" to NULL will add zero-ed out element
bool arrayAdd_P(array* a, void* val);
/// @brief Add multiple elements to an array
/// @param a The array to modify
/// @param elements The elements to add
/// @param count The number of elements to add
/// @return Wether the array has been reallocated
/// @note Setting "elements" to NULL will add zero-ed out element
bool arrayAdd_Multiple(array* a, void* elements, uint count);
/// @brief Add array elements to another array
/// @param a The array to modify
/// @param b The array to sample from
/// @param count The number of elements to add
/// @return Wether the array has been reallocated
bool arrayAdd_Array(array* a, array* b, uint count);

/// @brief Get the number of objects in an array
/// @param a The array to inspect
/// @return The number of objects in the array
uint arrayCount(array* a);
/// @brief Get the capacity of an array
/// @param a The array to inspect
/// @return The capacity of the array
uint arrayCapacity(array* a);
/// @brief Get the data stored in an array
/// @param a The array to inspect
/// @return A pointer to the data
/// @warning Be careful when manipulating this data, and most importantly do not free it as it will most likely cause a seg-fault
void* arrayData(array* a);

/// @brief Create an array ON STACK
/// @param type The type of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define createArray(type, initialCapacity) (createArray_P(sizeof(type), initialCapacity))
/// @brief Create a new array ON HEAP
/// @param type The type of an array element
/// @param initialCapacity The initial capacity of the array (will use closest power of two above)
/// @return The newly created array
#define newArray(type, initialCapacity) (newArray_P(sizeof(type), initialCapacity))

/// @brief Get a pointer to an element of the array
/// @param a The array to modify
/// @param type The type of an array element
/// @param i The index of the element to look up
/// @return The pointer to the element or NULL if index out of bounds
#define arrayGet(a, type, i) ((type*)arrayGet_P(a, i))
/// @brief Set the i-th element of the array to the value val
/// @param a The array to modify
/// @param i The index in the array (Positive is relative to 0, Negative is relative to last element)
/// @param val The value to set
#define arraySet(a, i, val) do { \
    typeof(val) VAL__ = val; \
    if (!arraySet_P(a, i, &VAL__)) failWithError("Cannot set element outside of array count"); \
} while (false)
/// @brief Add an element to an array
/// @param a The array to modify
/// @param val The value to add
#define arrayAdd(a, val) do { \
    typeof(val) VAL__ = val; \
    (void)arrayAdd_P(a, &VAL__); \
} while (false)

/// @brief Loop over every element in an array
/// @param array The array
/// @param type The type of an array element
/// @param varname The name of the variable holding the current array element
#define array_foreach(array, type, varname) \
    for (type *varname = array.data, *end = (type*)(array.data + array.count * array.elemSize); varname < end; varname++)

#endif