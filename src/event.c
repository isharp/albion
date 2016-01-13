#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libircclient.h"
#include "log.h"
#include "event.h"
#include "url.h"
#include "title.h"
#include "sed.h"

void dump_event(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
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


void event_join(irc_session_t *session, const char *event,
                const char *origin, const char **params, unsigned int count)
{
    irc_ctx_t *ctx;
    ctx = irc_get_ctx(session);
    dump_event(session, event, origin, params, count);
    irc_cmd_user_mode(session, "+i");
    if (!strncmp(origin, ctx->nick, strlen(ctx->nick)))
        irc_cmd_msg(session, params[0], "HWÆT! Hell is empty and all the"
                                                        " devils are here.");
}

void event_connect(irc_session_t *session, const char *event,
                const char *origin, const char **params, unsigned int count)
{
    irc_ctx_t *ctx = (irc_ctx_t *) irc_get_ctx (session);
    dump_event(session, event, origin, params, count);
    irc_cmd_join(session, ctx->channel, 0);
}

void event_privmsg(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    dump_event(session, event, origin, params, count);

    printf("'%s' said me (%s): %s\n", 
        origin ? origin : "someone",
        params[0], params[1]);
}

void event_kick(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    dump_event(session, event, origin, params, count);
    irc_cmd_join(session, params[0], 0);

}

void event_channel(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    char *title;
    char *url;
    unsigned int num;
    char num_str[4];
    char sed_return_str[1024];

    if (count != 2)
        return;

    printf("'%s' said in channel %s: %s\n", origin ? origin : "someone",
                                                    params[0], params[1] );
    log_msg(params[1], origin);

    if (!origin)
        return;
    
    url = get_url(params[1]);
    if (url) {
        title = get_title(url);
        if (title) {
            irc_cmd_msg(session, params[0], title);
            free(url);
            free(title);
        }
    }
    if (!strcmp(params[1], ".help")) {
        irc_cmd_msg(session, params[0], ".help, .ping, .s, .dubs, .decide, "
                                                        ".urban frat 3");
    } else if (!strcmp(params[1], ".ping")) {
        irc_cmd_msg(session, params[0], "pong");
    } else if (!strcmp(params[1], ".urban frat 3")) {
        irc_cmd_msg(session, params[0],
                  "a place that breeds retarded fucks (very similar to a colleg"
                  "iate football program). These guys usually care about nothin"
                  "g but getting drunk and raping ignorant women that are dumb "
                  "enough to let it happen. They have attitudes that closely re"
                  "semble that of the ass holes you hated in jr high. I love ho"
                  "w they make new recruits wear dress clothes and act all upst"
                  "anding and important, yet they are the worst piles of trash "
                  "on campus. Most members end u");
    } else if (!strcmp(params[1], ".dubs")) {
        num = rand() % 90 + 10;
        sprintf(num_str, "%u", num);
        irc_cmd_msg(session, params[0], num_str);
    } else if (!strncmp(params[1], ".decide", 7)) {
        num = rand() % 2;
        if (num)
            irc_cmd_msg(session, params[0], "Yes.");
        else
            irc_cmd_msg(session, params[0], "No.");
    } /* else if (!strncmp(params[1], ".s/", 3)) {
        irc_cmd_msg(session, params[0], find_msg(params[1]));
    } */

}

void event_numeric(irc_session_t *session, unsigned int event, 
                const char *origin, const char **params, unsigned int count)
{
    char buf[24];
    sprintf(buf, "%d", event);

    dump_event(session, buf, origin, params, count);
}
