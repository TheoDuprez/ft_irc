/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:15 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/12 00:29:09 by acarlott         ###   ########lyon.fr   */
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
#define SSTR( NUMBER ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << NUMBER ) ).str()
// RPLY Macro
#define RPL_WELCOME( CLIENT, USERNAME) ":server 001 " + CLIENT + " :Welcome to the localhost Network, " + CLIENT + "!" + USERNAME + "@localhost"
#define RPLY_NEWNICKMESSAGE( PASTNICK, NEWNICK ) ":" + PASTNICK + " NICK " + NEWNICK
#define ERR_ERRONEUSNICKNAME( CLIENT, NICKNAME ) ":server 432 * " + CLIENT + " " + NICKNAME + " :Erroneus nickname"
#define ERR_NICKNAMEINUSE( CLIENT, NICKNAME ) ":server 433 * " + CLIENT + " " + NICKNAME + " :Nickname is already in use"
#define ERR_NONICKNAMEGIVEN( CLIENT ) ":server 431 * "  + CLIENT + " :No nickname given"
#define ERR_ALREADYREGISTERED( CLIENT ) ":server 462 * " + CLIENT + " :You may not reregister"
#define ERR_PASSWDMISMATCH( CLIENT ) ":server 464 * " + CLIENT + " :Password incorrect"
#define ERR_INVALIDUSERCHAR( NAME )  "USER: \"" + NAME + "\" invalid character"
#define ERR_NEEDMOREPARAMS(CLIENT, COMMAND ) ":server 461 " + CLIENT + " " + COMMAND + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL( CLIENT, CHANNEL ) ":server 403 " + CLIENT + " " + CHANNEL + " :No such channel"
#define ERR_NOTONCHANNEL( CLIENT, CHANNEL ) ":server 442 " + CLIENT + " " + CHANNEL + " :You're not on that channel"
#define ERR_CHANOPRIVSNEEDED( CLIENT, CHANNEL ) ":server 482 " + CLIENT + " " + CHANNEL + " :You're not channel operator"
#define ERR_USERNOTINCHANNEL( CLIENT, USER, CHANNEL ) ":server 441 " + CLIENT + " " + CHANNEL + " " + USER + " :They aren't on that channel"
// KICK Macro
#define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"
#define KICK_MESSAGE_OPS( USER, CHANNEL, MESSAGE ) currentClient->getClientFd(), ":" + currentClient->getNickName() + " KICK " + CHANNEL + " " + USER + " " + MESSAGE
#define KICK_MESSAGE_USERS( USER, CHANNEL, MESSAGE ) clientIt->second->getClient()->getClientFd(), ":" + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost KICK " + CHANNEL + " " + USER + " " + MESSAGE

typedef std::string::iterator					stringIterator;

typedef std::map<int, Client*>					clientMap;

typedef std::vector<pollfd>						pollVector;

typedef std::vector<std::vector<std::string> >	commandsVector;

typedef std::vector<std::string>				commandTokensVector;

typedef std::map<std::string, Channel*>			channelsMap;

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
		std::string		_serverName;
		static bool		_isServUp;
        channelsMap     _channelsMap;

		bool	_isValidUserName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidRealName(Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidUserCommand(size_t i, Client  *currentClient, std::vector<std::string> *cmd);
		bool	_isValidNickCommand(Client  *currentClient, std::vector<std::string> *cmd);
		bool    _isValidKickCommand(std::vector<std::string> cmd, Client *currentClient, ClientInfos *&targetOp, ClientInfos *&targetUser, Channel *&targetChannel);

        // modeCommand methods
		void						manageModes(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr, bool adjustMode);
		void						modeCommandChannel(std::string modeString, std::vector<std::string> modeArguments, Client* client, Channel* channelPtr);
		std::vector<std::string>	fillModeArguments(commandTokensVector& cmd);
		void						manageKey(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);
		void						manageUsersLimit(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);
		void						manageOperator(Channel* channelPtr, Client* client, std::vector<std::string>& modeArguments, std::vector<std::string>::iterator& argumentsIt, bool adjustMode);

	public:
		Server(char* port, std::string password);
		~Server(void);

		void				initServer(void);
		void				launchServer(void);
		static void			stopServer(int);
		void				createPollFd(int fd);
		void				serverLoop(void);
		void				acceptClient(void);

		std::string const	&getServerName(void) const;
		Channel				*getChannelByName(std::string const &name);
		const Client		*getClientByName(const std::string& name);
        // Join methods
		pollfd				&getPollFd(void);
		void				printLogMessage(std::string message, bool isError);
		std::string	const	getCurrentTimeStamp(void);

		std::vector<std::vector<std::string> >	createCommandsVector(std::string buffer);
		void									handleCommand(std::vector<std::vector<std::string> > cmd, Client* client);
		void									joinCommand(commandTokensVector cmd, Client* client);
		void									modeCommand(commandTokensVector cmd, Client* client);
		void									privmsgCommand(commandTokensVector cmd, Client* client);
		void									clientManager(void);
		void									passCommand(std::vector<std::string> cmd, int fd);
		void									nickCommand(std::vector<std::string> cmd, int fd);
		void									userCommand(std::vector<std::string> cmd, int fd);
		void									kickCommand(std::vector<std::string> cmd, Client *currentClient);
		void									errorCommand(Client *currentClient);
};

#endif
