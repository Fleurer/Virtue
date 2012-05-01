#ifndef POOL_H
#define POOL_H

#include <stddef.h>

/* A quick and dirty memory pool implementation, just for making 
 * memory deallocating easier yet, it should be replaced with a 
 * chunk allocating mechanism later.
 * */

typedef struct vt_pool {
    STAILQ_HEAD(, vt_pool_entry) entries;
} vt_pool_t;

typedef struct vt_pool_entry {
    STAILQ_HEAD(vt_pool_entry) entry;
    size_t size;
    void *mem;
} vt_pool_entry_t;

vt_pool_t* vt_pool_new();
void* vt_pool_free(vt_pool_t *pl);
void* vt_palloc(vt_pool_t *pl, size_t size);

#endif
