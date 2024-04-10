/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:12 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 17:33:47 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd)
{
    std::locale	loc;
    std::string excluded = "#&: ";

    if ((*cmd).size() != 2 || (*cmd)[1].empty()) {
        this->printLogMessage("ERR_NONICKNAMEGIVEN (431)\n", ERROR);
        return false;
    }
    // Check if nick is already used
    for (clientMap::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (!(*cmd)[1].compare(it->second->getNickName())) {
            sendMessage(currentClient->getClientFd(), ":server 433 * nickname :" + (*cmd)[1] + " is already in use");
            this->printLogMessage("ERR_NICKNAMEINUSE (433)\n", ERROR);
            return false;
        }
    }
    // Check if nick content is allowed
    for (stringIterator it = (*cmd)[1].begin(); it != (*cmd)[1].end(); it++) {
        if ((it == (*cmd)[1].begin() && std::isdigit(*it, loc)) || (*cmd)[1].find_first_of(excluded) != std::string::npos || !std::isprint(*it, loc)) {
            this->printLogMessage("ERR_ERRONEUSNICKNAME (432)\n", ERROR);
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
        sendMessage(currentClient->getClientFd(), "you are now known as " + cmd[1]);
        for (channelsMap::iterator channelIt = this->_channelsList.begin(); channelIt != this->_channelsList.end(); channelIt++) {
            if (channelIt->second->isClientExist(currentClient)) {
                for (clientsListMapIterator clientIt = channelIt->second->getClientsList()->begin(); clientIt != channelIt->second->getClientsList()->end(); clientIt++) {
                    Client const    *targetClient = clientIt->second->getClient();
                    if (targetClient->getClientFd() != currentClient->getClientFd())
                    sendMessage(targetClient->getClientFd(), targetClient->getNickName() + "!" + targetClient->getUserName() + "@localhost NICK " + cmd[1]);
                }
            }
            //Need to put here : sendMessage for all customers who communicate by private message with the currentClient
        }
    }
    this->printLogMessage("<past nickName> NICK <new nickName>\n", OK);
    currentClient->setnickName(cmd[1]);
    // Register client if he already set a password and user
    if (!currentClient->getIsRegister() && !currentClient->getServerPassword().empty() && !currentClient->getUserName().empty() && !currentClient->getRealName().empty()) {
        sendMessage(fd, ":server 001 " + currentClient->getNickName() + " :Welcome to the localhost Network, " + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost");
        currentClient->setIsRegister(true);
    }
}
