#include "Server.hpp"

void    quitBroadcast(const clientsMap &clientsDataMap, const std::string &reason, const std::string &nick);
void	removeClientFromAllChannels(channelsMap &channels, const std::string &client, std::vector<std::string> &cmd);
void	removeClientFromContacted(clientMap &clients, Client *client);

void	Server::quitCommand(std::vector<std::string> cmd, Client *client)
{

	/* QUIT without arguments */
	int fd = client->getClientFd(); // get client's fd for further use
	if (cmd.size() < 2 || cmd[1][0] != ':') {
		sendMessage(fd, QUIT_USAGE);
		return ;
	}
	
	std::string clientNick = client->getNickName();

	/* delete client from all channels */
	removeClientFromAllChannels(this->_channelsMap, clientNick, cmd);

	//erase currentclient from all mpUsers concerned
	removeClientFromContacted(this->_clients, client);

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
	static std::vector<int> messagedFds;

    for (clientsMap::const_iterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
		std::vector<int>::iterator fdIt = std::find(messagedFds.begin(), messagedFds.end(), fd);
		if (fdIt == messagedFds.end()) {
			sendMessage(fd, QUIT_BROADCAST(nick, reason));
			messagedFds.push_back(fd);
		}
    }
}

void	removeClientFromAllChannels(channelsMap &channels, const std::string &client, std::vector<std::string> &cmd) 
{
	for (channelsMap::iterator i = channels.begin(); i != channels.end();) {  // iterate through all the channels on the server
		const clientsMap &clients = i->second->getClientsDataMap();

		if (i->second->isInvitedClient(client))
			i->second->unsetInvitedClients(client);

		if (i->second->removeClient(client)) {
			/* if channel is empty remove from channels */
			delete i->second;
			channels.erase(i++);
		} else {
			std::string reason = "Quit: " + cmd[1].substr(1);
			for (std::vector<std::string>::iterator it = cmd.begin() + 2; it != cmd.end(); it++) {
				reason += " " + *it;
			}
			quitBroadcast(clients, reason, client);
			i++;
		}
	}
}

void	removeClientFromContacted(clientMap &clients, Client *client)
{
	for (clientMap::iterator clientIt = clients.begin(); clientIt != clients.end(); clientIt++) {
		if (clientIt->second->getContactedClientByNick(client->getNickName()))
			clientIt->second->setContactedClient(client, MP_UNSET);
	}
}
