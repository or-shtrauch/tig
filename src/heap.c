#include "heap.h"
#include "allocator.h"

heap_t *heap_init()
{
    heap_t *heap = (heap_t *)alloc_fn(sizeof(heap_t));
    if (!heap) {
        return NULL;
    }

    if (!allocator_init(&heap->alloc, "heap")) {
        free_fn(heap);
        return NULL;
    }

    return heap;
}

void *__heap_alloc(heap_t *const heap, size_t size)
{
    struct allocation *item = allocator_alloc(&heap->alloc, size);
    if (!item) {
        return NULL;
    }

    return (void *)item->data;
}

void __heap_destroy(heap_t *heap)
{
    struct allocator *allocator = &heap->alloc;
    __allocator_destroy(allocator);

    free_fn(heap);
}
