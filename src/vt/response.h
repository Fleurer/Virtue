#ifndef RESPONSE_H
#define RESPONSE_H

struct vresponse {
    int     rsp_stat;
    int     rsp_env;
};

struct vresponse* vresponse_new();
int               vresponse_free(struct vresponse*);
int               vresponse_send(struct vresponse*);



#endif 
