#ifndef __SL_STRUCTURES_H__
#define __SL_STRUCTURES_H__

#include <stdint.h>
#include <stdbool.h>

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t

#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define int64 int64_t

#define uint unsigned int

#define char8 char
#define char16 wchar_t
#define char32 uint32

#define ALWAYS_INLINE inline __attribute__((always_inline))

/// @brief Just for synthaxic purposes, may conflict with some libs
#define IN ,

#define as(newType, value)  (*(newType*)(void*)&(value))
#define as_(newType, value) ((newType*)(void*)(value))

#endif