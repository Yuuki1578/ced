#pragma once

#include <stddef.h>
#include <stdint.h>

#ifndef CED_MEMORY
#define CED_MEMORY

// default limit is set to 3GiB
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

// simple memory layout, providing basic features such as:
// 1. the size of the type
// 2. the memory capacity
// 3. the pointer length
// 4. status of the current block of memory, which can be:
//   1. UNIQUE == -1
//   2. NULLPTR == 0
//   3. NONNULL == 1
typedef struct Layout Layout;

struct Layout {
  uint16_t t_size;
  size_t cap;
  size_t len;
  int status;
};

// create a new layout, if the t_size == 0, it's set to sizeof(char) by default
Layout layout_new(uint16_t t_size, size_t default_len);

// adding necessary element to the Layout, incrementing the length and capacity
void layout_add(Layout *layout, size_t count);

// removing <count> element from Layout and the capacity
void layout_min(Layout *layout, size_t count);

// allocate memory from the heap based on Layout->capacity using C malloc
// flag is used to tell the allocator, if the successful memory should
// be set to zero or not
void *layout_alloc(Layout *layout, int flag);

// reallocating the memory based on Layout->capacity
void *layout_realloc(Layout *layout, void *dst);

// deallocating the memory, returning the memory back to the operating system
// and set the Layout capacity and length to zero, but keep the type size untouched
void layout_dealloc(Layout *layout, void *dst);

#endif
