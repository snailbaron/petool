/**
 * @file
 *
 * Structures for visual representation of PE file
 */

#ifndef VIS_STRUCT_H
#define VIS_STRUCT_H

#include <stdint.h>
#include <stdio.h>
#include "store.h"

#define MAX_NAME_LEN 100
#define MAX_DESCR_LEN 5000

typedef uint64_t vis_value_t;

/**
 * Structure with fields for visual representation
 */
typedef struct vis_struct_t
{
    char    name[MAX_NAME_LEN + 1];
    store_t fields;
} vis_struct_t;

vis_struct_t * vis_create_struct(const char *name);
vis_struct_t * vis_find_struct(const char *name);

typedef enum
{
    VIS_UINT,
    VIS_ENUM,
    VIS_FLAG,
    VIS_TIME,
} vis_field_type_t;

/**
 * A field in visual structure
 */
typedef struct vis_field_t
{
    char             name[MAX_NAME_LEN + 1];
    size_t           size;
    vis_field_type_t type;
    char             description[MAX_DESCR_LEN + 1];
    vis_value_t      value;
    store_t          valid_values;
} vis_field_t;

vis_field_t * vis_add_field(vis_struct_t *st, const char *name, size_t size, vis_field_type_t type, const char *description);
vis_field_t * vis_find_field(vis_struct_t *st, const char *field_name);


/**
 * Information about a valid field value
 */
typedef struct vis_value_info_t
{
    char name[MAX_NAME_LEN + 1];
    vis_value_t value;
    char description[MAX_DESCR_LEN + 1];
} vis_value_info_t;

void vis_add_value_info(vis_field_t *field, const char *name, vis_value_t value, const char *description);
const char * vis_field_value_str(vis_field_t *field);

void vis_print_all();

void vis_read_struct(FILE *infile, const char *struct_name);

#endif
