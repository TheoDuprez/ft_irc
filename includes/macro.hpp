
// ERR_RPL
#define ERR_USERNOTINCHANNEL( USER, CHANNEL ) currentClient->getClientFd(), ":server 441 " + currentClient->getNickName() + " " + CHANNEL + " " + USER + " :They aren't on that channel"
#define ERR_NOSUCHCHANNEL(NICKNAME, CHANNEL) ":server 403 " + NICKNAME + " " + CHANNEL + " :No such channel"
#define ERR_NEEDMOREPARAMS(NICKNAME, COMMAND) ":server 461 " + NICKNAME + " " + COMMAND + " :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED( CLIENT, CHANNEL ) ":server 482 " + CLIENT + " " + CHANNEL + " :You're not channel operator"

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
