#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <openssl/sha.h>
#include <openssl/rand.h>

#include "tig.pb-c.h"
#include "pb.h"

static uint64_t generate_commit_hash()
{
    uint64_t random_id;

    if (RAND_bytes((unsigned char *)&random_id, sizeof(uint64_t)) != 1) {
        fprintf(stderr, "Error: Failed to generate secure random bytes.\n");
        return 0;
    }

    return random_id;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    heap_t *heap = heap_init();

    Tig *t = heap_alloc_object(heap, Tig);

    *t           = (Tig){
        .base = {
            .descriptor = &tig__descriptor,
        },
        .n_commits = 4,
        .commits   = NULL,
    };

    t->commits = heap_alloc_array(heap, t->n_commits, Commit *);

    for (size_t i = 0; i < t->n_commits; i++) {
        t->commits[i] = heap_alloc_object(heap, Commit);
        *t->commits[i] = (Commit) {
            .base = {
                .descriptor = &commit__descriptor,
            },
            .has_hash = true,
            .hash = generate_commit_hash(),
        };
    }
    char  *w_buffer = NULL;
    size_t size     = 0;
    pb_pack_to_buffer_heap(heap, t, &w_buffer, &size);
    printf("w_buffer size: %zu\n", size);

    FILE *fp = fopen("tmp.bin", "w");
    fwrite(w_buffer, 1, size, fp);
    fclose(fp);

    fp             = fopen("tmp.bin", "r");
    char *r_buffer = heap_alloc(heap, 1024, char *);
    size           = fread(r_buffer, 1, 1024, fp);
    printf("r_buffer size: %zu\n", size);
    fclose(fp);


    Tig *a = NULL;
    pb_unpack_heap(heap, r_buffer, size, tig, &a);

    for (size_t i = 0; i < a->n_commits; i++) {
        printf("commit hash: %" PRIu64 "\n", a->commits[i]->hash);
    }

    heap_destroy(heap);
    return 0;
}