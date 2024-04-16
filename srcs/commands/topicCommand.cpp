#include "Server.hpp"

void    newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan);

void    Server::topicCommand(std::vector<std::string> cmd, Client *client)
{
    // --- Debug message ---
    std::cout << std::endl;
    std::cout << " ----- Input of topicCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;

    int cmdSize = cmd.size();
    int fd = client->getClientFd(); // get client's fd for further use

    /* ERR_NEEDMOREPARAMS (461)  */
    if (cmdSize < 2) {
        sendMessage(fd, ":server 461 * " + cmd[0] + " :Not enough parameters");
        this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
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
        sendMessage(fd, ":server 403 * " + cmd[1] + " :No such channel");
        this->printLogMessage("ERR_NOSUCHCHANNEL (403)\n", ERROR);
        return ;
    }

    /* ↓ Channel exists ↓ */

    std::string clientNick = client->getNickName(); // get client's nickname for further use
    clientsMap chanClients = *(chanIt->second->getClientsList()); // access the Channel's clientsDataMap
    clientsMapIterator chanClientsIt = chanClients.find(clientNick); // create iterator that points to current client in channel

    /* ERR_NOTONCHANNEL (442) (this error should only occur when new topic is provided) */
    if (chanClientsIt == chanClients.end() && cmdSize == 3) {
        sendMessage(fd, ":server 442 * " + cmd[1] + " :You're not on that channel");
        this->printLogMessage("ERR_NOTONCHANNEL (442)\n", ERROR);
        return ;
    }

    bool isOp = chanClientsIt->second->getIsOperator();

    /* TOPIC with args, set channel's new topic */
    if (cmdSize == 3) {
        if (isOp && cmd[2].empty()) {
            /* Clear topic and reply with 331 */
            chanIt->second->setTopic("");
            chanIt->second->topicTime = "";
            chanIt->second->topicAuth = "";
            sendMessage(fd, ":server 331 * " + cmd[1] + " :No topic is set");
            return ;
        }
        if (isOp) {
            /* If operator change topic and reply with 332 */
            std::string newTopic = cmd[2].substr(1);
            time_t timestamp = time(NULL);
            chanIt->second->setTopic(newTopic);
            chanIt->second->topicTime = SSTR(timestamp);
            chanIt->second->topicAuth = clientNick;
            newTopicBroadcast(chanClients, clientNick, newTopic, cmd[1]);
            return ;
        }
        sendMessage(fd, ":server 482 * " + cmd[1] + " :You're not channel operator");
        this->printLogMessage("ERR_CHANOPRIVSNEEDED (482)\n", ERROR);
        return ;
    }

    /* TOPIC with no args, view topic of the channel */
    std::string topic = chanIt->second->getTopic();

    if (cmdSize == 2) {
        if (topic.empty()) {
            /*  RPL_NOTOPIC (331)  */
            sendMessage(fd, ":server 331 * " + cmd[1] + " :No topic is set");
        } else {
            /* RPL_TOPIC (332) */
            sendMessage(fd, ":server 332 * " + cmd[1] + " :" + topic);
            /* RPL_TOPICWHOTIME (333) */
            sendMessage(fd, ":server 333 * " + cmd[1] + " " + chanIt->second->topicAuth + " " + chanIt->second->topicTime);
        }
    }
}

void    newTopicBroadcast(clientsMap &clientsDataMap, const std::string &nick, const std::string &topic, const std::string &chan)
{
    for (clientsMapIterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, ":" + nick + " TOPIC " + chan + " :" + topic);
    }
}
