#ifndef REQUEST_H
#define REQUEST_H

struct vrequest {
    int         req_stat;
    int         req_fd;
    vconn_t  *req_conn;
};

struct vrequest*  vrequest_parse(struct vconn*);
int               vrequest_free(struct vrequest*);
int               on_request(struct vrequest*);

#endif 
