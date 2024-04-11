#include "../includes/Channel.hpp"

// ----- Class clientInfos (used by channel and only channel) ----- //

ClientInfos::ClientInfos(Client *client, bool isOperator): _client(client), _isOperator(isOperator) {}

const Client* ClientInfos::getClient(void) const { return this->_client; }

const bool& ClientInfos::getIsOperator(void) const { return this->_isOperator; }

void ClientInfos::setIsOperator(bool isOperator) { this->_isOperator = isOperator; }

// ----- Class Channel ----- //

Channel::Channel(std::string channelName, Client* client): _hasUsersLimit(false), _hasPassword(false), _isOnInvite(false), _channelName(channelName), _password("")
{
    this->_clientsDataMap.insert(std::make_pair(client->getNickName(), new ClientInfos(client, true)));
    sendMessage(client->getClientFd(), ":" + client->getNickName() + " JOIN " + this->_channelName);
    sendMessage(client->getClientFd(), ":server 353 " + client->getNickName() + " = " + this->_channelName + " :@" + client->getNickName());
}

Channel::~Channel(void) {}

void    			Channel::setPassword(const std::string& password) { this->_password = password; }

void				Channel::setUsersLimit(const int usersLimit) { this->_usersLimit = usersLimit; }

void				Channel::setHasPassword(const bool hasPassword) { this->_hasPassword = hasPassword; }

void				Channel::setHasUsersLimit(const bool hasUsersLimit) { this->_hasUsersLimit = hasUsersLimit; }

void				Channel::setIsOnInvite(const bool isOnInvite) { this->_isOnInvite = isOnInvite; }

const std::string&	Channel::getPassword(void) const { return this->_password; }

size_t				Channel::getUsersLimit(void) const { return this->_usersLimit; }

bool				Channel::getHasPassword(void) const { return this->_hasPassword; }

bool				Channel::getHasUsersLimit(void) const { return this->_hasUsersLimit; }

bool				Channel::getIsOnInvite(void) const { return this->_isOnInvite; }

const clientsMap&	Channel::getClientsDataMap(void) const { return this->_clientsDataMap; }

const std::string&	Channel::getModes(void) const { return this->_modes; }

const std::string&	Channel::getChannelName(void) const { return this->_channelName; }

void				Channel::setModes(const std::string modes) { this->_modes = modes; }

bool    Channel::addClient(Client *client, std::string password)
{
    if ( (!this->_hasPassword || this->_password == password) && (!this->_hasUsersLimit || this->_usersLimit > this->_clientsDataMap.size()) ) {

        this->_clientsDataMap.insert(std::make_pair(client->getNickName(), new ClientInfos(client, false)));
        for (clientsMap::iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
            sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " JOIN " + this->_channelName);
        }
        sendMessage(client->getClientFd(), ":server 353 " + client->getNickName() + " = " + this->_channelName + " :" + this->formatClientsListAsString());
        return true;
    }
    return false;
}

bool    Channel::isClientExist(const Client* client) const
{
    for (clientsMap::const_iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
        if (it->second->getClient() == client)
            return true;
    }
    return false;
}

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

void        Channel::privmsg(std::vector<std::string> cmd, Client *client)
{
//    sendMessage(client->getClientFd())
    for (clientsMap::iterator it = this->_clientsDataMap.begin(); it != this->_clientsDataMap.end(); it++) {
        if (it->second->getClient() != client)
            sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " PRIVMSG " + this->_channelName + " :" +  cmd.at(2).substr(1, cmd.at(2).length()));
    }
}

std::string Channel::createModesString(void) const
{
	std::string modesString("+");

	if (this->_hasPassword)
		modesString += "k";
	if (this->_hasUsersLimit)
		modesString += "l";
	return modesString;
}
