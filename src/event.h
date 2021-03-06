#ifndef EVENT_H
#define EVENT_H

#include <stddef.h>
#include <sys/select.h>
#include "queue.h"

struct vt_cycle;
struct vt_event;
struct vt_buf;

typedef void (*vt_event_cb_t)(struct vt_event *);

typedef struct vt_cycle {
    TAILQ_HEAD(, vt_event) events;
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    size_t max_fd;
} vt_cycle_t;


/* note that vt_event do NOT own any resource, so 
 * it doesn't require any destruction. */
typedef struct vt_event {
    /* public */
    int fd;
    int flag;
    vt_event_cb_t on_readable;
    vt_event_cb_t on_writeable;
    vt_event_cb_t on_error;
    /* private */
    TAILQ_ENTRY(vt_event) entry;
    struct vt_buf *buf;
    struct vt_conn *conn;
    struct vt_cycle *cycle;
} vt_event_t;

typedef struct vt_timer {
    /* TODO */
} vt_timer_t;

enum {
    EV_READ = 0x1,
    EV_WRITE = 0x10,
};

int vt_cycle_init(vt_cycle_t *cl);
int vt_event_process(vt_cycle_t *cl);
int vt_event_init(vt_event_t *ev, int fd, int flag, struct vt_conn *conn);
int vt_event_bind(vt_event_t *ev, int flag, vt_event_cb_t cb);
int vt_event_add(vt_cycle_t *elp, vt_event_t *ep);
int vt_event_remove(vt_cycle_t *elp, vt_event_t *ep);

#endif
