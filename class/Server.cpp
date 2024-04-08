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

// ---------------------------------------------------------------------------------------------- //

std::vector<std::vector<std::string> >            Server::createCmdVector(std::string buffer) // function to hard code parsing, not definitive
{
	std::vector<std::vector<std::string> >	retVectorCommands;
	std::string								tempBuffer(buffer);
	std::vector<std::string>				test(split(buffer, '\n'));

	for (std::vector<std::string>::iterator it = test.begin(); it != test.end(); it++) {
		retVectorCommands.push_back(split(it->substr(0, it->length() - 1), ' '));
	}
    // std::vector<std::string> vec(split(buffer.substr(0, buffer.length() - 2), ' '));

    return retVectorCommands;
}

void                Server::handleCommand(std::vector<std::vector<std::string> > cmd, Client* client) // Handle command function, waiting for a parsing logic to implement it correctly
{
	for (std::vector<std::vector<std::string> >::iterator it = cmd.begin(); it != cmd.end(); it++) {
		if (it->at(0) == "JOIN")
			join(std::vector<std::string>(it->begin() + 1, it->end()), client);
		else if (it->at(0) == "USER")
			userCommand(*it, client->getClientFd());
		else if (it->at(0) == "NICK")
			nickCommand(*it, client->getClientFd());
		else if (it->at(0) == "PASS")
			passCommand(*it, client->getClientFd());
		else
			std::cout << "Error: " << it->at(0) << " is not a command" << std::endl;
	}
		// std::vector<std::string>	test;
	// test.push_back("USER");
	// test.push_back("testuserdelalongeur");
	// test.push_back("0");
	// test.push_back("*");
	// test.push_back("testreal");
	// // std::cout << "test\n";
	// // passCommand(test, it->fd);
	// // nickCommand(test, it->fd);
	// userCommand(test, it->fd);
	// //printLogMessage(std::string(buffer), false);
    // else if (cmd.at(0) == "PRIVMSG") { // Quick implementation of PRIVMSG to test
    //     for (size_t i = 0; i < this->_clientList.size(); i++) {
    //         if (this->_clientList[i] != client)
    //             sendMessage(this->_clientList[i]->getClientFd(), ":tduprez PRIVMSG #test " + cmd.at(2));
    //     }
    // }
}

void                Server::join(cmdVector cmd, Client* client)
{
    std::vector<std::string>            channelsNameList(split(*cmd.begin(), ','));
    std::vector<std::string>            channelsPasswordList;
    std::string                         password;

    if (cmd.size() > 1)
        channelsPasswordList = std::vector<std::string>(split(*(cmd.begin() + 1), ','));

    for (size_t i = 0; i < channelsNameList.size(); i++) { // To handle all client given to the join command
        password = (i < channelsPasswordList.size()) ? channelsPasswordList[i] : ""; // Give a password to the channel if specified, otherwise "" is given ("" mean no password for the Channel)

        if (channelsNameList[i].at(0) != '#' && channelsNameList[i].at(0) != '&') // Create error if channel name isn't ok
            sendMessage(client->getClientFd(), ":server 403 tduprez " + channelsNameList[i] + ": Invalid channel name");
        else if (this->_channelsList.find(channelsNameList[i]) == this->_channelsList.end()) { // Create channel if not exist
            this->_channelsList.insert(std::make_pair(channelsNameList[i], new Channel(channelsNameList[i], client)));
            sendMessage(client->getClientFd(), ":tduprez JOIN " + channelsNameList[i]);
        }
        else if (this->_channelsList.find(channelsNameList[i]) != this->_channelsList.end()) { // Join channel if exist
            if (this->_channelsList.find(channelsNameList[i])->second->addClient(client, password))
                sendMessage(client->getClientFd(), ":tduprez2 JOIN " + channelsNameList[i]);
            else
                std::cout << "Error while joining the server : bad password" << std::endl;
        }
    }
}

void    Server::sendMessage(int fd, std::string msg)
{
    msg += "\r\n";
    std::cout << "Message send to client is : " << msg << std::endl;
    send(fd, msg.c_str(), msg.size(), NO_FLAG);
}

// ---------------------------------------------------------------------------------------------- //

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
				handleCommand(createCmdVector(buffer), this->_clients.at(it->fd));
				// std::vector<std::string>	test;
				// test.push_back("USER");
				// test.push_back("testuserdelalongeur");
				// test.push_back("0");
				// test.push_back("*");
				// test.push_back("testreal");
				// // std::cout << "test\n";
				// // passCommand(test, it->fd);
				// // nickCommand(test, it->fd);
				// userCommand(test, it->fd);
				// //printLogMessage(std::string(buffer), false);
			}
		}
    }
}

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
	std::cout << " ----- Input of passCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
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
		std::cout << "Password = " << this->_password << " | password try = " << cmd[1].c_str() << std::endl;
		this->printLogMessage("ERR_PASSWDMISMATCH (464)\n", ERROR);
		//Here close the client connection with ERROR COMMAND
	}
	currentClient->setServerPassword(cmd[1]);
	// std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
}

void	Server::nickCommand(std::vector<std::string> cmd, int fd)
{
	std::cout << " ----- Input of nickCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
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
	std::cout << " ----- Input of userCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
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
