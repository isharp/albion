#include <stdio.h>

#include "libircclient.h"

void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status,
                            void * ctx, const char * data, unsigned int length)
{
    static int count = 0;
    char buf[11];

    switch (status)
    {
    case LIBIRC_ERR_CLOSED:
        printf("DCC %d: chat closed\n", id);
        break;

    case -1:
        if (!data) {
            printf("DCC %d: chat connected\n", id);
            irc_dcc_msg(session, id, "Hehe");
        } else {
            printf("DCC %d: %s\n", id, data);
            sprintf(buf, "DCC [%d]: %d", id, count++);
            irc_dcc_msg(session, id, buf);
        }
        break;

    default:
        printf("DCC %d: error %s\n", id, irc_strerror(status));
        break;
    }
}

void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status,
                            void * ctx, const char * data, unsigned int length)
{
    if (status == -1 && length == 0) {
        printf("File sent successfully\n");

        if (ctx)
            fclose((FILE*) ctx);
    } else if (status) {
        printf("File sent error: %d\n", status);
        if (ctx)
            fclose((FILE*) ctx);
    } else {
        if (ctx)
            fwrite(data, 0, length, (FILE*) ctx);
        printf("File sent progress: %d\n", length);
    }
}
