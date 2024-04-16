/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 22:25:29 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/12 14:57:44 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientFd): _cliendFd(clientFd)
{
    this->_isRegister = false;
    this->_isOperator = false;
}

Client::Client(void)
{
    this->_isRegister = false;
    this->_isOperator = false;
}

Client::~Client(void)
{

}

const int&          Client::getClientFd(void) const
{
    return this->_cliendFd;
}

bool	Client::getIsRegister() const
{
    return (this->_isRegister);
}

const std::string&	Client::getServerPassword() const
{
    return (this->_serverPassword);
}

const std::string&	Client::getNickName() const
{
    return (this->_nickName);
}

const std::string&	Client::getUserName() const
{
    return (this->_userName);
}

const std::string&	Client::getRealName() const
{
    return (this->_realName);
}

const std::vector<std::string>&	Client::getContactedClients(void) const
{
	return this->_contactedClients;
}

void	Client::setContactedClient(std::string nickName, bool setClient)
{
	std::vector<std::string>::iterator clientIt = std::find(this->_contactedClients.begin(), this->_contactedClients.end(), nickName);

	if (setClient)
		this->_contactedClients.push_back(nickName);
	else if (clientIt != this->_contactedClients.end())
		this->_contactedClients.erase(clientIt);
}

const std::string* Client::getContactedClientByNick(const std::string& nickName)
{
	std::vector<std::string>::iterator clientIt = std::find(this->_contactedClients.begin(), this->_contactedClients.end(), nickName);
	for (size_t i = 0; i < this->_contactedClients.size(); i++) {
		if (this->_contactedClients[i] == nickName)
			return &this->_contactedClients[i];
	}

	if (clientIt != this->_contactedClients.end())
		return new std::string(clientIt->data());
	return NULL;
}

void	Client::setIsRegister(bool reg)
{
    this->_isRegister = reg;
}

void	Client::setServerPassword(std::string pswd)
{
    this->_serverPassword = pswd;
}

void	Client::setnickName(std::string nickName)
{
    this->_nickName = nickName;
}

void	Client::setuserName(std::string userName)
{
    this->_userName = userName;
}

void	Client::setrealName(std::string realName)
{
    this->_realName = realName;
}