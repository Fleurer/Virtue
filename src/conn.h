#ifndef CONN_H
#define CONN_H

struct vt_pool;

typedef struct vt_conn {
    int fd;
    struct vt_event event;
    struct sockaddr_in sockaddr;
    struct vt_pool *pool;
} vt_conn_t;

vt_conn_t* vt_accept(int sockfd);
void vt_conn_destroy(vt_conn_t *conn);

#endif
