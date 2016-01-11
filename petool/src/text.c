#include <stdbool.h>
#include <assert.h>
#include "text.h"

/**
 * Returns true, if c is a space character (word separator).
 * Returns false otherwise.
 */
bool _is_space(char c)
{
    return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v');
}

/**
 * Returns true, if c is a letter (non-space non-null character).
 * Returns false otherwise.
 */
bool _is_letter(char c)
{
    return (!_is_space(c) && c != '\0');
}

bool _fits(const char *start, const char *end, size_t size)
{
    return ((size_t)(end - start) <= size);
}

const char * _first_nonspace(const char *str)
{
    const char *p = str;
    while (_is_space(*p)) {
        p++;
    }
    return p;
}

const char * _first_nonletter(const char *str)
{
    const char *p = str;
    while (_is_letter(*p)) {
        p++;
    }
    return p;
}

/**
 * Get first line of the given text. If possible, wrap it at a word end
 * so that it fits width.
 */
text_line_t text_line(const char *text, size_t width)
{
    // Part of the line ready to be returned. Begin with an empty line.
    const char *line_start = _first_nonspace(text);
    const char *line_end = line_start;

    // Find first word to check
    const char *word_start = _first_nonspace(line_end);
    const char *word_end = _first_nonletter(word_start);

    // Add words, while text is not finished, and the next word fits the line
    while (*word_start != '\0' && (size_t)(word_end - line_start) <= width) {
        line_end = word_end;
        word_start = _first_nonspace(word_end);
        word_end = _first_nonletter(word_start);
    }

    // If a single word does not fit the line, add it anyway
    if (line_end == line_start && word_start != '\0') {
        line_end = word_end;
    }

    // Return the formed line
    text_line_t tl = { .start = (char *)line_start, .size = (size_t)(line_end - line_start) };
    return tl;
}

/**
 * Get number of lines (of specific width) in text.
 */
int text_line_num(const char *text, size_t width)
{
    int line_num = 0;
    text_line_t tl = text_line(text, width);
    while (tl.size > 0) {
        line_num++;
        tl = text_line(tl.start + tl.size, width);
    }
    return line_num;
}
