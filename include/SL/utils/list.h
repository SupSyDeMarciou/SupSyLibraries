#ifndef __SL_UTILS_LIST_H__
#define __SL_UTILS_LIST_H__

#include "../structures.h" 

#define __SL_DEFINE_LIST_NODE_0P(type, type_with_p_instead_of_stars_l) typedef struct type_with_p_instead_of_stars_l##_node { struct type_with_p_instead_of_stars_l##_node *next; type    value; } type_with_p_instead_of_stars_l##_node
#define __SL_DEFINE_LIST_NODE_1P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l_node { struct type_with_p_instead_of_stars##_l_node *next; type*   value; } type_with_p_instead_of_stars##_l_node
#define __SL_DEFINE_LIST_NODE_2P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l_node { struct type_with_p_instead_of_stars##_l_node *next; type**  value; } type_with_p_instead_of_stars##_l_node
#define __SL_DEFINE_LIST_NODE_3P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l_node { struct type_with_p_instead_of_stars##_l_node *next; type*** value; } type_with_p_instead_of_stars##_l_node

#define __SL_DEFINE_LIST_0P(type, type_with_p_instead_of_stars_l) typedef struct type_with_p_instead_of_stars_l   { type_with_p_instead_of_stars_l##_node *first, *last; uint count; } type_with_p_instead_of_stars_l
#define __SL_DEFINE_LIST_1P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l { type_with_p_instead_of_stars##_l_node *first, *last; uint count; } type_with_p_instead_of_stars##_l
#define __SL_DEFINE_LIST_2P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l { type_with_p_instead_of_stars##_l_node *first, *last; uint count; } type_with_p_instead_of_stars##_l
#define __SL_DEFINE_LIST_3P(type, type_with_p_instead_of_stars)   typedef struct type_with_p_instead_of_stars##_l { type_with_p_instead_of_stars##_l_node *first, *last; uint count; } type_with_p_instead_of_stars##_l

