#ifndef VIS_STRUCT_H
#define VIS_STRUCT_H

#include <stdint.h>
#include "da.h"

#define MAX_NAME_LEN 100
#define MAX_DESCR_LEN 5000

typedef uint64_t vis_value_t;

// Information about a special value of a field
typedef struct vis_value_info_t
{
    char name[MAX_NAME_LEN + 1];
    vis_value_t value;
    char description[MAX_DESCR_LEN + 1];
} vis_value_info_t;

typedef enum
{
    UINT,
    ENUM,
    FLAG,
    TIME,
} vis_field_type_t;

// A single field of a visual structure
typedef struct vis_field_t
{
    char name[MAX_NAME_LEN + 1];
    size_t size;
    vis_field_type_t type;
    char description[MAX_DESCR_LEN + 1];
    vis_value_t value;
    da_t(vis_value_info_t) valid_values;
} vis_field_t;

// Structure for visual representation
typedef struct vis_struct_t
{
    char name[MAX_NAME_LEN + 1];
    da_t(vis_field_t) fields;
} vis_struct_t;

#endif
