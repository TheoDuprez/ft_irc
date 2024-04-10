#include "Server.hpp"

void	Server::addModeToChannel(std::string modeString, std::vector<std::string> modeArguments, int clientFd, Channel* channelPtr) {
	(void)clientFd;

	std::vector<std::string>::iterator argumentsIt = modeArguments.begin();

	for (size_t i = 1; i < modeString.length(); i++) {
		switch (modeString[i]) {
			case 'k':
				if (argumentsIt != modeArguments.end()) {
					channelPtr->setPassword(*argumentsIt);
					argumentsIt++;
				}
				break;
			case 'l':
				if (argumentsIt != modeArguments.end()) {
					channelPtr->setUsersLimit(std::atoi(argumentsIt->c_str()));
					argumentsIt++;
				}
		}
	}
}

void	Server::modeCommandChannel(std::string modeString, std::vector<std::string> modeArguments, int clientFd, Channel* channelPtr)
{
	if (modeString.at(0) == '+') {
		addModeToChannel(modeString, modeArguments, clientFd, channelPtr);
	}
}

void	Server::modeCommand(commandTokensVector cmd, int clientFd)
{
	std::vector<std::string>	modeArguments;
	std::string					modeString;

	if (cmd.size() < 3) {
		std::cout << "Reply ERR_NEEDMOREPARAMS" << std::endl;
		return ;
	}

	modeString = cmd.at(2);
	modeArguments = fillModeArguments(cmd);
	if ((cmd.at(1)[0] == '#' || cmd.at(1)[0] == '&') && this->_channelsList.find(cmd.at(1)) != this->_channelsList.end())
		modeCommandChannel(modeString, modeArguments, clientFd, this->_channelsList.find(cmd.at(1))->second);
	else
		std::cout << "Error 403 no such channel" << std::endl;
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