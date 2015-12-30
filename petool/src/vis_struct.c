#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "vis_struct.h"

// Currently existing visual structures
static store_t structs = store_init(vis_struct_t);

// Create a new visual structure
vis_struct_t * vis_create_struct(const char *name)
{
    vis_struct_t * st = store_alloc(&structs);
    strncpy_s(st->name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    store_create(&st->fields, vis_field_t);
    return st;
}

// Find a visual structure by its name
vis_struct_t * vis_find_struct(const char *name)
{
    for (size_t i = 0; i < structs.size; i++) {
        vis_struct_t *st = store_pget(&structs, i);
        if (strcmp(st->name, name) == 0) {
            return st;
        }
    }
    return NULL;
}

// Add a new field to a visual structure
vis_field_t * vis_add_field(vis_struct_t *st, const char *name, size_t size, vis_field_type_t type, const char *description)
{
    vis_field_t * field = store_alloc(&st->fields);
    strncpy_s(field->name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    field->size = size;
    field->type = type;
    strncpy_s(field->description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
    //field->value undefined
    store_create(&field->valid_values, vis_value_info_t);
    return field;
}

// Find a field of a visual structure
vis_field_t * vis_find_field(vis_struct_t *st, const char *field_name)
{
    for (size_t i = 0; i < st->fields.size; i++) {
        vis_field_t *f = store_pget(&st->fields, i);
        if (strcmp(f->name, field_name) == 0) {
            return f;
        }
    }
    return NULL;
}

// Print current value of the field
void vis_print_field_value(vis_field_t *field)
{
    switch (field->type)
    {
        case VIS_UINT:
        {
            printf("%lld", field->value);
            break;
        }

        case VIS_ENUM:
        {
            vis_value_info_t *matching_vi = NULL;
            for (size_t i = 0; i < field->valid_values.size; i++) {
                vis_value_info_t *vi = store_pget(&field->valid_values, i);
                if (vi->value == field->value) {
                    matching_vi = vi;
                    break;
                }
            }

            if (matching_vi) {
                printf("%s", matching_vi->name);
            } else {
                printf("Unknown (0x%llx)", field->value);
            }

            break;
        }

        case VIS_FLAG:
        {
            for (size_t i = 0; i < field->valid_values.size; i++) {
                vis_value_info_t *vi = store_pget(&field->valid_values, i);
                if (field->value & vi->value) {
                    printf("%s ", vi->name);
                }
            }
            break;
        }

        case VIS_TIME:
        {
            printf("%s", ctime((time_t *)&field->value));
            break;
        }
    }
}

// Add a new valid value to a field of a visual structure
void vis_add_value_info(vis_field_t *field, const char *name, vis_value_t value, const char *description)
{
    vis_value_info_t *vi = store_alloc(&field->valid_values);
    strncpy_s(vi->name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    vi->value = value;
    strncpy_s(vi->description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
}

// Read a value from file
vis_value_t vis_read_value(FILE *infile, size_t size)
{
    vis_value_t value = 0;
    if (size == 1) {
        uint8_t v;
        fread(&v, 1, 1, infile);
        value = v;
    } else if (size == 2) {
        uint16_t v;
        fread(&v, 2, 1, infile);
        value = v;
    } else if (size == 4) {
        uint32_t v;
        fread(&v, 4, 1, infile);
        value = v;
    } else if (size == 8) {
        uint64_t v;
        fread(&v, 8, 1, infile);
        value = v;
    }
    return value;
}




const char * vis_field_type_to_str(vis_field_type_t type)
{
    switch (type)
    {
        case VIS_UINT: return "UINT";
        case VIS_ENUM: return "ENUM";
        case VIS_FLAG: return "FLAG";
        case VIS_TIME: return "TIME";
    }
    return "Unknown";
}

// Print all known structures to stdout
void vis_print_all()
{
    for (size_t i = 0; i < structs.size; i++) {
        vis_struct_t *st = store_pget(&structs, i);
        printf("STRUCTURE: %s\n", st->name);

        for (size_t j = 0; j < st->fields.size; j++) {
            vis_field_t *f = store_pget(&st->fields, j);
            printf("    Field: %s (%s, %lld bytes) = ", f->name, vis_field_type_to_str(f->type), f->size);
            vis_print_field_value(f);
            printf("\n");

            printf("           %s\n", f->description);

            for (size_t k = 0; k < f->valid_values.size; k++) {
                vis_value_info_t *vi = store_pget(&f->valid_values, k);
                printf("        %-40s 0x%04llx %s\n", vi->name, vi->value, vi->description);
            }
        }
    }
}

// Read a structure from current position in a file
void vis_read_struct(FILE *infile, const char *struct_name)
{
    vis_struct_t *st = vis_find_struct(struct_name);
    assert(st);
    
    for (size_t i = 0; i < st->fields.size; i++) {
        vis_field_t *field = store_pget(&st->fields, i);
        field->value = vis_read_value(infile, field->size);
    }
}