/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 12:46:48 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 15:04:41 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#ifndef MACRO_HPP
#define MACRO_HPP

// ERR_RPL
#define RPL_WELCOME( CLIENT, USERNAME) ":server 001 " + CLIENT + " :Welcome to the localhost Network, " + CLIENT + "!" + USERNAME + "@localhost"
#define RPL_INVITING( CLIENT, NICK, CHANNEL ) ":server 341 " + CLIENT + " " + NICK + " " + CHANNEL
#define ERR_NOSUCHNICK( CLIENT, CHANNEL ) ":server 403 " + CLIENT + " " + CHANNEL + " :No such nick"
#define ERR_NOSUCHCHANNEL( CLIENT, CHANNEL ) ":server 403 " + CLIENT + " " + CHANNEL + " :No such channel"
#define ERR_NONICKNAMEGIVEN( CLIENT ) ":server 431 * "  + CLIENT + " :No nickname given"
#define ERR_ERRONEUSNICKNAME( CLIENT, NICKNAME ) ":server 432 * " + CLIENT + " " + NICKNAME + " :Erroneus nickname"
#define ERR_NICKNAMEINUSE( CLIENT, NICKNAME ) ":server 433 * " + CLIENT + " " + NICKNAME + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL( CLIENT, USER, CHANNEL ) ":server 441 " + CLIENT + " " + CHANNEL + " " + USER + " :They aren't on that channel"
#define ERR_NOTONCHANNEL( CLIENT, CHANNEL ) ":server 442 " + CLIENT + " " + CHANNEL + " :You're not on that channel"
#define ERR_USERONCHANNEL( CLIENT, NICK, CHANNEL ) ":server 443 " + CLIENT + " " + NICK + " " + CHANNEL + " :is already on channel"
#define ERR_NEEDMOREPARAMS(NICKNAME, COMMAND) ":server 461 " + NICKNAME + " " + COMMAND + " :Not enough parameters"
#define ERR_ALREADYREGISTERED( CLIENT ) ":server 462 * " + CLIENT + " :You may not reregister"
#define ERR_PASSWDMISMATCH( CLIENT ) ":server 464 * " + CLIENT + " :Password incorrect"
#define ERR_CHANOPRIVSNEEDED( CLIENT, CHANNEL ) ":server 482 " + CLIENT + " " + CHANNEL + " :You're not channel operator"
#define ERR_INVALIDUSERCHAR( NAME )  "USER: \"" + NAME + "\" invalid character"
#define RPLY_NEWNICKMESSAGE( PASTNICK, NEWNICK ) ":" + PASTNICK + " NICK " + NEWNICK
#define RPL_INVITED( CLIENT, NICK, CHANNEL ) ":" + CLIENT + " INVITE " + NICK + " " + CHANNEL
#define RPL_NOTOPIC( CHANNEL ) ":server 331 * " + CHANNEL + " :No topic is set"
#define RPL_TOPIC( CHANNEL, TOPIC ) ":server 332 * " + CHANNEL + " :" + TOPIC
#define RPL_TOPICWHOTIME( CHANNEL, AUTH, TIME ) ":server 333 * " + CHANNEL + " " + AUTH + " " + TIME

// RPL
#define RPL_CHANNELMODEIS( NICKNAME, CHANNELPTR ) ":server 324 " + NICKNAME + " " + CHANNELPTR->getChannelName() + " " + CHANNELPTR->createModesString()

// MODE
#define MODE_MESSAGE_ADD_KEY ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +k " + *argumentsIt
#define MODE_MESSAGE_REMOVE_KEY ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -k "
#define MODE_MESSAGE_ADD_USERS_LIMITS ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +l " + *argumentsIt
#define MODE_MESSAGE_REMOVE_USERS_LIMITS ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -l "
#define MODE_MESSAGE_ADD_OPERATOR ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " +o " + *argumentsIt
#define MODE_MESSAGE_REMOVE_OPERATOR ":" + client->getNickName() + " MODE " + channelPtr->getChannelName() + " -o " + *argumentsIt

// KICK
#define KICK_USAGE "Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"
#define KICK_MESSAGE_OPS( USER, CHANNEL, MESSAGE ) currentClient->getClientFd(), ":" + currentClient->getNickName() + " KICK " + CHANNEL + " " + USER + " " + MESSAGE
#define KICK_MESSAGE_USERS( USER, CHANNEL, MESSAGE ) clientIt->second->getClient()->getClientFd(), ":" + currentClient->getNickName() + "!" + currentClient->getUserName() + "@localhost KICK " + CHANNEL + " " + USER + " " + MESSAGE

//INVITE
#define INVITE_USAGE "Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel (needs chanop)"

//TOPIC
#define TOPIC_USAGE "Usage: TOPIC <#channel> :<topic>"
#define TOPIC_SET( NICKNAME, CHANNEL, TOPIC) ":" + NICKNAME + " TOPIC " + CHANNEL + " :" + TOPIC
#define TOPIC_CLEAR( NICKNAME, CHANNEL ) ":" + NICKNAME + " TOPIC " + CHANNEL + " :" + ""

#endif
