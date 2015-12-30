#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "vis_struct.h"

#define MAX_ERROR_LEN 5000
#define MAX_SIZE_LEN 10
#define MAX_TYPE_LEN 10
#define MAX_VALUE_LEN 10

static FILE *file = NULL;
static int c = '\0';
static unsigned int line_number = 1;
static unsigned int sym_number = 0;

static char table_name[MAX_NAME_LEN + 1] = "";
static vis_struct_t *current_struct = NULL;

static vis_field_t *current_field = NULL;
static size_t field_size = 0;
static vis_field_type_t field_type;

static char field_name[MAX_NAME_LEN + 1] = "";
static char description[MAX_DESCR_LEN + 1] = "";
static size_t descr_idx = 0;
static char value_name[MAX_NAME_LEN + 1] = "";
static vis_value_t value = 0;

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


/*
 * Scanner function. Obtains 1 character at a time.
 * Performes translation:
 *     any kind of end-of-line (CR, LF, CRLF) -> \n
 * Other characters are delivered as-is.
*/
void get_char()
{
    // Get next character from the file
    c = getc(file);
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

#define CHECKER(NAME, CONDITION) bool NAME() { return (CONDITION); }

CHECKER( is_letter,          c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'                         )
CHECKER( is_dec_digit,       c >= '0' && c <= '9'                                                 )
CHECKER( is_hex_digit,       c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f' )
CHECKER( is_whitespace,      c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n'         )
CHECKER( is_line_space,      c == ' ' || c == '\t'                                                )
CHECKER( is_eol,             c == '\r' || c == '\n'                                               )
CHECKER( is_non_eol,         c != '\r' && c != '\n'                                               )
CHECKER( is_alnum,           is_letter() || is_dec_digit()                                        )
CHECKER( is_value_name_char, is_letter() || is_dec_digit() || c == '_'                            )
CHECKER( is_table_name_char, is_letter() || is_dec_digit() || c == ' '                            )

#undef CHECKER


/*
 * Low-level parsing functions.
*/

// Parse a single pre-defined character
void parse_char(char ch)
{
    check_status();
    if (c != ch) {
        err("Expected: %c\n", ch);
        return;
    }
    get_char();
}

// Parse a single character using a checker function
void parse_char_check(bool (*checker)(), char *expected_descr)
{
    check_status();
    if (checker()) {
        get_char();
        return;
    }
    err("Expected: %s\n", expected_descr);
}

// Parse a sequence of characters
void parse_string(char str[])
{
    check_status();
    for (char *p = str; *p != '\0'; p++) {
        parse_char(*p);
    }
}


/*
 * Functions for retrieving values from file being parsed
*/

// Read a single character from the file being parsed, using a checker.
void read_char(bool (*checker)(), char *dst, char *name)
{
    if (checker()) {
        *dst = c;
    }
    parse_char_check(checker, name);
}

// Read a value from the file being parsed to a fixed-length buffer.
// Returns: Number of characters written to buffer, not counting the final
//          null character.
size_t read_value(bool (*checker)(), char *dst, size_t max_len, char *name)
{
    size_t i = 0;
    while (checker()) {
        if (i >= max_len) {
            err("Error: %s is too long\n", name);
            break;
        }
        dst[i++] = c;
        get_char();
    }
    dst[i] = '\0';
    return i;
}


/*
** Parsing functions for grammar non-terminals
*/

void parse_eol()
{
    check_status();
    if (c == '\n') {
        get_char();
    } else if (c == '\r') {
        get_char();
        if (c == '\n') {
            get_char();
        }
    } else {
        err("Expected: %s\n", "newline");
    }
}

void parse_value_number()
{
    check_status();
    parse_string("0x");

    char buf[MAX_VALUE_LEN + 1];
    if (is_hex_digit()) {
        buf[0] = c;
    }
    parse_char_check(is_hex_digit, "hex digit");
    read_value(is_hex_digit, buf + 1, MAX_VALUE_LEN - 1, "value");

    value = strtoull(buf, NULL, 16);
}

void parse_field_size()
{
    check_status();
    char buf[MAX_SIZE_LEN + 1];
    if (is_dec_digit()) {
        buf[0] = c;
    }
    parse_char_check(is_dec_digit, "decimal digit");
    read_value(is_dec_digit, buf + 1, MAX_SIZE_LEN - 1, "field size");
    check_status();
    field_size = atoll(buf);
}

void parse_field_separator()
{
    check_status();
    while (is_line_space()) {
        get_char();
    }
    parse_char('|');
    while (is_line_space()) {
        get_char();
    }
}

void parse_separator()
{
    check_status();
    parse_char('-');
    while (c == '-') {
        get_char();
    }
    parse_eol();
}

void parse_description_line()
{
    check_status();
    size_t written = read_value(is_non_eol, description + descr_idx, MAX_DESCR_LEN - descr_idx, "description");
    descr_idx += written;
    parse_eol();
}

void parse_description()
{
    check_status();

    descr_idx = 0;
    parse_description_line();
    while (c == ' ') {
        if (descr_idx >= MAX_DESCR_LEN) {
            err("Error: %s\n", "description is too long");
            break;
        }
        description[descr_idx++] = ' ';
        parse_char(' ');
        while (c == ' ') {
            get_char();
        }
        parse_description_line();
    }
    description[descr_idx] = '\0';
}

void parse_value_name()
{
    check_status();
    size_t i = 0;
    if (is_letter()) {
        value_name[i++] = c;
    }
    parse_char_check(is_letter, "letter");
    read_value(is_value_name_char, value_name + 1, MAX_NAME_LEN - 1, "value");
}

void parse_value()
{
    check_status();
    parse_value_name();
    parse_field_separator();
    parse_value_number();
    parse_field_separator();
    parse_description();

    vis_add_value_info(current_field, value_name, value, description);
}

void parse_field_definition()
{
    check_status();
    parse_separator();
    parse_value();
    check_status();
    while (c != '-') {
        parse_value();
    }
    parse_separator();
}

void parse_field_name()
{
    check_status();
    size_t i = 0;
    if (is_letter()) {
        field_name[i++] = c;
    }
    parse_char_check(is_letter, "letter");

    read_value(is_alnum, field_name + 1, MAX_NAME_LEN - 1, "field name");
}

void parse_field_type()
{
    check_status();
    if (c == 'E') {
        parse_string("ENUM");
        check_status();
        field_type = VIS_ENUM;
    } else if (c == 'U') {
        parse_string("UINT");
        check_status();
        field_type = VIS_UINT;
    } else if (c == 'T') {
        parse_string("TIME");
        check_status();
        field_type = VIS_TIME;
    } else if (c == 'F') {
        parse_string("FLAG");
        check_status();
        field_type = VIS_FLAG;
    } else {
        err("Expected: %s\n", "field type");
    }
}

void parse_field()
{
    check_status();
    parse_field_size();
    parse_field_separator();
    parse_field_type();
    parse_field_separator();
    parse_field_name();
    parse_field_separator();
    parse_description();

    vis_add_field(current_struct, field_name, field_size, field_type, description);
}

void parse_table_definition()
{
    check_status();
    parse_separator();
    parse_field();
    check_status();
    while (c != '-') {
        parse_field();
    }
    parse_separator();
}

void parse_table_name()
{
    check_status();

    size_t i = 0;
    if (is_letter()) {
        table_name[i++] = c;
    }
    parse_char_check(is_letter, "letter");

    read_value(is_table_name_char, table_name + 1, MAX_NAME_LEN - 1, "table name");
}

void parse_definition()
{
    check_status();
    parse_char('[');
    parse_table_name();
    parse_char(']');
    while (is_line_space()) {
        get_char();
    }

    if (is_eol()) {
        parse_eol();

        current_struct = vis_create_struct(table_name);
        parse_table_definition();
    } else {
        parse_char(':');
        while (is_line_space()) {
            get_char();
        }
        parse_field_name();

        current_struct = vis_find_struct(table_name);
        current_field = vis_find_field(current_struct, field_name);

        while (is_line_space()) {
            get_char();
        }
        parse_eol();
        parse_field_definition();

        printf("\n");
    }
}

void parse_config()
{
    check_status();
    while (is_whitespace()) {
        get_char();
    }

    while (c == '[') {
        parse_definition();
        while (is_whitespace()) {
            get_char();
        }
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
        fprintf(stderr, "%s", error);
    }

    fclose(file);
}