#ifndef PET_STRUCT_H
#define PET_STRUCT_H


#define MAX_NAME_LEN 100
#define MAX_DESCR_LEN 5000

typedef uint64_t pet_value_t;


typedef struct
{
    char name[MAX_NAME_LEN + 1];
    pet_value_t value;
    char description[MAX_DESCR_LEN + 1];
} pet_value_info_t;


typedef enum
{
    UINT,
    ENUM,
    FLAG,
    TIME,
} pet_field_type_t;

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    size_t size;
    pet_field_type_t type;
    char description[MAX_DESCR_LEN + 1];
    pet_value_t value;
    pet_value_info_t *valid_values;
} pet_field_t;

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    pet_field_t *fields;
    size_t field_count;
} pet_struct_t;

#endif
