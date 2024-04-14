#include "Server.hpp"

void	Server::partCommand(commandTokensVector cmd, Client* client)
{
	std::vector<std::string>			channelsNameList;
	std::string							partReason;

	if (cmd.size() == 1) {
		sendMessage(client->getClientFd(), ERR_NEEDMOREPARAMS(client->getNickName(), cmd[0]));
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
			sendMessage(client->getClientFd(), ERR_NOSUCHCHANNEL(client->getNickName(), *it));
		}
		else if (!getChannelByName(*it)->isClientExist(client)) {
			sendMessage(client->getClientFd(), ERR_NOTONCHANNEL(client->getNickName(), *it));
		}
		else {
			if (partReason.empty())
				sendMessageToAllClients(getChannelByName(*it), PART_MESSAGE(client->getNickName(), *it));
			else
				sendMessageToAllClients(getChannelByName(*it), PART_MESSAGE_REASON(client->getNickName(), *it, partReason));
			leaveChannel(client, getChannelByName(*it));
		}
	}
}

void	Server::leaveChannel(Client* clientPtr, Channel* channelPtr)
{
	if (channelPtr->getClientsDataMap().size() == 1) {
		this->_channelsMap.erase(channelPtr->getChannelName());
		delete channelPtr;
	}
	else
		channelPtr->removeClient(clientPtr);
}