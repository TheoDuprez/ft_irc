/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 22:25:29 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 17:33:33 by acarlott         ###   ########lyon.fr   */
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

bool	Client::isValidBuffer(const std::string tmpBuffer)
{
	if (tmpBuffer[tmpBuffer.length() - 1] != '\n')
		return false;
	return true;
}

std::string const&	Client::receiveMessage(int fd)
{
    int 		recvReturn;
    char 		buffer[MESSAGE_SIZE] = {0};
    
    recvReturn = recv(fd, &buffer, MESSAGE_SIZE - 2 - this->_tempBuffer.size(), NO_FLAG);
    if (recvReturn == -1)
        throw (std::runtime_error(strerror(errno)));
    else if (recvReturn == 0) {
        throw BreakException();
	}
    else {
		if (!isValidBuffer(buffer) && std::string(buffer).size() != MESSAGE_SIZE - 2) {
			this->_tempBuffer += buffer;
			if (this->_tempBuffer.size() != MESSAGE_SIZE - 2)
                throw ContinueException();
		}
		if (this->_tempBuffer.size() != MESSAGE_SIZE - 2)
			this->_tempBuffer += buffer;
		if (this->_tempBuffer.find('\n') == std::string::npos)
			this->_tempBuffer += "\r\n";
    }
    return (_tempBuffer);
}

void	Client::clearTempBuffer(void)
{
    this->_tempBuffer.clear();
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

const std::vector<Client*>&	Client::getContactedClients(void) const
{
	return this->_contactedClients;
}

void	Client::setContactedClient(Client *targetClient, bool setClient)
{
	std::vector<Client* >::iterator clientIt = std::find(this->_contactedClients.begin(), this->_contactedClients.end(), targetClient);

	if (setClient && clientIt == this->_contactedClients.end()) {
		this->_contactedClients.push_back(targetClient);
    }
	else if (!setClient && clientIt != this->_contactedClients.end())
		this->_contactedClients.erase(clientIt);
}

Client* Client::getContactedClientByNick(const std::string& nickName) const
{    
	for (size_t i = 0; i < this->_contactedClients.size(); i++) {
		if (this->_contactedClients[i]->getNickName() == nickName)
			return this->_contactedClients[i];
	}
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

const char* ContinueException::what() const throw()
{
	return ("");
}

const char* BreakException::what() const throw()
{
	return ("");
}