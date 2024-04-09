/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:18:42 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/09 23:59:03 by acarlott         ###   ########lyon.fr   */
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
		if (it->at(0) == "JOIN" && client->getIsRegister())
			join(std::vector<std::string>(it->begin() + 1, it->end()), client);
		else if (it->at(0) == "USER")
			userCommand(*it, client->getClientFd());
		else if (it->at(0) == "NICK")
			nickCommand(*it, client->getClientFd());
		else if (it->at(0) == "PASS")
			passCommand(*it, client->getClientFd());
        else if (it->at(0) == "PRIVMSG")
            privmsg(*it, client);
		else
			std::cout << "Error: " << it->at(0) << " is not a command. Full cmd is : " << std::endl;
            for (std::vector<std::string>::iterator itTest = it->begin(); itTest != it->end(); itTest++)
                std::cout << *itTest << " ";
            std::cout << std::endl;
	}
}

void                Server::privmsg(cmdVector cmd, Client* client)
{
    std::string channelName;

    if (cmd.at(1).find("#") != std::string::npos || cmd.at(1).find("&")) {// Is a channel target
        std::cout << "Is good\n";
        this->_channelsList.find(cmd.at(1))->second->privmsg(cmd, client);
//        channelName = cmd.at(1).find("#")
    }
//    sendMessage()
}

void                Server::join(cmdVector cmd, Client* client)
{
    std::vector<std::string>            channelsNameList(split(*cmd.begin(), ','));
    std::vector<std::string>::iterator  nameIt;
    std::vector<std::string>            channelsPasswordList;
    std::vector<std::string>::iterator  passwordIt;
    std::string                         password;

    if (cmd.size() > 1)
        channelsPasswordList = std::vector<std::string>(split(*(cmd.begin() + 1), ','));

    nameIt = channelsNameList.begin();
    passwordIt = channelsPasswordList.begin();
    for (; nameIt != channelsNameList.end(); nameIt++) {
        password = (passwordIt != channelsPasswordList.end()) ? *(passwordIt++) : "";

        if (nameIt->at(0) != '#' && nameIt->at(0) != '&') {
            sendMessage(client->getClientFd(), ":server 403 " + client->getNickName() + " " + *nameIt + ": Invalid channel name");
        }
        else if (this->_channelsList.find(*nameIt) == this->_channelsList.end()) {
            this->_channelsList.insert(std::make_pair(*nameIt, new Channel(*nameIt, client)));
        }
        else if (this->_channelsList.find(*nameIt) != this->_channelsList.end()) {
            if (!this->_channelsList.find(*nameIt)->second->addClient(client, password))
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
				//Here close the client connection with ERROR messqge check irc doc
			} else
				handleCommand(createCmdVector(buffer), this->_clients.at(it->fd));
		}
    }
}

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
	// --- Debug message ---
	std::cout << " ----- Input of passCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
	// ---------------------
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
	// --- Debug message ---
	// std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
	// ---------------------
}

void	Server::nickCommand(std::vector<std::string> cmd, int fd)
{
	// --- Debug message ---
	std::cout << " ----- Input of nickCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
	// ---------------------
	std::locale	loc;
	std::string excluded = "#&: ";
	Client	*currentClient = this->_clients[fd];

	if (cmd.size() != 2 || cmd[1].empty()) {
		this->printLogMessage("ERR_NONICKNAMEGIVEN (431)\n", ERROR);
		return;
	}
	for (clientIterator	it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (!cmd[1].compare(it->second->getNickName())) {
            sendMessage(fd, ":server 433 * nickname :" + cmd[1] + " is already in use");
			this->printLogMessage("ERR_NICKNAMEINUSE (433)\n", ERROR);
			return;
		}
	}
	for (stringIterator it = cmd[1].begin(); it != cmd[1].end(); it++) {
		if ((it == cmd[1].begin() && std::isdigit(*it, loc)) || cmd[1].find_first_of(excluded) != std::string::npos || !std::isprint(*it, loc)) {
			this->printLogMessage("ERR_ERRONEUSNICKNAME (432)\n", ERROR);
			return;
		}
	}
	this->printLogMessage("<past nickName> NICK <new nickName>\n", OK);
	currentClient->setnickName(cmd[1]);
    if (!currentClient->getIsRegister() && !currentClient->getServerPassword().empty() && !currentClient->getUserName().empty() && !currentClient->getRealName().empty()) {
        sendMessage(fd, ":server 001 " + currentClient->getNickName() + " :Welcome to the localhost Network, " + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost");
        currentClient->setIsRegister(true);
    }
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
				if (!currentClient->getNickName().empty()) {
					(*cmd)[4] = currentClient->getNickName();
				}
				else {
					this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
					return false;
				}	
			}
			if ((*cmd)[4].at(0) == ':') {
					(*cmd)[4] = (*cmd)[4].substr(1, (*cmd)[4].size());
			}
			break;
	}
	return (true);
}

void	Server::userCommand(std::vector<std::string> cmd, int fd)
{
	// --- Debug message ---
	std::cout << " ----- Input of userCommand ----- " << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << cmd[i] << " ";
	std::cout << std::endl;
	// ---------------------
	Client	*currentClient = this->_clients[fd];
		
	if (currentClient->getIsRegister() == true) {
		this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
		return;
	}
	if (cmd.size() < 5) {
		sendMessage(fd, "USER :Not enough parameters");
		return;
	}
	for (size_t i = 1; i != cmd.size(); i++) {
		if (this->_isValidUserCommand(i, currentClient, &cmd) == false)
			return;
	}
	currentClient->setuserName(cmd[1]);
	currentClient->setrealName(cmd[4]);
	if (!currentClient->getServerPassword().empty() && !currentClient->getNickName().empty()) {
        sendMessage(fd, ":server 001 " + currentClient->getNickName() + " :Welcome to the localhost Network, " + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost");
		currentClient->setIsRegister(true);
    }
	// std::cout << "usercmd test: username set as: " << currentClient->getUserName() << std::endl;
	// std::cout << "usercmd test: realname set as: " << currentClient->getRealName() << std::endl;
	// std::cout << "usercmd test: isRegister set as: " << std::string(currentClient->getIsRegister() ? "true" : "false") << std::endl;

}

bool	Server::_isServUp;
