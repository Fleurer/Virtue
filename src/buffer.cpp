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

vbuf_head_t* vbuf_head_new(int type){
    vbuf_head_t *bhp;
    
    bhp = (vbuf_head_t*)malloc(sizeof(vbuf_head_t));
    bhp->bh_type = type;
    bhp->bh_bufs = new list<vbuf_t*>();
    return bhp;
}

int vbuf_head_free(vbuf_head_t *bhp){
    vbuf_t *bp;
    list<vbuf_t*> *lp;
    list<vbuf_t*>::iterator pos;

    lp = bhp->bh_bufs;
    for (pos=lp->begin(); pos!=lp->end(); ++pos) {
        bp = *pos;
        vbuf_free(bp);
    }
    delete bhp->bh_bufs;
    return 0;
}

/* ------------------------------------ */

/* returns 0 on finished. */
int vbuf_read(int fd, vbuf_head_t *bhp){
    vbuf_t *bp;
    int r;

    bp = vbuf_append(bhp);
    r = recv(fd, bp->b_data, BUF_SIZE, 0);
    if (r < 0) 
        return r;
    if (r == 0) {
        vbuf_erase_last(bhp);
    }
    return r;
}

/* pop the first buffer of the output buffer queue, 
 * and write it out. 
 * returns -1 on error, or 0 on finished. */
int vbuf_write(int fd, vbuf_head_t *bhp){
    vbuf_t *bp;
    int r;

    bp = bhp->bh_bufs->front();
    r = send(fd, bp->b_data, BUF_SIZE, 0);
    if (r < 0) {
    }
    if (r == 0) {
    }
    vbuf_erase_first(bhp);
    return r;
}

/* ------------------------------------ */

/* */
vbuf_t* vbuf_append(vbuf_head_t *bhp) {
    list<vbuf_t*> *lp;
    vbuf_t *bp;

    bp = vbuf_alloc();
    lp = bhp->bh_bufs;
    lp->push_back(bp);
    return bp;
}

/* called on the end */
int vbuf_erase_last(vbuf_head_t *bhp){
    list<vbuf_t*> *lp;
    vbuf_t *bp;

    lp = bhp->bh_bufs;
    bp = lp->back();
    if (bp != 0) {
        vbuf_free(bp);
        return 1;
    }
    lp->pop_back();
    return 0;
}

/* called on each write action */
int vbuf_erase_first(vbuf_head_t *bhp){
    list<vbuf_t*> *lp;
    vbuf_t *bp;

    lp = bhp->bh_bufs;
    bp = lp->front();
    if (bp != 0) {
        vbuf_free(bp);
        return 1;
    }
    lp->pop_front();
    return 0;
}

/* ----------------------------------------- */

/* TODO: add a freelist here, and check the return value of malloc.
 * */
vbuf_t* vbuf_alloc(){
    vbuf_t *bp;

    bp = (vbuf_t*)malloc(sizeof(vbuf_t));
    bp->b_size = 0;
    bp->b_data = (char*)malloc(BUF_SIZE);
    return bp;
}

int vbuf_free(vbuf_t* bp){
    free(bp->b_data);
    free(bp);
    return 0;
}
