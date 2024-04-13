/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:58:02 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 14:58:04 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::modeCommand(commandTokensVector cmd, Client* client)
{
	std::vector<std::string>	modeArguments;
	std::string					modeString;

	if (cmd.size() < 2) {
    	sendMessage(client->getClientFd(), ERR_NEEDMOREPARAMS(client->getNickName(), "MODE"));
		return ;
	}
	if (!getChannelByName(cmd[1])) {
		sendMessage(client->getClientFd(), ERR_NOSUCHCHANNEL(client->getNickName(), cmd[1]));
		return ;
	}
	if (cmd.size() < 3) {
		sendMessage(client->getClientFd(), RPL_CHANNELMODEIS(client->getNickName(), getChannelByName(cmd[1])));
		return ;
	}
	if (!getChannelByName(cmd[1])->getClientsInfoByNick(client->getNickName())->getIsOperator()) {
		sendMessage(client->getClientFd(), ERR_CHANOPRIVSNEEDED(client->getNickName(), cmd[1]));
		return ;
	}

	modeString = cmd[2];
	modeArguments = fillModeArguments(cmd);
	if (modeString[0] == '+')
		manageModes(modeString, modeArguments, client, getChannelByName(cmd[1]), true);
	else if (modeString[0] == '-')
		manageModes(modeString, modeArguments, client, getChannelByName(cmd[1]), false);
}

void	Server::manageModes(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr, bool adjustMode)
{
	std::vector<std::string>::iterator argumentsIt = modeArguments.begin();

	for (size_t i = 1; i < modeString.length(); i++) {
		switch (modeString[i]) {
			case 'k':
				if (argumentsIt != modeArguments.end())
					manageKey(channelPtr, client, argumentsIt, adjustMode);
				break;
			case 'l':
				if (!adjustMode || argumentsIt != modeArguments.end())
					manageUsersLimit(channelPtr, client, argumentsIt, adjustMode);
				break ;
			case 'o':
				if (argumentsIt != modeArguments.end())
					manageOperator(channelPtr, client, argumentsIt, adjustMode);
				break ;
			case 't':
				manageTopic(channelPtr, client, adjustMode);
				break ;
			case 'i':
				manageInvite(channelPtr, client, adjustMode);
				break ;
		}
	}
}

void	Server::manageKey(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (adjustMode && (!channelPtr->getHasPassword() || channelPtr->getPassword() != *argumentsIt)) {
			channelPtr->setPassword(*argumentsIt);
			channelPtr->setHasPassword(true);
			sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_KEY);
	}
	else if (!adjustMode && channelPtr->getPassword() == *argumentsIt) {
		channelPtr->setPassword("");
		channelPtr->setHasPassword(false);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_KEY);
	}
	argumentsIt++;
}

void	Server::manageUsersLimit(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (adjustMode) {
		if (channelPtr->getHasUsersLimit() && ullToString(channelPtr->getUsersLimit()) == *argumentsIt) {
			argumentsIt++;
			return ;
		}
		try {
			channelPtr->setUsersLimit(strtost(*argumentsIt));
			channelPtr->setHasUsersLimit(true);
			sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_USERS_LIMITS);
		} catch (const std::runtime_error& e) { }
	}
	else if (channelPtr->getHasUsersLimit()) {
		channelPtr->setHasUsersLimit(false);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_USERS_LIMITS);
	}
	argumentsIt++;
}

void	Server::manageOperator(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	ClientInfos* clientPrivilege;

	if (channelPtr->getClientsDataMap().find(*argumentsIt) == channelPtr->getClientsDataMap().end()) // If client not found, return
		return ;

	clientPrivilege = channelPtr->getClientsDataMap().find(*argumentsIt)->second; // Get the client
	if (adjustMode) {
		if (!clientPrivilege->getIsOperator()) {
			clientPrivilege->setIsOperator(true);
			sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_OPERATOR);
		}
	}
	else {
		if (clientPrivilege->getIsOperator()) {
			clientPrivilege->setIsOperator(false);
			sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_OPERATOR);
		}
	}
	argumentsIt++;
}

void	Server::manageTopic(Channel* channelPtr, Client* client, bool adjustMode)
{
	if (adjustMode && !channelPtr->getIsTopicOperatorMode()) {
		channelPtr->setIsTopicOperatorMode(true);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_TOPIC_ON_OPERATOR);
	}
	else if (!adjustMode && channelPtr->getIsTopicOperatorMode()) {
		channelPtr->setIsTopicOperatorMode(false);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_TOPIC_ON_OPERATOR);
	}
}

void	Server::manageInvite(Channel* channelPtr, Client* client, bool adjustMode)
{
	if (adjustMode && !channelPtr->getIsOnInvite()) {
		channelPtr->setIsOnInvite(true);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_INVITE);
	}
	else if (!adjustMode && channelPtr->getIsOnInvite()) {
		channelPtr->setIsOnInvite(false);
		sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_INVITE);
	}
}

void	Server::sendMessageToAllClients(const Channel* channelPtr, const std::string& message) const
{
	for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
		sendMessage(it->second->getClient()->getClientFd(), message);
	}
}


std::string Channel::createModesString(void) const
{
	std::string modesString("+");

	if (this->_isTopicOperatorMode)
		modesString += "t";
	if (this->_isOnInvite)
		modesString += "i";
	if (this->_hasPassword)
		modesString += "k";
	if (this->_hasUsersLimit)
		modesString += "l";
	return modesString;
}













std::vector<std::string>	Server::fillModeArguments(commandTokensVector& cmd)
{
	std::vector<std::string> retModeArguments;
	
	if (cmd.size() > 3) {
		for (std::vector<std::string>::iterator it = cmd.begin() + 3; it != cmd.end(); it++)
			retModeArguments.push_back(*it);
	}
	return retModeArguments;
}