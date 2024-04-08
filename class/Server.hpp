/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/05 14:10:51 by acarlott         ###   ########lyon.fr   */
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
#include <fstream> // std::ofstream
#include <ctime> // std::localtime
#include "Client.hpp"
#include "Channel.hpp"
#include "../includes/parsing.hpp"

#define IP 0
#define NO_FLAG 0
#define MESSAGE_SIZE 512
#define PENDING_QUEUE 50
#define POLL_NO_TIMEOUT -1
#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

typedef std::vector<Client*>	clientVector;
typedef	clientVector::iterator	clientIterator;

typedef std::vector<pollfd>		pollVector;
typedef	pollVector::iterator	pollIterator;

typedef std::vector<std::string> cmdVector;
typedef cmdVector::iterator cmdVectorIterator;

typedef std::map<std::string, Channel*> channelsMap;

//typedef std::vector<std::string> cmdArgsVector;


class Server
{
	private:
		std::ofstream	_logFile;
		clientVector	_clientList;
		pollVector		_pollFds;
		unsigned short	_port;
		sockaddr_in		_serverAddress;
		socklen_t		_serverAddressSize;
		std::string		_password;
		static bool		_isServUp;
        channelsMap     _channelsList;

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

        cmdVector                   createCmdVector(std::string buffer);
        void                        handleCommand(cmdVector cmd, Client* client);
        void                        join(cmdVector cmd, Client* client);
        void                        sendMessage(int fd, std::string msg);
};

#endif
