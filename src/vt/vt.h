#ifndef VT_H
#define VT_H

#include <map>
#include <cstring>

struct lstr {
    bool operator()(const char *str1, const char *str2){
        return strcmp(str1, str2) < 0;
    }
};

typedef std::map<const char*, char*, lstr> venv_t;

#define vpanic(str) do{ printf("%s:%d %s\n", __FILE__, __LINE__, (str)); exit(1); } while(0)

/* prototypes */
int vloop(int listenfd);

/* includes */
#include "buffer.h"
#include "conn.h"

using namespace std;

#endif 
