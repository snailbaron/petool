#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "vis_struct.h"

#define MAX_ERROR_LEN 5000      // max length of error message
#define MAX_VALUE_LEN 10        // max length of a value string
#define MAX_VARIATIONS 5        // max number of structure variations


// Scanner state: file, character, and position
static FILE *file = NULL;
static int c = '\0';
static unsigned int line_number = 1;
static unsigned int sym_number = 0;


/* Common data: used when parsing any type of definition, structure or field */

// Name of the affected structure, may have variations
static char struct_name[MAX_NAME_LEN + 1] = "";

// Description of current item
static char description[MAX_DESCR_LEN + 1] = "";
static size_t descr_idx = 0;



/* Data from structure definition parsing */

// Structure variations that need to be created, and corresponding vis objects
static char struct_variations[MAX_VARIATIONS][MAX_NAME_LEN + 1];
static vis_struct_t current_structs[MAX_VARIATIONS];
static size_t struct_num = 0;

// Variations the field is applied to
static char field_variations[MAX_VARIATIONS][MAX_NAME_LEN + 1];
static size_t field_variation_num = 0;

// Field data in structure definition
static size_t field_sizes[MAX_VARIATIONS];
static size_t field_size_num = 0;
static vis_field_type_t field_type;
static char field_name[MAX_NAME_LEN + 1] = "";


/* Data from field definition parsing */

// Field being defined
static char target_field[MAX_NAME_LEN + 1] = "";
static vis_field_t *current_field = NULL;

// Value name in field definition
static char value_name[MAX_NAME_LEN + 1] = "";

// Current numeric value
static vis_value_t value = 0;


// Error status
static char error[MAX_ERROR_LEN + 1] = "";
static int error_pos = 0;
static bool status = true;

// Raise an error during file parsing
#define err(FORMAT, ...)                                                        \
do {                                                                            \
    error_pos += sprintf_s(error + error_pos, MAX_ERROR_LEN + 1 - error_pos,    \
        "%d:%d " FORMAT, line_number, sym_number, __VA_ARGS__);                 \
    status = false;                                                             \
} while (0)

// Check current parser status, and return immediately, if something is wrong
#define check_status()                                                          \
do {                                                                            \
    if (!status) {                                                              \
        return;                                                                 \
    }                                                                           \
} while (0)

#define check_status_ret(RET)                                                   \
do {                                                                            \
    if (!status) {                                                              \
        return (RET);                                                           \
    }                                                                           \
} while (0)


/*
 * Scanner function. Obtains 1 character at a time.
 * Skips comments.
 * Performes translation:
 *     any kind of end-of-line (CR, LF, CRLF) -> \n
 * Other characters are delivered as-is.
*/
void get_char()
{
    // Get next character from the file
    c = getc(file);

    // Skip comments: from '#' till the end of line or file
    if (c == '#') {
        while (c != '\r' && c != '\n' && c != EOF) {
            c = getc(file);
        }
        if (c == '\r') {
            c = getc(file);
        }
        if (c == '\n') {
            c = getc(file);
        }
    }

    // Convert any type of EOL to LF
    if (c == '\r') {
        // Skip the \r in \r\n sequence
        int c2 = getc(file);
        if (c2 != '\n') {
            fseek(file, -1, SEEK_CUR);
        }
        c = '\n';
    }

    // Advance current line and symbol
    if (c == '\n') {
        sym_number = 0;
        line_number++;
    } else {
        sym_number++;
    }
}


/*
 * Checker functions for various character classes
 * Checkers operate on currently observed character ('c').
 * A checker can use other checkers.
*/

// Definition of checker functions
#define CHECKER(NAME, CONDITION)                        \
bool is_ ## NAME ()                                     \
{                                                       \
    return (CONDITION);                                 \
}                                                       \
                                                        \
void skip_ ## NAME ()                                   \
{                                                       \
    check_status();                                     \
    if (is_ ## NAME()) {                                \
        get_char();                                     \
        return;                                         \
    }                                                   \
    err("%s\n", "Unexpected");                          \
}                                                       \
                                                        \
void skip_any_ ## NAME ()                               \
{                                                       \
    check_status();                                     \
    while (is_ ## NAME()) {                             \
        skip_ ## NAME();                                \
    }                                                   \
}                                                       \
                                                        \
void read_ ## NAME (char *dst)                          \
{                                                       \
    check_status();                                     \
    if (is_ ## NAME()) {                                \
        *dst = c;                                       \
    }                                                   \
    skip_ ## NAME();                                    \
}                                                       \
                                                        \
