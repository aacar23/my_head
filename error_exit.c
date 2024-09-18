#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "error_exit.h"

void error_exit(const char *format, ...)
{
    va_list args;
    va_start(args, format);


    vfprintf(stderr, format, args);

    va_end(args);

    exit(EXIT_FAILURE);
}
