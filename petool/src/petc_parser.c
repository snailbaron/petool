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
#define MAX_VARIATIONS 5

static FILE *file = NULL;
static int c = '\0';
static unsigned int line_number = 1;
static unsigned int sym_number = 0;

static char table_name[MAX_NAME_LEN + 1] = "";
static vis_struct_t *current_struct = NULL;

static vis_field_t *current_field = NULL;
static size_t field_sizes[MAX_VARIATIONS] = { 0 };
static vis_field_type_t field_type;

static char struct_vars[MAX_VARIATIONS][MAX_NAME_LEN + 1];
static size_t struct_var_num = 0;
static char field_vars[MAX_VARIATIONS][MAX_NAME_LEN + 1];
static size_t field_var_num = 0;
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
 * Skips comments.
 * Performes translation:
 *     any kind of end-of-line (CR, LF, CRLF) -> \n
 * Other characters are delivered as-is.
*/
void get_char()
{
    // Get next character from the file
    c = getc(file);

    // Skip comment if present, including end of line
    if (c == '#') {
        while (c != '\r' && c != '\n') {
            c = getc(file);
        }
    }
    if (c == '\r') {
        c = getc(file);
    }
    if (c == '\n') {
        c = getc(file);
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

#define CHECKER(NAME, CONDITION) bool NAME() { return (CONDITION); }

CHECKER( is_digit,           c >= '0' && c <= '9'                                                      )
CHECKER( is_hex_digit,       c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'      )
CHECKER( is_name_letter,     c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'                              )
CHECKER( is_name_char,       is_name_letter() || is_digit()                                            )
CHECKER( is_whitespace,      c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v' )
CHECKER( is_line_space,      c == ' ' || c == '\t'                                                     )
CHECKER( is_eol,             c == '\n'                                                                 )
CHECKER( is_non_eol,         !is_eol()                                                                 )

#undef CHECKER


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

// Parse a single character using a checker function
void parse_check(bool (*checker)(), char *description)
{
    check_status();
    if (checker()) {
        get_char();
        return;
    }
    err("Expected: %s\n", description);
}

// Parse zero or more characters satisfying a checker
void parse_check_any(bool (*checker)())
{
    check_status();
    while (checker()) {
        get_char();
    }
}

// Parse one or more characters satisfying a checker
void parse_check_some(bool (*checker)(), char *description)
{
    check_status();
    parse_check(checker, description);
    parse_check_any(checker);
}
    

/*
 * Functions for retrieving values from file being parsed
*/

// Read a single character from the file being parsed, using a checker.
void read_char(bool (*checker)(), char *dst, char *name)
{
    check_status();
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
    check_status();
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

void parse_field_sep()
{
    check_status();
    parse_check_any(is_line_space);
    parse_char('|');
    parse_check_any(is_line_space);
}

void parse_sep()
{
    check_status();
    parse_char_some('-');
    parse_char_check(is_eol, "newline");
}

void parse_descr_line()
{
    check_status();
    size_t written = read_value(is_non_eol, description + descr_idx, MAX_DESCR_LEN - descr_idx, "description");
    descr_idx += written;
    parse_char_check(is_eol, "newline");
}

void parse_descr()
{
    check_status();
    descr_idx = 0;
    parse_descr_line();
    while (c == ' ') {
        if (descr_idx >= MAX_DESCR_LEN) {
            err("Error: %s\n", "description is too long");
            break;
        }
        description[descr_idx++] = ' ';
        parse_char_some(' ');
        parse_descr_line();
    }
    description[descr_idx] = '\0';
}

void parse_value_name()
{
    check_status();
    read_char(is_name_letter, value_name, "value name");
    read_value(is_name_char, value_name + 1, MAX_NAME_LEN - 1, "value name");
}

void parse_value()
{
    check_status();
    char buf[MAX_VALUE_LEN + 1];

    if (c == '0') {
        get_char();
        if (c == 'x') {
            get_char();
            read_value(is_hex_digit, buf, MAX_VALUE_LEN, "value");
            value = strtoull(buf, NULL, 16);
            return;
        } else {
            fseek(file, -1, SEEK_CUR);
            c = '0';
        }
    }
    read_value(is_digit, buf, MAX_VALUE_LEN, "value");
    value = strtoull(buf, NULL, 10);
}

void parse_value_info()
{
    check_status();
    parse_value_name();
    parse_field_sep();
    parse_value();
    parse_field_sep();
    parse_descr();
    vis_add_value_info(current_field, value_name, value, description);
}

void parse_field_def()
{
    check_status();
    parse_sep();
    do {
        parse_value_info();
        check_status();
    } while (c != '-');
    parse_sep();
}

void read_variation(char *dst)
{
    check_status();
    read_char(is_name_letter, dst, "variation");
    read_value(is_name_char, dst + 1, MAX_NAME_LEN, "variation");
}

void read_variation_list(char **dst, size_t *size)
{
    check_status();
    size_t i = 0;
    read_variation(dst[i++]);
    parse_check_any(is_line_space);

    while (c == ',') {
        if (i >= MAX_VARIATIONS) {
            err("%s\n", "Too many variations");
            return;
        }
        parse_char(',');
        parse_check_any(is_line_space);
        read_variation(dst[i++]);
    }
    *size = i;
}

void parse_field_vars()
{
    check_status();
    read_variation_list(field_vars, &field_var_num);
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

void read_field_size(size_t *dst)
{
    check_status();
    char buf[MAX_SIZE_LEN + 1];
    read_value(is_digit, buf, MAX_SIZE_LEN, "field size");
    check_status();
    size_t size = atoll(buf);
    if (size != '1' && size != '2' && size != '4' && size != '8') {
        err("Expected: %s\n", "field size");
        return;
    }
    *dst = size;
}

void parse_field_sizes()
{
    check_status();
    size_t i = 0;
    read_field_size(&field_sizes[i++]);
    while (c == '/') {
        if (i >= MAX_VARIATIONS) {
            err("%s\n", "Too many field sizes");
            return;
        }
        parse_char('/');
        read_field_size(&field_sizes[i++]);
    }
}

void parse_field_name()
{
    check_status();
    read_char(is_name_letter, field_name, "field name");
    read_value(is_name_char, field_name + 1, MAX_NAME_LEN - 1, "field name");
}

void parse_field()
{
    check_status();
    if (c == '[') {
        parse_char('[');
        parse_field_vars();
        parse_char(']');
        parse_check_any(is_line_space);
        parse_check(is_eol, "end of line");
    }
    parse_field_sizes();
    parse_field_sep();
    parse_field_type();
    parse_field_sep();
    parse_field_name();
    parse_field_sep;
    parse_descr();
}

void parse_struct_def()
{
    check_status();
    parse_sep();
    parse_field();
    check_status();
    while (c != '-') {
        parse_field();
        check_status();
    }
    parse_sep();
}

void parse_struct_name()
{
    check_status();
    read_char(is_name_letter, table_name, "struct name");
    read_value(is_name_char, table_name + 1, MAX_NAME_LEN - 1, "struct name");
}

void parse_decl()
{
    check_status();
    parse_struct_name();
    if (c == ':') {
        parse_char(':');
        parse_check_any(is_line_space);
        read_variation_list(struct_vars, &struct_var_num);
    }
}

void parse_def()
{
    check_status();
    parse_char('[');
    parse_decl();
    parse_char(']');
    if (c == ':') {
        parse_char(':');
        parse_check_any(is_line_space);
        parse_field_name();
        parse_check_any(is_line_space);
        parse_check(is_eol, "end of line");
        parse_field_def();
    } else {
        parse_check_any(is_line_space);
        parse_check(is_eol, "end of line");
        parse_struct_def();
    }
}

void parse_config()
{
    check_status();
    parse_check_any(is_whitespace);
    while (c == '[') {
        parse_def();
        parse_check_any(is_whitespace);
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