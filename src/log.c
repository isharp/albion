#include <stdarg.h>
#include <stdio.h>

void addlog(const char * fmt, ...)
{
    FILE * fp;
    char buf[1024];
    va_list va_alist;

    va_start(va_alist, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va_alist);
    va_end(va_alist);

    printf("%s\n", buf);

    if ((fp = fopen("albion.log", "ab")) != 0) {
        fprintf(fp, "%s\n", buf);
        fclose(fp);
    }
}
