#include "vt.h"
#include "event.h"
#include "sock.h"
#include "conn.h"

#define PORT 4000
#define LISTEN_SIZ 10

int vt_sockfd_on_readable(vt_event_t *ev) {
    vt_conn_t *conn;

    conn = vt_accept(ev->fd);
    if (conn == NULL) {
        vt_log("new connection failed\n");
    }
    vt_log("new connection established, fd: %d\n", conn->fd);
    return 0;
}

int vt_main() {
    vt_cycle_t cl;
    vt_event_t ev;
    int sockfd;

    vt_cycle_init(&cl);
    sockfd = vt_sockfd_new();
    if (sockfd < 0) {
        return -1;
    }
    vt_event_init(&ev, sockfd, EV_READ, NULL);
    vt_event_bind(&ev, EV_READ, (vt_event_cb_t)&vt_sockfd_on_readable);
    vt_event_add(&cl, &ev);
    vt_event_process(&cl);
    return 0;
}

int main(int argc, char *argv[]) {
    vt_main();
    return 0;
}
