#include "heap.h"
#include "pb.h"
#include "allocator.h"

static void *pb_allocator_alloc(void *alloc, size_t size)
{
    struct allocator  *a    = (struct allocator *)alloc;
    struct allocation *item = allocator_alloc(a, size);
    if (!item) {
        return NULL;
    }

    return (void *)item->data;
}

static void pb_allocator_free(void *alloc, void *ptr)
{
    if (!ptr || !alloc)
        return;

    struct allocator  *a    = (struct allocator *)alloc;
    struct allocation *item = item_from_offset(ptr, struct allocation, data);

    allocator_free(a, item);
}

bool __pb_pack_to_buffer(struct allocator *const allocator,
                         const ProtobufCMessage *message, uint8_t **buffer,
                         size_t *size)
{
    if (!allocator || !message || !buffer || !size)
        return false;

    *size   = protobuf_c_message_get_packed_size(message);
    *buffer = (uint8_t *)allocator_alloc(allocator, *size);

    if (!*buffer) {
        return false;
    }

    return protobuf_c_message_pack(message, *buffer) == *size;
}

bool __pb_unpack(struct allocator *const allocator, const uint8_t *buffer,
                 size_t len, const ProtobufCMessageDescriptor *descriptor,
                 void **message)
{
    if (!allocator || !buffer || !descriptor || !message)
        return false;

    *message = protobuf_c_message_unpack(descriptor,
                                         &(ProtobufCAllocator){
                                             .alloc = pb_allocator_alloc,
                                             .free  = pb_allocator_free,
                                             .allocator_data = allocator,
                                         },
                                         len, buffer);
    return *message != NULL;
}
