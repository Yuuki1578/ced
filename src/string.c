#include <ced/memory.h>
#include <ced/string.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

char *str(str_t *string) {
  char *wildcard = "";

  if (string == nullptr || string->raw_str == nullptr)
    return wildcard;

  return string->raw_str;
}

str_t str_new(void) {
  return (str_t){
      .raw_str = nullptr,
      .len = 0,
      .layout = layout_new(sizeof(char), 0),
  };
}

void str_reserve(str_t *string, size_t count) {
  layout_t *layout;

  if (string == nullptr || !count)
    return;

  if (!string->layout.t_size)
    string->layout.t_size = sizeof(char);

  layout = &string->layout;
  layout_add(layout, count);

  switch (string->layout.status) {
  case non_null:
    string->raw_str = layout_realloc(layout, string->raw_str);
    break;

  default:
    string->raw_str = layout_alloc(layout, CED_DEFAULT);
    break;
  }
}

void str_push(str_t *string, ...) {
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

    if (string->layout.status == null_ptr) {
      layout_add(&string->layout, CED_STRING_STEP);
      string->raw_str = layout_alloc(&string->layout, CED_ZEROING);
    }

    else if (string->len == string->layout.cap) {
      layout_add(&string->layout, CED_STRING_STEP);
      string->raw_str = layout_realloc(&string->layout, string->raw_str);
    }

    if (string->layout.status == null_ptr)
      break;

    string->raw_str[string->len++] = args_recv;
  }

  va_end(variadic);
}

void str_pushch(str_t *string, char ch) {
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
  case null_ptr:
    return;
  default:
    string->raw_str[string->len++] = ch;
  }
}

void str_pushstr(str_t *string, char *cstr) {
  size_t len;

  if (string == nullptr || cstr == nullptr)
    return;

  len = strlen(cstr);

  for (size_t count = 0; count < len; count++)
    str_pushch(string, cstr[count]);
}

char *str_at(str_t *string, size_t index) {
  if (string == nullptr || string->layout.status == null_ptr)
    return nullptr;

  if (string->len <= index)
    return nullptr;

  return &string->raw_str[index];
}

void str_trim(str_t *string) {
  layout_t *current;

  if (string == nullptr || string->layout.status == null_ptr)
    return;

  current = &string->layout;

  if (string->len == current->cap)
    return;

  layout_min(current, current->cap - string->len);
  string->raw_str = layout_realloc(current, string->raw_str);
}

void str_free(str_t *string) {
  if (string == nullptr)
    return;

  layout_dealloc(&string->layout, string->raw_str);
}
