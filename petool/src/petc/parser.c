#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../vis_struct.h"
#include "../label.h"

#define MAX_ERROR_LEN 5000      // max length of error message
#define MAX_VALUE_LEN 10        // max length of a value string
#define MAX_VARIATIONS 5        // max number of structure variations
#define MAX_STRING_LEN 5000     // max length of description string
#define MAX_WORD_LEN 100



static char struct_name[MAX_NAME_LEN + 1] = "";
static char struct_variation_list[MAX_VARIATIONS][MAX_NAME_LEN + 1];
static size_t struct_variation_num = 0;

static vis_struct_t *structs[MAX_VARIATIONS];
static size_t struct_num = 0;


/* Common data: used when parsing any type of definition, structure or field */

// Name of the affected structure, may have variations

// Description of current item
static char description[MAX_DESCR_LEN + 1] = "";
static size_t descr_idx = 0;



/* Data from structure definition parsing */


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

void err(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}



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
** Parsing interface
*/

void parse_file(const char *fname)
{
    fopen_s(&file, fname, "r");

    scan();
    while (c != EOF) {
        printf("%d:%d %s\n", line_number, sym_number, char_to_str(c));
        scan();
    }


    //get_char();

    //parse_config();

    //if (!status) {
    //    fprintf(stderr, "Error parsing %s:\n%s", fname, error);
    //}

    fclose(file);
}
