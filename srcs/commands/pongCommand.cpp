/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pongCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 04:01:17 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 04:36:22 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::pongCommand(commandTokensVector cmd, Client const *currentClient, time_t start_time) const
{
    time_t end_time;
    std::string diffStr;
    std::stringstream strConverter;
    
    if (cmd.size() == 1) {
        sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(currentClient->getNickName(), "PING"));
        return ;
    }
    time(&end_time);
    double diff = difftime(end_time, start_time);
    strConverter << diff;
    strConverter >> diffStr;
    std::string pong_reply = ":" + currentClient->getNickName() + " PONG :" + this->_serverName + " " + cmd[1] + " :" + diffStr + " second(s)";
    sendMessage(currentClient->getClientFd(), pong_reply);
}