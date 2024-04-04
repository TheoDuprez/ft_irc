/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tduprez <tduprez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/04 13:23:48 by tduprez          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netinet/in.h> // For socket data struct
#include <cstring> // For memset function
#include <csignal> // For signal function
#include <cstdlib>
#include <errno.h>
#include <limits>
#include <unistd.h>
#include <poll.h>

#define IP 0
#define NO_FLAG 0
#define MESSAGE_SIZE 512
#define PENDING_QUEUE 5
#define POLL_NO_TIMEOUT -1

class Server
{
	private:
		unsigned short	_port;
		int				_serverFd;
		int				_clientFd;
		sockaddr_in		_serverAddress;
		socklen_t		_serverAddressSize;
		std::string		_password;
		static bool		_isRunning;

		Server(void);
		Server(const Server& obj);
		Server& operator=(const Server& obj);

	public:
		Server(char* port, std::string password);
		~Server(void);

		void		initServer(void);
		void		launchServer(void);
		static void	stopServer(int);
};

#endif
