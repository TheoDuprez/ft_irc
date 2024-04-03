/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tduprez <tduprez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:18:42 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/03 17:55:15 by tduprez          ###   ########lyon.fr   */
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
	Server::_isRunning = false;
}

Server::Server(const Server& obj)
{
	static_cast<void>(obj);
}

Server::~Server(void)
{
}

void	Server::initServer(void)
{
	this->_serverAddressSize = sizeof(this->_serverAddress);
	this->_serverFd = socket(AF_INET, SOCK_STREAM, IP);
	if (this->_serverFd == -1)
		throw (std::runtime_error(strerror(errno)));
	memset(&this->_serverAddress, 0, this->_serverAddressSize);
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(this->_port);
	if (bind(this->_serverFd, reinterpret_cast<sockaddr*>(&this->_serverAddress), this->_serverAddressSize) == -1)
		throw (std::runtime_error(strerror(errno)));
}

void	Server::launchServer(void)
{
	if (listen(this->_serverFd, PENDING_QUEUE) == -1)
		throw (std::runtime_error(strerror(errno)));
	this->_clientFd = accept(this->_serverFd, reinterpret_cast<sockaddr*>(&this->_serverAddress), &this->_serverAddressSize);
	if (this->_clientFd == -1)
		throw (std::runtime_error(strerror(errno)));

	this->_isRunning = true;
	std::signal(SIGINT, Server::stopServer);
	while (this->_isRunning == true)
	{
		char	buffer[MESSAGE_SIZE] = {0};
		recv(this->_clientFd, &buffer, MESSAGE_SIZE, NO_FLAG);
		std::cout << buffer;
	}
	close(this->_serverFd);
	close(this->_clientFd);
}

void	Server::stopServer(int signum)
{
	static_cast<void>(signum);
	Server::_isRunning = false;
}

bool	Server::_isRunning;
