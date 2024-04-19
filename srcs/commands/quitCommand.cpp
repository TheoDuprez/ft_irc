#include "Server.hpp"

static void    quitBroadcast(Client *client, const clientsMap &clientsDataMap, const std::string &reason, const std::string &nick);
static void	removeClientFromAllChannels(channelsMap &channels, Client *client, std::vector<std::string> &cmd);
static void	removeClientFromContacted(clientMap &clients, Client *client);
static void	removePollFd(int fd, pollVector &pollfds);

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
	removeClientFromAllChannels(this->_channelsMap, client, cmd);

	//erase currentclient from all mpUsers concerned
	removeClientFromContacted(this->_clients, client);

	sendMessage(fd, QUIT_CONNECTION);
	this->removeClientFromServer(fd);

	/* remove client's pollfd from server's pollVector */
	removePollFd(fd, this->_pollFds);
}

static void    quitBroadcast(Client *client, const clientsMap &clientsDataMap, const std::string &reason, const std::string &nick)
{
	std::vector<int> &messagedFds = client->getQuitMessageSentFd();

    for (clientsMap::const_iterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
		std::vector<int>::iterator fdIt = std::find(messagedFds.begin(), messagedFds.end(), fd);
		if (fdIt == messagedFds.end()) {
			sendMessage(fd, QUIT_BROADCAST(nick, reason));
			messagedFds.push_back(fd);
		}
    }
}

static void	removeClientFromAllChannels(channelsMap &channels, Client *client, std::vector<std::string> &cmd) 
{
	std::string clientNick = client->getNickName();
	for (channelsMap::iterator i = channels.begin(); i != channels.end();) {  // iterate through all the channels on the server
		const clientsMap &clients = i->second->getClientsDataMap();

		if (i->second->isInvitedClient(clientNick))
			i->second->unsetInvitedClients(clientNick);

		if (i->second->removeClient(clientNick)) {
			/* if channel is empty remove from channels */
			delete i->second;
			channels.erase(i++);
		} else {
			std::string reason = "Quit: " + cmd[1].substr(1);
			for (std::vector<std::string>::iterator it = cmd.begin() + 2; it != cmd.end(); it++) {
				reason += " " + *it;
			}
			quitBroadcast(client, clients, reason, clientNick);
			i++;
		}
	}
}

static void	removeClientFromContacted(clientMap &clients, Client *client)
{
	for (clientMap::iterator clientIt = clients.begin(); clientIt != clients.end(); clientIt++) {
		if (clientIt->second->getContactedClientByNick(client->getNickName()))
			clientIt->second->setContactedClient(client, MP_UNSET);
	}
}

static void	removePollFd(int fd, pollVector &pollfds)
{
	for (pollVector::iterator it = pollfds.begin(); it != pollfds.end();) {
		(it->fd == fd) ? it = pollfds.erase(it) : it++;
		if (it == pollfds.end()) {
			throw QuitClientException();
		}
	}
}