#ifndef __SL_UTILS_ARENA_ALLOC_H__
#define __SL_UTILS_ARENA_ALLOC_H__

#include <stdlib.h>

typedef struct ArenaAllocator arena_alloc;

/// @brief Create a new arena allocator
/// @param capacity The initial allocator
/// @return The newly created allocator
arena_alloc* newArenaAllocator(size_t capacity);
/// @brief Free an arena allocator
/// @param toFree The arena to free
void freeArenaAllocator(arena_alloc* toFree);
/// @brief Allocate some memory
/// @param allocator The arena allocator
/// @param size The size of the element to allocate
/// @return The newly allocated memory
void* arenaAlloc(arena_alloc* allocator, size_t size);

#endif