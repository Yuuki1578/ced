#pragma once

#include <stddef.h>
#include <stdint.h>

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

typedef struct Layout Layout;

struct Layout {
  uint16_t t_size;
  size_t cap;
  size_t len;
  int status;
};

Layout layout_new(uint16_t t_size, size_t default_len);
void layout_add(Layout *layout, size_t count);
void layout_min(Layout *layout, size_t count);
void *layout_alloc(Layout *layout, int flag);
void *layout_realloc(Layout *layout, void *dst);
void layout_dealloc(Layout *layout, void *dst);

#endif
