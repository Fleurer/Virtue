#ifndef buf_H
#define buf_H

#include <list>
#include "conn.h"

typedef struct vbuf {
    char   *b_data;
    int     b_size;
} vbuf_t;

typedef struct vbuf_head {
    int     bh_type;
    int     bh_stat;
    vconn_t             *bh_conn;
    std::list<vbuf_t*>  *bh_bufs;
} vbuf_head_t;

/* type */
#define BUF_READ     1
#define BUF_WRITE    2

/* stat */
#define BUF_ERROR    0x1
#define BUF_COMPLETE 0x2

/* max size */
#define BUF_SIZE 4

// buf head
vbuf_head_t*      vbuf_head_new(int type);
int               vbuf_head_free(vbuf_head_t *);
//
vbuf_t*           vbuf_append(vbuf_head_t *);
int               vbuf_erase_first(vbuf_head_t *bhp);
int               vbuf_erase_last(vbuf_head_t *);
/**/
int               vbuf_free(vbuf_t* bp);
vbuf_t*           vbuf_alloc();
/**/
int               vbuf_read(int fd, vbuf_head_t *bhp);
int               vbuf_write(int fd, vbuf_head_t *bhp);
int               on_buf_complete(struct vbuf_head *);

#endif 
