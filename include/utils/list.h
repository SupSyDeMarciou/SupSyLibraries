#ifndef __SL_UTILS_LIST
#define __SL_UTILS_LIST

#include "../structures.h" 

/*
    Node: {
        sizeof(void*) : pointer to next node
        elemSize : actual value
    }
*/ 
typedef void node_t;

typedef struct List {
    node_t* first;
    node_t* last;
    uint length;
} list_t;

#define n_next(node) (*(void**)(node)) /// @brief Get pointer to next node
#define n_elem_p(node) ((node) + sizeof(node_t*)) /// @brief Get pointer to value in node
#define n_elem(type, node) *(type*)((node) + sizeof(node_t*)) /// @brief Get pointer to value in node

/// @brief Iterate over every element of a list
/// @param list The list to iterate through
/// @param type The type of the elements in the list
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define list_foreach_t(list, type, varname, code) do { \
    node_t* varname##NODE__ = ((list_t)(list)).first;\
    while (varname##NODE__) {\
        node_t* varname##NODE_TEMP__ = n_next(varname##NODE__);\
        type varname = n_elem(type, varname##NODE__); \
        code\
        varname##NODE__ = varname##NODE_TEMP__;\
    }\
} while (false)
/// @brief Iterate over every element of a list
/// @param list The list to iterate through
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define list_foreach(list, varname, code) list_foreach_t(list, void*, varname, code)

/// @brief Create a new chained pointer list on the stack
#define STATIC_NEW_LIST(type) ((list_t){.first = NULL, .last = NULL, .length = 0})

/// @brief Create a new chained list on the heap
/// @return The newly created list
list_t* newList();
/// @brief Free a list
/// @param toFree The list to free
/// @warning This function does not free the values pointed to by the nodes of the freed list
void freeList(list_t* toFree);
/// @brief Free list and iterate over every element
/// @param list The list to free
/// @param type The type of the elements in the list
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define freeList_foreach_t(list, type, varname, code) list_foreach_t(list, type, varname, code free(varname##NODE__);)
/// @brief Free list and iterate over every element
/// @param list The list to free
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define freeList_foreach(list, varname, code) freeList_foreach_t(list, void*, varname, code)

/// @brief Add a value at the end of a list
/// @param list The list in which to add
/// @param ptrToVal A pointer to the value to add
/// @param elemSize The size of the element to add
void listAddLast_P(list_t* list, void* ptrToVal, size_t elemSize);
/// @brief Add a value at the end of a list
/// @param list The list in which to add
/// @param val The value to add
#define listAddLast(list, val) do { typeof(val) TEMP__ = val; listAddLast_P(list, &TEMP__, sizeof(TEMP__)); } while (false)
/// @brief Add a value at the start of a list
/// @param list The list in which to add
/// @param ptrToVal A pointer to the value to add
/// @param elemSize The size of the element to add
void listAddFirst_P(list_t* list, void* ptrToVal, size_t elemSize);
/// @brief Add a value at the start of a list
/// @param list The list in which to add
/// @param val The value to add
#define listAddFirst(list, val) do { typeof(val) TEMP__ = val; listAddFirst_P(list, &TEMP__, sizeof(TEMP__)); } while (false)

/// @brief Get the value at index i in a list
/// @param list The list in which to search
/// @param i The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
void* listGet(list_t* list, uint i);

/// @brief Disconnect the i-th node of a list and return it
/// @param list The list to modify
/// @param i The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds 
node_t* listPop_Node(list_t* list, uint i);
/// @brief Remove and return the i-th value from a list
/// @param list The list to modify
/// @param i The place of the node to disconnect in the list
/// @param var The value in which to store the value
#define listPop(list, idx, var) do { node_t* var##NODE__ = listPop_Node(list, i); if (var##NODE__) var = n_elem(typeof(var), var##NODE__); free(var##NODE__); } while (false)

/// @brief Remove the i-th element of a list
/// @param list The list to modify
/// @param i The place of the element to remove in the list
/// @return Wether an elemnt was removed
bool listRemove(list_t* list, uint i);

#endif