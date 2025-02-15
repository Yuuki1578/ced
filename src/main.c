#include <ced/memory.h>
#include <stdio.h>

int main(int argc, char **argv) {
  Layout layout;
  long *array;

  layout = layout_new(sizeof(long));
  layout_add(&layout, 10);

  array = layout_alloc(&layout, CED_ZEROING);

  if (layout.status == NULLPTR) {
    return 1;
  }

  layout_add(&layout, 20);
  array = layout_realloc(&layout, array);

  for (int i = 0; i < layout.len; i++, array[i] = i % 2 == 0 ? i : i + 1) {
    printf("%li\n", array[i]);
  }

  layout_free(&layout, array);

  return 0;
}
