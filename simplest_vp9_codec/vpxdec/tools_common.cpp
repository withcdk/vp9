
#include <stdio.h>// NULL
#include <stdarg.h>// va_start()
#include <stdlib.h>

#include "tools_common.h"


#define LOG_ERROR(label)               \
  do {                                 \
    const char *l = label;             \
    va_list ap;                        \
    va_start(ap, fmt);                 \
    if (l) fprintf(stderr, "%s: ", l); \
    vfprintf(stderr, fmt, ap);         \
    fprintf(stderr, "\n");             \
    va_end(ap);                        \
  } while (0)

void die(const char *fmt, ...)
{
	LOG_ERROR(NULL);
	usage_exit();
}

void usage_exit(void)
{
    //show_help(stderr, 1);
    exit(EXIT_FAILURE);
}


