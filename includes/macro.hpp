
// ERR_RPL
#define ERR_NEEDMOREPARAMS(NICKNAME, COMMAND) ":server 461 " + NICKNAME + " " + COMMAND + " :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED( CLIENT, CHANNEL ) ":server 482 " + CLIENT + " " + CHANNEL + " :You're not channel operator"
#define RPL_WELCOME( CLIENT, USERNAME) ":server 001 " + CLIENT + " :Welcome to the localhost Network, " + CLIENT + "!" + USERNAME + "@localhost"
#define RPLY_NEWNICKMESSAGE( PASTNICK, NEWNICK ) ":" + PASTNICK + " NICK " + NEWNICK
#define ERR_ERRONEUSNICKNAME( CLIENT, NICKNAME ) ":server 432 * " + CLIENT + " " + NICKNAME + " :Erroneus nickname"
#define ERR_NICKNAMEINUSE( CLIENT, NICKNAME ) ":server 433 * " + CLIENT + " " + NICKNAME + " :Nickname is already in use"
#define ERR_NONICKNAMEGIVEN( CLIENT ) ":server 431 * "  + CLIENT + " :No nickname given"
#define ERR_ALREADYREGISTERED( CLIENT ) ":server 462 * " + CLIENT + " :You may not reregister"
#define ERR_PASSWDMISMATCH( CLIENT ) ":server 464 * " + CLIENT + " :Password incorrect"
#define ERR_INVALIDUSERCHAR( NAME )  "USER: \"" + NAME + "\" invalid character"
#define ERR_NOSUCHCHANNEL( CLIENT, CHANNEL ) ":server 403 " + CLIENT + " " + CHANNEL + " :No such channel"
#define ERR_NOTONCHANNEL( CLIENT, CHANNEL ) ":server 442 " + CLIENT + " " + CHANNEL + " :You're not on that channel"
#define ERR_USERNOTINCHANNEL( CLIENT, USER, CHANNEL ) ":server 441 " + CLIENT + " " + CHANNEL + " " + USER + " :They aren't on that channel"

// RPL
#define RPL_CHANNELMODEIS(NICKNAME, CHANNELPTR ) ":server 324 " + NICKNAME + " " + CHANNELPTR->getChannelName() + " " + CHANNELPTR->createModesString()

// MODE
#define MODE_MESSAGE_ADD_KEY ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +k " + *argumentsIt
#define MODE_MESSAGE_REMOVE_KEY ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -k "
#define MODE_MESSAGE_ADD_USERS_LIMITS ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +l " + *argumentsIt
#define MODE_MESSAGE_REMOVE_USERS_LIMITS ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -l "
#define MODE_MESSAGE_ADD_OPERATOR ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +o " + *argumentsIt
#define MODE_MESSAGE_REMOVE_OPERATOR ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -o " + *argumentsIt

// KICK
#define KICK_MESSAGE_OPS( USER, CHANNEL, MESSAGE ) currentClient->getClientFd(), ":" + currentClient->getNickName() + " KICK " + CHANNEL + " " + USER + " " + MESSAGE
#define KICK_MESSAGE_USERS( USER, CHANNEL, MESSAGE ) clientIt->second->getClient()->getClientFd(), ":" + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost KICK " + CHANNEL + " " + USER + " " + MESSAGE
#define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"
