#ifndef __SL_UTILS_LIST_H__
#define __SL_UTILS_LIST_H__

#include "../structures.h" 

/*
    Node: {
        sizeof(void*) : pointer to next node
        elemSize : actual value
    }
*/ 
/// @brief Type of a node in a list. The data can be accessed with the "n_"
typedef void list_node;
#define n_next(node) (*(void**)(node)) /// @brief Get pointer to next node
#define n_elem_p(type, node) ((type*)((size_t)(node) + sizeof(list_node*))) /// @brief Get pointer to value in node
#define n_elem(type, node) (*(type*)((size_t)(node) + sizeof(list_node*))) /// @brief Get pointer to value in node
/// @brief Free a list node
/// @param toFree The node to free
/// @warning Use on the returned values of listPop
void freeListNode(list_node* toFree);

typedef struct List {
    list_node* first;
    list_node* last;
    uint count;
} list;

/// @brief Iterate over every element of a list
/// @param l The list to iterate through
/// @param type The type of the elements in the list
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define list_foreach_t(l, type, varname) if ((l)->first) for ( \
    type* varname = n_elem_p(type, (l)->first), *varname##_NODE = (l)->first, *varname##_NODE_TEMP = n_next(varname##_NODE); \
    varname##_NODE != NULL; \
    varname##_NODE = varname##_NODE_TEMP, varname##_NODE_TEMP = varname##_NODE ? n_next(varname##_NODE) : NULL, varname = varname##_NODE ? n_elem_p(type, varname##_NODE) : NULL \
)
/// @brief Iterate over every element of a list
/// @param list The list to iterate through
/// @param varname The name of the variable containing a pointer to the value at the current iteration
/// @param code The code to run for each node in the list
#define list_foreach(l, varname) list_foreach_t(l, void, varname)

/// @brief Create a new chained list ON STACK
/// @return The newly created list
list createList();
/// @brief Create a new chained list ON HEAP
/// @return The newly created list
list* newList();
/// @brief Destroy a list
/// @param toFree The list to destroy
void destroyList(list toDestroy);
/// @brief Free a list
/// @param toFree The list to free
void freeList(list* toFree);

/// @brief Add a value at the end of a list
/// @param list The list in which to add
/// @param ptrToVal A pointer to the value to add
/// @param elemSize The size of the element to add
void listAddLast_P(list* list, void* ptrToVal, size_t elemSize);
/// @brief Add a value at the end of a list
/// @param list The list in which to add
/// @param val The value to add
#define listAddLast(l, val) do { typeof(val) __SL_LIST_TEMP__ = val; listAddLast_P(l, &__SL_LIST_TEMP__, sizeof(__SL_LIST_TEMP__)); } while (false)
/// @brief Add a value at the start of a list
/// @param list The list in which to add
/// @param ptrToVal A pointer to the value to add
/// @param elemSize The size of the element to add
void listAddFirst_P(list* list, void* ptrToVal, size_t elemSize);
/// @brief Add a value at the start of a list
/// @param list The list in which to add
/// @param val The value to add
#define listAddFirst(l, val) do { typeof(val) __SL_LIST_TEMP__ = val; listAddFirst_P(l, &__SL_LIST_TEMP__, sizeof(__SL_LIST_TEMP__)); } while (false)
/// @brief Add a value at a specific position in a list
/// @param list The list in which to add
/// @param ptrToVal A pointer to the value to add
/// @param elemSize The size of the element to add
/// @param idx The position in which to insert
void listInsert_P(list* list, void* ptrToVal, size_t elemSize, uint idx);
/// @brief Add a value at the start of a list
/// @param list The list in which to add
/// @param val The value to add
#define listInsert(l, val, idx) do { typeof(val) __SL_LIST_TEMP__ = val; listInsert_P(l, &__SL_LIST_TEMP__, sizeof(__SL_LIST_TEMP__), idx); } while (false)

/// @brief Get the value at index i in a list
/// @param list The list in which to search
/// @param i The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
void* listGet_P(list* list, uint i);
/// @brief Get the value at index i in a list
/// @param l The list in which to search
/// @param type The type of the element to get
/// @param i The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
#define listGet(l, type, idx) ((type*)listGet_P(l, idx))

/// @brief Disconnect the i-th node of a list and return it
/// @param list The list to modify
/// @param i The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds 
list_node* listPop(list* list, uint i);

/// @brief Remove the i-th element of a list
/// @param list The list to modify
/// @param i The place of the element to remove in the list
/// @return Wether an elemnt was removed
bool listRemove(list* list, uint i);

#endif