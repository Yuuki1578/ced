#ifndef CED_STRING_H
#define CED_STRING_H

#include <ced/memory.h>
#include <limits.h>
#include <stddef.h>

#define CED_STRING_STEP 128ULL

#define CED_STRING_OK   0
#define CED_STRING_ERR  -1

/*
 * non-terminated, ASCII-based, and heap-allocated string type: String
 * */
typedef struct {
    char      *raw_str;
    size_t    len;
    layout_t  layout;
} string_t;

/*
 * initialize null string
 * */
string_t string_new(void);

/*
 * return the first pointer of the char* from String
 * */
char *string_into(string_t *string);

/*
 * allocate additional <count> bytes memory space to raw buffer
 * can be use to minimalize the malloc() or realloc() call
 * */
ssize_t string_reserve(string_t *string, size_t count);

/*
 * pushing a single character at the end of the raw buffer
 * */
int string_push(string_t *string, char ch);

/*
 * pushing a C-string at the end of the raw buffer
 * */
ssize_t string_pushstr(string_t *string, char *cstr);

/*
 * return the address of a single character at index <index>
 * return null if index >= string_t->len
 * */
char *string_at(string_t *string, size_t index);

/*
 * truncate remaining unused bytes in the buffer
 * the capacity is now string_t->len * layout_t->t_size
 * */
void string_crop(string_t *string);

/*
 * deallocate the inner buffer, freeing it's memory
 * */
void string_free(string_t *string);

#endif
