#ifndef CONN_H
#define CONN_H

struct vt_pool;
struct vt_request;
struct vt_response;

typedef struct vt_conn {
    int state;
    int fd;
    struct vt_event event;
    struct sockaddr_in sockaddr;
    struct vt_pool *pool;
    TAILQ_HEAD(, vt_request) requests;
    TAILQ_HEAD(, vt_response) responses;
} vt_conn_t;

vt_conn_t* vt_accept(int sockfd);
void vt_conn_destroy(vt_conn_t *conn);

#endif
