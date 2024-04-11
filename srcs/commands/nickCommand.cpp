/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:12 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 00:30:15 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd)
{
    std::locale	loc;
    std::string excluded = "#&: ";

    if ((*cmd).size() != 2 || (*cmd)[1].empty()) {
        sendMessage(currentClient->getClientFd(), ERR_NONICKNAMEGIVEN(this->getServerName()));
        return false;
    }
    // Check if nick is already used
    for (clientMap::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (!(*cmd)[1].compare(it->second->getNickName())) {
            sendMessage(currentClient->getClientFd(), ERR_NICKNAMEINUSE(this->getServerName(), (*cmd)[1]));
            return false;
        }
    }
    // Check if nick content is allowed
    for (stringIterator it = (*cmd)[1].begin(); it != (*cmd)[1].end(); it++) {
        if ((it == (*cmd)[1].begin() && std::isdigit(*it, loc)) || (*cmd)[1].find_first_of(excluded) != std::string::npos || !std::isprint(*it, loc)) {
            sendMessage(currentClient->getClientFd(), ERR_ERRONEUSNICKNAME(this->getServerName(), (*cmd)[1]));
            return false;
        }
    }
    return true;
}

void	Server::nickCommand(std::vector<std::string> cmd, int fd)
{
    // --- Debug message ---
    std::cout << " ----- Input of nickCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    Client	*currentClient = this->_clients[fd];

    if (this->_isValidNickCommand(currentClient, &cmd) == false)
        return;
    // Send message to other client if nick is changed
    if (!currentClient->getNickName().empty()) {
        sendMessage(currentClient->getClientFd(), RPLY_NEWNICKMESSAGE(currentClient->getNickName(), cmd[1]));
        // sendMessage(currentClient->getClientFd(), "you are now known as " + cmd[1]);
        for (channelsMap::iterator channelIt = this->_channelsMap.begin(); channelIt != this->_channelsMap.end(); channelIt++) {
            if (channelIt->second->isClientExist(currentClient)) {
                for (clientsListMapIterator clientIt = channelIt->second->getClientsList()->begin(); clientIt != channelIt->second->getClientsList()->end(); clientIt++) {
                    Client const    *targetClient = clientIt->second->getClient();
                    if (targetClient->getClientFd() != currentClient->getClientFd())
                        sendMessage(targetClient->getClientFd(), RPLY_NEWNICKMESSAGE(currentClient->getNickName(), cmd[1]));
                }
            }
            //Need to put here : sendMessage for all user who communicate by private message with the currentClient
        }
        for (channelsMap::iterator channelIt = this->_channelsMap.begin(); channelIt != this->_channelsMap.end(); channelIt++) {
            if (channelIt->second->isClientExist(currentClient) == true)
                channelIt->second->changeClientName(currentClient->getNickName(), cmd[1]);
        }
    }
    currentClient->setnickName(cmd[1]);
    // Register client if he already set a password and user
    if (!currentClient->getIsRegister() && !currentClient->getServerPassword().empty() && !currentClient->getUserName().empty() && !currentClient->getRealName().empty()) {
        sendMessage(fd, RPL_WELCOME(currentClient->getNickName(), currentClient->getUserName()));
        currentClient->setIsRegister(true);
    }
}
