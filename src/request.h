#ifndef REQUEST_H
#define REQUEST_H

#include "queue.h"

struct vt_buf;
struct vt_pool;
struct vt_conn;
struct vt_response;

typedef struct vt_request {
    int state;
    struct vt_conn *conn;
    struct vt_buf *buf;
    struct vt_pool *pool;
    struct vt_response *resp;
    TAILQ_ENTRY(vt_request) entry;
} vt_request_t;

enum {
    VT_REQ_READY = 0,
    VT_REQ_HEAD,
    VT_REQ_BODY,
    VT_REQ_FINISHED
};

vt_request_t* vt_request_new(struct conn *conn);
void vt_request_destroy(vt_request_t *req);

vt_request_t* vt_request_first(struct conn *conn);
vt_request_t* vt_request_last(struct conn *conn);


#endif
