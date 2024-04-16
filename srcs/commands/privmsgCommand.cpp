/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:06 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/16 23:31:18 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void                Server::privmsgCommand(commandTokensVector cmd, Client* clientPtr)
{
	std::string message;

	if (cmd.size() == 1) {
		sendMessage(clientPtr->getClientFd(), ERR_NORECIPIENT(cmd[0]));
		return ;
	}
	if (cmd.size() == 2) {
		sendMessage(clientPtr->getClientFd(), ERR_NOTEXTTOSEND);
		return ;
	}

	for (size_t i = 2; i < cmd.size(); i++) {
			message += cmd[i];
		if (i + 1 != cmd.size())
			message += " ";
	}

	if (getClientByName(cmd[1])) {
		Client *targetClient = clientPtr->getContactedClientByNick(cmd[1]);
		std::cout << "The message is : '" << message << "'" << std::endl;
		sendMessage(getClientByName(cmd[1])->getClientFd(), ":" + clientPtr->getNickName() + " PRIVMSG " + getClientByName(cmd[1])->getNickName() + " " + message);
		if (targetClient == NULL)
			clientPtr->setContactedClient(getClientByName(cmd[1]), true);
	}
	else if (getChannelByName(cmd[1]))
        this->_channelsMap.find(cmd[1])->second->privmsg(cmd, clientPtr);
	else
		sendMessage(clientPtr->getClientFd(), ERR_NOSUCHNICK(clientPtr->getNickName(), cmd[1]));
}