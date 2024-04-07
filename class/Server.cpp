/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:18:42 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/05 14:18:28 by acarlott         ###   ########lyon.fr   */
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
	int	recvReturn;
	
	while (this->_isServUp == true)
	{
		char	buffer[MESSAGE_SIZE] = {0};
		if (poll(this->_pollFds.data(), this->_pollFds.size(), POLL_NO_TIMEOUT) == -1) {
			if (this->_isServUp == true)
				throw (std::runtime_error(strerror(errno)));
		}
		else if (this->_pollFds[0].revents & POLLIN)
			acceptClient();
		else {
            int i = 0; // Variable to identify a the client, temporary while waiting for a Client map
			for (pollIterator it = this->_pollFds.begin() + 1; it != this->_pollFds.end() && i != -1; it++, i++) {
				if (it->revents & POLLIN) {
					recvReturn = recv(it->fd, &buffer, MESSAGE_SIZE, NO_FLAG);
					if (recvReturn == -1)
						throw (std::runtime_error(strerror(errno)));
					else if (recvReturn == 0) {
						break;
					} else {
                        std::cout << "Buffer = " << buffer << std::endl;
                        handleCommand(createCmdVector(buffer), this->_clientList.at(i));
//						printLogMessage(std::string(buffer), false);
					}
				}
            }
		}
	}
}

// ---------------------------------------------------------------------------------------------- //

cmdVector            Server::createCmdVector(std::string buffer) // function to hard code parsing, not definitive
{
    std::vector<std::string> vec(split(buffer.substr(0, buffer.length() - 2), ' '));

    return vec;
}

void                Server::handleCommand(cmdVector cmd, Client* client) // Handle command function, waiting for a parsing logic to implement it correctly
{
    if (cmd.at(0) == "JOIN")
        join(std::vector<std::string>(cmd.begin() + 1, cmd.end()), client);
    else if (cmd.at(0) == "PRIVMSG") { // Quick implementation of PRIVMSG to test
        for (size_t i = 0; i < this->_clientList.size(); i++) {
            if (this->_clientList[i] != client)
                sendMessage(this->_clientList[i]->getFd(), ":tduprez PRIVMSG #test " + cmd.at(2));
        }
    }
    else
        std::cout << "Error: " << cmd.at(0) << " is not a command" << std::endl;
}

void                Server::join(cmdVector cmd, Client* client)
{
    std::vector<std::string>            channels(split(*cmd.begin(), ','));
    std::vector<std::string>            channelsPassword;
    std::string                         password;

    if (cmd.size() > 1)
        channelsPassword = std::vector<std::string>(split(*(cmd.begin() + 1), ','));

    for (size_t i = 0; i < channels.size(); i++) { // To handle all client given to the join command
        password = (i < channelsPassword.size()) ? channelsPassword[i] : ""; // Give a password to the channel if specified, otherwise "" is given ("" mean no password for the Channel)

        if (channels[i].at(0) != '#' && channels[i].at(0) != '&') // Create error if channel name isn't ok
            sendMessage(client->getFd(), ":server 403 tduprez " + channels[i] + ": Invalid channel name");
        else if (this->_channelsList.find(channels[i]) == this->_channelsList.end()) { // Create channel if not exist
            this->_channelsList.insert(std::make_pair(channels[i], new Channel(channels[i], client)));
            sendMessage(client->getFd(), ":tduprez JOIN " + channels[i]);
        }
        else if (this->_channelsList.find(channels[i]) != this->_channelsList.end()) { // Join channel if exist
            if (this->_channelsList.find(channels[i])->second->addClient(client, password))
                sendMessage(client->getFd(), ":tduprez2 JOIN " + channels[i]);
            else
            std::cout << "Error while joining the server : bad password" << std::endl;
        }
    }
}

std::vector<std::string> Server::split(const std::string& str, char delim)
{
    std::vector<std::string>    result;
    std::string                 token;
    std::istringstream          tokenSource(str);

    while (std::getline(tokenSource, token, delim)) {
        result.push_back(token);
    }
    return result;
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
	this->_clientList.push_back(new Client(this->getPollFd()));
}

pollfd		&Server::getPollFd(void)
{
	return (*(this->_pollFds.end() - 1));
}

void		Server::printLogMessage(std::string message, bool isError)
{
	this->_logFile << "[" << this->getCurrentTimeStamp() << "]" << std::flush;
	if (isError == true)
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


bool	Server::_isServUp;
