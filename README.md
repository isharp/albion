#albion - A simple, lightweight IRC bot written in C.

####requires libcurl and libxml2

BUILD: 'make'

USAGE: 
`./albion [#][#]<server>:<port> <nick> <channel>`

`(#<server> for SSL.)`

`(##<server> for no-verify SSL.)`

Depending on your shell configuration,
you may need to escape the '#' characters.

Sample invocation:
`./albion ##irc.myserver.net:6697 albion #mychannel`


The bot will grab titles of web pages whose URLs appear in the channel and
send the title to the channel.

albion supports the following commands:

.help  -- displays list of commands

.ping  -- replies with 'pong'

.s -- allows for sed-like substitutions to recent messages, e.g. ".s/match/replace"

.dubs  -- generates a random integer on the interval [0, 99]

.decide -- replies with "Yes." or "No." with equal probability.
