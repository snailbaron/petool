#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "vis_struct.h"

#define MAX_ERROR_LEN 5000
#define MAX_SIZE_LEN 10
#define MAX_TYPE_LEN 10

static FILE *file = NULL;
static int c = '\0';
static unsigned int line_number = 1;
static unsigned int sym_number = 0;

static char table_name[MAX_NAME_LEN + 1] = "";
static char field_size[MAX_SIZE_LEN + 1] = "";
static char field_type[MAX_TYPE_LEN + 1] = "";
static char field_name[MAX_NAME_LEN + 1] = "";
static char description[MAX_DESCR_LEN + 1] = "";

static char error[MAX_ERROR_LEN + 1] = "";
static int error_pos = 0;
static bool status = true;

#define set_parse_error(FORMAT, ...)                                \
do {                                                                \
    int written = sprintf_s(                                        \
        error + error_pos,                                          \
        MAX_ERROR_LEN + 1 - error_pos,                              \
        "%d:%d " FORMAT,                                            \
        line_number, sym_number, __VA_ARGS__);                      \
    error_pos += written;                                           \
    status = false;                                                 \
} while (0)

#define check_status() do { if (!status) { return false; } } while (0)

void get_char()
{
    c = getc(file);
    sym_number++;
    if (c == '\n') {
        sym_number = 0;
        line_number++;
    }
}

bool is_letter()
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool is_dec_digit()
{
    return (c >= '0' && c <= '9');
}

bool is_hex_digit()
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

bool is_whitespace()
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n');
}

bool is_line_space()
{
    return (c == ' ' || c == '\t');
}

bool is_eol()
{
    return (c == '\r' || c == '\n');
}

bool is_char(char ch)
{
    return (c == ch);
}

bool parse_char(char ch)
{
    check_status();
    if (!is_char(ch)) {
        set_parse_error("Expected: %c\n", ch);
        return false;
    }
    get_char();
    return status;
}

bool parse_string(char str[])
{
    check_status();
    for (char *p = str; *p != '\0'; p++) {
        parse_char(*p);
    }
    return status;
}

bool parse_eol()
{
    check_status();

    if (c == '\n') {
        get_char();
        return true;
    } else if (c == '\r') {
        get_char();
        if (c == '\n') {
            get_char();
        }
        return true;
    }
    set_parse_error("Expected: %s\n", "newline");
    return status;
}

bool accept_letter()
{
    check_status();
    if (is_letter()) {
        get_char();
        return true;
    }
    return false;    
}

bool parse_letter()
{
    check_status();
    if (is_letter()) {
        get_char();
        return true;
    }
    set_parse_error("Expected: %s\n", "letter");
    return false;
}

bool parse_decimal_digit()
{
    check_status();
    if (c >= '0' && c <= '9') {
        get_char();
        return true;
    }
    set_parse_error("Expected: %s\n", "decimal digit");
    return status;
}

bool parse_hexadecimal_digit()
{
    check_status();
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
        get_char();
        return true;
    }
    set_parse_error("Expected: %s\n", "hexadecimal digit");
    return status;
}

bool parse_hexadecimal_number()
{
    check_status();
    parse_string("0x");
    parse_hexadecimal_digit();
    while (is_hex_digit()) {
        parse_hexadecimal_digit();
    }

    if (!status) {
        set_parse_error("Expected: %s\n", "hexadecimal number");
    }
    return status;
}

bool parse_field_size()
{
    check_status();
    size_t i = 0;
    if (is_dec_digit()) {
        field_size[i++] = c;
    }
    parse_decimal_digit();
    while (is_dec_digit()) {
        if (i >= MAX_SIZE_LEN) {
            set_parse_error("Error: %s\n", "field size is too long");
            break;
        }
        field_size[i++] = c;
        get_char();
    }
    field_size[i] = '\0';
    return status;
}

bool parse_field_separator()
{
    check_status();
    while (is_line_space()) {
        get_char();
    }
    parse_char('|');
    while (is_line_space()) {
        get_char();
    }
    return status;
}

bool parse_separator()
{
    check_status();
    parse_char('-');
    while (c == '-') {
        get_char();
    }
    parse_eol();
    return status;
}

bool parse_description_line()
{
    check_status();
    while (c != '\n' && c != '\r') {
        get_char();
    }
    parse_eol();
    return status;
}

