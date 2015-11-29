#include "albion_init.h"
#include "event.h"
#include "libircclient.h"
#include "string.h"

void set_default_callbacks(irc_callbacks_t *callbacks)
{
    memset(callbacks, 0, sizeof(*callbacks));

    callbacks->event_connect = event_connect;
    callbacks->event_join = event_join;
    callbacks->event_nick = dump_event;
    callbacks->event_quit = dump_event;
    callbacks->event_part = dump_event;
    callbacks->event_mode = dump_event;
    callbacks->event_topic = dump_event;
    callbacks->event_kick = dump_event;
    callbacks->event_channel = event_channel;
    callbacks->event_privmsg = event_privmsg;
    callbacks->event_notice = dump_event;
    callbacks->event_invite = dump_event;
    callbacks->event_umode = dump_event;
    callbacks->event_ctcp_rep = dump_event;
    callbacks->event_ctcp_action = dump_event;
    callbacks->event_unknown = dump_event;
    callbacks->event_numeric = event_numeric;

    callbacks->event_dcc_chat_req = NULL;
    callbacks->event_dcc_send_req = NULL;
}
