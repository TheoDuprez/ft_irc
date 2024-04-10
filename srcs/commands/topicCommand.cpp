#include "Server.hpp"

void    Server::topicCommand(std::vector<std::string> cmd, int fd)
{
    (void)fd;
    // --- Debug message ---
    std::cout << " ----- Input of topicCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // if (cmd[1][0] != '#') {
    //     std::cout << "not a channel" << std::endl;
    //     this->printLogMessage("ERR_ERRONEUSNICKNAME (432)\n", ERROR);
    // }
    for (channelsMap::iterator i = this->_channelsList.begin(); i != this->_channelsList.end(); i++)
        std::cout << i->second->getChannelName() << std::endl;
}