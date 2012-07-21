#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include "queue.h"

/* A quick and dirty memory pool implementation, just for making 
 * memory deallocating easier yet. It should be replaced with a 
 * chunk allocating mechanism later.
 * */

struct vt_pool_entry;

typedef void (*vt_dtor_cb_t)(void *mem);

typedef struct vt_pool {
    STAILQ_HEAD(, vt_pool_entry) entries;
} vt_pool_t;

typedef struct vt_pool_entry {
    void            *mem;
    size_t          size;
    vt_dtor_cb_t    dtor;
    STAILQ_ENTRY(vt_pool_entry) entry;
} vt_pool_entry_t;

vt_pool_t* vt_pool_new();
void vt_pool_destroy(vt_pool_t *pl);
void* vt_palloc(vt_pool_t *pl, size_t size, vt_dtor_cb_t dtor);

void *vt_malloc(size_t size);
void vt_free(void *mem);

#endif
