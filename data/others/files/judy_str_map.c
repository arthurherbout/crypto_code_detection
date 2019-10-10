
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef JUDYERROR
#define JUDYERROR(CallerFile, CallerLine, JudyFunc, JudyErrno, JudyErrID) \
{                                                                         \
   if ((JudyErrno) == JU_ERRNO_NOMEM)                                     \
       goto out_of_memory;                                                \
}
#include <Judy.h>

#include "judy_str_map.h"

#define MAX_NUM_RETRIES 16

struct jsm_item{
    uint64_t id;
    uint64_t length;
    char value[0];
} __attribute__((packed));

static uint64_t jsm_get_large(struct judy_str_map *jsm,
                              const char *buf,
                              uint64_t length,
                              uint32_t num_retries)
{

    Word_t *ptr;
    Word_t key;

    XXH64_reset(&jsm->hash_state, num_retries + 1);
    XXH64_update(&jsm->hash_state, buf, length);
    key = XXH64_digest(&jsm->hash_state);

    JLG(ptr, jsm->large_map, key);
    if (ptr){
        const struct jsm_item *item_ro =
            (const struct jsm_item*)&jsm->buffer[*ptr - 1];

        if (item_ro->length == length && !memcmp(item_ro->value, buf, length))
            return item_ro->id;
        else if (++num_retries < MAX_NUM_RETRIES)
            return jsm_get_large(jsm, buf, length, num_retries);
    }
    return 0;
}

static uint64_t jsm_insert_large(struct judy_str_map *jsm,
                                 const char *buf,
                                 uint64_t length,
                                 uint32_t num_retries)
{
    Word_t *ptr;
    Word_t key;

    XXH64_reset(&jsm->hash_state, num_retries + 1);
    XXH64_update(&jsm->hash_state, buf, length);
    key = XXH64_digest(&jsm->hash_state);

    JLI(ptr, jsm->large_map, key);
    if (*ptr){
        const struct jsm_item *item_ro =
            (const struct jsm_item*)&jsm->buffer[*ptr - 1];

        if (item_ro->length == length && !memcmp(item_ro->value, buf, length))
            return item_ro->id;
        else{
            if (++num_retries < MAX_NUM_RETRIES)
                return jsm_insert_large(jsm, buf, length, num_retries);
            else{
                fprintf(stderr, "All hash lookups failed for a key of size %"
                                PRIu64". Very strange!\n", length);
                return 0;
            }
        }
    }else{
        struct jsm_item item;

        if (jsm->buffer_offset + length + sizeof(item) > jsm->buffer_size){
            while (jsm->buffer_offset + length + sizeof(item) >
                   jsm->buffer_size)
                jsm->buffer_size *= 2;
            if (!(jsm->buffer = realloc(jsm->buffer, jsm->buffer_size)))
                return 0;
        }

        *ptr = jsm->buffer_offset + 1;
        item.id = ++jsm->num_keys;
        item.length = length;
        memcpy(&jsm->buffer[jsm->buffer_offset], &item, sizeof(item));
        jsm->buffer_offset += sizeof(item);
        memcpy(&jsm->buffer[jsm->buffer_offset], buf, length);
        jsm->buffer_offset += length;
        return item.id;
    }

out_of_memory:
    return 0;
}

/*
fold must return IDs in the ascending order, e.g store_lexicon()
relies on this
*/
void *jsm_fold(const struct judy_str_map *jsm,
               judy_str_fold_fn fun,
               void *state)
{
    uint64_t offset = 0;

    while (offset < jsm->buffer_offset){
        const struct jsm_item *item =
            (const struct jsm_item*)&jsm->buffer[offset];

        state = fun(item->id, item->value, item->length, state);
        offset += item->length + sizeof(struct jsm_item);
    }

    return state;
}

uint64_t jsm_insert(struct judy_str_map *jsm, const char *buf, uint64_t length)
{
    if (length == 0)
        return 0;
    return jsm_insert_large(jsm, buf, length, 0);
}

uint64_t jsm_get(struct judy_str_map *jsm,
                 const char *buf,
                 uint64_t length)
{
    if (length == 0)
        return 0;
    return jsm_get_large(jsm, buf, length, 0);
}

int jsm_init(struct judy_str_map *jsm)
{
    memset(jsm, 0, sizeof(struct judy_str_map));
    jsm->buffer_size = BUFFER_INITIAL_SIZE;
    if (!(jsm->buffer = malloc(jsm->buffer_size)))
        return 1;
    return 0;
}

void jsm_free(struct judy_str_map *jsm)
{
    Word_t tmp;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
    JLFA(tmp, jsm->large_map);
#pragma GCC diagnostic pop
    free(jsm->buffer);

out_of_memory:
    return;
}

uint64_t jsm_num_keys(const struct judy_str_map *jsm)
{
    return jsm->num_keys;
}

uint64_t jsm_values_size(const struct judy_str_map *jsm)
{
    return jsm->buffer_offset - jsm->num_keys * sizeof(struct jsm_item);
}

#ifdef JSM_MAIN
int main(int argc, char **argv)
{
    void *print_key(uint64_t id, const char *value, uint64_t len, void *state)
    {
        printf("%s", value);
        return NULL;
    }

    FILE *in = fopen(argv[1], "r");
    char *line = NULL;
    size_t len = 0;
    struct judy_str_map jsm;
    ssize_t read;

    jsm_init(&jsm);

    while ((read = getline(&line, &len, in)) != -1)
        jsm_insert(&jsm, line, read + 1);

    fprintf(stderr, "Found %"PRIu64" unique lines\n", jsm_num_keys(&jsm));
    jsm_fold(&jsm, print_key, NULL);

    return 0;
}
#endif
