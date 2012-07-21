#include "../src/hash.h"
#include "../src/str.h"
#include <stdio.h>
#include <assert.h>

int each_kv(vt_str_t *key, vt_str_t *val){
    printf("%s => %s\n", key->str, val->str);
    return 0;
}

int main(int argc, char **argv){
    vt_hash_t hash;
    vt_hash_elm_t *elm;
    vt_str_t k1 = vt_string("a");
    vt_str_t v1 = vt_string("1");
    vt_str_t k2 = vt_string("b");
    vt_str_t v2 = vt_string("2");
    int r;

    vt_hash_init(&hash);
    vt_hash_insert(&hash, &k1, &v1);
    vt_hash_foreach(&hash, &each_kv);
    printf("\n");
    //
    vt_hash_insert(&hash, &k2, &v2);
    r = vt_hash_foreach(&hash, &each_kv);
    printf("r=%d\n", r);
    assert(r == 2);
    //
    elm = vt_hash_find_elm(&hash, &k1);
    assert(elm!=NULL);
    //
    printf("k1: %s result: %s\n", k1.str, elm->val->str);
    r = vt_hash_remove(&hash, &k1);
    printf("removed k1: %s:\n", k1.str);
    r = vt_hash_foreach(&hash, &each_kv);
    assert(r == 1);
    return 0;
}
