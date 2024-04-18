#include "Server.hpp"

void    newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan);
void    clearTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &chan);

void    Server::topicCommand(std::vector<std::string> cmd, Client *client)
{
    int cmdSize = cmd.size();
    int fd = client->getClientFd(); // get client's fd for further use
    std::string clientNick = client->getNickName(); // get client's nickname for further use

    /* ERR_NEEDMOREPARAMS (461)  */
    if (cmdSize < 2) {
        sendMessage(fd, ERR_NEEDMOREPARAMS(clientNick, cmd[0]));
        return ;
    }

    /* Not a channel name */

    if (cmd[1][0] != '#' && cmd[1][0] != '&') {
        return ;
    }

    /* Arg is channel name (starting with # or &), check if exist */

    channelsMap channels = this->_channelsMap; // access Server's channelsMap
    channelsMap::iterator chanIt = channels.find(cmd[1]); // create iterator for channelsMap

    /* ERR_NOSUCHCHANNEL (403) */
    if (chanIt == channels.end()) {
        sendMessage(fd, ERR_NOSUCHCHANNEL(clientNick, cmd[1]));
        return ;
    }

    /* ↓ Channel exists ↓ */

    clientsMap chanClients = *(chanIt->second->getClientsList()); // access the Channel's clientsDataMap
    clientsMapIterator chanClientsIt = chanClients.find(clientNick); // create iterator that points to current client in channel
	Channel* channel = getChannelByName(cmd[1]);

    /* ERR_NOTONCHANNEL (442) (this error should only occur when new topic is provided) */
    if (chanClientsIt == chanClients.end() && cmdSize > 2) {
        sendMessage(fd, ERR_NOTONCHANNEL(clientNick, cmd[1]));
        return ;
    }

    bool isOp = !channel->getIsTopicOperatorMode() || chanClientsIt->second->getIsOperator();

    /* TOPIC with args, set channel's new topic */
    if (cmdSize > 2) {
        if (cmd[2][0] != ':') {
            sendMessage(fd, TOPIC_USAGE);
            return ;
        }
        cmd[2] = cmd[2].substr(1);
        if (isOp) {
            chanIt->second->setTopicInfo(cmd[2], clientNick);
            if (cmd[2].empty()) {
                clearTopicBroadcast(chanClients, clientNick, cmd[1]);
                return ;
            }
            newTopicBroadcast(chanClients, clientNick, cmd[2], cmd[1]);
            return ;
        }
        sendMessage(fd, ERR_CHANOPRIVSNEEDED(clientNick, cmd[1]));
        return ;
    }

    /* TOPIC with no args, view topic of the channel */
    const std::string &topic = chanIt->second->getTopic();

    if (cmdSize == 2) {
        if (topic.empty()) {
            /*  RPL_NOTOPIC (331)  */
            sendMessage(fd, RPL_NOTOPIC(cmd[1]));
        } else {
            /* RPL_TOPIC (332) */
            sendMessage(fd, RPL_TOPIC(cmd[1], topic));
            /* RPL_TOPICWHOTIME (333) */
            sendMessage(fd, RPL_TOPICWHOTIME(cmd[1], chanIt->second->topicAuth, chanIt->second->topicTime));
        }
    }
}

void    newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan)
{
    for (clientsMapIterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, TOPIC_SET(nick, chan, topic));
    }
}

void    clearTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &chan)
{
     for (clientsMapIterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, TOPIC_CLEAR(nick, chan));
    }
}