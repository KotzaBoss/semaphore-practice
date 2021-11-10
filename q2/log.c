#include "log.h"
#include <stdio.h>
#include <stdarg.h>

void logf(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

void log(const char* msg)
{
	fprintf(stderr, msg);
}
