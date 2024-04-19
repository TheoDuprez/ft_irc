/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 04:00:58 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 04:32:18 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::pingCommand(commandTokensVector cmd,  Client const *currentClient) const
{
    time_t start_time;
    if (cmd.size() == 1) {
        sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(currentClient->getNickName(), "PING"));
        return ;
    }
    time(&start_time);
    this->pongCommand(cmd, currentClient, start_time);
}