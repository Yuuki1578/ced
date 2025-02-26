#include <ced/memory.h>
#include <stdint.h>
#include <stdlib.h>

Layout layout_new(uint16_t t_size, size_t default_len) {
    if (!default_len) {
        return (Layout){
            .t_size = t_size != 0 ? t_size : sizeof(char),
            .cap = 0,
            .len = 0,
            .status = NULLPTR,
        };
    }

    return (Layout){
        .t_size = t_size,
        .cap = t_size * default_len,
        .len = default_len,
        .status = NULLPTR,
    };
}

void layout_add(Layout *layout, size_t count) {
    if (layout == nullptr || !count)
        return;

    if (!layout->t_size)
        layout->t_size = sizeof(char);

    layout->cap += (layout->t_size * count);
    layout->len += count;
}

void layout_min(Layout *layout, size_t count) {
    if (layout == nullptr || !count)
        return;

    if (layout->len < count)
        return;

    layout->cap -= (layout->t_size * count);
    layout->len -= count;
}

void *layout_alloc(Layout *layout, int flag) {
    void *dump;

    if (layout == nullptr)
        // return unique pointer that can be fed to free() later
        return malloc(CED_ALLOC_UNSPEC);

    // cannot allocate more than default limit (3GiB)
    if (!layout->cap || layout->cap > CED_ALLOC_LIMIT) {
        dump = malloc(CED_ALLOC_UNSPEC);

        if (dump == nullptr)
            layout->status = NULLPTR;

        /*
         * flag to unique pointer, do NOT use the unique pointer
         * instead, free it using free()
         * */
        layout->status = UNIQUE;
        return dump;
    }

    dump = malloc(layout->cap);

    if (dump == nullptr)
        layout->status = NULLPTR;

    // set all the block of memory into 0
    if (flag != CED_DEFAULT) {
        size_t type_size = layout->t_size;

        if (!layout->t_size)
            type_size = sizeof(char);

        memset(dump, 0, type_size);
    }

    layout->status = NONNULL;
    return dump;
}

void *layout_realloc(Layout *layout, void *dst) {

    // Uninitialize
    void *dump;

    if (layout == nullptr || dst == nullptr)

        /*
         * malloc to zero, return a unique pointer that can be fed to free()
         * or let it be null pointer and set errno on error
         * see the malloc(3)
         * */
        return malloc(CED_ALLOC_UNSPEC);

    dump = realloc(dst, layout->cap);

    if (dump == nullptr)
        layout->status = NULLPTR;

    return dump;
}

void layout_dealloc(Layout *layout, void *dst) {

    // @WARNING never EVER use NULL or nullptr in dst if layout->status is NONNULL
    if (layout == nullptr || layout->status != NONNULL) {

        if (layout->status != UNIQUE)
            return;

        // free the unique pointer
        free(dst);
        return;
    }

    if (layout == nullptr || dst == nullptr)
        return;

    free(dst);

    // invalidate the pointer
    dst = nullptr;
    layout->cap = 0;
    layout->len = 0;
    layout->status = NULLPTR;
}
