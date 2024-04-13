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
	std::string message;

	if (cmd.size() == 1) {
		sendMessage(client->getClientFd(), ERR_NORECIPIENT(cmd[0]));
		return ;
	}
	if (cmd.size() == 2) {
		sendMessage(client->getClientFd(), ERR_NOTEXTTOSEND);
		return ;
	}

	for (size_t i = 2; i < cmd.size(); i++) {
			message += cmd[i];
		if (i + 1 != cmd.size())
			message += " ";
	}

	if (getClientByName(cmd[1])) {
		std::cout << "The message is : '" << message << "'" << std::endl;
		sendMessage(getClientByName(cmd[1])->getClientFd(), ":" + client->getNickName() + " PRIVMSG " + getClientByName(cmd[1])->getNickName() + " " + message);
	}
	else if (getChannelByName(cmd[1]))
        this->_channelsMap.find(cmd[1])->second->privmsg(cmd, client);
	else
		sendMessage(client->getClientFd(), ERR_NOSUCHNICK(client->getNickName(), cmd[1]));
}