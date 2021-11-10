
#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>

#define logf(fmt, args...) fprintf(stderr, fmt, args)
#define log(msg) fprintf(stderr, msg)

#endif
