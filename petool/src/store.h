#ifndef STORE_H
#define STORE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    void *data;
    size_t elsize;
    size_t size;
    size_t cap;
} store_t;

#define store_init(TYPE) { NULL, sizeof(TYPE), 0, 0 }

void store_create_by_size(store_t *store, size_t elsize);

#define store_create(PSTORE, TYPE) store_create_by_size(PSTORE, sizeof(TYPE));

void store_add(store_t *store, void *value);

void * store_alloc(store_t *store);

typedef struct
{
    store_t *store;
    void *p;
} store_iter_t;

void * store_pget(store_t *store, size_t idx);

#endif
