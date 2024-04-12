#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include "utils.hpp"
#include <map>
#include <vector>

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
typedef clientsMap::iterator                    	clientsListMapIterator;

class Channel {
private:
    size_t								_usersLimit;
	bool								_hasUsersLimit;
	bool								_hasPassword;
	bool								_isOnInvite;
    std::string                         _channelName;
    std::string                         _password;
    clientsMap                      	_clientsDataMap;
	std::string 						_modes;

public:
    Channel(std::string channelName, Client* client);
    ~Channel(void);

    const std::string     		&getchannelName() const;
    ClientInfos             	*getClientsInfoByNick(std::string nick);
    clientsMap        			*getClientsList(void);
    void        				setPassword(const std::string& password);
	void						setUsersLimit(const int usersLimit);
	void						setHasPassword(const bool hasPassword);
	void						setHasUsersLimit(const bool hasUsersLimit);
	void						setIsOnInvite(const bool isOnInvite);
	const std::string&			getPassword(void) const;
	size_t						getUsersLimit(void) const;
	bool						getHasPassword(void) const;
	bool						getHasUsersLimit(void) const;
	bool						getIsOnInvite(void) const;
	const clientsMap&			getClientsDataMap(void) const;
	const std::string&			getModes(void) const;
	const std::string&			getChannelName(void) const;
	void						setModes(const std::string modes);

    void        changeClientName(std::string oldNick, std::string newNick);
    bool        isClientExist(const Client* client) const;
    bool        addClient(Client* client, std::string password);
    std::string formatClientsListAsString(void) const;
    void        privmsg(std::vector<std::string> cmd, Client* client);
	std::string createModesString(void) const;
};

#endif