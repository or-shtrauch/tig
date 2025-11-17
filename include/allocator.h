#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stdint.h>
#include <stdlib.h>
#include "list.h"

#define alloc_fn malloc
#define free_fn  free

struct allocation
{
    struct list_head list;
    size_t           size;
    uint8_t          data[0];
};

struct allocator
{
    const char      *name;
    struct list_head list;
};

bool               allocator_init(struct allocator *alloc, const char *name);
struct allocation *allocator_alloc(struct allocator *allocator, size_t size);
void allocator_free(struct allocator *allocator, struct allocation *alloc);
void __allocator_destroy(struct allocator *allocator);


#define allocator_destroy(allocator)                                           \
    do {                                                                       \
        __allocator_destroy(allocator);                                        \
        allocator = NULL;                                                      \
    } while (0)

#endif // __ALLOCATOR_H__