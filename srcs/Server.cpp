/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:18:42 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/08 12:06:43 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char* port, std::string password): _password(password)
{
	long double		tempPort;
	
	this->_logFile.open("server.log", std::ofstream::app);
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

Server::~Server(void)
{
	this->_logFile.close();
	for (pollVector::iterator it = this->_pollFds.begin(); it != this->_pollFds.end(); it++)
		close(it->fd);
	for (clientMap::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		delete it->second;
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
		if (poll(this->_pollFds.data(), this->_pollFds.size(), POLL_NO_TIMEOUT) == -1) {
			if (this->_isServUp == true)
				throw (std::runtime_error(strerror(errno)));
		}
		else if (this->_pollFds[0].revents & POLLIN)
			acceptClient();
		else
			clientManager();
	}
}

commandsVector  Server::createCommandsVector(std::string buffer) // function to hard code parsing, not definitive
{
	commandsVector              retVectorCommands;
	std::vector<std::string>    commands(split(buffer, '\n'));

	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); it++) {
		retVectorCommands.push_back(split(it->substr(0, it->length() - 1), ' '));
	}

    return retVectorCommands;
}

void                Server::handleCommand(commandsVector commands, Client* client)
{
	for (commandsVector::iterator it = commands.begin(); it != commands.end(); it++) {
		if (it->at(0) == "JOIN" && client->getIsRegister())
			joinCommand(*it, client);
        else if (it->at(0) == "MODE")
            modeCommand(*it, client->getClientFd());
		else if (it->at(0) == "USER")
			userCommand(*it, client->getClientFd());
		else if (it->at(0) == "NICK")
			nickCommand(*it, client->getClientFd());
		else if (it->at(0) == "PASS")
			passCommand(*it, client->getClientFd());
        else if (it->at(0) == "PRIVMSG")
            privmsgCommand(*it, client);
		else
			std::cout << "Error: " << it->at(0) << " is not a command. Full cmd is : " << std::endl;
            for (commandTokensVector::iterator itTest = it->begin(); itTest != it->end(); itTest++)
                std::cout << *itTest << " ";
            std::cout << std::endl;
	}
}

void	Server::acceptClient(void) 	
{
	int		clientFd;

	clientFd = accept(this->_pollFds[0].fd, reinterpret_cast<sockaddr*>(&this->_serverAddress), &this->_serverAddressSize);
	if (clientFd == -1)
		throw (std::runtime_error(strerror(errno)));
	createPollFd(clientFd);
	this->_clients.insert(std::make_pair(clientFd, new Client(clientFd)));
}

pollfd		&Server::getPollFd(void)
{
	return (*(this->_pollFds.end() - 1));
}

void		Server::printLogMessage(std::string message, bool isError)
{
	this->_logFile << "[" << this->getCurrentTimeStamp() << "]" << std::flush;
	if (isError == ERROR)
		this->_logFile << " ERROR: " << message << std::flush;
	else
		this->_logFile << " " << message << std::flush;
}

std::string	const	Server::getCurrentTimeStamp(void)
{
	std::string retTime;
	std::time_t currentTime = std::time(NULL);
	std::tm *localTime = std::localtime(&currentTime);

	retTime = SSTR(localTime->tm_year + 1900) + "/" + SSTR(localTime->tm_mon) + "/" + SSTR(localTime->tm_mday) + " ";
	if (localTime->tm_mon < 10)
		retTime.insert(5, "0");
	if (localTime->tm_mday < 10)
		retTime.insert(8, "0");
	retTime = retTime + SSTR(localTime->tm_hour) + ":" + SSTR(localTime->tm_min) + ":" + SSTR(localTime->tm_sec);
	if (localTime->tm_hour < 10)
		retTime.insert(11, "0");
	if (localTime->tm_min < 10)
		retTime.insert(14, "0");
	if (localTime->tm_sec < 10)
		retTime.insert(17, "0");
	return retTime;
}

void	Server::clientManager(void) {
    int recvReturn;
    char buffer[MESSAGE_SIZE] = {0};

    for (pollVector::iterator it = this->_pollFds.begin() + 1; it != this->_pollFds.end(); it++) {
        if (it->revents & POLLIN) {
            recvReturn = recv(it->fd, &buffer, MESSAGE_SIZE, NO_FLAG);
            if (recvReturn == -1)
                throw (std::runtime_error(strerror(errno)));
            else if (recvReturn == 0) {
                break;
                //Here close the client connection with ERROR COMMAND
            } else {
                handleCommand(createCommandsVector(buffer), this->_clients.at(it->fd));
            }
        }
    }
}

bool	Server::_isServUp;