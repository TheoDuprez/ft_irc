/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/08 10:08:44 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netinet/in.h> // For socket data struct
#include <cstring> // For memset function
#include <csignal> // For signal function
#include <cstdlib>
#include <sstream>
#include <errno.h>
#include <limits>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <map>
#include <locale>  // std::locale - std::isalpha
#include <fstream> // std::ofstream
#include <ctime> // std::localtime
#include "Client.hpp"

#define IP 0
#define NO_FLAG 0
#define MESSAGE_SIZE 512
#define PENDING_QUEUE 50
#define POLL_NO_TIMEOUT -1
#define USERLEN 12
#define ERROR true
#define OK false
#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

typedef std::string::iterator	stringIterator;

typedef std::map<int, Client*>	clientMap;
typedef clientMap::iterator		clientIterator;

typedef std::vector<pollfd>		pollVector;
typedef	pollVector::iterator	pollIterator;

class Server
{
	private:
		std::ofstream	_logFile;
		clientMap		_clients;
		pollVector		_pollFds;
		unsigned short	_port;
		sockaddr_in		_serverAddress;
		socklen_t		_serverAddressSize;
		std::string		_password;
		static bool		_isServUp;

		Server(void);
		Server(const Server& obj);
		Server& operator=(const Server& obj);

		bool	_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd);

	public:
		Server(char* port, std::string password);
		~Server(void);

		void				initServer(void);
		void				launchServer(void);
		static void			stopServer(int);
		void				createPollFd(int fd);
		void				serverLoop(void);
		void				acceptClient(void);
		pollfd				&getPollFd(void);
		void				printLogMessage(std::string message, bool isError);
		std::string	const	getCurrentTimeStamp(void);
		void				clientManager(void);
		void				passCommand(std::vector<std::string> cmd, int fd);
		void				nickCommand(std::vector<std::string> cmd, int fd);
		void				userCommand(std::vector<std::string> cmd, int fd);
};

#endif
