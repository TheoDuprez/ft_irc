#include "Server.hpp"

void    quitBroadcast(const clientsMap &clientsDataMap, const std::string &reason, const std::string &nick);

void	Server::quitCommand(std::vector<std::string> cmd, Client *client)
{

	/* QUIT without arguments */
	int fd = client->getClientFd(); // get client's fd for further use
	if (cmd.size() != 2 || cmd[1][0] != ':') {
		sendMessage(fd, QUIT_USAGE);
		return ;
	}

	/* delete client from all channels */
	for (channelsMap::iterator i = this->_channelsMap.begin(); i != this->_channelsMap.end();) {  // iterate through all the channels on the server
		const clientsMap &clients = i->second->getClientsDataMap();

		if (i->second->removeClient(client->getNickName())) {
			/* if channel is empty remove from channels */
			delete i->second;
			this->_channelsMap.erase(i++);
		} else {
			std::string reason = "Quit: " + cmd[1].substr(1);
			quitBroadcast(clients, reason, client->getNickName());
			i++;
		}
	}
	for (channelsMap::iterator channelIt = this->_channelsMap.begin(); channelIt != this->_channelsMap.end(); channelIt++) {
		if (channelIt->second->isInvitedClient(client->getNickName()) == true)
			channelIt->second->unsetInvitedClients(client->getNickName());
	}
	//erase currentclient from all mpUsers concerned
	for (clientMap::iterator clientIt = this->_clients.begin(); clientIt != this->_clients.end(); clientIt++) {
		if (clientIt->second->getContactedClientByNick(client->getNickName()))
			clientIt->second->setContactedClient(client, MP_UNSET);
	}
	sendMessage(fd, QUIT_CONNECTION);
	close(fd); // close the fd of the current client and disconnect the socket
	delete this->_clients.at(fd); // delete the client object
	this->_clients.erase(fd);
	for (pollVector::iterator it = this->_pollFds.begin(); it != this->_pollFds.end();) {
		(it->fd == fd) ? it = this->_pollFds.erase(it) : it++;
		if (it == this->_pollFds.end()) {
			throw QuitClientException();
		}
	}
}

void    quitBroadcast(const clientsMap &clientsDataMap, const std::string &reason, const std::string &nick)
{
    for (clientsMap::const_iterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, QUIT_BROADCAST(nick, reason));
    }
}
