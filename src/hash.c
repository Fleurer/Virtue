#include <assert.h>
#include <string.h>
#include "hash.h"

vt_hash_t* vt_hash_new(struct vt_pool *pl, size_t nbuckets) {
    vt_hash_t *hash;

    hash = (vt_hash_t*)vt_palloc(pl, sizeof(vt_hash_t), (vt_dtor_cb_t)&vt_hash_destroy);
    assert(hash != NULL);
    hash->nelms = 0;
    hash->nbuckets = nbuckets;
    hash->buckets = (vt_hash_elm_t*)vt_malloc(sizeof(vt_hash_elm_t) * nbuckets);
    memset(hash->buckets, 0, sizeof(vt_hash_elm_t) * nbuckets);
    return hash;
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

struct vt_hash_elm* vt_hash_find_elm(struct vt_hash *hash, struct vt_str *key){
    unsigned int hval;
    vt_hash_elm_t *helm;

    assert(key != NULL);
    assert(key->str != NULL);
    //
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    helm = &hash->buckets[ hval % hash->nbuckets ];
    while (helm->next != NULL) {
        if (strcmp(key->str, helm->str)) {
            return helm;
        }
        //
        helm = helm->next;
    }
    return NULL;
}

struct vt_hash_elm* vt_hash_insert(vt_hash_t *hash, struct vt_str *key, struct vt_str *val) {
    unsigned int hval;
    vt_hash_elm_t *bkt, *elm;

    elm = vt_hash_find_elm(hash, key);
    if (elm) {
        elm->val = val;
        return elm;
    }
    // 
    elm = (vt_hash_elm_t*)vt_malloc(sizeof(vt_hash_elm_t));
    // prepend elm into the bucket's list
    hval = murmur_hash2(key->str, key->size, VT_HASH_SEED);
    bkt = &hash->buckets[ hval % hash->nbuckets ];
    elm->next = bkt->next;
    bkt->next = elm;
    return elm;
}

// find and remove the elm from the singly-linked list.
int vt_hash_remove(struct vt_hash *hash, struct vt_str *key) {
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

