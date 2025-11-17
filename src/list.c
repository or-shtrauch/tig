#include "list.h"

void list_init(struct list_head *head)
{
    head->next = head;
    head->prev = head;
}

inline void __list_add(struct list_head *new, struct list_head *prev,
                       struct list_head *next)
{
    next->prev = new;
    new->next  = next;
    new->prev  = prev;
    prev->next = new;
}

void list_add_head(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    list_init(entry);
}

bool list_is_empty(const struct list_head *head)
{
    return head->next == head;
}

size_t list_size(const struct list_head *head)
{
    size_t                   count = 0;
    const struct list_head *curr  = head->next;

    while (curr != head) {
        count++;
        curr = curr->next;
    }
    return count;
}