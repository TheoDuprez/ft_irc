/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:57:26 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 12:29:11 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

// ----- Class clientInfos (used by channel and only channel) ----- //

ClientInfos::ClientInfos(Client *clientPtr, bool isOperator): _client(clientPtr), _isOperator(isOperator) {}

const bool& ClientInfos::getIsOperator(void) const { return this->_isOperator; }

void ClientInfos::setIsOperator(bool isOperator) { this->_isOperator = isOperator; }

const Client* ClientInfos::getClient(void) const { return this->_client; }

// ----- Class Channel ----- //

Channel::Channel(std::string channelName, Client* clientPtr): _hasUsersLimit(false), _hasPassword(false), _isOnInvite(false), _isTopicOperatorMode(true) ,_channelName(channelName), _password(""), _topic("")
{
    this->_clientsDataMap.insert(std::make_pair(clientPtr->getNickName(), new ClientInfos(clientPtr, true)));
    sendMessage(clientPtr->getClientFd(), JOIN_SUCCESS);
    sendMessage(clientPtr->getClientFd(), JOIN_NAMERPLY);
	sendMessage(clientPtr->getClientFd(), JOIN_ENDOFNAMES);
}

Channel::~Channel(void) {
    for (clientsMap::iterator clientIt = this->_clientsDataMap.begin(); clientIt != this->_clientsDataMap.end(); clientIt++)
        delete (clientIt->second);
    this->_clientsDataMap.clear();
}

// METHODS

void    Channel::addClient(Client *clientPtr, std::string password)
{
	if (this->_hasPassword && this->_password != password) {
		sendMessage(clientPtr->getClientFd(), ERR_BADCHANNELKEY(this->_channelName));
		return ;
	}
	if (this->_isOnInvite && std::find(this->_invitedVector.begin(), this->_invitedVector.end(), clientPtr->getNickName()) == this->_invitedVector.end()) {
		sendMessage(clientPtr->getClientFd(), ERR_INVITEONLYCHAN(this->_channelName));
		return ;
	}
	if (this->_hasUsersLimit && this->_clientsDataMap.size() >= this->_usersLimit) {
		sendMessage(clientPtr->getClientFd(), ERR_CHANNELISFULL(this->_channelName));
		return ;
	}

	this->_clientsDataMap.insert(std::make_pair(clientPtr->getNickName(), new ClientInfos(clientPtr, false)));
	for (clientsMap::iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
		sendMessage(it->second->getClient()->getClientFd(), JOIN_SUCCESS);
	}
	if (this->_topic != "") {
		sendMessage(clientPtr->getClientFd(), RPL_TOPIC(this->_channelName, this->_topic));
		sendMessage(clientPtr->getClientFd(), RPL_TOPICWHOTIME(this->_channelName, this->topicAuth, this->topicTime));
	}
	sendMessage(clientPtr->getClientFd(), JOIN_NAMERPLY);
	sendMessage(clientPtr->getClientFd(), JOIN_ENDOFNAMES);
}

bool    Channel::removeClient(std::string nickname)
{
	clientsMapIterator it = this->_clientsDataMap.find(nickname);
	if (it != this->_clientsDataMap.end()) {
		delete it->second;
		this->_clientsDataMap.erase(it);
	}
	return this->_clientsDataMap.empty();
}

// For nick command

void        Channel::changeChannelClientNick(std::string oldNick, std::string newNick)
{
    clientsMap::iterator  clientIt;

    clientIt = this->_clientsDataMap.find(oldNick);
    if (clientIt != this->_clientsDataMap.end()) {
        this->_clientsDataMap.insert(std::make_pair(newNick, clientIt->second));
        this->_clientsDataMap.erase(oldNick);
    }
}

void        Channel::changeInvitedClientNick(std::string oldNick, std::string newNick)
{
    std::vector<std::string>::iterator  clientIt;

    clientIt = std::find(this->_invitedVector.begin(), this->_invitedVector.end(), oldNick);
    if (clientIt != this->_invitedVector.end()) {
        this->_invitedVector.erase(clientIt);
        this->_invitedVector.push_back(newNick);
    }
}

// For privmsg command

