#include "../../SL.h"
#include <memory.h>

///// HASH TABLE

struct HashTable {
    void* table; // bool USED + (elemSize) DATA
    size_t elemSize;
    uint capa;
    func_hash hash;
    func_comp comp;
};
#define hash_used(ptr) (*(bool*)(ptr))

hash_tbl createHashTable_P(size_t elemSize, func_hash hash, func_comp comp, uint baseCapacity) {
    hash_tbl table;
    table.comp = comp;
    table.hash = hash;

    table.capa = SL_closestPow2(baseCapacity);
    table.elemSize = elemSize + sizeof(bool);
    table.table = calloc(table.capa, table.elemSize);
    if (!table.table) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate hash table data!");

    return table;
}
uint hashTblHash(hash_tbl* table, void* elem) { return table->hash(table, elem); }



///// DICTIONNARY

struct Dictionnary {
    hash_tbl table;
    size_t elemSize;
    size_t keySize;
};
#define dict_key(d, ptr) ((ptr) + sizeof(bool))
#define dict_val(d, ptr) ((ptr) + sizeof(bool) + (d)->keySize)

dict createDictionnary_P(size_t elemSize, size_t keySize, func_hash hash, func_comp comp, uint baseCapacity) {
    return (dict){.elemSize = elemSize, .keySize = keySize, .table = createHashTable_P(elemSize + keySize, hash, comp, baseCapacity)};
}
void* dictGet_P(dict* d, void* key) {
    uint idx = d->table.hash(&d->table, key);
    void* ptr = d->table.table + idx * d->table.elemSize;
    // if (!hash_used(ptr)) return NULL;

    uint i = 0;
    for (; i < d->table.capa; i++) {
        // if (!hash_used(ptr)) return NULL;
        if (hash_used(ptr) && d->table.comp(dict_key(d, ptr), key)) break;
        idx = (idx + 1) % d->table.capa;
        ptr = d->table.table + idx * d->table.elemSize;
    }
    return i == d->table.capa ? NULL : ptr + sizeof(bool) + d->keySize;
}
bool dictAdd_P(dict* d, void* key, void* value) {
    uint idx = d->table.hash(&d->table, key);

    void* ptr = d->table.table + idx * d->table.elemSize;
    for (uint i = 0; i < d->table.capa; i++) {
        if (!hash_used(ptr)) break;
        if (d->table.comp(dict_key(d, ptr), key)) {
            memcpy(ptr + sizeof(bool) + d->keySize, value, d->elemSize);
            return false;
        }
        idx = (idx + 1) % d->table.capa;
        ptr = d->table.table + idx * d->table.elemSize;
    }
    if (hash_used(ptr)) return false;

    *(bool*)(ptr) = true;
    memcpy(ptr + sizeof(bool), key, d->keySize);
    memcpy(ptr + sizeof(bool) + d->keySize, value, d->elemSize);
    return true;
}
bool dictRemove_P(dict* d, void* key, void* out) {
    uint idx = d->table.hash(&d->table, key);

    void* ptr = d->table.table + idx * d->table.elemSize;
    for (uint i = 0; i < d->table.capa; i++) {
        if (d->table.comp(dict_key(d, ptr), key)) {
            if (out) memcpy(out, ptr + sizeof(bool) + d->keySize, d->elemSize);
            hash_used(ptr) = false;
            return true;
        }
        idx = (idx + 1) % d->table.capa;
        ptr = d->table.table + idx * d->table.elemSize;
    }

    return false;
}