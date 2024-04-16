/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:28:52 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/14 12:07:13 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::_isValidKickCommand(std::vector<std::string> cmd, Client *currentClient, ClientInfos *&targetOp, ClientInfos *&targetUser, Channel *&targetChannel)
{
    if (cmd.size() <= 2) {
        if (cmd.size() == 1)
            sendMessage(currentClient->getClientFd(), KICK_USAGE);
        else
            sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(currentClient->getNickName(), "KICK"));
        return false;
    }
    targetChannel = this->getChannelByName(cmd[1]);
    if (!targetChannel) {
        sendMessage(currentClient->getClientFd(), ERR_NOSUCHCHANNEL(currentClient->getNickName(), cmd[1]));
        return false;
    }
    targetOp = targetChannel->getClientsInfoByNick(currentClient->getNickName());
    if (!targetOp) {
        sendMessage(currentClient->getClientFd(), ERR_NOTONCHANNEL(currentClient->getNickName(), cmd[1]));
        return false;
    }
    if (targetOp->getIsOperator() == false) {
        sendMessage(currentClient->getClientFd(), ERR_CHANOPRIVSNEEDED(currentClient->getNickName(), cmd[1]));
        return false;
    }
    targetUser = targetChannel->getClientsInfoByNick(cmd[2]);
    if (!targetUser) {
        sendMessage(currentClient->getClientFd(), ERR_USERNOTINCHANNEL(currentClient->getNickName(), cmd[2], cmd[1]));
        return false;
    }
    return true;
}

void	Server::kickCommand(std::vector<std::string> cmd, Client *currentClient)
{
    std::string message;
    ClientInfos *targetOp = NULL;
    ClientInfos *targetUser = NULL;
    Channel     *targetChannel = NULL;

    // --- Debug message ---
    std::cout << " ----- Input of kickCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
    if (this->_isValidKickCommand(cmd, currentClient, targetOp, targetUser, targetChannel) == false)
        return ;
    // builds the remaining string with the additional commands, or just the nickname if no message is specified
    if (cmd.size() >= 4) {
        for (size_t i = 3; i < cmd.size(); i++) {
            message += cmd[i];
            if (i + 1 != cmd.size())
                message += " ";
        }
    } else
        message = currentClient->getNickName();
    // send kick info to all client in channel
    sendMessage(currentClient->getClientFd(), KICK_MESSAGE(cmd[2], cmd[1], message));
    sendMessageToAllChannelUsers(currentClient, targetChannel, KICK_MESSAGE(cmd[2], cmd[1], message));
    targetChannel->getClientsList()->erase(cmd[2]);
    delete (targetUser);

}
