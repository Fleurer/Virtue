#ifndef EVENT_H
#define EVENT_H

#include <sys/select.h>
#include "queue.h"

struct vt_event_loop;
struct vt_io_event;
struct vt_buf;

typedef void (*vt_io_event_cb_t)(struct vt_io_event *);

typedef struct vt_event_loop {
    TAILQ_HEAD(, vt_io_event) io_events;
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
} vt_event_loop_t;

typedef struct vt_io_event {
    /* public */
    int fd;
    int flag;
    vt_io_event_cb_t on_readable;
    vt_io_event_cb_t on_writeable;
    vt_io_event_cb_t on_error;
    /* private */
    TAILQ_ENTRY(vt_event) entry;
    struct vt_buf *buf;
} vt_io_event_t;

int vt_event_process(vt_event_loop_t *elp);
int vt_io_event_init(vt_io_event_t *ev, int flag, int fd);
int vt_io_event_add(vt_event_loop_t *elp, vt_io_event_t *ep);
int vt_io_event_remove(vt_event_loop_t *elp, vt_io_event_t *ep);

#endif
