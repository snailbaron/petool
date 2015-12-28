#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pet_struct.h"

static pet_struct_t *pet_structs = NULL;

pet_struct_t * pet_create_struct(const char *name)
{
    pet_struct_t *st = malloc(sizeof(pet_struct_t));
    strncpy_s(st->name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    st->fields = NULL;
    st->field_count = 0;
    return st;
}

pet_field_t * pet_create_field(const char *name, size_t size, pet_field_type_t type, const char *description)
{
    pet_field_t *field = malloc(sizeof(pet_field_t));
    strncpy_s(field->name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    field->size = size;
    field->type = type;
    strncpy_s(field->description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
    field->value = 0;
    field->valid_values = NULL;
    return field;
}

pet_value_info_t pet_make_value_info(const char *name, pet_value_t value, const char *description)
{
    pet_value_info_t vi;
    strncpy_s(vi.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    vi.value = value;
    strncpy_s(vi.description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
    return vi;
}