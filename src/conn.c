#include "vt.h"
#include "event.h"
#include "pool.h"
#include "conn.h"

static void vt_conn_on_readable(vt_event_t *ev);

/* ----------------------------------- */

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
    conn->pool = vt_pool_new();
    vt_event_init(&conn->event, conn->fd, EV_READ | EV_WRITE, conn);
    vt_event_bind(&conn->event, EV_READ, (vt_event_cb_t)&vt_conn_on_readable);
    return conn;
}

void vt_conn_destroy(vt_conn_t *conn) {
    close(conn->fd);
    vt_pool_destroy(conn->pool);
    vt_free(conn);
}

/* ------------------------------- */

static void vt_conn_on_readable(vt_event_t *ev) {
    char buf[1024];
    int n;

    n = read(ev->fd, buf, 1024);
    buf[n] = '\0';
    printf("%s\n", buf);
}
