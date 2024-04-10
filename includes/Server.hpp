/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/10 21:29:24 by acarlott         ###   ########lyon.fr   */
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
#include <sstream> // std::ostringstream
#include <locale>  // std::locale - std::isalpha
#include <fstream> // std::ofstream
#include <ctime> // std::localtime
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

#define IP 0
#define NO_FLAG 0
#define MESSAGE_SIZE 512
#define PENDING_QUEUE 50
#define POLL_NO_TIMEOUT -1
#define USERLEN 12
#define TARGMAX 4
#define ERROR true
#define OK false
#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

typedef std::string::iterator	                stringIterator;

typedef std::map<int, Client*>	                clientMap;

typedef std::vector<pollfd>		                pollVector;

typedef std::vector<std::vector<std::string> >  commandsVector;

typedef std::vector<std::string>                commandTokensVector;

typedef std::map<std::string, Channel*>         channelsMap;

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
        channelsMap     _channelsList;

		bool	_isValidUserName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidRealName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd);

	public:
		Server(char* port, std::string password);
		~Server(void);

		void				initServer(void);
		void				launchServer(void);
		static void			stopServer(int);
		void				createPollFd(int fd);
		void				serverLoop(void);
		void				acceptClient(void);

        // Join methods
		pollfd				&getPollFd(void);
		void				printLogMessage(std::string message, bool isError);
		std::string	const	getCurrentTimeStamp(void);

        std::vector<std::vector<std::string> >	createCommandsVector(std::string buffer);
        void                        			handleCommand(std::vector<std::vector<std::string> > cmd, Client* client);
        void                        			joinCommand(commandTokensVector cmd, Client* client);
        void                                    privmsgCommand(commandTokensVector cmd, Client* client);
		void									clientManager(void);
		void									passCommand(std::vector<std::string> cmd, int fd);
		void									nickCommand(std::vector<std::string> cmd, int fd);
		void									userCommand(std::vector<std::string> cmd, int fd);
		void									kickCommand(std::vector<std::string> cmd, int fd);
		void									errorCommand(int clientFd, std::string error, std::string reason);
};

#endif
