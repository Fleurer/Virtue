#ifndef STRING_H
#define STRING_H

typedef vt_string {
    char *str;
    unsigned long size;
} vt_string_t;

#define vt_string(str) { str, sizeof(str) }

#endif
