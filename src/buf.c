#include "vt.h"
#include "buf.h"
#include "pool.h"

vt_buf_entry_t* vt_buf_grow(vt_buf_t *bp) {
    vt_buf_entry_t *be;
    
    be = (vt_buf_entry_t*)vt_palloc(bp->pool, sizeof(vt_buf_entry_t));
    be->buf = vt_palloc(bp->pool, VT_BUF_MAXSIZ);
    be->size = 0;
    STAILQ_INSERT_TAIL(&bp->entries, be, entry);
    return be;
}

vt_buf_t* vt_buf_new(vt_pool_t *pool) {
    vt_buf_t *bp;

    bp = (vt_buf_t*)vt_palloc(pool, sizeof(vt_buf_t));
    bp->pool = pool;
    STAILQ_INIT(&bp->entries);
    vt_buf_grow(bp);
    return bp;
}

vt_buf_entry_t* vt_buf_last_entry(vt_buf_t *bp){
    return STAILQ_LAST(&bp->entries, vt_buf_entry, entry);
}

int vt_buf_write(vt_buf_t *bp, char *buff, int size) {
    vt_buf_entry_t *be;
    size_t max_size = VT_BUF_MAXSIZ;
    size_t s;

    be = vt_buf_last_entry(bp);
    while (size > (s = max_size - be->size)) {
        memcpy(be->buf, buff, s);
        be->size = max_size;
        size -= s;
        be = vt_buf_grow(bp);
    }
    return 0;
}