void        Channel::privmsg(std::vector<std::string> cmd, Client* clientPtr)
{
	std::string message;

	for (size_t i = 2; i < cmd.size(); i++) {
		message += cmd[i];
		if (i + 1 != cmd.size())
			message += " ";
	}
    for (clientsMap::iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
        if (it->second->getClient() != clientPtr)
            sendMessage(it->second->getClient()->getClientFd(), ":" + clientPtr->getNickName() + " PRIVMSG " + this->_channelName + " " + message);
    }
}

// For join command

std::string Channel::formatClientsListAsString(void) const
{
    std::string retClientsList;
    for (clientsMap::const_iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end();) {
        if (it->second->getIsOperator())
            retClientsList += "@";
        retClientsList += it->first;
        if (++it != this->_clientsDataMap.end())
            retClientsList += " ";
    }
    return retClientsList;
}

// For mode command

std::string Channel::createModesString(void) const
{
	std::string modesString("+");

	if (this->_isTopicOperatorMode)
		modesString += "t";
	if (this->_isOnInvite)
		modesString += "i";
	if (this->_hasPassword)
		modesString += "k";
	if (this->_hasUsersLimit)
		modesString += "l";
	return modesString;
}

// For quit command

void    Channel::unsetInvitedClients(const std::string& nick)
{
    std::vector<std::string>::iterator    it = std::find(this->_invitedVector.begin(), this->_invitedVector.end(), nick);
    if (it != this->_invitedVector.end())
        this->_invitedVector.erase(it);
}

// CHECKERS

bool    Channel::isInvitedClient(const std::string& nick) const
{
    std::vector<std::string>::const_iterator    clientIt;

    clientIt = std::find(this->_invitedVector.begin(), this->_invitedVector.end(), nick);
    if (clientIt != this->_invitedVector.end())
        return (true);
    return (false);
}

bool    Channel::isClientExist(const Client* clientPtr) const
{
    for (clientsMap::const_iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
        if (it->second->getClient() == clientPtr)
            return true;
    }
    return false;
}

// SETTERS

void    Channel::setPassword(const std::string& password) { this->_password = password; }

void	Channel::setUsersLimit(const int usersLimit) { this->_usersLimit = usersLimit; }

void	Channel::setHasPassword(const bool hasPassword) { this->_hasPassword = hasPassword; }

void	Channel::setHasUsersLimit(const bool hasUsersLimit) { this->_hasUsersLimit = hasUsersLimit; }

void	Channel::setIsOnInvite(const bool isOnInvite) { this->_isOnInvite = isOnInvite; }

void	Channel::setIsTopicOperatorMode(const bool isTopicOperatorMode ) { this->_isTopicOperatorMode = isTopicOperatorMode; }

void	Channel::setModes(const std::string modes) { this->_modes = modes; }

void    Channel::setTopic(std::string topic) { this->_topic = topic; }

void    Channel::setNewInvitedClient(const std::string& clientNickName) { this->_invitedVector.push_back(clientNickName); }

void    Channel::setTopicInfo(const std::string& topic, const std::string& nickname)
{
    time_t timestamp = time(NULL);
    this->_topic = topic;
    this->topicTime = SSTR(timestamp);
    this->topicAuth = nickname;
}

// GETTERS

const std::string&			Channel::getPassword(void) const { return this->_password; }

size_t						Channel::getUsersLimit(void) const { return this->_usersLimit; }

bool						Channel::getHasPassword(void) const { return this->_hasPassword; }

bool						Channel::getHasUsersLimit(void) const { return this->_hasUsersLimit; }

bool						Channel::getIsOnInvite(void) const { return this->_isOnInvite; }

bool						Channel::getIsTopicOperatorMode(void) const { return this->_isTopicOperatorMode; }

const std::string&			Channel::getModes(void) const { return this->_modes; }

const std::string&			Channel::getTopic(void) const { return this->_topic; }

const clientsMap&			Channel::getClientsDataMap(void) const { return this->_clientsDataMap; }

std::vector<std::string>    Channel::getInvitedClients(void) const { return this->_invitedVector; }

const std::string&			Channel::getChannelName(void) const { return this->_channelName; }

clientsMap*					Channel::getClientsList(void) { return &this->_clientsDataMap; }

ClientInfos*	Channel::getClientsInfoByNick(std::string nick)
{
    clientsMap::iterator    clientIt;

    clientIt = this->_clientsDataMap.find(nick);
    if (clientIt != this->_clientsDataMap.end())
        return (clientIt->second);
    return (NULL);
}
