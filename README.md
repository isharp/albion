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

.urban frat 3  -- a joke command that should probably be ignored by anyone who
isn't in on the joke.
