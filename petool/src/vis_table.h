#ifndef VIS_TABLE_H
#define VIS_TABLE_H

#include <stddef.h>
#include <stdint.h>

#define MAX_NAME_LEN 30
#define MAX_DESCR_LEN 5000

// A special numeric value for visual representation,
// with name and description
typedef struct
{
    char name[MAX_NAME_LEN + 1];
    uint64_t value;
    char description[MAX_DESCR_LEN + 1];
} vis_value_t;

// Create a value
vis_value_t vis_make_value(const char *name, uint64_t value, const char *description);



// A record in a visual table. Represents a parameter of a structure.
// Some parameters have a finite list of valid values.
typedef struct
{
    char name[MAX_NAME_LEN + 1];
    size_t size;
    uint64_t value;
    vis_value_t *valid_values;
    size_t value_cap;
    size_t value_count;
} vis_record_t;

// Create a visual record
vis_record_t vis_make_record(const char *name, size_t size, uint64_t value);

// Add a valid value to the list of valid values for the given record
void vis_add_value(vis_record_t *rec, vis_value_t value);


// A visual table representing a structure.
typedef struct
{
    char name[MAX_NAME_LEN + 1];
    vis_record_t *records;
    size_t record_cap;
    size_t record_count;
} vis_table_t;

vis_table_t vis_make_table(const char *name);
void vis_add_record(vis_table_t *table, vis_record_t rec);


#endif