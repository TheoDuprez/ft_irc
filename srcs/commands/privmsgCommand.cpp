#include "Server.hpp"

void                Server::privmsgCommand(commandTokensVector cmd, Client* client)
{
    if (cmd.at(1).find("#") != std::string::npos || cmd.at(1).find("&")) { // Is a channel target
        this->_channelsList.find(cmd.at(1))->second->privmsg(cmd, client);
    }
}