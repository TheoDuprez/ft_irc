/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/17 18:44:26 by acarlott         ###   ########lyon.fr   */
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
#include "macro.hpp"

#define IP 0
#define PENDING_QUEUE 50
#define POLL_NO_TIMEOUT -1
#define USERLEN 12
#define TARGMAX 4
#define MP_UNSET false
#define MP_SET true
#define ERROR true
#define OK false
#define SSTR( NUMBER ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << NUMBER ) ).str()

typedef std::string::iterator					stringIterator;

typedef std::map<int, Client*>					clientMap;

typedef std::vector<pollfd>						pollVector;

typedef std::vector<std::vector<std::string> >	commandsVector;

typedef std::vector<std::string>				commandTokensVector;

typedef std::map<std::string, Channel*>			channelsMap;

class Server
{
	private:
		std::string		_cmdBuffer;
		std::ofstream	_logFile;
		clientMap		_clients;
		pollVector		_pollFds;
		unsigned short	_port;
		sockaddr_in		_serverAddress;
		socklen_t		_serverAddressSize;
		std::string		_password;
		std::string		_serverName;
		static bool		_isServUp;
        channelsMap     _channelsMap;


		bool	_isValidUserName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidRealName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd);
		bool    _isValidKickCommand(std::vector<std::string> cmd, Client *currentClient, ClientInfos *&targetOp, Channel *&targetChannel);
		bool    _isValidInviteCommand(std::vector<std::string> cmd, Client *currentClient, Channel *targetChannel);
		void    _nickApplyChange(std::vector<std::string> cmd, Client *currentClient);

        // modeCommand methods
		void						_manageModes(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr, bool adjustMode);
		void						_modeCommandChannel(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr);
		std::vector<std::string>	_fillModeArguments(commandTokensVector& cmd);
		void						_manageKey(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);
		void						_manageUsersLimit(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);
		void						_manageOperator(Channel* channelPtr, Client* client, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);
		void						_manageTopic(Channel* channelPtr, Client* client, bool adjustMode);
		void						_manageInvite(Channel* channelPtr, Client* client, bool adjustMode);
		void						_sendMessageToAllClients(const Channel* channelPtr, const std::string& message) const;

		// partCommand methods
		void						_leaveChannel(Client* clientPtr, Channel* channelPtr);

	public:
		Server(char* port, std::string password);
		~Server(void);

		void				initServer(void);
		void				launchServer(void);
		static void			stopServer(int);
		void				createPollFd(int fd);
		void				serverLoop(void);
		void				acceptClient(void);
		void				removeClientFromServer(int fd);

		bool				isValidPaquet(std::string buffer);

		std::string const	&getServerName(void) const;
		Channel				*getChannelByName(std::string const &name);
		Client				*getClientByName(const std::string& name);
        // Join methods
		pollfd				&getPollFd(void);
		void				printLogMessage(std::string message, bool isError);
		std::string	const	getCurrentTimeStamp(void);

		bool									isValidBuffer(const std::string tmpBuffer);
		void									sendMessageToAllChannelUsers(Client *currentClient, Channel *channel, std::string const &message) const;
		std::vector<std::vector<std::string> >	createCommandsVector(std::string buffer);
		void									handleCommand(std::vector<std::vector<std::string> > cmd, Client* clientPtr);
		void									joinCommand(commandTokensVector cmd, Client* clientPtr);
		void									partCommand(commandTokensVector cmd, Client* clientPtr);
		void									modeCommand(commandTokensVector cmd, Client* clientPtr);
		void									privmsgCommand(commandTokensVector cmd, Client* clientPtr);
		void									clientManager(void);
		void    								inviteCommand(std::vector<std::string> cmd, Client *currentClient);
		void									passCommand(std::vector<std::string> cmd, int fd);
		void									nickCommand(std::vector<std::string> cmd, int fd);
		void									userCommand(std::vector<std::string> cmd, int fd);
		void									kickCommand(std::vector<std::string> cmd, Client *currentClient);
		void									errorCommand(Client *currentClient);
		void									topicCommand(std::vector<std::string> cmd, Client *client);
		void									quitCommand(std::vector<std::string> cmd, Client *client);
};

class QuitClientException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

#endif
