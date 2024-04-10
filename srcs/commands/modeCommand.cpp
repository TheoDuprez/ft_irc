#include "Server.hpp"

void	Server::modeCommand(commandTokensVector cmd, Client* client)
{
	std::vector<std::string>	modeArguments;
	std::string					modeString;

	if (cmd.size() < 3) {
		std::cout << "Reply ERR_NEEDMOREPARAMS" << std::endl;
		return ;
	}

	modeString = cmd.at(2);
	modeArguments = fillModeArguments(cmd);
	if ((cmd.at(1)[0] == '#' || cmd.at(1)[0] == '&') && this->_channelsMap.find(cmd.at(1)) != this->_channelsMap.end())
		modeCommandChannel(modeString, modeArguments, client, this->_channelsMap.find(cmd.at(1))->second);
	else
		std::cout << "Error 403 no such channel" << std::endl;
}

void	Server::modeCommandChannel(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr)
{
	bool	isOperator = channelPtr->getClientsDataMap().find(client->getNickName())->second->getIsOperator();

	if (modeString.at(0) == '+' && isOperator) {
		manageModes(modeString, modeArguments, client, channelPtr, true);
	}
	else if (modeString.at(0) == '-' && isOperator) {
		std::cout << "Delete mode" << std::endl;
		manageModes(modeString, modeArguments, client, channelPtr, false);
	}
	else
		std::cout << "Cannot mod" << std::endl;
		// deleteModeChannel
}

void	Server::manageModes(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr, bool adjustMode)
{
	std::vector<std::string>::iterator argumentsIt = modeArguments.begin();

	for (size_t i = 1; i < modeString.length(); i++) {
		switch (modeString[i]) {
			case 'k':
				manageKey(channelPtr, client, modeArguments, argumentsIt, adjustMode);
				break;
			case 'l':
				manageUsersLimit(channelPtr, client, modeArguments, argumentsIt, adjustMode);
				break ;
			case 'o':
				manageOperator(channelPtr, client, modeArguments, argumentsIt, adjustMode);
				break ;
		}
	}
}

void	Server::manageKey(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (argumentsIt == modeArguments.end())
		return ;
	if (adjustMode) {
		if (channelPtr->getPassword() != *argumentsIt) {
			channelPtr->setPassword(*argumentsIt);
			channelPtr->setHasPassword(true);
			for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
				sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +k " + *argumentsIt);
			}
			argumentsIt++;
		}
	}
	else if (channelPtr->getPassword() == *argumentsIt) {
		channelPtr->setPassword("");
		channelPtr->setHasPassword(false);
		for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
			sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -k ");
		}
		argumentsIt++;
	}
}

void	Server::manageUsersLimit(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (argumentsIt == modeArguments.end())
		return ;
	if (adjustMode) {
		// Add condition to check if the arguments in iterator is the same of usersLimit (converted with itoa)
		try {
			channelPtr->setUsersLimit(strtost(*argumentsIt));
			for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
				sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +l " + *argumentsIt);
			}
		} catch (const std::runtime_error& e) { }
		argumentsIt++;
	}
	else {
		for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
			sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -l");
		}
		channelPtr->setUsersLimit(-1);
		argumentsIt++;
	}
}

void	Server::manageOperator(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode)
{
	if (argumentsIt == modeArguments.end())
		return ;
	if (adjustMode) {
		if (channelPtr->getClientsDataMap().find(*argumentsIt) != channelPtr->getClientsDataMap().end() && !channelPtr->getClientsDataMap().find(*argumentsIt)->second->getIsOperator()) {
			channelPtr->getClientsDataMap().find(*argumentsIt)->second->setIsOperator(true);
			for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
					sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +o " + *argumentsIt);
			}
		}
		argumentsIt++;
	}
	else {
		if (channelPtr->getClientsDataMap().find(*argumentsIt) != channelPtr->getClientsDataMap().end() && channelPtr->getClientsDataMap().find(*argumentsIt)->second->getIsOperator()) {
			channelPtr->getClientsDataMap().find(*argumentsIt)->second->setIsOperator(false);
			for (clientsMap::const_iterator it = channelPtr->getClientsDataMap().begin(); it != channelPtr->getClientsDataMap().end(); it++) {
				sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -o " + *argumentsIt);
			}
		}
		argumentsIt++;
	}
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