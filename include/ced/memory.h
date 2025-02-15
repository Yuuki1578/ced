#pragma once

#include <stddef.h>
#include <stdint.h>

#if __STDC_VERSION__ != 202311L
#error Expected C23 version
#endif

#ifndef CED_MEMORY
#define CED_MEMORY

#ifndef CED_ALLOC_LIMIT
#define CED_ALLOC_LIMIT ((((3L * 1024L) * 1024L) * 1024L))
#endif

#define CED_ALLOC_UNSPEC (0L)

#define UNIQUE (-1)
#define NULLPTR (0)
#define NONNULL (1)

// allocation flag
#define CED_DEFAULT (0)
#define CED_ZEROING (1)

#define MaybeNull _Nullable

typedef struct Layout Layout;
typedef void *_Nonnull UniquePtr;

struct Layout {
  uint16_t t_size;
  size_t cap;
  size_t len;
  int status;
};

Layout layout_new(uint16_t t_size);
void layout_add(Layout *MaybeNull layout, size_t count);
void layout_min(Layout *MaybeNull layout, size_t count);
UniquePtr layout_alloc(Layout *MaybeNull layout, int flag);
UniquePtr layout_realloc(Layout *MaybeNull layout, UniquePtr dst);
void layout_free(Layout *MaybeNull layout, UniquePtr dst);

#endif
