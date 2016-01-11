#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>

typedef struct text_line_t text_line_t;

struct text_line_t
{
    char *start;
    size_t size;
};

text_line_t text_line(const char *text, size_t width);
int text_line_num(const char *text, size_t width);

#endif
