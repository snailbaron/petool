#include <stdlib.h>
#include <string.h>
#include "store.h"

void store_create_by_size(store_t *store, size_t elsize)
{
    store->data = NULL;
    store->elsize = elsize;
    store->size = 0;
    store->cap = 0;
}

void store_add(store_t *store, void *value)
{
    if (store->size == store->cap) {
        store->cap = store->cap * 2 + 1;
        store->data = realloc(store->data, store->elsize * store->cap);
    }
    memcpy((char *)store->data + store->elsize * store->size, value, store->elsize);
    store->size++;
}

void * store_alloc(store_t *store)
{
    if (store->size == store->cap) {
        store->cap = store->cap * 2 + 1;
        store->data = realloc(store->data, store->elsize * store->cap);
    }
    store->size++;
    return (char *)store->data + store->elsize * (store->size - 1);
}

void * store_pget(store_t *store, size_t idx)
{
    return (char *)store->data + store->elsize * idx;
}

