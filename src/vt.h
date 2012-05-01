#ifndef VT_H
#define VT_H

#include "stdio.h"

/* TODO: for debug yet, the fprintf(stderr, ...) stuff is unavailable for daemons. */
#define vt_log(fmt, ...) do { \
    fprintf(stderr, "%s: " fmt, __func__, ##__VA_ARGS__); } while(0)

#endif
