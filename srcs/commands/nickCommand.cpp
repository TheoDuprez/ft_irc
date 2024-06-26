/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:12 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 12:53:01 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd)
{
	std::locale	loc;
	std::string excluded = "#&: ,*?!@.";
    std::string firstCharExcluded = "~%+";

    if ((*cmd).size() != 2 || (*cmd)[1].empty()) {
        sendMessage(currentClient->getClientFd(), ERR_NONICKNAMEGIVEN(this->getServerName()));
        return false;
    }
    if ((*cmd)[1].size() > NICKLEN)
        (*cmd)[1] = (*cmd)[1].substr(0, NICKLEN);
    // Check if nick already exist
    if (this->getClientByName((*cmd)[1])) {
        if (currentClient->getNickName().empty())
            sendMessage(currentClient->getClientFd(), ERR_NICKNAMEINUSE((*cmd)[1], this->getServerName()));
        else
            sendMessage(currentClient->getClientFd(), ERR_CHANGENICKNAMEINUSE((*cmd)[1]));
        return false;
    }
    // Check if nick content is allowed
    if (std::isdigit((*cmd)[1][0], loc) || firstCharExcluded.find((*cmd)[1][0]) != std::string::npos || (*cmd)[1].find_first_of(excluded) != std::string::npos) {
        if (currentClient->getNickName().empty())
            sendMessage(currentClient->getClientFd(), ERR_ERRONEUSNICKNAME((*cmd)[1], this->getServerName()));
        else
            sendMessage(currentClient->getClientFd(), ERR_CHANGEERRONEUSNICKNAME((*cmd)[1]));
        return false;
    }
    for (stringIterator it = (*cmd)[1].begin(); it != (*cmd)[1].end(); it++) {
        if (!std::isprint(*it, loc)) {
             if (currentClient->getNickName().empty())
                sendMessage(currentClient->getClientFd(), ERR_ERRONEUSNICKNAME((*cmd)[1], this->getServerName()));
            else
                sendMessage(currentClient->getClientFd(), ERR_CHANGEERRONEUSNICKNAME((*cmd)[1]));
            return false;
        }
    }
    return true;
}

void    Server::_nickApplyChange(std::vector<std::string> cmd, Client *currentClient)
{
    Client const *mpClient = NULL;
    sendMessage(currentClient->getClientFd(), RPLY_NEWNICKMESSAGE(currentClient->getNickName(), cmd[1]));
    //sendMessage for all user who communicate by private message with the currentClient
    for  (std::vector<Client*>::const_iterator it = currentClient->getContactedClients().begin(); it != currentClient->getContactedClients().end(); it++) {
        mpClient = this->getClientByName((*it)->getNickName());
        if (mpClient)
            sendMessage(mpClient->getClientFd(), RPLY_NEWNICKMESSAGE(currentClient->getNickName(), cmd[1]));
    }
    //sendMessage for all channel who currentClient is present
    for (channelsMap::iterator channelIt = this->_channelsMap.begin(); channelIt != this->_channelsMap.end(); channelIt++) {
        if (channelIt->second->isClientExist(currentClient))
            this->_sendMessageToAllChannelUsers(currentClient, channelIt->second, RPLY_NEWNICKMESSAGE(currentClient->getNickName(), cmd[1]));
    }
    //operate changes everywhere currentClient is
    for (channelsMap::iterator channelIt = this->_channelsMap.begin(); channelIt != this->_channelsMap.end(); channelIt++) {
        if (channelIt->second->isClientExist(currentClient) == true)
            channelIt->second->changeChannelClientNick(currentClient->getNickName(), cmd[1]);
        else if (channelIt->second->isInvitedClient(currentClient->getNickName()) == true)
            channelIt->second->setNewInvitedClient(currentClient->getNickName());
    }
}

void	Server::nickCommand(std::vector<std::string> cmd, int fd)
{
	Client	*currentClient = this->_clients[fd];

    if (this->_isValidNickCommand(currentClient, &cmd) == false)
        return;
    // Send message to other client if nick is changed
    if (!currentClient->getNickName().empty())
        this->_nickApplyChange(cmd, currentClient);
    currentClient->setnickName(cmd[1]);
    // Register client if he already set a password and user
    if (!currentClient->getIsRegister() && !currentClient->getServerPassword().empty() && !currentClient->getUserName().empty() && !currentClient->getRealName().empty()) {
        currentClient->setIsRegister(true);
        sendInitialMessages(currentClient);
    }
}
