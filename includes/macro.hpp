/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:46:48 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 00:43:14 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#ifndef MACRO_HPP
#define MACRO_HPP

#define ERR_CHANGEERRONEUSNICKNAME( NICKNAME ) NICKNAME + " :Erroneus nickname: Illegal characters"
#define ERR_CHANGENICKNAMEINUSE( NICKNAME ) NICKNAME + " :Nickname is already in use"
// ERR_RPL
#define RPL_INVITING( CLIENT, NICK, CHANNEL ) ":server 341 " + CLIENT + " " + NICK + " " + CHANNEL
#define ERR_NOSUCHNICK( CLIENT, CHANNEL ) ":server 401 " + CLIENT + " " + CHANNEL + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL( CLIENT, CHANNEL ) ":server 403 " + CLIENT + " " + CHANNEL + " :No such channel"
#define ERR_BADCHANNMASK( CHANNEL ) ":server 476 " + CHANNEL + " :Bad Channel Mask"
#define ERR_NORECIPIENT( COMMAND ) ":server 411 * :No recipient given (" + COMMAND + ")"
#define ERR_NONICKNAMEGIVEN( CLIENT ) ":server 431 * "  + CLIENT + " :No nickname given"
#define ERR_ERRONEUSNICKNAME( NICKNAME, CLIENT ) ":server 432 * " + NICKNAME + " " + CLIENT + " :Erroneus nickname"
#define ERR_NICKNAMEINUSE( NICKNAME, CLIENT ) ":server 433 * " + NICKNAME + " " + CLIENT + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL( CLIENT, USER, CHANNEL ) ":server 441 " + CLIENT + " " + CHANNEL + " " + USER + " :They aren't on that channel"
#define ERR_NOTONCHANNEL( CLIENT, CHANNEL ) ":server 442 " + CLIENT + " " + CHANNEL + " :You're not on that channel"
#define ERR_USERONCHANNEL( CLIENT, NICK, CHANNEL ) ":server 443 " + CLIENT + " " + NICK + " " + CHANNEL + " :is already on channel"
#define ERR_NEEDMOREPARAMS( NICKNAME, COMMAND ) ":server 461 " + NICKNAME + " " + COMMAND + " :Not enough parameters"
#define ERR_ALREADYREGISTERED( CLIENT ) ":server 462 * " + CLIENT + " :You may not reregister"
#define ERR_PASSWDMISMATCH( CLIENT ) ":server 464 * " + CLIENT + " :Password incorrect"
#define ERR_CHANOPRIVSNEEDED( CLIENT, CHANNEL ) ":server 482 " + CLIENT + " " + CHANNEL + " :You're not channel operator"
#define ERR_INVALIDUSERCHAR( NAME )  "USER: \"" + NAME + "\" invalid character"
#define ERR_NOTEXTTOSEND ":server 412 * :No text to send"
#define ERR_CHANNELISFULL( CHANNEL ) ":server 471 * " + CHANNEL + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN( CHANNEL ) ":server 473 * " + CHANNEL + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY( CHANNEL ) ":server 475 * " + CHANNEL + " :Cannot join channel (+k)"
#define RPLY_NEWNICKMESSAGE( PASTNICK, NEWNICK ) ":" + PASTNICK + " NICK " + NEWNICK
#define RPL_INVITED( CLIENT, NICK, CHANNEL ) ":" + CLIENT + " INVITE " + NICK + " " + CHANNEL
#define RPL_NOTOPIC( CHANNEL ) ":server 331 * " + CHANNEL + " :No topic is set"
#define RPL_TOPIC( CHANNEL, TOPIC ) ":server 332 * " + CHANNEL + " :" + TOPIC

