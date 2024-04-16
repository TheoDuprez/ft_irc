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

void	Server::modeCommand(commandTokensVector cmd, Client* clientPtr)
{
	std::vector<std::string>	modeArguments;
	std::string					modeString;

	if (cmd.size() < 2) {
    	sendMessage(clientPtr->getClientFd(), ERR_NEEDMOREPARAMS(clientPtr->getNickName(), "MODE"));
		return ;
	}
	if (!getChannelByName(cmd[1])) {
		sendMessage(clientPtr->getClientFd(), ERR_NOSUCHCHANNEL(clientPtr->getNickName(), cmd[1]));
		return ;
	}
	if (cmd.size() < 3) {
		sendMessage(clientPtr->getClientFd(), RPL_CHANNELMODEIS(clientPtr->getNickName(), getChannelByName(cmd[1])));
		return ;
	}
	if (!getChannelByName(cmd[1])->getClientsInfoByNick(clientPtr->getNickName())->getIsOperator()) {
		sendMessage(clientPtr->getClientFd(), ERR_CHANOPRIVSNEEDED(clientPtr->getNickName(), cmd[1]));
		return ;
	}

	modeString = cmd[2];
	modeArguments = _fillModeArguments(cmd);
	if (!_isValidModeString(modeString))
		return ;
	if (modeString[0] == '+')
		_manageModes(modeString, modeArguments, clientPtr, getChannelByName(cmd[1]), true);
	else
		_manageModes(modeString, modeArguments, clientPtr, getChannelByName(cmd[1]), false);
}

std::vector<std::string>	Server::_fillModeArguments(commandTokensVector& cmd)
{
	std::vector<std::string> retModeArguments;

	if (cmd.size() > 3) {
		for (std::vector<std::string>::iterator it = cmd.begin() + 3; it != cmd.end(); it++)
			retModeArguments.push_back(*it);
	}
	return retModeArguments;
}

void	Server::_manageModes(std::string modeString, std::vector<std::string> modeArguments, Client* clientPtr, Channel* channelPtr, bool adjustMode)
{
	std::vector<std::string>::iterator argumentsIt = modeArguments.begin();

	for (size_t i = 1; i < modeString.length(); i++) {
		switch (modeString[i]) {
			case 'k':
				if (argumentsIt != modeArguments.end())
					_manageKey(channelPtr, clientPtr, argumentsIt, adjustMode);
				break;
			case 'l':
				if (!adjustMode || argumentsIt != modeArguments.end())
					_manageUsersLimit(channelPtr, clientPtr, argumentsIt, adjustMode);
				break ;
			case 'o':
				if (argumentsIt != modeArguments.end())
					_manageOperator(channelPtr, clientPtr, argumentsIt, adjustMode);
				break ;
			case 't':
					_manageTopic(channelPtr, clientPtr, adjustMode);
				break ;
			case 'i':
					_manageInvite(channelPtr, clientPtr, adjustMode);
				break ;
		}
	}
}

void	Server::_manageKey(Channel* channelPtr, Client* clientPtr, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (adjustMode && (!channelPtr->getHasPassword() || channelPtr->getPassword() != *argumentsIt)) {
		channelPtr->setPassword(*argumentsIt);
		channelPtr->setHasPassword(true);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_KEY);
	}
	else if (!adjustMode && channelPtr->getPassword() == *argumentsIt) {
		channelPtr->setPassword("");
		channelPtr->setHasPassword(false);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_KEY);
	}
	argumentsIt++;
}

void	Server::_manageUsersLimit(Channel* channelPtr, Client* clientPtr, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (adjustMode) {
		if (channelPtr->getHasUsersLimit() && ullToString(channelPtr->getUsersLimit()) == *argumentsIt) {
			argumentsIt++;
			return ;
		}
		try {
			channelPtr->setUsersLimit(strtost(*argumentsIt));
			channelPtr->setHasUsersLimit(true);
			_sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_USERS_LIMITS);
		} catch (const std::runtime_error& e) { }
	}
	else if (channelPtr->getHasUsersLimit()) {
		channelPtr->setHasUsersLimit(false);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_USERS_LIMITS);
	}
	argumentsIt++;
}

void	Server::_manageOperator(Channel* channelPtr, Client* clientPtr, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	ClientInfos* clientPrivilege;

	if (channelPtr->getClientsDataMap().find(*argumentsIt) == channelPtr->getClientsDataMap().end()) {
		argumentsIt++;
		return ;
	}

	clientPrivilege = channelPtr->getClientsDataMap().find(*argumentsIt)->second;
	if (adjustMode) {
		if (!clientPrivilege->getIsOperator()) {
			clientPrivilege->setIsOperator(true);
			_sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_OPERATOR);
		}
	}
	else {
		if (clientPrivilege->getIsOperator()) {
			clientPrivilege->setIsOperator(false);
			_sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_OPERATOR);
		}
	}
	argumentsIt++;
}

void	Server::_manageTopic(Channel* channelPtr, Client* clientPtr, bool adjustMode)
{
	if (adjustMode && !channelPtr->getIsTopicOperatorMode()) {
		channelPtr->setIsTopicOperatorMode(true);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_TOPIC_ON_OPERATOR);
	}
	else if (!adjustMode && channelPtr->getIsTopicOperatorMode()) {
		channelPtr->setIsTopicOperatorMode(false);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_TOPIC_ON_OPERATOR);
	}
}

void	Server::_manageInvite(Channel* channelPtr, Client* clientPtr, bool adjustMode)
{
	if (adjustMode && !channelPtr->getIsOnInvite()) {
		channelPtr->setIsOnInvite(true);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_ADD_INVITE);
	}
	else if (!adjustMode && channelPtr->getIsOnInvite()) {
		channelPtr->setIsOnInvite(false);
		_sendMessageToAllClients(channelPtr, MODE_MESSAGE_REMOVE_INVITE);
	}
}

bool	Server::_isValidModeString(const std::string& modeString)
{
	std::string checkDuplicate;
	std::string allowedChars("lkiot");

	if (modeString[0] != '-' && modeString[0] != '+')
		return false;
	for (size_t i = 1; i < modeString.length(); i++) {
		if (!islower(modeString[i]))
			return false;
		else if (allowedChars.find(modeString[i]) != std::string::npos && checkDuplicate.find(modeString[i]) == std::string::npos)
			checkDuplicate += modeString[i];
		else
			return false;
	}
	return true;
}

void	Server::_sendMessageToAllClients(const Channel* channelPtr, const std::string& message) const
{
	for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
		sendMessage(it->second->getClient()->getClientFd(), message);
	}
}