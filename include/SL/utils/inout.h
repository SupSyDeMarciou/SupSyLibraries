#ifndef __SL_UTILS_INOUT
#define __SL_UTILS_INOUT

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../structures.h"

#ifdef _glfw3_h_
    /// @brief Terminates execution and prints to error output
    /// @param message The message to be displayed when called
    /// @note If using GLFW, will call terminate automatically
    #define failWithError(message, ...) do { \
        fprintf(stderr, "ERROR - [%s]:\n\t"message"\nat %s:%d.", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__); \
        glfwTerminate(); \
        exit(EXIT_FAILURE); \
    } while (false)
#else
    /// @brief Terminates execution and prints to error output
    /// @param message The message to be displayed when called
    /// @note If using GLFW, will call terminate automatically
    #define failWithError(message, ...) do { \
        fprintf(stderr, "ERROR - [%s]:\n\t"message"\nat %s:%d.", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } while (false)
#endif

#define throwWarning(message, ...) fprintf(stderr, "WARNING - [%s]:\n\t"message"\nat %s:%d.", __FUNCTION__, ##__VA_ARGS__, __LINE__, __FILE__)

/// @brief Reads the contents of a file at filePath in a string
/// @param filePath The path to the file (full path)
/// @returns A string containing the entire file
/// @note If the file is not found, execution stops
/// @warning The returned string should be freed after use
char* SL_readFile(const char* filePath);

/// @brief Converts a character to an int
/// @param c The character to convert
/// @warning c must represent a digit  
#define SL_toInt(c) (int)((c) - '0') // Convert ASCII to Int

/// @brief If a number is single-digit
/// @param n The number to test
/// @return If it is a single-digit number
static inline int SL_isFigure(int n) {
    return (n >= 0 && n < 10);
}

/// @brief If a character represents a number
/// @param c The character to test
/// @return If it is a number
static inline int SL_isNum(char c) {
    return (c >= '0' && c <= '9');
}

/// @brief Reads a number in a line
/// @param line The line from which to read
/// @param i The index of one of the digits (can be any one of them)
/// @warning i must not exceed line width
/// @return The number read
int SL_readNum(const char* line, uint i);

/// @brief Print an object as a sequence of bits
/// @param val The object to print
#define SL_printBits(val) do { \
    typeof(val) LOCAL__ = val; \
    uint8* LOCAL_PTR__ = (uint8*)&LOCAL__; \
    printf("0b"); \
    uint LOCAL_i__ = sizeof(LOCAL__) - 1; \
    do { \
        uint8 v = LOCAL_PTR__[LOCAL_i__]; \
        printf("%c%c%c%c%c%c%c%c%c", v & 128 ? '1' : '0', v & 64 ? '1' : '0', v & 32 ? '1' : '0', v & 16 ? '1' : '0', v & 8 ? '1' : '0', v & 4 ? '1' : '0', v & 2 ? '1' : '0', v & 1 ? '1' : '0', LOCAL_i__ ? ' ' : 0); \
    } while (LOCAL_i__--); \
} while (false)

/// @brief Print an object as a sequence of hex values
/// @param val The object to print
#define SL_printHex(val) do { \
    typeof(val) LOCAL__ = val; \
    uint8* LOCAL_PTR__ = (uint8*)&LOCAL__; \
    printf("0x"); \
    uint LOCAL_i__ = sizeof(LOCAL__) - 1; \
    do { \
        uint8 v = LOCAL_PTR__[LOCAL_i__]; \
        uint8 h = (v >> 4) & 0b00001111; \
        uint8 l = v & 0b00001111; \
        printf("%c%c%c", h > 9 ? (h + 'A' - 10) : (h + '0'), l > 9 ? (l + 'A' - 10) : (l + '0'), LOCAL_i__ ? ' ' : 0); \
    } while (LOCAL_i__--); \
} while (false)

/// @brief Compare two strings
/// @param a Left string
/// @param b Right string
/// @return Wether the two strings matched
bool SL_cmpStr(const char* a, const char* b);
/// @brief Match a factor inside of a string
/// @param text The text to inspect
/// @param factor The factor to find
/// @return Wether the factor appears in the text
bool SL_matchFactor(const char* text, const char* factor);

/// @brief Reallocate chunk of memory
/// @param data The previously allocated data
/// @param lastSize The size of the previous chunk
/// @param newSize The size of the new chunk
/// @return The newly allocated (and filled) chunk of memory
void* SL_realloc(void* data, size_t lastSize, size_t newSize);

#endif