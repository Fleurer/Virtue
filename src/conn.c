#include "vt.h"
#include "pool.h"
#include "conn.h"

vt_conn_t* vt_accept(int sockfd) {
    int fd;
    vt_conn_t *conn;
    socklen_t socklen;

    conn = (vt_conn_t*)vt_malloc(sizeof(vt_conn_t));
    socklen = sizeof(conn->sockaddr);
    fd = accept(sockfd, (struct sockaddr*)&conn->sockaddr, &socklen);
    if (fd < 0) {
        vt_free(conn);
        return NULL;
    }
    //
    conn->fd = fd;
    conn->pool = vt_pool_create();
    return conn;
}

void vt_conn_destroy(vt_conn_t *conn) {
    close(conn->fd);
    vt_pool_destroy(conn->pool);
    vt_free(conn);
}
