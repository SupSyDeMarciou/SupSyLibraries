#include "arenaAlloc.h"
#include "inout.h"

struct ArenaAllocator {
    void* data;
    void* pointer;
    size_t capacity;
};

arena_alloc* newArenaAllocator(size_t capacity) {
    arena_alloc* new = (arena_alloc*)malloc(sizeof(arena_alloc));
    if (!new) SL_throwError("INSUFFICIENT MEMORY - Failed to allocate arena!");
    new->data = malloc(capacity);
    new->pointer = new->data;
    new->capacity = capacity;
    return new;
}
void freeArenaAllocator(arena_alloc* toFree) {
    free(toFree->data);
    free(toFree);
}

void* arenaAlloc(arena_alloc* allocator, size_t size) {
    void* returnPointer = allocator->pointer;
    allocator->pointer += size;
    if ((size_t)(allocator->pointer - allocator->data) >= allocator->capacity) return NULL;
    return returnPointer;
}