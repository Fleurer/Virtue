#ifndef BUF_H
#define BUF_H

#include <stddef.h>

#define VT_BUFSIZ 4096

typedef struct vt_buf_entry {
    char buf[VT_BUFSIZ];
    size_t size;
    struct vt_buf_entry *next;
} vt_buf_entry_t;

typedef struct vt_buf {
    struct vt_buf_entry entry;
} vt_buf_t;

#endif
