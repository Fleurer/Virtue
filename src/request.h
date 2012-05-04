#ifndef REQUEST_H
#define REQUEST_H

#include "queue.h"

typedef struct vt_request {
    int state;
    vt_conn_t *conn;
    vt_buf_t *buf;
    TAILQ_ENTRY(vt_request) entry;
} vt_request_t;

enum {
    VT_REQ_READY = 0,
    VT_REQ_HEAD,
    VT_REQ_BODY,
    VT_REQ_FINISHED
};


#endif
