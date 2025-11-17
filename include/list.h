#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include <stdbool.h>

#define item_from_offset(ptr, type, member)                                    \
    ((type *)((char *)(ptr) - (uintptr_t)(&((type *)0)->member)))

struct list_head
{
    struct list_head *next;
    struct list_head *prev;
};

void list_init(struct list_head *head);

void __list_add(struct list_head *new, struct list_head *prev,
                struct list_head *next);
void __list_del(struct list_head *prev, struct list_head *next);

void list_add_head(struct list_head *new, struct list_head *head);
void list_add_tail(struct list_head *new, struct list_head *head);

void   list_del(struct list_head *entry);
bool   list_is_empty(const struct list_head *head);
size_t list_size(const struct list_head *head);


#define list_for_each(pos, head)                                               \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head)                                       \
    for (pos = (head)->next, n = pos->next; pos != (head);                     \
         pos = n, n = pos->next)

#define list_for_each_prev(pos, head)                                          \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#endif // __LIST_H__