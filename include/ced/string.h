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
#define CED_STRING_STEP (8L)
#endif

#ifdef END_CH
#define string_pushvar(string_ptr, ...)                                          \
  string_push(string_ptr, __VA_ARGS__, END_CH)
#endif

// non-terminated, ASCII-based, and heap-allocated string type: String
typedef struct String String;

struct String {
    char *raw_str;
    size_t len;
    Layout layout;
};

char *string(String *string);
String string_new(void);
void string_push(String *string, ...);
void string_pushch(String *string, char ch);
void string_pushstr(String *string, char *cstr);
char *string_at(String *string, size_t index);
void string_dealloc(String *string);

#endif
