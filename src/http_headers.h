#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include "st.h"

typedef struct vt_http_headers {
    st_table *hash;
} vt_http_headers_t;
    
void vt_http_headers_init(vt_http_headers_t *hh);
void vt_http_headers_set(char *key, char *val);
char* vt_http_headers_get(char *key);

#endif
