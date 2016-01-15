#include "label.h"
#include <string.h>
#include <stdlib.h>

label_t label_init()
{
    label_t label = { .str = NULL, .len = 0, .cap = 1 };
    label.str = malloc(1);
    label.str[0] = '\0';
    return label;
}

void label_append_char(label_t *label, const char ch)
{
    if (label->len + 2 > label->cap) {
        label->cap = label->cap * 2 + 1;
        label->str = realloc(label->str, label->cap);
    }
    label->str[label->len++] = ch;
}

void label_clear(label_t *label)
{
    if (label->str) {
        free(label->str);
    }
    *label = label_init();
}
