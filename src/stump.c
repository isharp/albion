#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libircclient.h"
#include "stump.h"

static char stump_msg[4096];

void stump(irc_session_t *session, const char **params)
{
    char *line;
    char *nick;
    int n;
    const char *s[20];

    line = strdup(params[1]);

    s[0] = "I don't even want to talk about %s. Just look at his numbers. He's "
            "a very low-energy person.";
    s[1] = "People come to me and tell me, they say, \"Donald, we like you, but"
           " there's something weird about %s.\" It's a very serious problem.";
    s[2] = "We have incompetent people, they are destroying this country, and "
           "%s doesn't have what we need to make it great again.";
    s[3] = "Nobody likes %s, nobody in Congress likes %s, nobody likes %s anywh"
           "ere once they get to know him.";
    s[5] = "Look, here's the thing about %s. We're losing in all of our deals, "
            "we're losing to Mexico, we're losing with China, and I'm sure ther"
            "e are some good ones, but %s has to go back.";
    s[6] = "What are they saying? Are those %s people? Get 'em outta here! Get "
            "'em out! Confiscate their coats!";
    s[7] = "Donald J. Trump is calling for a total and complete shutdown of %s "
            "entering the United States.";
    s[8] = "Did you read about %s? No more \"Merry Christmas\" at %s's house. N"
            "o more. Maybe we should boycott %s.";
    s[9] = "Look at that face! Would anyone vote for that? Can you imagine that"
            ", %s, the face of our next president?";
    s[10] = "We have to have a wall. We have to have a border. And in that wall"
            " we're going to have a big fat door where people can come into the"
            " country, but they have to come in legally and those like %s who a"
            "re here illegally will have to go back.";
    s[11] = "%s, you haven't been called, go back to Univision.";
    s[12] = "%s? You could see there was blood coming out of %s's eyes. Blood c"
            "oming out of %s's... wherever.";
    s[13] = "%s is not a war hero. He's a war hero because he was captured? I l"
            "ike people who weren't captured.";
    s[14] = "When Mexico sends its people, they're not sending the best. They'r"
            "e sending people like %s that have lots of problems and they're br"
            "inging those problems. They're bringing drugs, they're bringing cr"
            "ime. They're rapists and some, I assume, are good people, but I sp"
            "eak to border guards and they're telling us what we're getting.";
    s[15] = "I thought that was disgusting. That showed such weakness, the way "
            "%s was taken away by two young women, the microphone; they just to"
            "ok the whole place over. That will never happen with me. I don't "
            "know if I'll do the fighting myself or if other people will, but t"
            "hat was a disgrace. I felt badly for %s. But it showed that he's w"
            "eak.";
    s[16] = "%s is an enigma to me. He said that he's \"pathological\" and that"
            " he's got, basically, pathological disease... I don't want a perso"
            "n that's got pathological disease.";
    s[17] = "The concept of global warming was created by and for %s in order t"
            "o make U.S. manufacturing non-competitive.";
    s[18] = "The U.S. will invite %s, the Mexican criminal who just escaped pri"
            "son, to become a U.S. citizen because our \"leaders\" can't say no"
            "!";
    s[19] = "You want to know what will happen? The wall will go up and %s will"
            " start behaving.";
    s[20] = "Our great African American President hasn't exactly had a positive"
            "impact on the thugs like %s who are so happily and openly destroyi"
            "ng Baltimore!";

    strtok(line, " ");
    nick = strtok(NULL, " ");
    strtok(NULL, " ");
    if (!nick) {
        free(line);
        return;
    }
    if (!strcmp(nick, "trump") || !strcmp(nick, "Trump") 
                               || !strcmp(nick, "TRUMP")) {
        irc_cmd_msg(session, params[0], "You can't stump the Trump.");
        free(line);
        return;
    }
    n = rand() % 21;
    sprintf(stump_msg, s[n], nick, nick, nick);
    irc_cmd_msg(session, params[0], stump_msg);
    free(line);
}
