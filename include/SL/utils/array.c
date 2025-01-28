#include "array.h"
#include <stdlib.h>
#include <memory.h>

array(void)* __SL_newArray(size_t elemSize, uint capacity) {
    array(void)* a = malloc(sizeof(array(void)));
    a->count = 0;
    a->capa = capacity;
    a->data = malloc(capacity * elemSize);
    return a;
}
void __SL_arrayCheckResize(array(void)* a, uint newCount, size_t elemSize) {
    if (a->capa == 0) {
        a->data = malloc(elemSize);
        a->capa = elemSize;
        return;
    }
    size_t needed = newCount * elemSize;
    if (needed > a->capa) { // Count == capa
        size_t newCapa = a->capa * 2;
        while (needed > newCapa) newCapa *= 2;

        void* newData = malloc(newCapa);
        memcpy(newData, a->data, a->capa);
        free(a->data);
        a->data = newData;
        a->capa = newCapa;
    }
}
void __SL_arrayAddMultiple(array(void)* a, void* data, uint dataCount, size_t elemSize) {
    __SL_arrayCheckResize(a, a->count + dataCount, elemSize);
    memcpy(a->data + a->count * elemSize, data, dataCount * elemSize);
    a->count += dataCount;
}