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

typedef struct String String;

struct String {
  char *raw_str;
  size_t len;
  Layout layout;
};

char *string(String *string);
String string_new(void);

void string_push(String *string, ...)
    __attribute__((deprecated("Warning, zero-termnated function, use with a "
                              "last sentinel END_CH value")));

void string_pushstr(String *string, char *cstr);
void string_dealloc(String *string);

#endif
