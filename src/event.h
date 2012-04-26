#ifndef EVENT_H
#define EVENT_H

struct vt_event_loop;
struct vt_io_event;
struct vt_io_poller;
struct vt_buf;

typedef (*vt_io_event_cb_t)(struct vt_io_event *);

typedef struct vt_event_loop {
    TAILQ_HEAD(, vt_io_event) io_events;
    struct vt_io_poller *poller;
} vt_event_loop_t;

typedef struct vt_io_event {
    int fd;
    int flag;
    vt_io_event_cb_t on_readable;
    vt_io_event_cb_t on_writeable;
    TAILQ_ENTRY(vt_event) entry;
    struct vt_buf_head buf_head;
} vt_io_event_t;

int vt_event_process(vt_event_loop_t *elp);
int vt_io_event_init(vt_io_event_t *ev, int flag, int fd, vt_io_event_cb_t on_readable, vt_io_event_cb_t on_writeable);
int vt_io_event_add(vt_event_loop_t *elp, ev_io_event_t *ep);
int vt_io_event_remove(vt_event_loop_t *elp, ev_io_event_t *ep);

#endif
