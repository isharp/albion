/* Extract URLs from a stream of characters - invoke as ./extract_url < urls.txt
 *
 * This program needs to be adapted to become a library function that doesn't
 * use stdin or stdout */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// states
#define S_h (1)
#define S_t1 (2)
#define S_t2 (3)
#define S_p (4)
#define S_s (5)
#define S_col (6)

/* Lower-case alpha only. tolower() will be used
 * when searching for legal characters. */
char *legal_chars =
    "abcdefghijklmnopqrstuvwxyz0123456789"
    "./\\~#%&amp;()_-+=;?";

int state;

void print_urls(char *s);

int main(int argc, char **argv) {
    char buff[1024];
    while(fgets(buff, 1023, stdin) != NULL) {
        print_urls(buff);
    }
}

void print_urls(char *s) {
    char *p, *mark;
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
                    while(mark < p) {
                        fputc(*mark, stdout);
                        mark++;
                    }
                    fputc('\n', stdout);
                    state = 0;
                    p--; // backtrack
                }
        }
    }
    if(state) {
        while(mark < p) {
            fputc(*mark, stdout);
            mark++;
        }
    }
}
