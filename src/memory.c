#include <ced/memory.h>
#include <stdlib.h>

Layout layout_new(uint16_t t_size) {
  return (Layout){
      .t_size = t_size != 0 ? t_size : sizeof(void),
      .cap = 0,
      .len = 0,
      .status = NULLPTR,
  };
}

void layout_add(Layout *MaybeNull layout, size_t count) {
  if (layout == nullptr || count == 0) {
    return;
  }

  if (layout->t_size == 0) {
    layout->t_size = sizeof(void);
  }

  layout->cap += (layout->t_size * count);
  layout->len += count;
}

void layout_min(Layout *MaybeNull layout, size_t count) {
  if (layout == nullptr || count == 0) {
    return;
  }

  if (layout->len < count) {
    return;
  }

  layout->cap -= (layout->t_size * count);
  layout->len -= count;
}

UniquePtr layout_alloc(Layout *MaybeNull layout, int flag) {
  UniquePtr dump;

  if (layout == nullptr) {

    // return unique pointer that can be fed to free() later
    return malloc(CED_ALLOC_UNSPEC);
  }

  // cannot allocate more than default limit (3GB)
  if (layout->cap == 0 || layout->cap > CED_ALLOC_LIMIT) {
    dump = malloc(CED_ALLOC_UNSPEC);

    if (dump == nullptr) {
      layout->status = NULLPTR;
    }

    else {

      // flag to unique pointer, do NOT use the unique pointer
      // instead, free it using free()
      layout->status = UNIQUE;
    }

    return dump;
  }

  dump = malloc(layout->cap);

  if (dump == nullptr) {
    layout->status = NULLPTR;
  }

  // set all the block of memory into 0
  if (flag != CED_DEFAULT) {
    size_t type_size = layout->t_size;

    if (layout->t_size == 0) {
      type_size = sizeof(void);
    }
    
    memset(dump, 0, type_size);
  }

  layout->status = NONNULL;
  return dump;
}

UniquePtr layout_realloc(Layout *MaybeNull layout, UniquePtr dst) {

  // Uninitialize
  UniquePtr dump;

  if (layout == nullptr || dst == nullptr) {

    // malloc to zero, return a unique pointer that can be fed to free()
    // or let it be null pointer and set errno on error
    // see the malloc(3)
    return malloc(CED_ALLOC_UNSPEC);
  }

  dump = realloc(dst, layout->cap);

  if (dump == nullptr) {
    layout->status = NULLPTR;
  }

  return dump;
}

void layout_free(Layout *MaybeNull layout, UniquePtr dst) {

  // @WARNING never EVER use NULL or nullptr in dst if layout->status is NONNULL
  if (layout == nullptr || layout->status == NONNULL) {

    if (layout->status != UNIQUE) {
      return;
    }
    
    // free the unique pointer
    free(dst);
    return;
  }

  if (layout == nullptr || dst == nullptr) {
    return;
  }

  free(dst);

  // invalidate the pointer
  dst = nullptr;
  layout->cap = 0;
  layout->len = 0;
  layout->status = NULLPTR;
}
