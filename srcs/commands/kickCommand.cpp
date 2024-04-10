/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:28:52 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/11 00:58:05 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kickCommand(std::vector<std::string> cmd, int fd)
{
    clientsListMap              *targetChannel;
    clientsListMap::iterator    targetClient;
    bool                        isValidChannel = false;
    bool                        isValidUser = false;
    bool                        isOnChannel = false;
    Client	                    *currentClient = this->_clients[fd];

    // --- Debug message ---
    std::cout << " ----- Input of kickCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    std::cout << cmd.size() << std::endl;
    if (cmd.size() == 1) {
        sendMessage(currentClient->getClientFd(), "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)");
        return ;
    }
    else if (cmd.size() == 2) {
        sendMessage(currentClient->getClientFd(), ":server 461 " + currentClient->getNickName() + "KICK :Not enough parameters");
        return ;
    }
    else if (cmd.size() > 4)
        return ;
    for (channelsMap::iterator channelIt = this->_channelsList.begin(); channelIt != this->_channelsList.end(); channelIt++) {
        if (!channelIt->second->getchannelName().compare(cmd[1])) {
            targetChannel = channelIt->second->getClientsList();
            isValidChannel = true;
            for (clientsListMap::iterator clientIt = channelIt->second->getClientsList()->begin(); clientIt != channelIt->second->getClientsList()->end(); clientIt++) {
                if (!clientIt->second->getClient()->getNickName().compare(currentClient->getNickName())) {
                    isOnChannel = true;
                    if (clientIt->second->getIsOperator() == false) {
                        sendMessage(currentClient->getClientFd(), ":server 482 " + currentClient->getNickName() + " " + cmd[2] + " :You're not channel operator");
                        return;
                    }
                }
                if (!clientIt->second->getClient()->getNickName().compare(cmd[2])) {
                    isValidUser = true;
                    targetClient = clientIt;
                }
            }
            break ;
        }
    }
    if (isValidChannel == false)
        sendMessage(currentClient->getClientFd(), ":server 403 " + currentClient->getNickName() + " " + cmd[1] + " :No such channel");
    else if (isOnChannel == false)
        sendMessage(currentClient->getClientFd(), ":server 442 " + currentClient->getNickName() + " " + cmd[1] + " :You're not on that channel");
    else if (isValidUser == false)
        sendMessage(currentClient->getClientFd(), ":server 441 " + currentClient->getNickName() + " " + cmd[2] + " " + cmd[1] + " :They aren't on that channel");
    else {
        std::string defaultMessage = "ne savait pas faire un write";
        sendMessage(currentClient->getClientFd(), ":" + currentClient->getNickName() + " KICK " + cmd[1] + " " + cmd[2] + " " + ":la raison est une raison");
        for (clientsListMap::iterator clientIt = this->_channelsList.find(cmd[1])->second->getClientsList()->begin(); clientIt != this->_channelsList.find(cmd[1])->second->getClientsList()->end(); clientIt++) {
            if (clientIt->first.compare(currentClient->getNickName()))
                sendMessage(clientIt->second->getClient()->getClientFd(), ":" + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost KICK " + cmd[1] + " " + cmd[2] + " " + defaultMessage);
        }
        targetChannel->erase(targetClient);
    }

}
