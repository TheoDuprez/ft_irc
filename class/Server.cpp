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

Server::Server(void)
{
}

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

Server::Server(const Server& obj)
{
	static_cast<void>(obj);
}

Server::~Server(void)
{
	this->_logFile.close();
	for (pollIterator it = this->_pollFds.begin(); it != this->_pollFds.end(); it++)
		close(it->fd);
	for (clientIterator it = this->_clients.begin(); it != this->_clients.end(); it++)
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

void	Server::acceptClient(void) 	
{
	int		clientFd;
	
	clientFd = accept(this->_pollFds[0].fd, reinterpret_cast<sockaddr*>(&this->_serverAddress), &this->_serverAddressSize);
	if (clientFd == -1)
		throw (std::runtime_error(strerror(errno)));
	createPollFd(clientFd);
	this->_clients.insert(std::make_pair(clientFd, new Client()));
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

void	Server::clientManager(void)
{
	int recvReturn;
	char	buffer[MESSAGE_SIZE] = {0};
	
	for (pollIterator it = this->_pollFds.begin() + 1; it != this->_pollFds.end(); it++) {
		if (it->revents & POLLIN) {
			recvReturn = recv(it->fd, &buffer, MESSAGE_SIZE, NO_FLAG);
			if (recvReturn == -1) 
				throw (std::runtime_error(strerror(errno)));
			else if (recvReturn == 0) {
				break;
				//Here close the client connection with ERROR COMMAND
			} else {
				std::vector<std::string>	test;
				test.push_back("USER");
				test.push_back("testuserdelalongeur");
				test.push_back("0");
				test.push_back("*");
				test.push_back("testreal");
				// std::cout << "test\n";
				// passCommand(test, it->fd);
				// nickCommand(test, it->fd);
				userCommand(test, it->fd);
				//printLogMessage(std::string(buffer), false);
			}
		}
    }
}

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
	// - TEST OK
	Client	*currentClient = this->_clients[fd];

	if (currentClient->getIsRegister() == true) {
		this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
		return;
	}
	else if (cmd.size() != 2 || cmd[1].empty()) {
		this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
		return;
	}
	if (this->_password.compare(cmd[1].c_str())) {
		this->printLogMessage("ERR_PASSWDMISMATCH (464)\n", ERROR);
		//Here close the client connection with ERROR COMMAND
	}
	currentClient->setServerPassword(cmd[1]);
	// std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
}

void	Server::nickCommand(std::vector<std::string> cmd, int fd)
{
	// NEED to test first for loop when parsing are down
	// the rest of the function is OK
	std::locale	loc;
	std::string excluded = "#&: ";
	Client	*currentClient = this->_clients[fd];

	if (cmd.size() != 2 || cmd[1].empty()) {
		this->printLogMessage("ERR_NONICKNAMEGIVEN (431)\n", ERROR);
		return;
	}
	for (clientIterator	it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (!cmd[1].compare(it->second->getNickName())) {
			this->printLogMessage("ERR_NICKNAMEINUSE (433)\n", ERROR);
			return;
		}
	}
	// std::cout << "nickcmd test: cmd[1]: " << cmd[1] << std::endl;
	for (stringIterator it = cmd[1].begin(); it != cmd[1].end(); it++) {
		// std::cout << "nickcmd test: actual char: " << *it << std::endl;
		if ((it == cmd[1].begin() && (std::isdigit(*it, loc) || cmd[1].find_first_of(excluded) != std::string::npos)) || !std::isalnum(*it, loc)) {
			this->printLogMessage("ERR_ERRONEUSNICKNAME (432)\n", ERROR);
			return;
		}
	}
	this->printLogMessage("<past nickName> NICK <new nickName>\n", OK);
	currentClient->setnickName(cmd[1]);
	// std::cout << "nickcmd test: nick set as: " << currentClient->getNickName() << std::endl;
}

bool	Server::_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd)
{
	switch (i) {
		case 1:
			if ((*cmd)[1].empty()) {
				this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
				return false;
			}
			(*cmd)[1] = '~' + (*cmd)[1];
			if ((*cmd)[1].size() > USERLEN)
				(*cmd)[1] = (*cmd)[1].substr(0, USERLEN);
			break;
		case 2:
			if ((*cmd)[2].size() != 1 || (*cmd)[2].at(0) != '0') {
				this->printLogMessage("userCommand: invalid command, usage: USER <USERNAME> 0 * :<REALNAME>\n", ERROR);
				return false;
			}
			break;
		case 3:
			if ((*cmd)[3].size() != 1 || (*cmd)[3].at(0) != '*') {
				this->printLogMessage("userCommand: invalid command, usage: USER <USERNAME> 0 * :<REALNAME>\n", ERROR);
				return false;
			}
			break;
		case 4:
			if ((*cmd)[4].empty() || ((*cmd)[4].at(0) == ':' && (*cmd)[4].size() == 1)) {
				if (!currentClient->getNickName().empty())
					(*cmd)[4] = currentClient->getNickName();
				else {
					this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
					return false;
				}
			}
			// necessary only if parsing return a ':'
			// if ((*cmd)[4].at(0) == ':') {
			// 		(*cmd)[4] = (*cmd)[4].substr(1, (*cmd)[4].size());
			// }
			break;
	}
	return (true);
}

void	Server::userCommand(std::vector<std::string> cmd, int fd)
{
	// TEST OK
	Client	*currentClient = this->_clients[fd];
	// We need to implement the Ident protocol here, but since our server is local, I don't think it's necessary.
	
	if (currentClient->getIsRegister() == true) {
		this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
		return;
	}
	if (cmd.size() != 5) {
		this->printLogMessage("userCommand function: Can't find the appropriate return\n", ERROR);
		return;
	}
	for (size_t i = 1; i != cmd.size(); i++) {
		if (this->_isValidUserCommand(i, currentClient, &cmd) == false)
			return;
	}
	currentClient->setuserName(cmd[1]);
	currentClient->setrealName(cmd[4]);
	if (!currentClient->getServerPassword().empty() && !currentClient->getNickName().empty())
		currentClient->setIsRegister(true);
	// else {
	// 	//error to handle, no pswd or nick for registration how manage this ?
	// }
	// std::cout << "usercmd test: username set as: " << currentClient->getUserName() << std::endl;
	// std::cout << "usercmd test: realname set as: " << currentClient->getRealName() << std::endl;
	// std::cout << "usercmd test: isRegister set as: " << std::string(currentClient->getIsRegister() ? "true" : "false") << std::endl;

}

bool	Server::_isServUp;
