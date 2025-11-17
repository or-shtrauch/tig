#ifndef __PB_H__
#define __PB_H__

#include <stdbool.h>
#include <protobuf-c/protobuf-c.h>

#include "tig.pb-c.h"
#include "heap.h"

bool __pb_pack_to_buffer(struct allocator *const allocator,
                         const ProtobufCMessage *message, uint8_t **buffer,
                         size_t *size);

#define pb_pack_to_buffer(allocator, message, buffer, size)                    \
    __pb_pack_to_buffer(allocator, (const ProtobufCMessage *)message,          \
                        (uint8_t **)buffer, size)

#define pb_pack_to_buffer_heap(heap, message, buffer, size)                    \
    pb_pack_to_buffer(&((heap)->alloc), message, buffer, size)


bool __pb_unpack(struct allocator *const allocator, const uint8_t *buffer,
                 size_t len, const ProtobufCMessageDescriptor *descriptor,
                 void **message);
#define pb_unpack(allocator, buffer, len, type, message)                       \
    __pb_unpack(allocator, (const uint8_t *)buffer, len, &type##__descriptor,  \
                (void **)message)

#define pb_unpack_heap(heap, buffer, len, type, message)                       \
    pb_unpack(&((heap)->alloc), buffer, len, type, message)


#endif // __PB_H__