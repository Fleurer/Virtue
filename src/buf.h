#ifndef BUF_H
#define BUF_H

#include "queue.h"

struct pool;

#define VT_BUF_MAXSIZ (4096*2)

typedef struct vt_buf_entry {
    char *buf;
    size_t size;
    STAILQ_ENTRY(vt_buf_entry) entry;
} vt_buf_entry_t;

typedef struct vt_buf {
    struct vt_pool *pool;
    STAILQ_HEAD(, vt_buf_entry) entries;
} vt_buf_t;

vt_buf_t* vt_buf_new(struct vt_pool *pool);
int vt_buf_write(vt_buf_t *bp, char *buff, int size);

#endif