#define SL_DEFINE_LIST(type) \
    __SL_DEFINE_LIST_NODE_0P(type, type##_l);  __SL_DEFINE_LIST_0P (type, type##_l  ); \
    __SL_DEFINE_LIST_NODE_1P(type, type##p);   __SL_DEFINE_LIST_1P (type, type##p   ); \
    __SL_DEFINE_LIST_NODE_2P(type, type##pp);  __SL_DEFINE_LIST_2P (type, type##pp  ); \
    __SL_DEFINE_LIST_NODE_3P(type, type##ppp); __SL_DEFINE_LIST_3P (type, type##ppp );

#define SL_DEFINE_LIST_NO_IMPLEMENTATION(type) \
    __SL_DEFINE_LIST_NODE_1P(type, type##p);   __SL_DEFINE_LIST_1P (type, type##p   ); \
    __SL_DEFINE_LIST_NODE_2P(type, type##pp);  __SL_DEFINE_LIST_2P (type, type##pp  ); \
    __SL_DEFINE_LIST_NODE_3P(type, type##ppp); __SL_DEFINE_LIST_3P (type, type##ppp );


#define list(type, ...) type##__VA_ARGS__##_l

SL_DEFINE_LIST_NO_IMPLEMENTATION(void);
extern void* __SL_TEMP_NODE_STATIC__;

/// @brief Create a list ON STACK
/// @param list_type The type of the list (Use "list(type)" with the type of the element to store)
/// @return The newly created list
#define createList(list_type) ((list_type){0})
/// @brief Destroy a void* list
/// @param toFree The list to destroy
void __SL_destroyListNodes(voidp_l list);
/// @brief Destroy a list
/// @param toDestroy The list to destroy
#define destroyList(toDestroy) (__SL_destroyListNodes(as(voidp_l, toDestroy)))
/// @brief Create a new list
/// @param list_type The type of the list (Use "list(type)" with the type of the element to store)
/// @return The newly created list
#define newList(list_type) ((list_type*)calloc(1, sizeof(list_type)))
/// @brief Free a list
/// @param toFree The list to free
#define freeList(toFree) do { __SL_destroyListNodes(*as_(voidp_l, toFree)); free(toFree); } while (0)

/// @brief Free a list node
/// @param toFree The node to free
/// @warning Use on the returned values of listPop ONLY
void freeListNode(void* toFree);

/// @brief Get first value in list
/// @param l The list
#define listFirst(l) ((l).first->value)
/// @brief Get first value in list
/// @param l The list
#define listFirst_(l) ((l)->first->value)
/// @brief Get last value in list
/// @param l The list
#define listLast(l) ((l).last->value)
/// @brief Get last value in list
/// @param l The list
#define listLast_(l) ((l)->last->value)

#define __SL_listAllocNode(l, val)  typeof(l.first)  __SL_TEMP_NODE__ = malloc(sizeof(*__SL_TEMP_NODE__)); *__SL_TEMP_NODE__ = (typeof(*__SL_TEMP_NODE__)){.next = NULL, .value = val}
#define __SL_listAllocNode_(l, val) typeof(l->first) __SL_TEMP_NODE__ = malloc(sizeof(*__SL_TEMP_NODE__)); *__SL_TEMP_NODE__ = (typeof(*__SL_TEMP_NODE__)){.next = NULL, .value = val}

void __SL_listAddNode_Last(voidp_l* l, voidp_l_node* node);
/// @brief Add a value at the end of a list
/// @param l The list
/// @param val The value to add
#define listAddLast(l, ...) do { \
    __SL_listAllocNode(l, (__VA_ARGS__)); \
    __SL_listAddNode_Last(as_(voidp_l, &l), as_(voidp_l_node, __SL_TEMP_NODE__)); \
} while (0)
/// @brief Add a value at the end of a list
/// @param l The list
/// @param val The value to add
#define listAddLast_(l, ...) do { \
    __SL_listAllocNode_(l, (__VA_ARGS__)); \
    __SL_listAddNode_Last(as_(voidp_l, l),as_(voidp_l_node, __SL_TEMP_NODE__)); \
} while (0)

void __SL_listAddNode_First(voidp_l* l, voidp_l_node* node);
/// @brief Add a value at the start of a list
/// @param l The list
/// @param val The value to add
#define listAddFirst(l, val) do { \
    __SL_listAllocNode(l, val); \
    __SL_listAddNode_First(as_(voidp_l, &l), as_(voidp_l_node, __SL_TEMP_NODE__)); \
} while (0)
/// @brief Add a value at the start of a list
/// @param l The list
/// @param val The value to add
#define listAddFirst_(l, val) do { \
    __SL_listAllocNode_(l, val); \
    __SL_listAddNode_First(as_(voidp_l, l), as_(voidp_l_node, __SL_TEMP_NODE__)); \
} while (0)

void __SL_listInsertNode(voidp_l* l, voidp_l_node* node_, uint i);
/// @brief Insert a value at specified index in a list
/// @param l The list
/// @param val The value to insert
/// @param index The index at which to insert the value
#define listInsert(l, val, index) do { \
    __SL_listAllocNode(l, val); \
    __SL_listInsertNode(as_(voidp_l, &l), as_(voidp_l_node, __SL_TEMP_NODE__), index); \
} while (0)
/// @brief Insert a value at specified index in a list
/// @param l The list
/// @param val The value to insert
/// @param index The index at which to insert the value
#define listInsert_(l, val, index) do { \
    __SL_listAllocNode_(l, val); \
    __SL_listInsertNode(as_(voidp_l, l), as_(voidp_l_node, __SL_TEMP_NODE__), index); \
} while (0)

void* __SL_listGetNode(voidp_l* l, uint i);
/// @brief Get the value at index i in a list
/// @param l The list
/// @param type The type of the element to get
/// @param index The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
#define listGet(l, index) (__SL_TEMP_NODE_STATIC__ = __SL_listGetNode(as_(voidp_l, &l), index), __SL_TEMP_NODE_STATIC__ ? &as(typeof(l.first), __SL_TEMP_NODE_STATIC__)->value : NULL)
/// @brief Get the value at index i in a list
/// @param l The list
/// @param type The type of the element to get
/// @param index The index of the researched element
/// @return The desired value if i is in bounds, NULL otherwise
#define listGet_(l, index) (__SL_TEMP_NODE_STATIC__ = __SL_listGetNode(as_(voidp_l, l), index), __SL_TEMP_NODE_STATIC__ ? &as(typeof(l->first), __SL_TEMP_NODE_STATIC__)->value : NULL)

/// @brief Disconnect the i-th node of a list and return it
/// @param l The list
/// @param index The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds 
/// @note Consider using "listPopInto" instead
/// @warning This node's memory is now YOUR PROBLEM, do not forget to deallocate it!
void* __SL_listPopNode(voidp_l* l, uint index);
/// @brief Disconnect the i-th node of a list and return it
/// @param l The list
/// @param index The place of the node to disconnect in the list
/// @return The disconnected node or NULL if i is out of bounds
/// @note Consider using "listPopInto" instead
/// @warning This node's memory is now YOUR PROBLEM, do not forget to deallocate it!
#define listPopNode_(l, type, index) as_list_node(type, __SL_listPopNode(l, index))

/// @brief Remove value at index from list and store it into a variable
/// @param l The list
/// @param index The place of the value to pop from the list
/// @param varname The variable in which to store the popped value
#define listPopInto_(l, index, varname) do { \
    list_node(typeof(varname))* __SL_TEMP_NODE___ = __SL_listPopNode(l, index); \
    if (__SL_TEMP_NODE___) varname = __SL_TEMP_NODE__->value; \
    freeNode(__SL_TEMP_NODE__); \
} while(0)

/// @brief Remove value at index from list
/// @param l The list
/// @param index The place of the value to remove from the list
bool __SL_listRemove(voidp_l* l, uint index);
/// @brief Remove value at index from list
/// @param l The list
/// @param index The place of the value to remove from the list
#define listRemove(l, index) __SL_listRemove(as_(voidp_l, &l), index)
/// @brief Remove value at index from list
/// @param l The list
/// @param index The place of the value to remove from the list
#define listRemove_(l, index) __SL_listRemove(as_(voidp_l, l), index)

#define __SL_list_foreach(varname, l) if ((l).first) for ( \
    typeof((l).first->value)* varname = &(l).first->value, *varname##_NODE = (void*)(l).first; \
    varname##_NODE; \
    varname##_NODE = (void*)as_(voidp_l_node, varname##_NODE)->next, varname = varname##_NODE ? (void*)&as_(voidp_l_node, varname##_NODE)->value : NULL \
)
/// @brief Iterate over every element in a list
/// @param VARNAME_in_LIST Should litteraly the name of the variable used to iterate followed by "in" then the name of the list
#define list_foreach(VARNAME_in_LIST) __SL_list_foreach(VARNAME_in_LIST)

#endif