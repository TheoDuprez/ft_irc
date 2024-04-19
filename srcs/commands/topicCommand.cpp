#include "Server.hpp"

static void     newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan);
static void     clearTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &chan);
static void     viewTopic(int fd, const Channel *channel);
static int      changeTopic(int fd, Channel *channel, const std::string &clientNick, std::vector<std::string> &cmd);

void    Server::topicCommand(std::vector<std::string> cmd, Client *client)
{
    int cmdSize = cmd.size();
    int fd = client->getClientFd(); // get client's fd for further use
    std::string clientNick = client->getNickName(); // get client's nickname for further use

    /* ERR_NEEDMOREPARAMS (461)  */
    if (cmdSize < 2) {
        sendMessage(fd, ERR_NEEDMOREPARAMS(clientNick, cmd[0]));
        this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
        return ;
    }

    /* Not a channel name */
    if (cmd[1][0] != '#' && cmd[1][0] != '&')
        return ;

    /* Arg is channel name (starting with # or &), check if exist */
	Channel* channel = getChannelByName(cmd[1]); // get channel pointer for further use

    /* ERR_NOSUCHCHANNEL (403) */
    if (!channel) {
        sendMessage(fd, ERR_NOSUCHCHANNEL(clientNick, cmd[1]));
        this->printLogMessage("ERR_NOSUCHCHANNEL (403)\n", ERROR);
        return ;
    }

    /* ↓ Channel exists ↓ */

    /* TOPIC with no args, view topic of the channel */
    if (cmdSize == 2) {
        viewTopic(fd, channel);
        return ;
    }

    /* TOPIC with args, set channel's new topic */
    clientsMap chanClients = *(channel->getClientsList()); // access the Channel's clientsDataMap
    clientsMapIterator chanClientsIt = chanClients.find(clientNick); // create iterator that points to current client in channel

    /* ERR_NOTONCHANNEL (442) (this error should only occur when new topic is provided) */
    if (chanClientsIt == chanClients.end() && cmdSize > 2) {
        sendMessage(fd, ERR_NOTONCHANNEL(clientNick, cmd[1]));
        this->printLogMessage("ERR_NOTONCHANNEL (442)\n", ERROR);
        return ;
    }

    if (!changeTopic(fd, channel, clientNick, cmd)) {
        sendMessage(fd, ERR_CHANOPRIVSNEEDED(clientNick, cmd[1]));
        this->printLogMessage("ERR_CHANOPRIVSNEEDED (482)\n", ERROR);
    }
    return ;
}

static void    newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan)
{
    for (clientsMapIterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, TOPIC_SET(nick, chan, topic));
    }
}

static void    clearTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &chan)
{
     for (clientsMapIterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, TOPIC_CLEAR(nick, chan));
    }
}

static void    viewTopic(int fd, const Channel *channel)
{
    const std::string &topic = channel->getTopic();
    const std::string &chanName = channel->getChannelName();

   if (topic.empty()) {
        /*  RPL_NOTOPIC (331)  */
        sendMessage(fd, RPL_NOTOPIC(chanName));
    } else {
        /* RPL_TOPIC (332) */
        sendMessage(fd, RPL_TOPIC(chanName, topic));
        /* RPL_TOPICWHOTIME (333) */
        sendMessage(fd, RPL_TOPICWHOTIME(chanName, channel->topicAuth, channel->topicTime));
    }
}

static int    changeTopic(int fd, Channel *channel, const std::string &clientNick, std::vector<std::string> &cmd)
{
    clientsMap chanClients = *(channel->getClientsList()); // access the Channel's clientsDataMap
    clientsMapIterator chanClientsIt = chanClients.find(clientNick); // create iterator that points to current client in channel

    std::string topic = cmd[2];

    bool isOp = !channel->getIsTopicOperatorMode() || chanClientsIt->second->getIsOperator();

     if (topic[0] != ':') {
        sendMessage(fd, TOPIC_USAGE);
        return 1;
    }
    topic = topic.substr(1);
    for (std::vector<std::string>::iterator it = cmd.begin() + 3; it != cmd.end(); it++)
        topic += " " + *it;
    if (topic.length() > TOPICLEN)
        topic = topic.substr(0, 307);
    if (isOp) {
        channel->setTopicInfo(topic, clientNick);
        if (topic.empty()) {
            clearTopicBroadcast(chanClients, clientNick, cmd[1]);
            return 1;
        }
        newTopicBroadcast(chanClients, clientNick, topic, cmd[1]);
        return 1;
    }
    return 0;
}
