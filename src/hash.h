#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

struct vt_str;

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

struct vt_hash* vt_hash_new(struct vt_pool *pl, size_t nbuckets);

struct vt_hash_elm* vt_hash_find_elm(struct vt_hash *hash, struct vt_str *key);
int vt_hash_remove(struct vt_hash *hash, struct vt_str *key);

unsigned int murmur_hash2(const void * key, int len, unsigned int seed);

#endif
