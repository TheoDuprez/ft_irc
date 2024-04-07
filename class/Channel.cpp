#include "Channel.hpp"

// ----- Class userInfos (used by channel and only channel) ----- //

UserInfos::UserInfos(Client *client, bool isOperator): _client(client), _isOperator(isOperator) {}

const Client* UserInfos::getClient(void) const { return this->_client; }

const bool& UserInfos::getIsOperator(void) const { return this->_isOperator; }

void UserInfos::setIsOperator(bool isOperator) { this->_isOperator = isOperator; }

// ----- Class Channel ----- //

Channel::Channel(std::string channelName, Client* client): _usersLimit(-1), _isChannelOnInvite(false), _channelName(channelName), _channelPassword("")
{
    this->_clientsList.insert(std::make_pair("First client test", new UserInfos(client, true)));
}

Channel::~Channel(void) {}

bool    Channel::addClient(Client *client, std::string password)
{
    if (this->_channelPassword == password) {
        this->_clientsList.insert(std::make_pair("First client test", new UserInfos(client, false)));
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