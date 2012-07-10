#include <stdlib.h>
#include <string.h>
#include "pool.h"
#include "vt.h"

vt_pool_t* vt_pool_new() {
    vt_pool_t *pl; 

    pl = (vt_pool_t*)vt_malloc(sizeof(vt_pool_t));
    STAILQ_INIT(&pl->entries);
    return pl;
}

void vt_pool_destroy(vt_pool_t *pl) {
    vt_pool_entry_t *pe, *npe;
    size_t nobj = 0;

    for (pe = STAILQ_FIRST(&pl->entries); pe; pe = npe, nobj++) {
        npe = STAILQ_NEXT(pe, entry);
        if (pe->dtor) {
            pe->dtor(pe->mem);
        }
        vt_free(pe->mem);
        vt_free(pe);
    }
    vt_log("free pool: %p, %d objects freed.\n", pl, nobj);
}

void* vt_palloc(vt_pool_t *pl, size_t size, vt_dtor_cb_t dtor) {
    vt_pool_entry_t *pe;

    pe = (vt_pool_entry_t*)vt_malloc(sizeof(vt_pool_entry_t));
    pe->mem = vt_malloc(size);
    pe->size = size;
    pe->dtor = dtor;
    STAILQ_INSERT_TAIL(&pl->entries, pe, entry);
    return pe->mem;
}

/* ----------------------------- */

void *vt_malloc(size_t size) {
    void *mem;

    mem = malloc(size);
    if (mem == NULL) {
        vt_log("out of memory");
    }
    return mem;
}

void vt_free(void *mem) {
    free(mem);
}


