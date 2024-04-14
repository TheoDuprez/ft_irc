#include "Server.hpp"

void    quitBroadcast(const clientsMap &clientsDataMap, const std::string &reason, const std::string &chan, const std::string &nick);

void	Server::quitCommand(std::vector<std::string> cmd, Client *client)
{
	 // --- Debug message ---
    std::cout << std::endl;
    std::cout << " ----- Input of quitCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;

	/* QUIT without arguments */
	int fd = client->getClientFd(); // get client's fd for further use
	// channelsMap &channels = this->_channelsMap; // get server's channlesMap for further use

	/* delete client from all channels */
	for (channelsMap::iterator i = this->_channelsMap.begin(); i != this->_channelsMap.end(); ++i) {  // iterate through all the channels on the server
		const clientsMap &clients = i->second->getClientsDataMap();

		if (i->second->removeClient(client->getNickName())) {
			/* if channel is empty remove from channels */
			this->_channelsMap.erase(i++);
		}
		if (i != this->_channelsMap.end()) {
			quitBroadcast(clients, cmd[1], i->second->getChannelName(), client->getNickName());
		}
	}
	sendMessage(fd, ":server ERROR : Connection closed by server.");
	close(fd); // close the fd of the current client and disconnect the socket
	delete this->_clients.at(fd); // delete the client object
	this->_clients.erase(fd);
}

void    quitBroadcast(const clientsMap &clientsDataMap, const std::string &reason, const std::string &chan, const std::string &nick)
{
    for (clientsMap::const_iterator it = clientsDataMap.begin(); it != clientsDataMap.end(); it++) {
        int fd = it->second->getClient()->getClientFd();
        sendMessage(fd, ":" + nick + " QUIT " + chan + " :" + reason);
    }
}

void	Server::printAllChannelClients()
{
	for (channelsMap::iterator it = this->_channelsMap.begin(); it != this->_channelsMap.end(); ++it) {
		std::cout << "Channel is : " << it->second->getChannelName() << std::endl;
		const clientsMap &clients = it->second->getClientsDataMap();
 		for (clientsMap::const_iterator j = clients.begin(); j != clients.end(); j++) {
			std::cout << j->second->getClient()->getNickName() << std::endl;
		}
	}
}
