#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "url.h"

// states
#define S_h (1)
#define S_t1 (2)
#define S_t2 (3)
#define S_p (4)
#define S_s (5)
#define S_col (6)

/* Accepts a c-string s and returns a c-string containing the first url in it.
 * The state machine will end early if a 0x00 byte is encountered in the middle
 * string pointed to by the parameter s, but RFC 2812 explicitly says that NUL
 * chars are not allowed in IRC messages.
 */
char *get_url(const char *s) {
    int state;
    int length;
    const char *p, *mark;
    char *url;;
    char *legal_chars =
        "abcdefghijklmnopqrstuvwxyz0123456789"
        "./\\~#%&amp;()_-+=;?";
    state = 0;
    for(p = s; *p ;p++) {
        switch(state) {
            case 0:
                if(*p == 'h') {
                    state = S_h;
                    mark = p;
                }
                break;
            case S_h:
                if(*p == 't')
                    state = S_t1;
                else
                    state = 0;
                break;
            case S_t1:
                if(*p == 't')
                    state = S_t2;
                else
                    state = 0;
                break;
            case S_t2:
                if(*p == 'p')
                    state = S_p;
                else
                    state = 0;
                break;
            case S_p:
                if(*p == ':')
                    state = S_col;
                else
                    if(*p == 's')
                        state = S_s;
                    else
                        state = 0;
                break;
            case S_s:
                if(*p == ':')
                    state = S_col;
                else
                    state = 0;
                break;
            case S_col:
                if(!strchr(legal_chars,tolower(*p))) {
                    length = p - mark;
                    url = calloc(length + 1, 1);
                    memcpy(url, mark, length);
                    url[length] = '\0';
                    return url;
                }
        }
    }
    if(state == S_col) {
        length = p - mark;
        url = calloc(length, 1);
        memcpy(url, mark, length);
        return url;
    }
    return NULL;
}
