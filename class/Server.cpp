/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:18:42 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/04 23:05:44 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{

}

Server::Server(char* port, std::string password): _password(password)
{
	long double	tempPort;

	for (size_t i = 0; port[i]; ++i) {
		if (!isdigit(port[i]))
			throw (std::runtime_error("Bad port"));
	}
	tempPort = std::strtold(port, NULL);
	if (tempPort > std::numeric_limits<unsigned short>::max() || tempPort <= 1024)
		throw (std::runtime_error("Bad port"));
	this->_port = atoi(port);
	Server::_isServUp = false;
}

Server::Server(const Server& obj)
{
	static_cast<void>(obj);
}

Server::~Server(void)
{
	for (pollIterator it = this->_pollFds.begin(); it != this->_pollFds.end(); it++)
		close(it->fd);
    for (clientIterator it = this->_clientList.begin(); it != this->_clientList.end(); it++) {
		delete *it;
    }
}

void	Server::initServer(void)
{
	int	serverFd;
	this->_serverAddressSize = sizeof(this->_serverAddress);
	serverFd = socket(AF_INET, SOCK_STREAM, IP);
	if (serverFd == -1)
		throw (std::runtime_error(strerror(errno)));
	memset(&this->_serverAddress, 0, this->_serverAddressSize);
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(this->_port);
	if (bind(serverFd, reinterpret_cast<sockaddr*>(&this->_serverAddress), this->_serverAddressSize) == -1)
		throw (std::runtime_error(strerror(errno)));
	createPollFd(serverFd);
}

void	Server::launchServer(void)
{
	if (listen(this->_pollFds[0].fd, PENDING_QUEUE) == -1)
		throw (std::runtime_error(strerror(errno)));
	this->_isServUp = true;
	std::signal(SIGINT, Server::stopServer);
	serverLoop();
}

void	Server::stopServer(int signum)
{
	static_cast<void>(signum);
	Server::_isServUp = false;
}

void	Server::createPollFd(int fd)
{
	pollfd	fds;
	
	fds.events = POLLIN;
	fds.fd = fd;
	this->_pollFds.push_back(fds);
}

void	Server::serverLoop(void)
{
	
	while (this->_isServUp == true)
	{
		char	buffer[MESSAGE_SIZE] = {0};
		if (poll(this->_pollFds.data(), this->_pollFds.size(), POLL_NO_TIMEOUT) == -1 && this->_isServUp == true)
			throw (std::runtime_error(strerror(errno)));
		else if (this->_pollFds[0].revents & POLLIN)
			acceptClient();
		else {
			for (pollIterator it = this->_pollFds.begin() + 1; it != this->_pollFds.end(); it++) {
				if (it->revents & POLLIN) {
					recv(it->fd, &buffer, MESSAGE_SIZE, NO_FLAG);
					std::cout << buffer;
                    std::cout << "TEST\n";
                    std::cout << it->fd << std::endl;
				}
            }
		}
		std::cout << buffer;
	}
}

void	Server::acceptClient(void)
{
	int		clientFd;
	
	clientFd = accept(this->_pollFds[0].fd, reinterpret_cast<sockaddr*>(&this->_serverAddress), &this->_serverAddressSize);
	if (clientFd == -1)
		throw (std::runtime_error(strerror(errno)));
	createPollFd(clientFd);
	this->_clientList.push_back(new Client(this->getPollFd()));
}

pollfd		&Server::getPollFd(void)
{
	return (*(this->_pollFds.end() - 1));
}

bool	Server::_isServUp;
