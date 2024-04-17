/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:38:25 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 17:45:04 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::_isValidInviteCommand(std::vector<std::string> cmd, Client *currentClient, Channel *targetChannel)
{
    if (!this->getClientByName(cmd[1])) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHNICK(currentClient->getNickName(), cmd[1]));
        return false;
    }
    if (!targetChannel) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHCHANNEL(currentClient->getNickName(), cmd[2]));
        return false;
    }
    if (!targetChannel->getClientsInfoByNick(currentClient->getNickName())) {
        sendMessage(currentClient->getClientFd(), ERR_NOTONCHANNEL(currentClient->getNickName(), cmd[2]));
        return false;
    }
    if (targetChannel->getClientsInfoByNick(cmd[1])) {
        sendMessage(currentClient->getClientFd(), ERR_USERONCHANNEL(currentClient->getNickName(), cmd[1], cmd[2]));
        return false; 
    }
	if (targetChannel->getIsOnInvite() && !targetChannel->getClientsInfoByNick(currentClient->getNickName())->getIsOperator()) {
		sendMessage(currentClient->getClientFd(), ERR_CHANOPRIVSNEEDED(currentClient->getNickName(), targetChannel->getChannelName()));
		return false;
	}
    return true;
}

void    Server::inviteCommand(std::vector<std::string> cmd, Client *currentClient)
{
    if (cmd.size() <= 2) {
        if (cmd.size() == 1) {
            //check if invite list is not empty here, if not send the invite list
            sendMessage(currentClient->getClientFd(), INVITE_USAGE);
        } else {
            //Maybe is the bad send here
            sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(currentClient->getNickName(), "INVITE"));
        }
        return ;
    }
    Channel *   targetChannel =  this->getChannelByName(cmd[2]);

    if (_isValidInviteCommand(cmd, currentClient, targetChannel) == false)
        return ;
    Client const  *targetClient = this->getClientByName(cmd[1]);
    if (targetChannel->isInvitedClient(cmd[1])) {
        sendMessage(currentClient->getClientFd(), ALREADY_INVITE(cmd[1], cmd[2]));
        return ;
    }
    sendMessage(currentClient->getClientFd(), RPL_INVITING(currentClient->getNickName(), cmd[1], cmd[2]));
    sendMessage(targetClient->getClientFd(), RPL_INVITED(currentClient->getNickName(), cmd[1], cmd[2]));
    targetChannel->setNewInvitedClient(cmd[1]);
}
