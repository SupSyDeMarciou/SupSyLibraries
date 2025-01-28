#ifndef __SL_UTILS_INOUT_H__
#define __SL_UTILS_INOUT_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../structures.h"

#define ESCAPE_CODE "\033"
#define ANSII ESCAPE_CODE"["

#define T_GRAPHICS(value) ANSII value "m"

#define TG_RESET "0"
#define TG_BOLD "1"
#define TG_FAINT "2"
#define TG_ITALIC "3"
#define TG_UNDERLINE "4"
#define TG_SLOW_BLINK "5"
#define TG_FAST_BLINK "6"
#define TG_INVERT "7"
#define TG_STRIKE_THROUGH "9"

#define TGC_FG "3"
#define TGC_BG "4"
#define TGC_FG_B "9"
#define TGC_BG_B "10"

#define TGC_BLACK "0"
#define TGC_RED "1"
#define TGC_GREEN "2"
#define TGC_YELLOW "3"
#define TGC_BLUE "4"
#define TGC_MAGENTA "5"
#define TGC_CYAN "6"
#define TGC_WHITE "7"

#define TG_COLOR(color, fgbg) T_GRAPHICS(fgbg color)
#define TG_COLOR_B(color, fgbg) T_GRAPHICS(fgbg##_B color)

/// @brief Terminates execution and prints to error output
/// @param message The message to be displayed when called
#define SL_throwError(message, ...) do { \
    fprintf(stderr, TG_COLOR(TGC_RED, TGC_FG) "ERROR - [%s]:\n\t"message"\n"TG_COLOR(TGC_RED, TGC_FG)"at %s:%d"T_GRAPHICS(TG_RESET)"\n", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
} while (false)
/// @brief Prints to error output
/// @param message The message to be displayed when called
#define SL_throwWarning(message, ...) fprintf(stderr, TG_COLOR(TGC_YELLOW, TGC_FG) "WARNING - [%s]:\n\t"message"\n"TG_COLOR(TGC_YELLOW, TGC_FG)"at %s:%d"T_GRAPHICS(TG_RESET)"\n", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

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
static inline bool SL_isFigure(int n) {
    return (n >= 0 && n < 10);
}

/// @brief If a character represents a number
/// @param c The character to test
/// @return If it is a number
static inline bool SL_isNum(char c) {
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
bool SL_match(const char* a, const char* b);
/// @brief Compare two strings with no regards for capitalization 
/// @param a Left string
/// @param b Right string
/// @return Wether the two strings matched
bool SL_match_NoCase(const char* a, const char* b);
/// @brief Find prefix at start of text
/// @param text Where to search
/// @param prefix The prefix to find
/// @return Wether the prefix has been found
bool SL_match_Start(const char* text, const char* prefix);
/// @brief Match a factor inside of a string
/// @param text The text to inspect
/// @param factor The factor to find
/// @return Wether the factor appears in the text
bool SL_match_Factor(const char* text, const char* factor);

/// @brief Reallocate chunk of memory
/// @param data The previously allocated data
/// @param lastSize The size of the previous chunk
/// @param newSize The size of the new chunk
/// @return The newly allocated (and filled) chunk of memory
void* SL_realloc(void* data, size_t lastSize, size_t newSize);

#endif