#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

void set_error(const char *msg);
const char * get_error();
bool has_error();
void clear_error();

#endif