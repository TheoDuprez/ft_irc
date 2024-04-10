#include "Server.hpp"

void    Server::topicCommand(std::vector<std::string> cmd, Client *client)
{
    // --- Debug message ---
    std::cout << std::endl;
    std::cout << " ----- Input of topicCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;

    int cmdSize = cmd.size();

    /* Not a channel name */
    if (cmd[1][0] != '#' && cmd[1][0] != '&') {
        return ;
    }

    int fd = client->getClientFd(); // get client's fd for further use
    channelsMap channels = this->_channelsList; // access Server's channelsMap
    channelsMap::iterator chanIt = channels.find(cmd[1]); // create iterator for channelsMap

    /* ERR_NOSUCHCHANNEL (403) */
    if (chanIt == channels.end()) {
        sendMessage(fd, ":server 403 * " + cmd[1] + " :No such channel");
        this->printLogMessage("ERR_NOSUCHCHANNEL (403)\n", ERROR);
        return ;
    }

    /* ↓ Channel exists ↓ */

    std::string clientNick = client->getNickName();
    clientsListMap chanClients = *(chanIt->second->getClientsList()); // access the Channel's clientsMap
    clientsListMapIterator chanClientsIt = chanClients.find(clientNick); // create iterator for clientsMap and find client using its nickname
    
    /* ERR_NOTONCHANNEL (442) (this error should only occur when new topic is provided) */
    if (chanClientsIt == chanClients.end() && cmdSize > 2) {
        sendMessage(fd, ":server 442 * " + cmd[1] + " :You're not on that channel");
        this->printLogMessage("ERR_NOSUCHCHANNEL (403)\n", ERROR);
        return ;
    }
   
}