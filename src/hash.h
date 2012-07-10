#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

struct vt_str;
struct vt_pool;

typedef int (*vt_keyval_cb_t)(struct vt_str *key, struct vt_str *val);

#define VT_HASH_NBUCKETS_DEFAULT 83

// this should be replaced with a random number 
// generated when the program is started running
// to avoid the hash collision attack.
#define VT_HASH_SEED 0x9221

typedef struct vt_hash_elm {
    struct vt_str       *key;
    struct vt_str       *val;
    struct vt_hash_elm  *next;
} vt_hash_elm_t;

typedef struct vt_hash {
    struct vt_hash_elm     *buckets;
    size_t                  nbuckets;
    size_t                  nelms;
} vt_hash_t;

int  vt_hash_init(vt_hash_t *hash);
int  vt_hash_init2(vt_hash_t *hash, size_t nbuckets);
void vt_hash_destroy(vt_hash_t *hash);
void vt_hash_foreach(vt_hash_t *hash, vt_keyval_cb_t cb);

struct vt_hash_elm* vt_hash_find_elm(struct vt_hash *hash, struct vt_str *key);
struct vt_hash_elm* vt_hash_insert(struct vt_hash *hash, struct vt_str *key, struct vt_str *val);
int                 vt_hash_remove(struct vt_hash *hash, struct vt_str *key);

unsigned int murmur_hash2(const void * key, int len, unsigned int seed);

#endif
