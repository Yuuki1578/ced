#include <ced/memory.h>
#include <ced/string.h>
#include <limits.h>
#include <string.h>

char *string_into(string_t *string)
{
    char *wildcard = "";

    if (string == nullptr || string->raw_str == nullptr)
        return wildcard;

    return string->raw_str;
}

string_t string_new(void)
{
    return (string_t) {
        .raw_str = nullptr,
        .len = 0,
        .layout = layout_new(sizeof(char), 0),
    };
}

ssize_t string_reserve(string_t *string, size_t count)
{
    layout_t    *layout;
    void        *tmp;

    if (string == nullptr || count == 0)
        return CED_STRING_ERR;

    if (string->layout.t_size == 0)
        string->layout.t_size = sizeof(char);

    layout = &string->layout;
    layout_add(layout, count);

    switch (string->layout.status) {
    case NON_NULL:
        tmp = layout_realloc(layout, string->raw_str);
        break;

    default:
        tmp = layout_alloc(layout);
        break;
    }

    if (string->layout.status == NULL_PTR)
        return CED_STRING_ERR;

    string->raw_str = tmp;
    return (ssize_t) count;
}

int string_push(str_t *string, char ch)
{
    char *tmp;

    if (string == nullptr)
        return CED_STRING_ERR;

    if (string->layout.cap == 0) {
        layout_add(&string->layout, CED_STRING_STEP);
        tmp = layout_alloc(&string->layout);
    }

    else if (string->layout.cap == string->layout.t_size * string->len) {
        layout_add(&string->layout, CED_STRING_STEP);
        tmp = layout_realloc(&string->layout, string->raw_str);
    }

    switch (string->layout.status) {
    case NULL_PTR:
    case UNIQUE_PTR:
        return CED_STRING_ERR;

    default:
        tmp[string->len++] = ch;
        string->raw_str = tmp;
    }

    return CED_STRING_OK;
}

ssize_t string_pushstr(string_t *string, char *cstr)
{
    size_t    len;
    char      *tmp;

    if (string == nullptr || cstr == nullptr)
        return CED_STRING_ERR;

    len = strlen(cstr);
    layout_add(&string->layout, len);

    if (string->layout.status == NULL_PTR)
        tmp = layout_alloc(&string->layout);
    else
        tmp = layout_realloc(&string->layout, string->raw_ptr);

    if (string->layout.status != NON_NULL)
        return CED_STRING_ERR;

    if (strlen(tmp) == 0)
        strncpy(tmp, cstr, len);
    else
        strncat(tmp, cstr, len);

    string->raw_str = tmp;
    return len;
}

char *string_at(string_t *string, size_t index)
{
    if (string == nullptr || string->layout.status == NULL_PTR)
        return nullptr;

    if (string->len <= index)
        return nullptr;

    return &string->raw_str[index];
}

void string_crop(string_t *string)
{
    layout_t    *current;
    char        *tmp

    if (string == nullptr || string->layout.status == NULL_PTR)
        return;

    current = &string->layout;

    if (string->len == current->cap)
        return;

    layout_min(current, current->cap - string->len);
    tmp = layout_realloc(current, string->raw_str);

    if (current->status != NON_NULL)
        return;

    string->raw_str = tmp;
}

void string_free(string_t *string)
{
    if (string == nullptr)
        return;

    layout_dealloc(&string->layout, string->raw_str);
}
