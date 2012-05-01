#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include "event.h"
#include "buf.h"
#include "vt.h"

int vt_cycle_init(vt_cycle_t *cl) {
    TAILQ_INIT(&cl->io_event_entries);
    cl->max_fd = 0;
    FD_ZERO(&cl->read_fds);
    FD_ZERO(&cl->write_fds);
    FD_ZERO(&cl->except_fds);
    return 0;
}

int vt_event_process(vt_cycle_t *cl) {
    int r; 
    vt_event_t *ev;
    vt_event_cb_t cb;

    while ((r = select(cl->max_fd + 1, &cl->read_fds, &cl->write_fds, NULL, NULL))) {
        if (r < 0) {
            vt_log("select() failed: %d, %s", errno, strerror(errno));
            continue;
        }
        TAILQ_FOREACH(ev, &cl->io_event_entries, entry) {
            // in read 
            if (FD_ISSET(ev->fd, &cl->read_fds)) {
                cb = ev->on_readable;
                if (cb)
                    cb(ev);
            }
            // in write
            if (FD_ISSET(ev->fd, &cl->write_fds)) {
                cb = ev->on_readable;
                if (cb)
                    cb(ev);
            }
        }
    }
    return 0;
}

/* ------------------ */

int vt_event_init(vt_event_t *ev, int fd, int flag) {
    ev->flag = flag;
    ev->fd = fd;
    ev->on_readable = NULL;
    ev->on_writeable = NULL;
    ev->on_error = NULL;
    return 0;
}

int vt_event_bind(vt_event_t *ev, int flag, vt_event_cb_t cb) {
    if (flag & EV_READ) {
        ev->on_readable = cb;
    }
    if (flag & EV_WRITE) {
        ev->on_writeable = cb;
    }
    return 0;
}

int vt_event_add(vt_cycle_t *cl, vt_event_t *ev) {
    if (ev->flag & EV_READ) {
        FD_SET(ev->fd, &cl->read_fds);
    }
    if (ev->flag & EV_WRITE) {
        FD_SET(ev->fd, &cl->write_fds);
    }
    if (ev->fd > cl->max_fd) {
        cl->max_fd = ev->fd;
    }
    TAILQ_INSERT_TAIL(&cl->io_event_entries, ev, entry);
    return 0;
}

int vt_event_remove(vt_cycle_t *cl, vt_event_t *ev) {
    if (ev->flag & EV_READ) {
        FD_CLR(ev->fd, &cl->read_fds);
    }
    if (ev->flag & EV_WRITE) {
        FD_CLR(ev->fd, &cl->write_fds);
    }
    TAILQ_REMOVE(&cl->io_event_entries, ev, entry);
    return 0;
}
