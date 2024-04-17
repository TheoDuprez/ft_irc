/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:06 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 17:46:37 by acarlott         ###   ########lyon.fr   */
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

	std::vector<std::string> targetsName(split(cmd[1], ','));

	for (std::vector<std::string>::iterator targetsIt = targetsName.begin(); targetsIt != targetsName.end(); targetsIt++) {
		if (getClientByName(*targetsIt)) {
			Client *targetClient = clientPtr->getContactedClientByNick(*targetsIt);
			sendMessage(getClientByName(*targetsIt)->getClientFd(), ":" + clientPtr->getNickName() + " PRIVMSG " + getClientByName(*targetsIt)->getNickName() + " " + message);
			if (targetClient == NULL)
				clientPtr->setContactedClient(getClientByName(*targetsIt), true);
		}
		else if (getChannelByName(*targetsIt))
			this->_channelsMap.find(*targetsIt)->second->privmsg(cmd, clientPtr);
		else
			sendMessage(clientPtr->getClientFd(), ERR_NOSUCHNICK(clientPtr->getNickName(), *targetsIt));
	}
}