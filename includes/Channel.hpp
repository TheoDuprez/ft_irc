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

// ------------------------------- Channel class ------------------------------ //

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

    //METHODS
    void        					addClient(Client* clientPtr, std::string password);
    bool        					removeClient(std::string nickname);

	// For nick command
    void        					changeInvitedClientNick(std::string oldNick, std::string newNick);
    void        					changeChannelClientNick(std::string oldNick, std::string newNick);

	// For privmsg command
    void        					privmsg(std::vector<std::string> cmd, Client* client);

	// For join command
    std::string 					formatClientsListAsString(void) const;

	// For mode command
	std::string 					createModesString(void) const;

	// For quit command
    void                            unsetInvitedClients(const std::string& nick);

    // CHECKERS
    bool    						isInvitedClient(const std::string& nick) const;
    bool    						isClientExist(const Client* client) const;

    // SETTERS
    void        				    setPassword(const std::string& password);
	void						    setUsersLimit(const int usersLimit);
	void						    setHasPassword(const bool hasPassword);
	void						    setHasUsersLimit(const bool hasUsersLimit);
	void						    setIsOnInvite(const bool isOnInvite);
	void							setIsTopicOperatorMode(const bool isTopicOperatorMode);
	void						    setModes(const std::string modes);
    void                  			setTopic(std::string topic);
    void	                        setNewInvitedClient(const std::string& clientNickName);
    void                        	setTopicInfo(const std::string& topic, const std::string& nickname);

    // GETTERS
	const std::string&			    getPassword(void) const;
	size_t						    getUsersLimit(void) const;
	bool						    getHasPassword(void) const;
	bool						    getHasUsersLimit(void) const;
	bool						    getIsOnInvite(void) const;
	bool							getIsTopicOperatorMode(void) const;
	const std::string&			    getModes(void) const;
    const std::string&				getTopic(void) const;
	const clientsMap&			    getClientsDataMap(void) const;
    std::vector<std::string>        getInvitedClients(void) const;
	const std::string&			    getChannelName(void) const;
    clientsMap*						getClientsList(void);
    ClientInfos*					getClientsInfoByNick(std::string nick);

	// For topic
    std::string             		topicAuth;
    std::string             		topicTime;
};

#endif