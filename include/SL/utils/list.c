#include "list.h"
#include "stdlib.h"
#include "memory.h"
#include "inout.h"

void* __SL_TEMP_NODE_STATIC__ = NULL;

void freeListNode(void* toFree) {
    free(toFree);
}

void __SL_destroyListNodes(voidp_l toDestroy) {
    voidp_l_node* node = toDestroy.first;
    while (node) {
        voidp_l_node* next = node->next;
        freeListNode(node);
        node = next;
    }
}

void __SL_listAddNode_Last(voidp_l* l, voidp_l_node* node) {

    if (!l->first) l->first = node;
    else l->last->next = node;
    
    l->last = node;
    ++l->count;
}
void __SL_listAddNode_First(voidp_l* l, voidp_l_node* node) {

    if (!l->first) l->last = node;

    node->next = l->first;
    l->first = node;
    ++l->count;
}
void __SL_listInsertNode(voidp_l* l, voidp_l_node* node_, uint i) {
    if (i == 0) return __SL_listAddNode_First(l, node_);
    if (i >= l->count) return __SL_listAddNode_Last(l, node_);

    voidp_l_node* node = l->first;
    while (--i) node = node->next;

    node_->next = node->next;
    node->next = node_;

    ++l->count;
}

void* __SL_listGetNode(voidp_l* l, uint i) {
    if (i == 0) return l->first;
    if (i == l->count - 1) return l->last;
    if (i >= l->count) return NULL;
    
    voidp_l_node* node = l->first;
    do node = node->next; while (--i);
    return node;
}

void* __SL_listPopNode(voidp_l* l, uint i) {
    if (!l->first) return NULL;
    if (i == 0) {
        voidp_l_node* n = l->first;
        l->first = n->next;
        --l->count;
        return n;
    }
    if (i >= l->count) return NULL;

    voidp_l_node* node = l->first;
    while (((int)--i) - 1) node = node->next;

    void* n = node->next;
    node->next = node->next->next;

    --l->count;
    return n;
}

bool __SL_listRemove(voidp_l* list, uint i) {
    void* ret = __SL_listPopNode(list, i);
    bool removed = ret != NULL;
    free(ret);
    return removed;
}