#include <stdio.h>
#include <string.h>

#include "libircclient.h"
#include "log.h"
#include "event.h"
#include "dcc.h"

void dump_event(irc_session_t * session, const char * event, 
                const char * origin, const char ** params, unsigned int count)
{
    char buf[512];
    int cnt;

    buf[0] = '\0';

    for (cnt = 0; cnt < count; cnt++){
        if (cnt)
            strcat(buf, "|");
        strcat(buf, params[cnt]);
    }
    
    addlog("Event \"%s\", origin: \"%s\", params: %d [%s]", event, 
                                            origin ? origin : "NULL", cnt, buf);
}


void event_join(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count)
{
    dump_event(session, event, origin, params, count);
    irc_cmd_user_mode(session, "+i");
    irc_cmd_msg(session, params[0], "HWÆT! Hell is empty and all the"
                                                        " devils are here.");
}

void event_connect(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count)
{
    irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
    dump_event(session, event, origin, params, count);
    irc_cmd_join(session, ctx->channel, 0);
}

void event_privmsg(irc_session_t * session, const char * event, 
                const char * origin, const char ** params, unsigned int count)
{
    dump_event(session, event, origin, params, count);

    printf("'%s' said me (%s): %s\n", 
        origin ? origin : "someone",
        params[0], params[1]);
}

void event_channel(irc_session_t * session, const char * event, 
                const char * origin, const char ** params, unsigned int count)
{
    char nickbuf[128];

    if (count != 2)
        return;

    printf("'%s' said in channel %s: %s\n", origin ? origin : "someone",
                                                    params[0], params[1] );

    if (!origin)
        return;

    irc_target_get_nick(origin, nickbuf, sizeof(nickbuf));

    if (!strcmp(params[1], ".help")) {
        irc_cmd_msg(session, params[0], ".help, .ping");
    } else if (!strcmp(params[1], ".ping")) {
        irc_cmd_msg(session, params[0], "pong");
    }
}

void event_numeric(irc_session_t * session, unsigned int event, 
                const char * origin, const char ** params, unsigned int count)
{
    char buf[24];
    sprintf(buf, "%d", event);

    dump_event(session, buf, origin, params, count);
}

void irc_event_dcc_chat(irc_session_t * session, const char * nick, 
                                            const char * addr, irc_dcc_t dccid)
{
    printf("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

    irc_dcc_accept(session, dccid, 0, dcc_recv_callback);
}

void irc_event_dcc_send(irc_session_t * session, const char * nick, 
                                    const char * addr, const char * filename, 
                                    unsigned long size, irc_dcc_t dccid)
{
    FILE * fp;
    printf("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", 
                                            dccid, nick, addr, filename, size);

    if ((fp = fopen("file", "wb")) == 0)
        abort();

    irc_dcc_accept(session, dccid, fp, dcc_file_recv_callback);
}
