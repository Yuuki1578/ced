#include <ced/memory.h>
#include <ced/string.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

char *string(String *string) {
  char *wildcard = "";

  if (string == nullptr || string->raw_str == nullptr)
    return wildcard;

  return string->raw_str;
}

String string_new(void) {
  return (String){
      .raw_str = NULL,
      .len = 0,
      .layout = layout_new(sizeof(char), 0),
  };
}

void string_reserve(String *string, size_t count) {
    Layout *layout;

    if (string == nullptr || !count)
        return;

    if (!string->layout.t_size)
        string->layout.t_size = sizeof(char);

    layout = &string->layout;
    layout_add(layout, count);

    switch (string->layout.status) {
    case NONNULL:
        string->raw_str = layout_realloc(layout, string->raw_str);
        break;

    default:
        string->raw_str = layout_alloc(layout, CED_DEFAULT);
        break;
    }
}

void string_push(String *string, ...) {
  va_list variadic;
  unsigned int args_recv;

  if (string == nullptr)
    return;

  va_start(variadic);

  // @WARNING last argument must be 0 or END_CH or CHAR_MIN
  // as a breakpoint
  while ((args_recv = va_arg(variadic, unsigned int)) != END_CH) {
    if (args_recv > CHAR_MAX || args_recv == 0)
      break;

    if (string->layout.status == NULLPTR) {
      layout_add(&string->layout, CED_STRING_STEP);
      string->raw_str = layout_alloc(&string->layout, CED_ZEROING);
    }

    else if (string->len == string->layout.cap) {
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

void string_pushch(String *string, char ch) {
  if (string == nullptr)
    return;

  if (string->layout.cap == 0) {
    layout_add(&string->layout, CED_STRING_STEP);
    string->raw_str = layout_alloc(&string->layout, CED_ZEROING);
  }

  else if (string->layout.cap == string->layout.t_size * string->len) {
    layout_add(&string->layout, CED_STRING_STEP);
    string->raw_str = layout_realloc(&string->layout, string->raw_str);
  }

  switch (string->layout.status) {
  case NULLPTR:
    return;
  default:
    string->raw_str[string->len++] = ch;
  }
}

void string_pushstr(String *string, char *cstr) {
  size_t len;

  if (string == nullptr || cstr == nullptr)
    return;

  len = strlen(cstr);

  for (size_t count = 0; count < len; count++) {
    string_pushch(string, cstr[count]);
  }
}

char *string_at(String *string, size_t index) {
  if (string == nullptr || string->layout.status == NULLPTR) {
    return nullptr;
  }

  if (string->len <= index) {
    return nullptr;
  }

  return &string->raw_str[index];
}

void string_dealloc(String *string) {
  if (string == nullptr)
    return;

  layout_dealloc(&string->layout, string->raw_str);
}
