#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vis_struct.h"
#include "da.h"

// Known structures
static da_declare(vis_struct_t, structs);

// Find a visual structure by its name
vis_struct_t * vis_find_struct(const char *name)
{
    for (size_t i = 0; i < structs.size; i++) {
        if (strcmp(da_get(structs, i).name, name) == 0) {
            return da_pget(structs, i);
        }
    }
    return NULL;
}

// Create a new visual structure
vis_struct_t * vis_create_struct(const char *name)
{
    // Create the structure
    vis_struct_t st = {
        .name = "",
        .fields = da_init(vis_field_t),
    };
    strncpy_s(st.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    
    // Add the new structure to global list, and return its address
    da_add(structs, st);    
    return da_pget(structs, structs.size - 1);
}

// Add a new field to a visual structure
vis_field_t * vis_add_field(vis_struct_t *st, const char *name, size_t size, vis_field_type_t type, const char *description)
{
    vis_field_t field = {
        .name = "",
        .size = size,
        .type = type,
        .description = "",
        .value = 0,
        .valid_values = da_init(vis_value_info_t),
    };
    strncpy_s(field.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    strncpy_s(field.description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
    
    da_add(st->fields, field);
    return da_pget(st->fields, st->fields.size - 1);
}

void vis_add_value_info(vis_field_t *field, const char *name, vis_value_t value, const char *description)
{
    vis_value_info_t vi = {
        .name = "",
        .value = value,
        .description = "",
    };
    strncpy_s(vi.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    strncpy_s(vi.description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);

    da_add(field->valid_values, vi);
}

