#include <SupSy/SL.h>

/// @brief Hash table structure
typedef struct HashTable hash_tbl;
/// @brief Hashing function for a hash table
typedef uint (*func_hash)(hash_tbl*, void*);
/// @brief Comparison function for a hash table
typedef bool (*func_comp)(void*, void*);

/// @brief Create a new Hash table
/// @param elemSize The size of an element in the table
/// @param hash The hash function
/// @param comp The compare function
/// @param baseCapacity The initial capacity of the table
/// @return The newly created hash table
hash_tbl createHashTable(size_t elemSize, func_hash hash, func_comp comp, uint baseCapacity);
/// @brief Hash an element using the hash function of a table
/// @param table The table to use
/// @param elem A pointer to the element to hash
/// @return The resulting hash
uint hashTblHash(hash_tbl* table, void* elem);



/// @brief Dictionnary structure
typedef struct Dictionnary dict;

/// @brief Create a Dictionnary
/// @param elemSize The size of an element in the dictionnary
/// @param keySize The size of a key
/// @param hash The hash function
/// @param comp The compare function
/// @param baseCapacity The initial capacity of the dictionnary
/// @return The newly created dictionnary
dict createDictionnary(size_t elemSize, size_t keySize, func_hash hash, func_comp comp, uint baseCapacity);

/// @brief Get an element by key from a dictionnary
/// @param d The dictionnary
/// @param key The key of the wanted element
/// @return A pointer to the element if found, NULL otherwise
void* dictGet(dict* d, void* key);
/// @brief Add an element with a key to a dictionnary
/// @param d The dictionnary
/// @param key The key of the element
/// @param value The element itself
/// @return Wether the value was successfuly added (returns false if the key has already been assigned)
bool dictAdd(dict* d, void* key, void* value);
/// @brief Remove an element from a dictionnary
/// @param d The dictionnary
/// @param key The key of the element to remove
/// @param out Memory in which the removed element is copied (if non-NULL and an element was found)
/// @return Wether an element has been removed
bool dictRemove(dict* d, void* key, void* out);