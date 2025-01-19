#ifndef __SL_UTILS_LIST_H__
#define __SL_UTILS_LIST_H__

#include "../structures.h" 

extern void* __SL_TEMP_NODE_STATIC__;
typedef struct List {
    void* first;
    void* last;
    uint count;
} list;

#define list_node(type) struct { void* next; type value; }
#define as_list_node(type, node) ((list_node(type)*)node)

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

/// @brief Free a list node
/// @param toFree The node to free
/// @warning Use on the returned values of listPop ONLY
void freeListNode(void* toFree);

/// @brief Get first value in list
/// @param l The list
/// @param type The type of the value
#define listFirst(l, type) as_list_node(type, (l)->first)
/// @brief Get Last value in list
/// @param l The list
/// @param type The type of the value
#define listLast(l, type) as_list_node(type, (l)->last)

void __SL_listAddNode_Last(list* l, void* node);
/// @brief Add a value at the end of a list
/// @param l The list
/// @param val The value to add
#define listAddLast(l, val) do { \
    list_node(typeof(val))* __SL_TEMP_NODE__ = malloc(sizeof(*__SL_TEMP_NODE__)); \
    *__SL_TEMP_NODE__ = (typeof(*__SL_TEMP_NODE__)){.next = NULL, .value = val}; \
    __SL_listAddNode_Last(l, __SL_TEMP_NODE__); \
} while (0)

void __SL_listAddNode_First(list* l, void* node);
/// @brief Add a value at the start of a list
/// @param l The list
/// @param val The value to add
#define listAddFirst(l, val) do { \
    list_node(typeof(val))* __SL_TEMP_NODE__ = malloc(sizeof(*__SL_TEMP_NODE__)); \
    *__SL_TEMP_NODE__ = (typeof(*__SL_TEMP_NODE__)){.next = NULL, .value = val}; \
    __SL_listAddNode_First(l, __SL_TEMP_NODE__); \
} while (0)

void __SL_listInsertNode(list* l, void* node_, uint i);
/// @brief Insert a value at specified index in a list
/// @param l The list
/// @param val The value to insert
/// @param index The index at which to insert the value
#define listInsert(l, val, index) do { \
    list_node(typeof(val))* __SL_TEMP_NODE__ = malloc(sizeof(*__SL_TEMP_NODE__)); \
    *__SL_TEMP_NODE__ = (typeof(*__SL_TEMP_NODE__)){.next = NULL, .value = val}; \
    __SL_listInsertNode(l, __SL_TEMP_NODE__, index); \
} while (0)

void* __SL_listGetNode(list* l, uint i);
/// @brief Get the value at index i in a list
/// @param l The list
/// @param type The type of the element to get
/// @param index The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
#define listGet(l, type, index) (__SL_TEMP_NODE_STATIC__ = __SL_listGetNode(l, index), __SL_TEMP_NODE_STATIC__ ? &as_list_node(type, __SL_TEMP_NODE_STATIC__)->value : (type*)NULL)

/// @brief Disconnect the i-th node of a list and return it
/// @param l The list
/// @param index The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds 
/// @note Consider using "listPopInto" instead
/// @warning This node's memory is now YOUR PROBLEM, do not forget to deallocate it!
void* __SL_listPopNode(list* l, uint index);
/// @brief Disconnect the i-th node of a list and return it
/// @param l The list
/// @param index The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds
/// @note Consider using "listPopInto" instead
/// @warning This node's memory is now YOUR PROBLEM, do not forget to deallocate it!
#define listPopNode(l, type, index) as_list_node(type, __SL_listPopNode(l, index))

/// @brief Remove value at index from list and store it into a variable
/// @param l The list
/// @param index The place of the value to pop from the list
/// @param varname The variable in which to store the popped value
#define listPopInto(l, index, varname) do { \
    list_node(typeof(varname))* __SL_TEMP_NODE___ = __SL_listPopNode(l, index); \
    if (__SL_TEMP_NODE___) varname = __SL_TEMP_NODE__->value; \
    freeNode(__SL_TEMP_NODE__); \
} while(0)

/// @brief Remove value at index from list
/// @param l The list
/// @param index The place of the value to remove from the list
bool listRemove(list* l, uint index);

#define __SL_list_foreach_t(type, varname, l) if ((l)->first) for ( \
    type* varname = (void*)&as_list_node(void*, (l)->first)->value, *varname##_NODE = (l)->first; \
    varname##_NODE; \
    varname##_NODE = as_list_node(void*, varname##_NODE)->next, varname = varname##_NODE ? (void*)&as_list_node(void*, varname##_NODE)->value : NULL \
)
/// @brief Iterate over every element in a list
/// @param type The type of the elements in the list
/// @param VARNAME_in_LIST Should litteraly the name of the variable used to iterate followed by "in" then the name of the list
#define list_foreach_t(type, VARNAME_in_LIST) __SL_list_foreach_t(type, VARNAME_in_LIST)
/// @brief Iterate over every element in a list
/// @param VARNAME_in_LIST Should litteraly the name of the variable used to iterate followed by "in" then the name of the list
#define list_foreach(VARNAME_in_LIST) __SL_list_foreach_t(void, VARNAME_in_LIST)

#endif