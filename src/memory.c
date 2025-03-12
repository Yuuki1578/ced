#include <ced/memory.h>
#include <stdint.h>
#include <stdlib.h>

layout_t layout_new(uint16_t t_size, size_t default_len) {
  if (t_size == 0) {
    t_size = sizeof(char);
  }

  return (layout_t){
      .t_size = t_size,
      .cap = t_size * default_len,
      .len = default_len,
      .status = null_ptr,
  };
}

void layout_add(layout_t *layout, size_t count) {
  if (layout == nullptr || !count)
    return;

  if (!layout->t_size)
    layout->t_size = sizeof(char);

  layout->cap += (layout->t_size * count);
  layout->len += count;
}

void layout_min(layout_t *layout, size_t count) {
  if (layout == nullptr || !count)
    return;

  if (layout->len < count)
    return;

  layout->cap -= (layout->t_size * count);
  layout->len -= count;
}

void *layout_alloc(layout_t *layout, int flag) {
  void *dump;

  if (layout == nullptr)
    return malloc(CED_ALLOC_UNSPEC);

  // cannot allocate more than default limit (3GiB)
  if (!layout->cap || layout->cap > CED_ALLOC_LIMIT) {
    dump = malloc(CED_ALLOC_UNSPEC);

    if (dump == nullptr)
      layout->status = null_ptr;

    layout->status = unique_ptr;
    return dump;
  }

  dump = malloc(layout->cap);

  if (dump == nullptr)
    layout->status = null_ptr;

  // set all the block of memory into 0
  if (flag != CED_DEFAULT) {
    size_t type_size = layout->t_size;

    if (!layout->t_size)
      type_size = sizeof(char);

    memset(dump, 0, type_size);
  }

  layout->status = non_null;
  return dump;
}

void *layout_realloc(layout_t *layout, void *dst) {
  void *dump;

  if (layout == nullptr || dst == nullptr)
    return malloc(CED_ALLOC_UNSPEC);

  dump = realloc(dst, layout->cap);

  if (dump == nullptr)
    layout->status = null_ptr;

  return dump;
}

void layout_dealloc(layout_t *layout, void *dst) {
  if (layout == nullptr || layout->status != non_null) {

    if (layout->status == null_ptr)
      return;

    goto end;
  }

  if (layout == nullptr || dst == nullptr)
    return;

end:
  free(dst);

  // invalidate the pointer
  dst = nullptr;
  layout->cap = 0;
  layout->len = 0;
  layout->status = null_ptr;
}
