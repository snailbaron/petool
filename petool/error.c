#include "error.h"
#include <string.h>

#define ERROR_MSG_MAX_LEN 4096

static char error[ERROR_MSG_MAX_LEN + 1];

void set_error(const char *msg)
{
    strncpy_s(error, ERROR_MSG_MAX_LEN + 1, msg, ERROR_MSG_MAX_LEN);
}

const char * get_error()
{
    return error;
}
