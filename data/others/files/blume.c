#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "blume.h"
#include "murmur.h"

// "M_0"
#define INITIAL_M (1024 * 1024)
// "r"
#define ERROR_TIGHTENING_RATIO 0.5
// "s"
#define GROWTH_FACTOR 2.0

/* On disk */
struct filter_header {
    double P;
    uint64_t k;
    uint64_t M;
    uint64_t n;
    uint64_t m;
    uint64_t keys_count;
    off_t array_bytes;
};

/* On disk */
struct filter {
    struct filter_header header;
    uint8_t bits[1];
};

#define MAX_FILTERS 32

/* On disk */
struct blume_header {
    off_t max_file_size;
    double P;
    uint64_t filter_count;
    off_t filters[MAX_FILTERS];
};

/* In memory */
struct blume {
    char *path;
    off_t fd_size;
    int fd;
    int for_write;
    size_t map_size;
    void *map;
};

blume_error
blume_validate(struct blume *bf)
{
    struct blume_header *bf_header = (struct blume_header *)bf->map;
    if (sizeof(struct blume_header) > bf->map_size) {
        return BLUME_ERROR_CORRUPT;
    }

    uint64_t idx;
    for (idx = 0; idx < bf_header->filter_count; idx += 1) {
        struct filter *f = (struct filter *)(bf->map + bf_header->filters[idx]);
        if ((bf_header->filters[idx] + sizeof(struct filter)) >= bf->map_size) {
            return BLUME_ERROR_CORRUPT;
        }

        if ((bf_header->filters[idx] + sizeof(struct filter_header) + f->header.array_bytes) > bf->map_size) {
            return BLUME_ERROR_CORRUPT;
        }
    }

    return BLUME_ERROR_SUCCESS;
}

static blume_error
resize_map(struct blume *bf, off_t new_size)
{
    blume_error rv = BLUME_ERROR_SUCCESS;
    void *new_map = NULL;

    msync(bf->map, bf->map_size, MS_SYNC);

    if (ftruncate(bf->fd, new_size) == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail;
    }
    bf->fd_size = new_size;

    if (munmap(bf->map, bf->map_size) == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail;
    }

    bf->map = NULL;
    bf->map_size = 0;

    if ((new_map = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, bf->fd, 0)) == MAP_FAILED) {
        bf->map = NULL;
        rv = BLUME_ERROR_ERRNO;
        goto fail;
    }

    bf->map = new_map;
    bf->map_size = new_size;

fail:
    return rv;
}

static blume_error
filter_create(struct blume *bf)
{
    blume_error rv = BLUME_ERROR_SUCCESS;
    struct blume_header *bf_header = (struct blume_header *)(bf->map);
    struct filter_header new_header;
    uint64_t new_index;
    size_t new_map_size;
    off_t new_filter_offset;

    if (bf_header->filter_count >= MAX_FILTERS) {
        rv = BLUME_ERROR_OUT_OF_FILTERS;
        goto fail0;
    }

    new_index = bf_header->filter_count;
    if (new_index == 0) {
        new_header.P = bf_header->P - bf_header->P * ERROR_TIGHTENING_RATIO;
        new_header.M = INITIAL_M;
        new_header.k = ceil(log(1.0 / new_header.P) / log(2.0));
        new_header.m = ceil(new_header.M / new_header.k);
    } else {
        struct filter_header *last_header = (struct filter_header *)(bf->map + bf_header->filters[new_index - 1]);
        new_header.P = last_header->P * ERROR_TIGHTENING_RATIO;
        new_header.m = ceil(last_header->m * GROWTH_FACTOR);
        new_header.k = ceil(log(1.0 / new_header.P) / log(2.0));
        new_header.M = new_header.m * new_header.k;
    }
    // This one is utter magic, wtf MATH.
    new_header.n = new_header.M * ((log(2) * log(2)) / fabs(log(new_header.P)));

    new_header.keys_count = 0;
    new_header.array_bytes = sizeof(struct filter_header) + (new_header.M / 8) + 1;

    new_map_size = bf->map_size;
    if ((new_map_size % 8) != 0) {
        new_map_size += 8 - (new_map_size % 8);
    }
    new_filter_offset = new_map_size;
    new_map_size += sizeof(struct filter_header) + new_header.array_bytes;

    if (new_map_size > bf_header->max_file_size) {
        rv = BLUME_ERROR_FULL;
        goto fail0;
    }

    if ((rv = resize_map(bf, new_map_size)) != BLUME_ERROR_SUCCESS) {
        goto fail0;
    }
    bf_header = (struct blume_header *)(bf->map);

    //fprintf(stderr, "new_filter_offset: 0x%016zx\n", new_filter_offset);
    struct filter *f = (struct filter *)(bf->map + new_filter_offset);
    memcpy(&f->header, &new_header, sizeof(struct filter_header));

    bf_header->filters[new_index] = new_filter_offset;
    bf_header->filter_count += 1;
    return rv;

fail0:
    return rv;
}

