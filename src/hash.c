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
    hash->buckets = (vt_hash_elm_t*)vt_malloc(sizeof(vt_hash_elm_t) * nbuckets);
    memset(hash->buckets, 0, sizeof(vt_hash_elm_t) * nbuckets);
    return 0;
}

void vt_hash_destroy(vt_hash_t *hash) {
    int i;
    vt_hash_elm_t *bkt, *nelm, *elm;

    for (i=0; i<hash->nbuckets; i++) {
        bkt = &hash->buckets[i];
        elm = bkt->next;
        while (elm) {
            nelm = elm->next;
            assert(elm != NULL);
            vt_free(elm);
            elm = nelm;
        }
    }
    vt_free(hash->buckets);
}

/* -------------------------------------------------------------- */

vt_hash_elm_t* vt_hash_find_elm(vt_hash_t *hash, vt_str_t *key){
    unsigned int hval;
    vt_hash_elm_t *elm;

    assert(key != NULL);
    assert(key->str != NULL);
    //
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    elm = hash->buckets[ hval % hash->nbuckets ].next;
    while (elm != NULL) {
        assert(elm->key != NULL);
        if (elm->key && (0==strcmp(key->str, elm->key->str))) {
            return elm;
        }
        elm = elm->next;
    }
    return NULL;
}

vt_hash_elm_t* vt_hash_insert(vt_hash_t *hash, vt_str_t *key, vt_str_t *val) {
    unsigned int hval;
    vt_hash_elm_t *bkt, *elm;

    elm = vt_hash_find_elm(hash, key);
    if (elm) {
        elm->val = val;
        return elm;
    }
    // 
    elm = (vt_hash_elm_t*)vt_malloc(sizeof(vt_hash_elm_t));
    elm->key = key;
    elm->val = val;
    // prepend elm into the bucket's list
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    bkt = &hash->buckets[ hval % hash->nbuckets ];
    elm->next = bkt->next;
    bkt->next = elm;
    return elm;
}

// find and remove the elm from the singly-linked list.
int vt_hash_remove(vt_hash_t *hash, vt_str_t *key) {
    vt_hash_elm_t *elm, *nelm;

    elm = vt_hash_find_elm(hash, key);
    if (elm == NULL) {
        return -1;
    }
    nelm = elm->next;
    if (nelm == NULL) {
        vt_free(elm);
        return 0;
    }
    memcpy(elm, nelm, sizeof(vt_hash_elm_t));
    vt_free(nelm);
    return 0;
}

// tranverse each key value in the hash table
void vt_hash_foreach(vt_hash_t *hash, vt_keyval_cb_t cb) {
    int i, r;
    vt_hash_elm_t *bkt, *nelm, *elm;

    for (i=0; i<hash->nbuckets; i++) {
        bkt = &hash->buckets[i];
        elm = bkt->next;
        while (elm) {
            nelm = elm->next;
            if (cb) {
                r = cb(elm->key, elm->val);
                if (r < 0)
                    return;
            }
            elm = nelm;
        }
    }
}
