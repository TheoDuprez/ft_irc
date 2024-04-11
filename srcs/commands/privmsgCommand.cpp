/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:06 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 13:03:07 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void                Server::privmsgCommand(commandTokensVector cmd, Client* client)
{
    if (cmd.at(1).find("#") != std::string::npos || cmd.at(1).find("&") != std::string::npos) { // Is a channel target
        this->_channelsMap.find(cmd.at(1))->second->privmsg(cmd, client);
    }
	else if (getClientByName(cmd.at(1))) {
		sendMessage(getClientByName(cmd.at(1))->getClientFd(), ":" + client->getNickName() + " PRIVMSG " + getClientByName(cmd.at(1))->getNickName() + " :" + cmd.at(2));
	}
}