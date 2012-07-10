#ifndef STR_H
#define STR_H

#include <string.h>

typedef vt_str {
    char *str;
    unsigned long size;
} vt_str_t;

#define vt_string(str) { (char*)str, sizeof(str)-1 }


#endif
