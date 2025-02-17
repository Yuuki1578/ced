#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <ced/memory.h>
#include <ced/string.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

char *string(String *string) {
  char *wildcard = "";

  if (string == nullptr || string->raw_str == nullptr) {
    return wildcard;
  }

  return string->raw_str;
}

String string_new(void) {
  return (String){
      .raw_str = NULL,
      .len = 0,
      .layout = layout_new(sizeof(char), 0),
  };
}

void string_push(String *string, ...) {
  va_list variadic;
  unsigned int args_recv;

  if (string == nullptr) {
    return;
  }

  va_start(variadic);

  while ((args_recv = va_arg(variadic, unsigned int)) != END_CH) {
    if (args_recv > CHAR_MAX || args_recv == 0) {
      break;
    }

    if (string->layout.status == NULLPTR) {
      layout_add(&string->layout, CED_STRING_STEP);
      string->raw_str = layout_alloc(&string->layout, CED_ZEROING);
    }

    if (string->len == string->layout.cap) {
      layout_add(&string->layout, CED_STRING_STEP);
      string->raw_str = layout_realloc(&string->layout, string->raw_str);
    }

    if (string->layout.status == NULLPTR) {
      break;
    }

    string->raw_str[string->len++] = args_recv;
  }

  va_end(variadic);
}

void string_pushstr(String *string, char *cstr) {
  size_t len;

  if (string == nullptr || cstr == nullptr) {
    return;
  }

  len = strlen(cstr);

  for (size_t count = 0; count < len; count++) {
    string_push(string, cstr[count], END_CH);
  }
}

void string_dealloc(String *string) {
  if (string == nullptr) {
    return;
  }

  layout_dealloc(&string->layout, string->raw_str);
}
