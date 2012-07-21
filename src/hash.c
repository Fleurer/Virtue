#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "str.h"
#include "pool.h"
#include "hash.h"

int vt_hash_init(vt_hash_t *hash){
    return vt_hash_init2(hash, VT_HASH_NBUCKETS_DEFAULT);
}

int vt_hash_init2(vt_hash_t *hash, size_t nbuckets) {
    assert(hash != NULL);
    hash->nelms = 0;
    hash->nbuckets = nbuckets;
    hash->buckets = (vt_hash_bucket_t*)vt_malloc(sizeof(vt_hash_bucket_t) * nbuckets);
    memset(hash->buckets, 0, sizeof(vt_hash_bucket_t) * nbuckets);
    return 0;
}

void vt_hash_destroy(vt_hash_t *hash) {
    int i;
    vt_hash_bucket_t *bkt;
    vt_hash_elm_t *elm;

    for (i=0; i<hash->nbuckets; i++) {
        bkt = &hash->buckets[i];
        elm = LIST_FIRST(bkt);
        for (; elm; elm = LIST_NEXT(elm, entry)) {
            assert(elm != NULL);
            vt_free(elm);
        }
    }
    vt_free(hash->buckets);
}

/* -------------------------------------------------------------- */

vt_hash_elm_t* vt_hash_find_elm(vt_hash_t *hash, vt_str_t *key){
    unsigned int hval;
    vt_hash_elm_t *elm;
    vt_hash_bucket_t *bkt;

    assert(key != NULL);
    assert(key->str != NULL);
    //
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    bkt = &hash->buckets[ hval % hash->nbuckets ];
    LIST_FOREACH(elm, bkt, entry) {
        assert(elm->key != NULL);
        if ((hval == elm->hash) && elm->key && (0==strcmp(key->str, elm->key->str))) {
            return elm;
        }
    }
    return NULL;
}

vt_hash_elm_t* vt_hash_insert(vt_hash_t *hash, vt_str_t *key, vt_str_t *val) {
    unsigned int hval;
    vt_hash_bucket_t *bkt;
    vt_hash_elm_t *elm;

    elm = vt_hash_find_elm(hash, key);
    if (elm) {
        elm->val = val;
        return elm;
    }
    // 
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    elm = (vt_hash_elm_t*)vt_malloc(sizeof(vt_hash_elm_t));
    elm->key = key;
    elm->val = val;
    elm->hash = hval;
    // prepend elm into the bucket's list
    bkt = &hash->buckets[ hval % hash->nbuckets ];
    LIST_INSERT_HEAD(bkt, elm, entry);
    return elm;
}

// find and remove the elm from the singly-linked list.
int vt_hash_remove(vt_hash_t *hash, vt_str_t *key) {
    vt_hash_elm_t *elm;

    elm = vt_hash_find_elm(hash, key);
    if (elm == NULL) {
        return -1;
    }
    LIST_REMOVE(elm, entry);
    vt_free(elm);
    return 0;
}

// tranverse each key value in the hash table
// returns the number of iterations.
size_t vt_hash_foreach(vt_hash_t *hash, vt_keyval_cb_t cb) {
    int i, r, cnt = 0;
    vt_hash_elm_t *elm;
    vt_hash_bucket_t *bkt;

    for (i=0; i<hash->nbuckets; i++) {
        bkt = &hash->buckets[i];
        LIST_FOREACH(elm, bkt, entry) {
            if (cb) {
                cnt++;
                r = cb(elm->key, elm->val);
                if (r < 0)
                    return cnt;
            }
        }
    }
    return cnt;
}