static void
get_hash_coefficients(const void *key, size_t len, uint64_t *h1, uint64_t *h2)
{
    uint64_t cs[4];

    MurmurHash3_x64_128(key, len, 0, cs);
    *h1 = cs[0];
    *h2 = cs[1];
}

static int
filter_is_full(struct filter *f)
{
    return (f->header.keys_count >= f->header.n);
}

static blume_error
filter_add(struct blume *bf, struct filter *f, uint64_t h1, uint64_t h2)
{
    blume_error rv = BLUME_ERROR_SUCCESS;
    uint64_t idx, offset;

    if (filter_is_full(f)) {
        return BLUME_ERROR_FULL;
    }

    for (idx = 0, offset = 0; idx < f->header.k; idx += 1, offset += f->header.m) {
        uint64_t bits_idx = offset + ((h1 + idx * h2) % f->header.m);
        f->bits[bits_idx / 8] |= 1 << (bits_idx % 8);
    }
    f->header.keys_count += 1;

    return rv;
}

static int
filter_check(struct filter *f, uint64_t h1, uint64_t h2)
{
    uint64_t idx, offset;
    for (idx = 0, offset = 0; idx < f->header.k; idx += 1, offset += f->header.m) {
        uint64_t bits_idx = offset + ((h1 + idx * h2) % f->header.m);
        if (!(f->bits[bits_idx / 8] & (1 << (bits_idx % 8)))) {
            return 0;
        }
    }
    return 1;
}


static void
blume_init(struct blume *bf, off_t max_file_size, double p)
{
    struct blume_header *header = (struct blume_header *)bf->map;
    header->max_file_size = max_file_size;
    header->P = p;
    header->filter_count = 0;
}

blume_error
blume_create(struct blume **bf, off_t max_file_size, double p, const char *path)
{
    blume_error rv = BLUME_ERROR_SUCCESS;

    (*bf) = (struct blume *) malloc(sizeof(struct blume));
    if (!(*bf)) {
        rv = BLUME_ERROR_OUT_OF_MEMORY;
        goto fail0;
    }

    (*bf)->path = NULL;
    (*bf)->fd = -1;
    (*bf)->fd_size = 0;
    (*bf)->map = NULL;
    (*bf)->for_write = 1;

    (*bf)->path = strdup(path);
    if (!(*bf)->path) {
        rv = BLUME_ERROR_OUT_OF_MEMORY;
        goto fail1;
    }

    (*bf)->fd = open(path, O_RDWR | O_CREAT, 0644);
    if ((*bf)->fd == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail2;
    }

    if (flock((*bf)->fd, LOCK_EX | LOCK_NB) != 0) {
        rv = BLUME_ERROR_LOCKED;
        goto fail2;
    }

    if (ftruncate((*bf)->fd, sizeof(struct blume_header)) == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail3;
    }
    (*bf)->fd_size = sizeof(struct blume_header);

    if (((*bf)->map = mmap(NULL, sizeof(struct blume_header), PROT_READ | PROT_WRITE, MAP_SHARED, (*bf)->fd, 0)) == MAP_FAILED) {
        (*bf)->map = NULL;
        rv = BLUME_ERROR_ERRNO;
        goto fail3;
    }

    (*bf)->map_size = sizeof(struct blume_header);

    blume_init(*bf, max_file_size, p);

    return rv;


fail3:
    close((*bf)->fd);

fail2:
    free((*bf)->path);

fail1:
    free(*bf);
    *bf = NULL;

fail0:
    return rv;
}






