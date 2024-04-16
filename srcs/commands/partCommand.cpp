#include "Server.hpp"

void	Server::partCommand(commandTokensVector cmd, Client* clientPtr)
{
	std::vector<std::string>			channelsNameList;
	std::string							partReason;

	if (cmd.size() == 1) {
		sendMessage(clientPtr->getClientFd(), ERR_NEEDMOREPARAMS(clientPtr->getNickName(), cmd[0]));
		return ;
	}
	if (cmd.size() > 2) {
		for (size_t i = 2; i < cmd.size(); i++) {
			partReason += cmd[i];
			if (i + 1 != cmd.size())
				partReason += " ";
		}
	}

	channelsNameList = split(*(cmd.begin() + 1), ',');
	for (std::vector<std::string>::iterator it = channelsNameList.begin(); it != channelsNameList.end(); it++) {
		if (!getChannelByName(*it)) {
			sendMessage(clientPtr->getClientFd(), ERR_NOSUCHCHANNEL(clientPtr->getNickName(), *it));
		}
		else if (!getChannelByName(*it)->isClientExist(clientPtr)) {
			sendMessage(clientPtr->getClientFd(), ERR_NOTONCHANNEL(clientPtr->getNickName(), *it));
		}
		else {
			if (partReason.empty())
				_sendMessageToAllClients(getChannelByName(*it), PART_MESSAGE(clientPtr->getNickName(), *it));
			else
				_sendMessageToAllClients(getChannelByName(*it), PART_MESSAGE_REASON(clientPtr->getNickName(), *it, partReason));
			_leaveChannel(clientPtr, getChannelByName(*it));
		}
	}
}

void	Server::_leaveChannel(Client* clientPtr, Channel* channelPtr)
{
	if (channelPtr->getClientsDataMap().size() == 1) {
		this->_channelsMap.erase(channelPtr->getChannelName());
		delete channelPtr;
	}
	else
		channelPtr->removeClient(clientPtr->getNickName());
}