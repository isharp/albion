#ifndef DCC_H
#define DCC_H

#include "libircclient.h"

void dcc_recv_callback(irc_session_t * session, irc_dcc_t id, int status,
                            void * ctx, const char * data, unsigned int length);

void dcc_file_recv_callback(irc_session_t * session, irc_dcc_t id, int status,
                            void * ctx, const char * data, unsigned int length);
#endif
