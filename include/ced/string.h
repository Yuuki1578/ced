#pragma once

#include <ced/memory.h>
#include <limits.h>
#include <stddef.h>

#ifndef CED_STRING
#define CED_STRING

#ifdef CHAR_MIN
#define END_CH CHAR_MIN
#else
#define END_CH (0)
#endif

#ifndef CED_STRING_STEP
#define CED_STRING_STEP (64L)
#endif

#ifdef END_CH
// same as string_push(), except it doesn't need to insert the END_CH value
#define string_pushvar(string_ptr, ...) string_push(string_ptr, __VA_ARGS__, END_CH)
#endif

// non-terminated, ASCII-based, and heap-allocated string type: String
typedef struct String {
    char *raw_str;
    size_t len;
    Layout layout;
} String;

// initialize null string
String string_new(void);

// return the first pointer of the char* from String
char *string(String *string);

// allocate additional <count> bytes memory space to raw buffer
// can be use to minimalize the malloc() or realloc() call
void string_reserve(String *string, size_t count);

// pushing a char variable or expression at the end of the raw buffer
// argument may be more than 2, but cannot less than 1
// always end the variadic arguments with END_CH macro
// or use the alternative string_pushvar() macro
void string_push(String *string, ...);

// pushing a single character at the end of the raw buffer
void string_pushch(String *string, char ch);

// pushing a C-string at the end of the raw buffer
void string_pushstr(String *string, char *cstr);

// return the address of String at index <index>
// return null if index >= String->len
char *string_at(String *string, size_t index);

// truncate remaining unused bytes in the buffer
// the capacity is now String->len * Layout->t_size
void string_clip(String *string);

// deallocate the inner buffer, freeing it's memory
void string_dealloc(String *string);

#endif
