#include "error.h"
#include <string.h>

#define ERROR_MSG_MAX_LEN 4096

static bool is_error = false;
static char error[ERROR_MSG_MAX_LEN + 1] = "";

void set_error(const char *msg)
{
    is_error = true;
    strncpy_s(error, ERROR_MSG_MAX_LEN + 1, msg, ERROR_MSG_MAX_LEN);
}

const char * get_error()
{
    return error;
}

bool has_error()
{
    return is_error;
}

void clear_error()
{
    is_error = false;
    error[0] = '\0';
}
