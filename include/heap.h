#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "list.h"
#include "allocator.h"

struct heap
{
    struct allocator alloc;
};

typedef struct heap heap_t;

heap_t *heap_init();
void   *__heap_alloc(heap_t *const heap, size_t size);
void    __heap_destroy(heap_t *heap);

#define _heap_alloc(heap, size, type) ((type)__heap_alloc(heap, size))

#define heap_alloc(heap, size, type) _heap_alloc(heap, size, type)

#define heap_alloc_object(heap, type) heap_alloc(heap, sizeof(type), type *)
#define heap_alloc_array(heap, count, type)                                    \
    heap_alloc(heap, count * sizeof(type), type *)


#define heap_destroy(heap)                                                     \
    do {                                                                       \
        __heap_destroy(heap);                                                  \
        heap = NULL;                                                           \
    } while (0)

static inline void __local_heap_cleanup(void *_p)
{
    void **p = (void **)_p;
    if (p && *p) {
        free_fn(*p);
        *p = NULL;
    }
}

#define CLEANUP(f) __attribute__((cleanup(f)))
#define DECLARE_LOCAL_HEAP(name)                                               \
    heap_t CLEANUP(__local_heap_cleanup) *name = heap_init()

#endif // __HEAP_H__