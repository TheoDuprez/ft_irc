#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include "utils.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include "macro.hpp"

class ClientInfos {
private:
    Client* _client;
    bool    _isOperator;

public:
    ClientInfos(Client* client, bool isOperator);

    const bool&     getIsOperator(void) const;
    void            setIsOperator(bool isOperator);
    const Client*   getClient(void) const;
};

typedef std::map<std::string, ClientInfos*>           clientsMap;
typedef clientsMap::iterator                    	  clientsMapIterator;

class Channel {
private:
    std::vector<std::string>            _invitedVector;
    size_t								_usersLimit;
	bool								_hasUsersLimit;
	bool								_hasPassword;
	bool								_isOnInvite;
	bool								_isTopicOperatorMode;
    std::string                         _channelName;
    std::string                         _password;
    clientsMap                      	_clientsDataMap;
	std::string 						_modes;
    std::string                         _topic;

public:
    Channel(std::string channelName, Client* client);
    ~Channel(void);

    const std::string		    &getChannelName(void) const;
    ClientInfos             	*getClientsInfoByNick(std::string nick);
    clientsMap        			*getClientsList(void);
    void        				setPassword(const std::string& password);
	void						setUsersLimit(const int usersLimit);
	void						setHasPassword(const bool hasPassword);
	void						setHasUsersLimit(const bool hasUsersLimit);
	void						setIsOnInvite(const bool isOnInvite);
	void						setIsTopicOperatorMode(const bool isTopicOperatorMode);
	const std::string&			getPassword(void) const;
	size_t						getUsersLimit(void) const;
	bool						getHasPassword(void) const;
	bool						getHasUsersLimit(void) const;
	bool						getIsOnInvite(void) const;
	bool						getIsTopicOperatorMode(void) const;
	const clientsMap&			getClientsDataMap(void) const;
	const std::string&			getModes(void) const;
	void						setModes(const std::string modes);
    void	                    setNewInvitedClient(std::string const &clientNickName);
    std::vector<std::string>    getInvitedClientVector(void) const;

    void        				changeClientName(std::string oldNick, std::string newNick);
    bool        				isClientExist(const Client* client) const;
    void        				addClient(Client* client, std::string password);
    std::string 				formatClientsListAsString(void) const;
    void       					privmsg(std::vector<std::string> cmd, Client* client);
	std::string 				createModesString(void) const;
    const std::string     		&getTopic(void) const;
    void                  		setTopic(std::string topic);
	void						removeClient(Client* client);
    std::string             	topicAuth;
    std::string             	topicTime;
};

#endif