/* Basic irc client. Capable of ping-pong.
   
   Example invocation:
  
   for SSL:
   ./albion ##irc.rizon.net:6697 albion #albion 
   
   for non-SSL:
   ./albion irc.rizon.net:6666 albion #albion 
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "libircclient.h"
#include "event.h"
#include "albion_init.h"

int main(int argc, char **argv)
{
    irc_callbacks_t callbacks;
    irc_ctx_t ctx;
    irc_session_t * s;
    unsigned short port = 6667;

    if (argc != 4) {
        printf("Usage: %s [#][#]<server> <nick> <channel>\n"
                            "(#<server> for SSL.)\n"
                            "(##<server> for no-verify SSL.)\n\n"
                            "Depending on your shell configuration,\n"
                            "you may need to escape the '#' characters.\n\n"
                            "Sample invocation:\n"
                            "./albion ##irc.myserver.net:6697 albion"
                            " #mychannel\n\n", argv[0]);
        return 1;
    }

    set_default_callbacks(&callbacks);
    s = irc_create_session(&callbacks);

    if (!s) {
        printf("Could not create session\n");
        return 1;
    }

    ctx.channel = argv[3];
    ctx.nick = argv[2];

    irc_set_ctx(s, &ctx);

    /* If the port number is specified in the server string, 
     * use the port 0 so it gets parsed */
    if (strchr( argv[1], ':' ) != 0)
        port = 0;

    /* To handle the "SSL certificate verify failed" from command line we allow
     * passing ## in front of the server name, and in this case tell 
     * libircclient not to verify the cert */
    if (argv[1][0] == '#' && argv[1][1] == '#') {
        /* Skip the first character as libircclient needs only one # for SSL 
         * support, i.e. #irc.freenode.net */
        argv[1]++;
        
        irc_option_set(s, LIBIRC_OPTION_SSL_NO_VERIFY);
    }
    
    srand(time(NULL));

    // Initiate the IRC server connection
    if (irc_connect(s, argv[1], port, 0, argv[2], 0, 0)) {
        printf("Could not connect: %s\n", irc_strerror(irc_errno(s)));
        return 1;
    }

    // and run into forever loop, generating events
    if (irc_run (s)) {
        printf("Could not connect or I/O error: %s\n", 
                            irc_strerror(irc_errno(s)));
        return 1;
    }

    return 1;
}
