#ifndef VT_H
#define VT_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>

#define NDEBUG 1

/* TODO: for debug yet, the fprintf(stderr, ...) stuff is unavailable for daemons. */
#define vt_log(fmt, ...) do { \
    fprintf(stderr, "%s: " fmt, __func__, ##__VA_ARGS__); } while(0)

#endif