blume_error
blume_open(struct blume **bf, const char *path, int for_write)
{
    blume_error rv = BLUME_ERROR_SUCCESS;
    struct stat st;
    int prot, mode;

    (*bf) = (struct blume *) malloc(sizeof(struct blume));
    if (!(*bf)) {
        rv = BLUME_ERROR_OUT_OF_MEMORY;
        goto fail0;
    }

    (*bf)->path = NULL;
    (*bf)->fd = -1;
    (*bf)->fd_size = 0;
    (*bf)->map = NULL;
    (*bf)->for_write = for_write;

    (*bf)->path = strdup(path);
    if (!(*bf)->path) {
        rv = BLUME_ERROR_OUT_OF_MEMORY;
        goto fail1;
    }

    mode = O_RDONLY;
    if (for_write) {
        mode = O_RDWR;
    }

    (*bf)->fd = open(path, mode, 0644);
    if ((*bf)->fd == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail2;
    }

    if (for_write) {
        if (flock((*bf)->fd, LOCK_EX | LOCK_NB) != 0) {
            rv = BLUME_ERROR_LOCKED;
            goto fail2;
        }
    }

    if (fstat((*bf)->fd, &st) == -1) {
        rv = BLUME_ERROR_ERRNO;
        goto fail3;
    }

    // TODO: DMB: Make sure it's a regular file, I guess.

    (*bf)->fd_size = st.st_size;

    prot = PROT_READ;
    if (for_write) {
        prot |= PROT_WRITE;
    }

    if (((*bf)->map = mmap(NULL, (*bf)->fd_size, prot, MAP_SHARED, (*bf)->fd, 0)) == MAP_FAILED) {
        (*bf)->map = NULL;
        rv = BLUME_ERROR_ERRNO;
        goto fail3;
    }

    (*bf)->map_size = (*bf)->fd_size;

    return rv;


fail3:
    close((*bf)->fd);

fail2:
    free((*bf)->path);

fail1:
    free(*bf);
    *bf = NULL;

fail0:
    return rv;
}



blume_error
blume_close(struct blume **bf)
{
    blume_error rv = BLUME_ERROR_SUCCESS;

    if ((*bf)->fd != -1) {
        close((*bf)->fd);
    }

    if ((*bf)->path) {
        free((*bf)->path);
    }

    if ((*bf)->map) {
        msync((*bf)->map, (*bf)->map_size, MS_SYNC);
        munmap((*bf)->map, (*bf)->map_size);
    }

    (*bf)->map_size = 0;
    free(*bf);
    *bf = NULL;

    return rv;
}

blume_error
blume_add(struct blume *bf, const void *key, size_t len)
{
    blume_error rv = BLUME_ERROR_SUCCESS;
    uint64_t h1, h2;
    struct blume_header *bf_header = (struct blume_header *)bf->map;
    struct filter *f = (struct filter *)(bf->map + bf_header->filters[bf_header->filter_count - 1]);

    if ((bf_header->filter_count == 0) || filter_is_full(f)) {
        if ((rv = filter_create(bf)) != BLUME_ERROR_SUCCESS) {
            return rv;
        }
        bf_header = (struct blume_header *)bf->map;
        f = (struct filter *)(bf->map + bf_header->filters[bf_header->filter_count - 1]);
    }

    get_hash_coefficients(key, len, &h1, &h2);
    return filter_add(bf, f, h1, h2);
}

int
blume_check(struct blume *bf, const void *key, size_t len)
{
    struct blume_header *bf_header = (struct blume_header *)bf->map;
    uint64_t idx;
    uint64_t h1, h2;

    get_hash_coefficients(key, len, &h1, &h2);
    for (idx = 0; idx < bf_header->filter_count; idx += 1) {
        struct filter *f = (struct filter *)(bf->map + bf_header->filters[idx]);
        if (filter_check(f, h1, h2)) {
            return 1;
        }
    }

    return 0;
}

uint64_t
blume_length(struct blume *bf)
{
    struct blume_header *bf_header = (struct blume_header *)bf->map;
    uint64_t idx, total = 0;
    for (idx = 0; idx < bf_header->filter_count; idx += 1) {
        struct filter *f = (struct filter *)(bf->map + bf_header->filters[idx]);
        total += f->header.keys_count;
    }

    return total;
}

