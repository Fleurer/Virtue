#include "../src/hash.h"
#include "../src/str.h"
#include <stdio.h>

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

    vt_hash_init(&hash);
    vt_hash_insert(&hash, &k1, &v1);
    vt_hash_foreach(&hash, &each_kv);
    printf("\n");
    //
    vt_hash_insert(&hash, &k2, &v2);
    vt_hash_foreach(&hash, &each_kv);
    elm = vt_hash_find_elm(&hash, &k1);
    if (!elm) {
        printf("bad result\n");
        return 1;
    }
    printf("k1: %s result: %s", k1.str, elm->val->str);
    return 0;
}
