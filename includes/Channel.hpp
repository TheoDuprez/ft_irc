#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "utils.hpp"
#include <map>

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
    std::vector<std::string>            _invitedVector;
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

    //METHOD
	std::string createModesString(void) const;
    std::string formatClientsListAsString(void) const;
    void        privmsg(std::vector<std::string> cmd, Client* client);
    void        changeInvitedClientNick(std::string oldNick, std::string newNick);
    void        changeChannelClientNick(std::string oldNick, std::string newNick);
    
    bool        addClient(Client* client, std::string password);

    //Checker
    bool    isInvitedClient(std::string const & nick) const;
    bool    isClientExist(const Client* client) const;

    //SETTER
	void						    setModes(const std::string modes);
	void						    setUsersLimit(const int usersLimit);
	void						    setIsOnInvite(const bool isOnInvite);
	void						    setHasPassword(const bool hasPassword);
    void        				    setPassword(const std::string& password);
	void						    setHasUsersLimit(const bool hasUsersLimit);
    void	                        setNewInvitedClient(std::string const &clientNickName);

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
};

#endif