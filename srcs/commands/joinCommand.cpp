/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:15 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 16:28:05 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::joinCommand(commandTokensVector cmd, Client* clientPtr)
{
	std::vector<std::string>			channelsNameList; // Les problemes
	std::vector<std::string>::iterator	nameIt;
	std::vector<std::string>			channelsPasswordList;
	std::vector<std::string>::iterator	passwordIt;
	std::string							password;

	if (cmd.size() == 1) {
		sendMessage(clientPtr->getClientFd(), ERR_NEEDMOREPARAMS(clientPtr->getNickName(), cmd[0]));
		return ;
	}
	if (cmd.size() > 2)
		channelsPasswordList = std::vector<std::string>(split(*(cmd.begin() + 2), ','));

	channelsNameList = split(*(cmd.begin() + 1), ',');
	nameIt = channelsNameList.begin();
	passwordIt = channelsPasswordList.begin();
	for (; nameIt != channelsNameList.end(); nameIt++) {
		password = (passwordIt != channelsPasswordList.end()) ? *(passwordIt++) : "";

		if (nameIt->at(0) != '#') {
			sendMessage(clientPtr->getClientFd(), ERR_NOSUCHCHANNEL(clientPtr->getNickName(), *nameIt));
		}
		else if (this->_channelsMap.find(*nameIt) == this->_channelsMap.end()) {
			this->_channelsMap.insert(std::make_pair(*nameIt, new Channel(*nameIt, clientPtr)));
		}
		else if (this->_channelsMap.find(*nameIt) != this->_channelsMap.end()) {
			this->_channelsMap.find(*nameIt)->second->addClient(clientPtr, password);
		}
	}
}