/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:38:25 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 15:05:33 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::_isValidInviteCommand(std::vector<std::string> cmd, Client *currentClient, Channel *&targetChannel)
{
    if (cmd.size() == 2) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHCHANNEL(currentClient->getNickName(), this->getServerName()));
        return false;
    }
    targetChannel =  this->getChannelByName(cmd[2]);
    if (!targetChannel) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHCHANNEL(currentClient->getNickName(), cmd[1]));
        return false;
    }
    if (!this->getClientByName(cmd[1])) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHNICK(currentClient->getNickName(), cmd[1]));
        return false;
    }
    if (!targetChannel->getClientsInfoByNick(currentClient->getNickName())) {
        sendMessage(currentClient->getClientFd(), ERR_NOTONCHANNEL(currentClient->getNickName(), cmd[1]));
        return false;
    }
    if (targetChannel->getClientsInfoByNick(cmd[1])) {
        sendMessage(currentClient->getClientFd(), ERR_USERONCHANNEL(currentClient->getNickName(), cmd[2], cmd[1]));
        return false; 
    }
    //here put a check for know if channel is on mode invite only and currentclient is operator, if not send 'ERR_CHANOPRIVSNEEDED'
    return true;
}

void    Server::inviteCommand(std::vector<std::string> cmd, Client *currentClient)
{
    // --- Debug message ---
    std::cout << " ----- Input of inviteCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    Channel *targetChannel;

    if (cmd.size() == 1) {
        //check if invite list is not empty here, if not send the invite list
        // if ()
        sendMessage(currentClient->getClientFd(), INVITE_USAGE);
        return ;
    }
    if (_isValidInviteCommand(cmd, currentClient, targetChannel) == false)
        return ;
    Client const  *targetClient = this->getClientByName(cmd[1]);
    sendMessage(currentClient->getClientFd(), RPL_INVITING(currentClient->getNickName(), cmd[1], cmd[2]));
    sendMessage(targetClient->getClientFd(), RPL_INVITED(currentClient->getNickName(), cmd[1], cmd[2]));
    targetChannel->setNewInvitedClient(cmd[1]);
}