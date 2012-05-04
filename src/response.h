#ifndef RESPONSE_H
#define RESPONSE_H

#include "queue.h"

typedef struct vt_response {
    int state;
    TAILQ_ENTRY(vt_response) entry;
} vt_response_t;

enum {
    VT_RESP_READY = 0
};


#endif
