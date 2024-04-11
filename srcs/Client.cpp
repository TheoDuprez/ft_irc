/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 22:25:29 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 01:50:42 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

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