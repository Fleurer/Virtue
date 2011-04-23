#ifndef CONN_H
#define CONN_H

#include "buffer.h"

typedef struct vconn {
    int     c_stat;
    int     c_fd;
    struct vbuf_head *c_buf_read;
    struct vbuf_head *c_buf_write;
} vconn_t;

#define CONN_READY   0
#define CONN_READING 1
#define CONN_WRITING 2


vconn_t*     vconn_new(int fd);
int          vconn_close(struct vconn*);
//
int          vread_begin(struct vconn*);
int          vwrite_begin(struct vconn*);
//
int          on_read_complete(struct vconn*);
int          on_write_complete(struct vconn*);
int          on_conn_error(struct vconn*);

#endif 