size_t read_any_ ## NAME (char *dst, size_t max_len)    \
{                                                       \
    check_status_ret(0);                                \
    size_t i = 0;                                       \
    while (is_ ## NAME()) {                             \
        if (i >= max_len) {                             \
            err("%s\n", "too long");                    \
            return i;                                   \
        }                                               \
        read_ ## NAME(&dst[i++]);                       \
    }                                                   \
    dst[i] = '\0';                                      \
    return i;                                           \
}                                                       \
                                                        \
size_t read_some_ ## NAME (char *dst, size_t max_len)   \
{                                                       \
    check_status_ret(0);                                \
    read_ ## NAME(dst);                                 \
    check_status_ret(0);                                \
    size_t num = read_any_ ## NAME(dst, max_len);       \
    check_status_ret(1);                                \
    return num;                                         \
}

// Checkers
CHECKER( ws,        c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' )
CHECKER( ls,        c == ' ' || c == '\t'                                                     )
CHECKER( eol,       c == '\n'                                                                 )
CHECKER( non_eol,   !is_eol()                                                                 )
CHECKER( digit,     c >= '0' && c <= '9'                                                      )
CHECKER( hex_digit, c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'      )
CHECKER( letter,    c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'                              )
CHECKER( name_char, is_letter() || is_digit() || c == '_' || c == '+'                         )

#undef CHECKER

/*
 * Manual checkers with arguments
*/

bool is_string(char *str)
{
    char *p = str;
    while (*p != '\0' && *p == c) {
        get_char();
    }
    bool found = (*p == '\0');
    fseek(file, (long)(str - p), SEEK_CUR);
    return found;
}


/*
 * Low-level parsing functions.
*/

// Parse a single given character
void parse_char(char ch)
{
    check_status();
    if (c != ch) {
        err("Expected: %c\n", ch);
        return;
    }
    get_char();
}

// Parse a given character repeated zero or more times
void parse_char_any(char ch)
{
    check_status();
    while (c == ch) {
        get_char();
    }
}

// Parse given character repeated one or more times
void parse_char_some(char ch)
{
    check_status();
    parse_char(ch);
    parse_char_any(ch);
}

// Parse given string of characters
void parse_string(char str[])
{
    check_status();
    for (char *p = str; *p != '\0'; p++) {
        parse_char(*p);
    }
}


void read_name(char *dst, char *name)
{
    check_status();
    read_some_name_char(dst, MAX_NAME_LEN);
}

size_t read_name_list(char dst[][MAX_NAME_LEN + 1], size_t max_names, char *name)
{
    check_status_ret(0);
    size_t i = 0;
    read_name(dst[i++], name);
    skip_any_ls();
    while (c == ',') {
        if (i >= max_names) {
            err("Too many names: %s\n", name);
            return i;
        }
        read_name(dst[i++], name);
    }
    return i;
}

void read_number(vis_value_t *dst, char *name)
{
    check_status();
    char buf[MAX_VALUE_LEN + 1];

    if (c == '0') {
        get_char();
        if (c == 'x') {
            get_char();
            read_some_hex_digit(buf, MAX_VALUE_LEN);
            check_status();
            *dst = strtoull(buf, NULL, 16);
            return;
        } else {
            fseek(file, -1, SEEK_CUR);
        }
    }
    read_some_digit(buf, MAX_VALUE_LEN);
    check_status();
    strtoull(buf, NULL, 10);
}


/*
** Parsing functions for grammar non-terminals
*/

// Parse field (vertical) separator
void parse_field_sep()
{
    check_status();
    skip_any_ls();
    parse_char('|');
    skip_any_ls();
}

// Parse horizontal (table) separator
void parse_sep()
{
    check_status();
    parse_char_some('-');
    skip_eol();
}

// Parse one line of description
void parse_descr_line()
{
    check_status();
    descr_idx += read_any_non_eol(description + descr_idx, MAX_DESCR_LEN - descr_idx);
    skip_eol();
}

// Parse description
void parse_descr()
{
    check_status();
    descr_idx = 0;
    parse_descr_line();
    while (is_ls()) {
        skip_any_ls();
        parse_descr_line();
    }
}

// Parse value in field definition
void parse_value()
{
    check_status();
    read_number(&value, "value");
}

// Parse value name in field definition
void parse_value_name()
{
    check_status();
    read_name(value_name, "value name");
}

// Parse value info in field definition
void parse_value_info()
{
    check_status();
    parse_value_name();
    parse_field_sep();
    parse_value();
    parse_field_sep();
    parse_descr();
}

// Parse field definition
void parse_field_def()
{
    check_status();
    parse_sep();
    while (c != '-') {
        parse_value_info();
    }
    parse_sep();
}

// Parse name of the field being defined
void parse_target_field_name()
{
    check_status();
    read_name(target_field, "defined field name");   
}

// Parse field name in structure definition
void parse_field_name()
{
    check_status();
    read_name(field_name, "field name");
}

// Parse field type in structure definition
void parse_field_type()
{
    check_status();
    if (is_string("ENUM")) {
        parse_string("ENUM");
        field_type = VIS_ENUM;
    } else if (is_string("UINT")) {
        parse_string("UINT");
        field_type = VIS_ENUM;
    } else if (is_string("TIME")) {
        parse_string("TIME");
        field_type = VIS_ENUM;
    } else if (is_string("FLAG")) {
        parse_string("FLAG");
        field_type = VIS_ENUM;
    } else {
        err("%s\n", "Expected: field type");
    }
}

// Parse a single field size
void parse_field_size(size_t *dst)
{
    check_status();
    size_t size = 0;
    switch (c) {
        case '1': size = 1; break;
        case '2': size = 2; break;
        case '4': size = 4; break;
        case '8': size = 8; break;
    }
    if (!size) {
        err("Expected: %s\n", "field size");
    }
    *dst = size;
    get_char();
}

// Parse field's size(s) in structure definition
void parse_field_sizes()
{
    check_status();
    size_t i = 0;
    parse_field_size(&field_sizes[i++]);
    while (c == '/') {
        if (i >= MAX_VARIATIONS) {
            err("%s\n", "size list is too long");
            return;
        }
        parse_field_size(&field_sizes[i++]);
    }
}

// Parse list of field variations
void parse_field_variation_list()
{
    check_status();
    field_variation_num = read_name_list(field_variations, MAX_VARIATIONS, "field variations");
}

// Parse field variations specification
void parse_field_spec()
{
    check_status();
    parse_char('[');
    parse_field_variation_list();
    parse_char(']');
    skip_any_ls();
    skip_eol();
}

// Parse field info inside structure definition
void parse_field()
{
    check_status();
    if (c == '[') {
        parse_field_spec();
    } else {
        field_variation_num = 0;
    }
    parse_field_sizes();
    parse_field_sep();
    parse_field_type();
    parse_field_sep();
    parse_field_name();
    parse_field_sep();
    parse_descr();
}

// Parse structure definition (field sizes, types, names and descriptions)
void parse_struct_def()
{
    check_status();
    parse_sep();
    while (c != '-') {
        parse_field();
    }
    parse_sep();
}

// Parse list of structure variations
void parse_struct_variation_list()
{
    check_status();
    struct_num = read_name_list(struct_variations, MAX_VARIATIONS, "variation list");
}

// Parse structure name
void parse_struct_name()
{
    check_status();
    read_name(struct_name, "structure name");
}

// Parse structure specification: structure name and its variations
void parse_struct_spec()
{
    check_status();
    parse_char('[');
    skip_any_ls();
    parse_struct_name();
    skip_any_ls();
    if (c == ':') {
        parse_char(':');
        skip_any_ls();
        parse_struct_variation_list();
    } else {
        struct_num = 0;
    }
    parse_char(']');
}

// Parse definition (can be structure or field definition)
void parse_def()
{
    check_status();
    parse_struct_spec();
    skip_any_ls();
    if (c == ':') {
        parse_char(':');
        skip_any_ls();
        parse_target_field_name();
        skip_any_ls();
        skip_eol();
        parse_field_def();
    } else {
        skip_eol();
        parse_struct_def();
    }
}

// Parse configuration file
void parse_config()
{
    check_status();
    skip_any_ws();
    while (c == '[') {
        parse_def();
        skip_any_ws();
    }

    if (c != EOF) {
        err("Unexpected symbol: %c\n", c);
    }
}


/*
** Parsing interface
*/

void parse_file(const char *fname)
{
    fopen_s(&file, fname, "r");
    get_char();

    parse_config();

    if (!status) {
        fprintf(stderr, "Error parsing %s:\n%s", fname, error);
    }

    fclose(file);
}