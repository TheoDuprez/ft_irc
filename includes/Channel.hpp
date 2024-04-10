#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"
#include "utils.hpp"
#include <map>
#include <vector>

class UserInfos {
private:
    Client* _client;
    bool    _isOperator;

public:
    UserInfos(Client* client, bool isOperator);

    const bool&     getIsOperator(void) const;
    void            setIsOperator(bool isOperator);
    const Client*   getClient(void) const;
};

typedef std::map<std::string, UserInfos*>           clientsListMap;
typedef clientsListMap::iterator                    clientsListMapIterator;

class Channel {
private:
    int									_usersLimit;
    bool                                _isChannelOnInvite;
    std::string                         _channelName;
    std::string                         _channelPassword;
    clientsListMap                      _clientsList;
	std::string 						_modes;

public:
    Channel(std::string channelName, Client* client);
    ~Channel(void);
    void        setPassword(const std::string& password);
	void		setUsersLimit(const int usersLimit);
    bool        addClient(Client* client, std::string password);
    bool        isClientExist(const Client* client) const;
    std::string formatClientsListAsString(void) const;
    void        privmsg(std::vector<std::string> cmd, Client* client);
};

#endif