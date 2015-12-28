#include "vis_table.h"
#include <stdlib.h>
#include <string.h>

vis_value_t vis_make_value(const char *name, uint64_t value, const char *description)
{
    vis_value_t v;
    strncpy_s(v.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    v.value = value;
    strncpy_s(v.description, MAX_DESCR_LEN + 1, description, MAX_DESCR_LEN);
    return v;
}


vis_record_t vis_make_record(const char *name, size_t size, uint64_t value)
{
    vis_record_t rec;
    strncpy_s(rec.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    rec.size = size;
    rec.value = value;
    rec.valid_values = NULL;
    rec.value_cap = 0;
    rec.value_count = 0;
}

void vis_add_value(vis_record_t *rec, vis_value_t value)
{
    if (rec->value_count == rec->value_cap) {
        rec->value_cap = rec->value_cap * 2 + 1;
        rec->valid_values = realloc(rec->valid_values, sizeof(vis_value_t) * rec->value_cap);
    }
    rec->valid_values[rec->value_count++] = value;
}



vis_table_t vis_make_table(const char *name)
{
    vis_table_t t;
    strncpy_s(t.name, MAX_NAME_LEN + 1, name, MAX_NAME_LEN);
    t.records = NULL;
    t.record_cap = 0;
    t.record_count = 0;
}

void vis_add_record(vis_table_t *table, vis_record_t rec)
{
    if (table->record_count == table->record_cap) {
        table->record_cap = table->record_cap * 2 + 1;
        table->records = realloc(table->records, sizeof(vis_record_t) * table->record_cap);
    }
    table->records[table->record_count++] = rec;
}