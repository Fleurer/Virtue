#include <pool.h>
#include <stdlib.h>

vt_pool_t* vt_pool_new() {
    vt_pool_t *pl; 

    pl = (vt_pool_t*)malloc(sizeof(vt_pool_t));
    STAILQ_INIT(&pl->entries);
    return pl;
}

void vt_pool_free(vt_pool_t *pl) {
    vt_pool_entry_t *pe, *npe;
    size_t nobj = 0;

    for (pe = STAILQ_FIRST(&pl->entries); pe; pe = npe, nobj++) {
        npe = STAILQ_NEXT(pe, entry);
        free(pe->mem);
        free(pe);
    }
    vt_log("free pool: %p, %d objects freed.\n", pl, nobj);
}

void* vt_palloc(vt_pool_t *pl, size_t size) {
    vt_pool_entry_t *pe;

    pe = (vt_pool_entry_t*)malloc(sizeof(vt_pool_entry_t));
    pe->mem = malloc(size);
    pe->size = size;
    STAILQ_INSERT_TAIL(pl->entries, pe);
    return pe->mem;
}
