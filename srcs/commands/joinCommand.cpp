/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:15 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 19:06:46 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::joinCommand(commandTokensVector cmd, Client* clientPtr)
{
	std::vector<std::string>			targetsChannel;
	std::vector<std::string>::iterator	channelIt;
	std::vector<std::string>			channelsPasswordList;
	std::vector<std::string>::iterator	passwordIt;
	std::string							password;

	if (cmd.size() == 1) {
		sendMessage(clientPtr->getClientFd(), ERR_NEEDMOREPARAMS(clientPtr->getNickName(), cmd[0]));
		return ;
	}
	if (cmd.size() > 2)
		channelsPasswordList = std::vector<std::string>(split(*(cmd.begin() + 2), ','));

	targetsChannel = split(*(cmd.begin() + 1), ',');
	channelIt = targetsChannel.begin();
	passwordIt = channelsPasswordList.begin();
	for (; channelIt != targetsChannel.end(); channelIt++) {
		password = (passwordIt != channelsPasswordList.end()) ? *(passwordIt++) : "";

		if (channelIt->at(0) != '#' || channelIt->find(':') != std::string::npos) {
			sendMessage(clientPtr->getClientFd(), ERR_BADCHANNMASK(*channelIt));
		}
		else if (this->_channelsMap.find(*channelIt) == this->_channelsMap.end()) {
			if ((*channelIt).length() > CHANNELLEN)
				*channelIt = (*channelIt).substr(0, 50);
			this->_channelsMap.insert(std::make_pair(*channelIt, new Channel(*channelIt, clientPtr)));
		}
		else if (this->_channelsMap.find(*channelIt) != this->_channelsMap.end()) {
			this->_channelsMap.find(*channelIt)->second->addClient(clientPtr, password);
		}
	}
}