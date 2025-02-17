#include <ced/string.h>
#include <stdio.h>

int main(void) {
  String str = string_new();

  string_pushstr(&str, "H");
  string_pushstr(&str, "e");
  string_pushstr(&str, "l");
  string_pushstr(&str, "l");
  string_pushstr(&str, "o");
  string_pushstr(&str, "!");
  string_push(&str, '!', END_CH);

  printf("Str: %s\n", string(&str));
  printf("Len: %li\n", str.len);
  printf("Cap: %li\n", str.layout.cap);

  string_dealloc(&str);

  return 0;
}
