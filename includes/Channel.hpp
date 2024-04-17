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

    //METHOD
	std::string 					createModesString(void) const;
    std::string 					formatClientsListAsString(void) const;
    void        					privmsg(std::vector<std::string> cmd, Client* client);
    void        					changeInvitedClientNick(std::string oldNick, std::string newNick);
    void        					changeChannelClientNick(std::string oldNick, std::string newNick);

    void        					addClient(Client* clientPtr, std::string password);
    bool        					removeClient(std::string nickname);

    //Checker
    bool    						isInvitedClient(std::string const & nick) const;
    bool    						isClientExist(const Client* client) const;

    //SETTER
    void                            unsetInvitedClients(std::string const &nick);
	void						    setModes(const std::string modes);
	void						    setUsersLimit(const int usersLimit);
	void						    setIsOnInvite(const bool isOnInvite);
	void						    setHasPassword(const bool hasPassword);
    void        				    setPassword(const std::string& password);
	void						    setHasUsersLimit(const bool hasUsersLimit);
    void	                        setNewInvitedClient(std::string const &clientNickName);
    void                  			setTopic(std::string topic);
    void                        	setTopicInfo(const std::string &topic, const std::string &nickname);

    //GETTER
	const std::string&			    getModes(void) const;
    clientsMap        			    *getClientsList(void);
    const std::string     		    &getchannelName() const;
	const std::string&			    getPassword(void) const;
	bool						    getIsOnInvite(void) const;
	size_t						    getUsersLimit(void) const;
	bool						    getHasPassword(void) const;
	const std::string&			    getChannelName(void) const;
	bool						    getHasUsersLimit(void) const;
	const clientsMap&			    getClientsDataMap(void) const;
    std::vector<std::string>        getInvitedClients(void) const;
    ClientInfos             	    *getClientsInfoByNick(std::string nick);
    const std::string     			&getTopic(void) const;

	void							setIsTopicOperatorMode(const bool isTopicOperatorMode);
	bool							getIsTopicOperatorMode(void) const;
    std::string             		topicAuth;
    std::string             		topicTime;
};

#endif