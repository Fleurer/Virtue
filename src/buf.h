#ifndef BUF_H
#define BUF_H

typedef struct vt_buf {
    struct vt_buf_entry entry;
} vt_buf_t;

typedef struct vt_buf_entry {
    char buf[BUFSIZ];
    size_t size;
    struct vt_buf_entry *next;
} vt_buf_entry_t;

#endif
