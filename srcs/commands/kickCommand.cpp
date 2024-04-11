/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:28:52 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/11 18:19:33 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::_isValidKickCommand(std::vector<std::string> cmd, Client *currentClient, UserInfos *&targetOp, UserInfos *&targetUser, Channel *&targetChannel)
{
    if (cmd.size() <= 2) {
        if (cmd.size() == 1)
            sendMessage(currentClient->getClientFd(), "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)");
        else
            sendMessage(currentClient->getClientFd(), ":server 461 " + currentClient->getNickName() + "KICK :Not enough parameters");
        return false;
    }
    targetChannel = this->getChannelByName(cmd[1]);
    if (!targetChannel) {
        sendMessage(currentClient->getClientFd(), ":server 403 " + currentClient->getNickName() + " " + cmd[1] + " :No such channel");
        return false;
    }
    targetOp = targetChannel->getClientsInfoByNick(currentClient->getNickName());
    if (!targetOp) {
        sendMessage(currentClient->getClientFd(), ":server 442 " + currentClient->getNickName() + " " + cmd[1] + " :You're not on that channel");
        return false;
    }
    if (targetOp->getIsOperator() == false) {
        sendMessage(currentClient->getClientFd(), ":server 482 " + currentClient->getNickName() + " " + cmd[1] + " :You're not channel operator");
        return false;
    }
    targetUser = targetChannel->getClientsInfoByNick(cmd[2]);
    if (!targetUser) {
        sendMessage(ERR_USERNOTINCHANNEL(cmd[2], cmd[1]));
        return false;
    }
    return true;
}

void	Server::kickCommand(std::vector<std::string> cmd, Client *currentClient)
{
    std::string                 message;
    UserInfos                   *targetOp = NULL;
    UserInfos                   *targetUser = NULL;
    Channel                     *targetChannel = NULL;

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
    sendMessage(KICK_MESSAGE_OPS(cmd[2], cmd[1], message));
    for (clientsListMap::iterator clientIt = targetChannel->getClientsList()->begin(); clientIt != targetChannel->getClientsList()->end(); clientIt++) {
        if (clientIt->first.compare(currentClient->getNickName()))
            sendMessage(KICK_MESSAGE_USERS(cmd[2], cmd[1], message));
    }
    targetChannel->getClientsList()->erase(cmd[2]);

}
