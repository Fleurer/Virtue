#include <sys/select.h>
#include "event.h"
#include "buf.h"

int vt_event_loop_init(vt_event_loop_t *evl) {
    TAILQ_INIT(&evl->io_events);
    evl->nfds = 0;
    FD_ZERO(&evl->read_fds);
    FD_ZERO(&evl->write_fds);
    FD_ZERO(&evl->except_fds);
    return 0;
}

int vt_io_event_init(vt_io_event_t *ev, int flag, int fd) {
    ev->flag = flag;
    ev->fd = fd;
    ev->on_readable = NULL;
    ev->on_writeable = NULL;
    ev->on_error = NULL;
    return 0;
}

int vt_io_event_add(vt_event_loop_t *evl, vt_io_event_t *ev) {
    if (evl->flag & EV_IO_READ) {
        FD_SET(ev->fd, &evl->read_fds);
    }
    if (evl->flag & EV_IO_WRITE) {
        FD_SET(ev->fd, &evl->write_fds);
    }
    if (ev->fd > evl->max_fd) {
        evl->max_fd = ev->fd;
    }
    TAILQ_INSERT_TAIL(&evl->io_events, ev, entry);
    return 0;
}

int vt_io_event_remove(vt_event_loop_t *evl, vt_io_event_t *ev) {
    if (evl->flag & EV_IO_READ) {
        FD_CLR(ev->fd, &evl->read_fds);
    }
    if (evl->flag & EV_IO_WRITE) {
        FD_CLR(ev->fd, &evl->write_fds);
    }
    TAILQ_REMOVE(&evl->io_events, ev, entry);
    return 0;
}

int vt_event_process(vt_event_loop_t *evl) {
    int r; 

    while (r = select(evl->max_fd + 1, evl->read_fds, evl->write_fds, NULL, NULL)) {
    }
    return 0;
}
