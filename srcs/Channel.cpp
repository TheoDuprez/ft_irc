#include "../includes/Channel.hpp"

// ----- Class userInfos (used by channel and only channel) ----- //

UserInfos::UserInfos(Client *client, bool isOperator): _client(client), _isOperator(isOperator) {}

const Client* UserInfos::getClient(void) const { return this->_client; }

const bool& UserInfos::getIsOperator(void) const { return this->_isOperator; }

void UserInfos::setIsOperator(bool isOperator) { this->_isOperator = isOperator; }

// ----- Class Channel ----- //

Channel::Channel(std::string channelName, Client* client): _usersLimit(-1), _isChannelOnInvite(false), _channelName(channelName), _channelPassword("")
{
    this->_clientsList.insert(std::make_pair(client->getNickName(), new UserInfos(client, true)));
    sendMessage(client->getClientFd(), ":" + client->getNickName() + " JOIN " + this->_channelName);
    sendMessage(client->getClientFd(), ":server 353 " + client->getNickName() + " = " + this->_channelName + " :@" + client->getNickName());
}

Channel::~Channel(void) {}

bool    Channel::addClient(Client *client, std::string password)
{
    if (this->_channelPassword == password || password != "") {
        this->_clientsList.insert(std::make_pair(client->getNickName(), new UserInfos(client, false)));
        for (clientsListMapIterator it = this->_clientsList.begin(); it != this->_clientsList.end(); it++) {
            sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " JOIN " + this->_channelName);
        }
        sendMessage(client->getClientFd(), ":server 353 " + client->getNickName() + " = " + this->_channelName + " :" + this->formatClientsListAsString());
        std::cout << "Size is : " << this->_clientsList.size() << std::endl;
        return true;
    }
    return false;
}

bool    Channel::isClientExist(const Client* client) const
{
    for (clientsListMap::const_iterator it = this->_clientsList.begin(); it != this->_clientsList.end(); it++) {
        if (it->second->getClient() == client)
            return true;
    }
    return false;
}

std::string Channel::formatClientsListAsString(void) const
{
    std::string retClientsList;
    for (clientsListMap::const_iterator it = this->_clientsList.begin(); it != this->_clientsList.end();) {
        if (it->second->getIsOperator())
            retClientsList += "@";
        retClientsList += it->first;
        if (++it != this->_clientsList.end())
            retClientsList += " ";
    }
    return retClientsList;
}

void        Channel::privmsg(std::vector<std::string> cmd, Client *client)
{
//    sendMessage(client->getClientFd())
    for (clientsListMapIterator it = this->_clientsList.begin(); it != this->_clientsList.end(); it++) {
        if (it->second->getClient() != client)
            sendMessage(it->second->getClient()->getClientFd(), ":" + client->getNickName() + " PRIVMSG " + this->_channelName + " :" +  cmd.at(2).substr(1, cmd.at(2).length()));
    }
}

clientsListMap  *Channel::getClientsList(void)
{
    return (&this->_clientsList);
}