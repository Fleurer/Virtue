#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
//
#include "vt/vt.h"

vconn_t* vconn_new(int fd) {
    vconn_t *cp = (vconn_t*)malloc(sizeof(vconn_t));
    cp->c_fd = fd;
    cp->c_stat = 0;
    cp->c_buf_read  = vbuf_head_new(BUF_READ);
    cp->c_buf_write = vbuf_head_new(BUF_WRITE);
    return cp;
}
