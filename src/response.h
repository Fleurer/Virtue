#ifndef RESPONSE_H
#define RESPONSE_H

#include "queue.h"

struct vt_request;

typedef struct vt_response {
    int state;
    struct vt_request *req;
    TAILQ_ENTRY(vt_response) entry;
} vt_response_t;

enum {
    VT_RESP_READY = 0
};


#endif