// RPL
#define RPL_WELCOME( CLIENT, USERNAME ) ":server 001 " + CLIENT + " :Welcome to the localhost Network, " + CLIENT + "!" + USERNAME + "@localhost"
#define RPL_YOURHOST( CLIENT, SERVERNAME ) ":server 002 " + CLIENT + " :Your host is " + SERVERNAME + ", running version 1.0.0"
#define RPL_CREATED( CLIENT, DATETIME ) ":server 003 " + CLIENT + " :This server was created " + DATETIME
#define RPL_MYINFO( CLIENT, SERVERNAME ) ":server 004 " + CLIENT + " " + SERVERNAME + " 1.0.0 No available user modes itkol"
#define RPL_ISUPPORT( CLIENT ) ":server 005 " + CLIENT + " NICKLEN=12 USERLEN=12 CHANNELLEN=12 TOPICLEN=12 NETWORK=ft_irc CASEMAPPING=ascii CHANTYPE=# PREFIX=@ CHANMODE=i,t,k,o,l :are supported by this server"
#define RPL_CHANNELMODEIS( NICKNAME, CHANNELPTR ) ":server 324 " + NICKNAME + " " + CHANNELPTR->getChannelName() + " " + CHANNELPTR->createModesString()

// JOIN
#define JOIN_SUCCESS ":" + clientPtr->getNickName() + " JOIN " + this->_channelName
#define JOIN_NAMERPLY ":server 353 " + clientPtr->getNickName() + " = " + this->_channelName + " :" + this->formatClientsListAsString()
#define JOIN_ENDOFNAMES ":server 366 " + clientPtr->getNickName() + " " + this->_channelName + " :END of /NAMES list"

// MODE
#define MODE_MESSAGE_ADD_KEY ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " +k " + *argumentsIt
#define MODE_MESSAGE_REMOVE_KEY ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " -k "
#define MODE_MESSAGE_ADD_USERS_LIMITS ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " +l " + *argumentsIt
#define MODE_MESSAGE_REMOVE_USERS_LIMITS ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " -l "
#define MODE_MESSAGE_ADD_OPERATOR ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " +o " + *argumentsIt
#define MODE_MESSAGE_REMOVE_OPERATOR ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " -o " + *argumentsIt
#define MODE_MESSAGE_ADD_TOPIC_ON_OPERATOR ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " +t"
#define MODE_MESSAGE_REMOVE_TOPIC_ON_OPERATOR ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " -t"
#define MODE_MESSAGE_ADD_INVITE ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " +i"
#define MODE_MESSAGE_REMOVE_INVITE ":" + clientPtr->getNickName() + " MODE " + channelPtr->getChannelName() + " -i"

// KICK
#define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"
#define KICK_MESSAGE( USER, CHANNEL, MESSAGE ) ":" + currentClient->getNickName() + " KICK " + CHANNEL + " " + USER + " " + MESSAGE

//INVITE
#define INVITE_USAGE "Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel (needs chanop)"
#define ALREADY_INVITE( NICK, CHANNEL ) NICK + " :already invited to " + CHANNEL

//TOPIC
#define TOPIC_USAGE "Usage: TOPIC <#channel> :<topic>"
#define TOPIC_SET( NICKNAME, CHANNEL, TOPIC) ":" + NICKNAME + " TOPIC " + CHANNEL + " :" + TOPIC
#define TOPIC_CLEAR( NICKNAME, CHANNEL ) ":" + NICKNAME + " TOPIC " + CHANNEL + " :" + ""

//QUIT
#define QUIT_USAGE "Usage: QUIT :[<reason>]"
#define QUIT_CONNECTION ":server QUIT :Connection closed by server."
#define QUIT_BROADCAST( NICKNAME, REASON ) ":" + NICKNAME + " QUIT :" + REASON

// TOPIC
#define RPL_TOPIC( CHANNEL, TOPIC ) ":server 332 * " + CHANNEL + " :" + TOPIC
#define RPL_TOPICWHOTIME( CHANNEL, NICKNAME, TIME ) ":server 333 * " + CHANNEL + " " + NICKNAME + " " + TIME

// PART
#define PART_MESSAGE(NICKNAME, CHANNEL) ":" + NICKNAME + " PART " + CHANNEL
#define PART_MESSAGE_REASON(NICKNAME, CHANNEL, REASON) ":" + NICKNAME + " PART " + CHANNEL + " " + REASON

#endif
