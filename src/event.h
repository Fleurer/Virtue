#ifndef EVENT_H
#define EVENT_H

struct vt_event_loop;
struct vt_io_event;
struct vt_io_poller;
struct vt_buf;

typedef (*vt_event_cb_t)(struct vt_event *);
typedef (*vt_io_event_cb_t)(struct vt_io_event *);

typedef struct vt_event_loop {
    TAILQ_HEAD(, vt_event) io_events;
    struct vt_io_poller *poller;
} vt_event_loop_t;

typedef struct vt_event {
    int type;
    int flag;
    TAILQ_ENTRY(vt_event) entry;
};

typedef struct vt_io_event {
    struct vt_event ev;
    int fd;
    struct vt_buf_head buf_head;
    vt_io_event_cb_t on_readable;
    vt_io_event_cb_t on_writeable;
} vt_io_event_t;

typedef struct vt_io_poller {
    void (*poll)(vt_event_loop_t*);
    void (*add)(vt_event_loop_t*, vt_io_event_t*);
    void (*remove)(vt_event_loop_t*, vt_io_event_t*);
    void *data;
} vt_event_op_t;

int vt_event_process(vt_event_loop_t *elp);
int vt_io_event_init(vt_io_event_t *ev);
int vt_io_event_add(vt_event_loop_t *elp, ev_io_event_t *ep);
int vt_io_event_remove(vt_event_loop_t *elp, ev_io_event_t *ep);

#endif
