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
void err(const char *msg) {
    error_pos += sprintf_s(error + error_pos, MAX_ERROR_LEN + 1 - error_pos,
        "%d:%d %s\n", line_number, sym_number, msg);
}

#define ferr(FORMAT, ...)                                                       \
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
#define CHECKER(NAME, STR, CONDITION)                   \
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
    ferr("Expected: %s\n", STR);                        \
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
void skip_some_ ## NAME ()                              \
{                                                       \
    check_status();                                     \
    skip_ ## NAME();                                    \
    check_status();                                     \
    skip_any_ ## NAME();                                \
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
            ferr("Too many of: %s\n", STR);             \
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
CHECKER( letter,    "letter",                c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'                              )
CHECKER( dec_digit, "decimal digit",         c >= '0' && c <= '9'                                                      )
CHECKER( hex_digit, "hexadecimal digit",     c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'      )
CHECKER( ws,        "whitespace character",  c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' )
CHECKER( ls,        "inline whitespace",     c == ' ' || c == '\t'                                                     )
CHECKER( eol,       "newline",               c == '\n'                                                                 )
CHECKER( non_eol,   "non-newline character", !is_eol()                                                                 )
CHECKER( name_char, "name character",        is_letter() || is_dec_digit() || c == '_' || c == '+'                     )
CHECKER( sep_char,  "-",                     c == '-'                                                                  )

#undef CHECKER

/*
 * Manual checkers with arguments
*/

bool is_string(char *str)
{
    int prev_char = c;

    char *p = str;
    while (*p != '\0' && *p == c) {
        p++;
        get_char();
    }
    bool found = (*p == '\0');
    fseek(file, (long)(str - p), SEEK_CUR);

    c = prev_char;
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
        ferr("Expected: %c\n", ch);
        return;
    }
    get_char();
}

// Parse given string of characters
void parse_string(char str[])
{
    check_status();
    for (char *p = str; *p != '\0'; p++) {
        parse_char(*p);
    }
}


/*
** Parsing functions for grammar non-terminals
*/

void parse_lss()
{
    check_status();
    while (is_ls()) {
        get_char();
    }
}

void parse_wss()
{
    check_status();
    while (is_ws()) {
        get_char();
    }
}

void parse_name(char dst[MAX_NAME_LEN + 1])
{
    check_status();
    read_some_name_char(dst, MAX_NAME_LEN);
}

void parse_name_list(char dst[][MAX_NAME_LEN + 1], size_t max_name_num)
{
    check_status();
    if (max_name_num == 0) {
        err("Too many names");
        return;
    }
    parse_name(dst[0]);
    parse_lss();
    if (c == ',') {
        parse_char(',');
        parse_lss();
        parse_name_list(dst + 1, max_name_num - 1);
    }
}

void parse_dec_number(vis_value_t *dst)
{
    check_status();
    char buf[MAX_VALUE_LEN + 1];
    read_some_dec_digit(buf, MAX_VALUE_LEN);
    check_status();
    *dst = strtoull(buf, NULL, 10);
}

void parse_hex_number(vis_value_t *dst)
{
    check_status();
    char buf[MAX_VALUE_LEN + 1];
    read_some_hex_digit(buf, MAX_VALUE_LEN);
    *dst = strtoull(buf, NULL, 16);
}

void parse_num_value(vis_value_t *dst)
{
    check_status();
    if (c == '0') {
        get_char();
        if (c == 'x') {
            get_char();
            parse_hex_number(dst);
            return;
        }
        fseek(file, -1, SEEK_CUR);
    }
    parse_dec_number(dst);
}

// Parse field (vertical) separator
void parse_field_sep()
{
    check_status();
    parse_lss();
    parse_char('|');
    parse_lss();
}

// Parse horizontal (table) separator
void parse_sep()
{
    check_status();
    skip_some_sep_char();
    skip_eol();
}

// Parse one line of description
void parse_descr_line()
{
    check_status();
    descr_idx += read_any_non_eol(description + descr_idx, MAX_DESCR_LEN - descr_idx);
    skip_eol();
}

// Parse one or more description lines
void parse_descr_line_list()
{
    check_status();
    parse_descr_line();
    check_status();
    if (is_ls()) {
        skip_ls();
        parse_lss();
        parse_descr_line_list();
    }
}

// Parse description
void parse_descr()
{
    check_status();
    descr_idx = 0;
    parse_descr_line_list();
}

// Parse value in field definition
void parse_value()
{
    check_status();
    parse_num_value(&value);
}

// Parse value name in field definition
void parse_value_name()
{
    check_status();
    parse_name(value_name);
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

// Parse list of values in field definition
void parse_value_info_list()
{
    check_status();
    if (is_name_char()) {
        parse_value_info();
        parse_value_info_list();
    }
}

// Parse field definition
void parse_field_def()
{
    check_status();
    parse_sep();
    parse_value_info_list();
    parse_sep();
}

// Parse name of the field being defined
void parse_target_field_name()
{
    check_status();
    parse_name(target_field);
}

// Parse field name in structure definition
void parse_field_name()
{
    check_status();
    parse_name(field_name);
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
        err("Expected: field type");
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
        err("Expected: field size");
    }
    *dst = size;
    get_char();
}

// Parse list of field sizes
void parse_field_size_list()
{
    check_status();
    size_t i = 0;
    parse_field_size(&field_sizes[i++]);
    check_status();
    if (c == '/') {
        parse_char('/');
        parse_field_size_list();
    }    
}

// Parse list of field variations
void parse_field_variation_list()
{
    check_status();
    parse_name_list(field_variations, MAX_VARIATIONS);
}

// Parse field variations specification
void parse_field_spec()
{
    check_status();
    parse_char('[');
    parse_field_variation_list();
    parse_char(']');
    parse_lss();
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
    parse_field_size_list();
    parse_field_sep();
    parse_field_type();
    parse_field_sep();
    parse_field_name();
    parse_field_sep();
    parse_descr();
}

// Parse list of fields in structure definition
void parse_field_list()
{
    check_status();
    if (is_dec_digit() || c == '[') {
        parse_field();
        parse_field_list();
    }
}

// Parse structure definition (field sizes, types, names and descriptions)
void parse_struct_def()
{
    check_status();
    parse_sep();
    parse_field_list();
    parse_sep();
}

// Parse list of structure variations
void parse_struct_variation_list()
{
    check_status();
    parse_name_list(struct_variations, MAX_VARIATIONS);
}

// Parse structure name
void parse_struct_name()
{
    check_status();
    parse_name(struct_name);
}

// Parse structure specification: structure name and its variations
void parse_struct_spec()
{
    check_status();
    parse_char('[');
    parse_lss();
    parse_struct_name();
    parse_lss();
    if (c == ':') {
        parse_char(':');
        parse_lss();
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
    parse_lss();

    if (is_eol()) {
        skip_eol();
        parse_struct_def();
    } else {
        parse_char(':');
        parse_lss();
        parse_target_field_name();
        parse_lss();
        skip_eol();
        parse_field_def();
    }
}

// Parse a (possibly empty) list of definitions
void parse_def_list()
{
    check_status();
    if (c == '[') {
        parse_def();
        parse_wss();
        parse_def_list();
    }    
}

// Parse configuration file
void parse_config()
{
    check_status();
    parse_wss();
    parse_def_list();
    if (c != EOF) {
        ferr("Unexpected symbol: %c\n", c);
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