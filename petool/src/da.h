#ifndef DA_H
#define DA_H

#include <stdlib.h>

typedef struct da_t da_t;

#define da_t(T)     \
struct              \
{                   \
    T *data;        \
    size_t elsize;  \
    size_t size;    \
    size_t cap;     \
}

#define da_init(T) { NULL, sizeof(T), 0, 0 }

#define da_declare(T, NAME) da_t(T) NAME = da_init(T)

#define da_set(DA, IDX, VALUE)                              \
do {                                                        \
    if (IDX >= DA.cap) {                                    \
        do {                                                \
            DA.cap = DA.cap * 2 + 1;                        \
        } while (IDX >= DA.cap);                            \
        DA.data = realloc(DA.data, DA.elsize * DA.cap);     \
    }                                                       \
    DA.data[IDX] = VALUE;                                   \
    if (IDX >= DA.size) {                                   \
        DA.size = IDX + 1;                                  \
    }                                                       \
} while (0)

#define da_get(DA, IDX) (DA.data[IDX])
#define da_pget(DA, IDX) (DA.data + IDX)
#define da_add(DA, VALUE) da_set(DA, DA.size - 1, VALUE)




/*
struct da_t
{
    void *data;
    size_t elsize;
    size_t size;
    size_t cap;
};
*/




#endif
