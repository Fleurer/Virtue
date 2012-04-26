#include "event.h"
#include "buf.h"

int vt_io_event_init(vt_io_event_t *ev, int flag, int fd) {
    ev->flag = flag;
    ev->fd = fd;
}

int vt_io_event_add(vt_event_loop_t *elp, vt_io_event_t *ep) {
    return 0;
}

int vt_io_event_remove(vt_event_loop_t *elp, vt_io_event_t *ep) {
    return 0;
}

int vt_event_process(vt_event_loop_t *elp) {
    return 0;
}
