#include <string.h>

#include "allocator.h"

bool allocator_init(struct allocator *alloc, const char *name)
{
    if (!name) {
        return false;
    }

    alloc->name = name;
    list_init(&alloc->list);
    return true;
}

struct allocation *allocator_alloc(struct allocator *allocator, size_t size)
{
    if (!allocator || size == 0) {
        return NULL;
    }

    struct allocation *item = alloc_fn(sizeof(struct allocation) + size);
    if (!item) {
        return NULL;
    }

    item->size = size;
    list_add_tail(&item->list, &allocator->list);

    return item;
}

void allocator_free(struct allocator *allocator, struct allocation *alloc)
{
    if (!allocator || !alloc) {
        return;
    }

    list_del(&alloc->list);
    free_fn(alloc);
}

void __allocator_destroy(struct allocator *allocator)
{
    if (!allocator) {
        return;
    }

    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &allocator->list)
    {
        struct allocation *item =
            item_from_offset(pos, struct allocation, list);

        if (item->size) {
            free_fn(item);
        }
    }
}
