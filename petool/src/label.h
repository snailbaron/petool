#ifndef LABEL_H
#define LABEL_H

#include <stddef.h>

typedef struct label_t label_t;

struct label_t
{
    char *str;
    size_t len;
    size_t cap;
};

static char empty_string[1] = "";

static const label_t empty_label = { .str = empty_string, .len = 0, .cap = 1 };

label_t label_init();
void label_append_char(label_t *label, const char ch);
void label_clear(label_t *label);

#endif
