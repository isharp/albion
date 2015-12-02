#ifndef EVENT_H
#define EVENT_H

#include "libircclient.h"

typedef struct {
    char *channel;
    char *nick;

} irc_ctx_t;

void dump_event(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void event_channel(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void event_connect(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void event_join(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void event_kick(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void event_numeric(irc_session_t * session, unsigned int event,
                const char * origin, const char ** params, unsigned int count);

void event_privmsg(irc_session_t * session, const char * event,
                const char * origin, const char ** params, unsigned int count);

void irc_event_dcc_chat(irc_session_t * session, const char * nick,
                                            const char * addr, irc_dcc_t dccid);

void irc_event_dcc_send(irc_session_t * session, const char * nick,
                const char * addr, const char * filename, unsigned long size,
                                                            irc_dcc_t dccid);
#endif