bool parse_description()
{
    check_status();

    size_t i = 0;
    while (c != '\n' && c != '\r') {
        if (i >= MAX_DESCR_LEN) {
            set_parse_error("Error: %s\n", "description is too long");
            break;
        }
        description[i++] = c;
        get_char();
    }
    parse_eol();

    while (c == ' ') {
        if (i >= MAX_DESCR_LEN) {
            set_parse_error("Error: %s\n", "description is too long");
            break;
        }
        description[i++] = ' ';
        parse_char(' ');
        while (c == ' ') {
            get_char();
        }

        while (c != '\n' && c != '\r') {
            if (i >= MAX_DESCR_LEN) {
                set_parse_error("Error: %s\n", "description is too long");
                break;
            }
            description[i++] = c;
            get_char();
        }
        parse_eol();
    }
    description[i] = '\0';

    return status;
}

bool parse_value_name()
{
    check_status();
    parse_letter();
    check_status();
    while (is_letter() || is_dec_digit() || c == '_') {
        get_char();
    }
    return status;
}

bool parse_value()
{
    check_status();
    parse_value_name();
    parse_field_separator();
    parse_hexadecimal_number();
    parse_field_separator();
    parse_description();
    return status;
}

bool parse_field_definition()
{
    check_status();
    parse_separator();
    parse_value();
    check_status();
    while (c != '-') {
        parse_value();
    }
    parse_separator();
    return status;
}

bool parse_field_name()
{
    check_status();
    size_t i = 0;
    if (is_letter()) {
        field_name[i++] = c;
    }
    parse_letter();
    while (is_letter() || is_dec_digit()) {
        if (i >= MAX_NAME_LEN) {
            set_parse_error("Error: %s\n", "field name is too long");
            break;
        }
        field_name[i++] = c;
        get_char();
    }
    field_name[i] = '\0';
    return status;
}

bool parse_field_type()
{
    check_status();
    if (c == 'E') {
        parse_string("ENUM");
        check_status();
        strncpy_s(field_type, MAX_TYPE_LEN + 1, "ENUM", MAX_TYPE_LEN);
    } else if (c == 'U') {
        parse_string("UINT");
        check_status();
        strncpy_s(field_type, MAX_TYPE_LEN + 1, "UINT", MAX_TYPE_LEN);
    } else if (c == 'T') {
        parse_string("TIME");
        check_status();
        strncpy_s(field_type, MAX_TYPE_LEN + 1, "TIME", MAX_TYPE_LEN);
    } else if (c == 'F') {
        parse_string("FLAG");
        check_status();
        strncpy_s(field_type, MAX_TYPE_LEN + 1, "FLAG", MAX_TYPE_LEN);
    } else {
        set_parse_error("Expected: %s\n", "field type");
    }
    return status;
}

bool parse_field()
{
    check_status();
    parse_field_size();
    parse_field_separator();
    parse_field_type();
    parse_field_separator();
    parse_field_name();
    parse_field_separator();
    parse_description();

    printf("FIELD:\n");
    printf("  Size: %s\n", field_size);
    printf("  Type: %s\n", field_type);
    printf("  Name: %s\n", field_name);
    printf("  Description: %s\n", description);

    return status;
}

bool parse_table_definition()
{
    check_status();
    parse_separator();
    parse_field();
    check_status();
    while (c != '-') {
        parse_field();
    }
    parse_separator();
    return status;
}

bool parse_table_name()
{
    check_status();

    size_t i = 0;
    if (is_letter()) {
        table_name[i++] = c;
    }
    parse_letter();

    while (is_letter() || is_dec_digit() || is_char(' ')) {
        if (i >= MAX_NAME_LEN) {
            set_parse_error("Error: %s\n", "table name is too long");
            break;
        }
        table_name[i++] = c;
        get_char();
    }
    table_name[i] = '\0';

    return status;
}

bool parse_definition()
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
        parse_table_definition();
    } else {
        parse_char(':');
        while (is_line_space()) {
            get_char();
        }
        parse_field_name();
        while (is_line_space()) {
            get_char();
        }
        parse_eol();
        parse_field_definition();
    }
    return status;
}

bool parse_config()
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
        set_parse_error("Unexpected symbol: %c\n", c);
    }

    return status;
}


void parse_file(const char *fname)
{
    fopen_s(&file, fname, "r");
    get_char();

    if (!parse_config()) {
        fprintf(stderr, "%s", error);
    }

    fclose(file);
}