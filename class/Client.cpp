/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 22:25:29 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/08 10:20:42 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
    this->_isRegister = false;
    this->_isOperator = false;
}

Client::~Client(void)
{
}

bool	Client::getIsRegister()
{
    return (this->_isRegister);
}

std::string	Client::getServerPassword()
{
    return (this->_serverPassword);
}

std::string	Client::getNickName()
{
    return (this->_nickName);
}

std::string	Client::getUserName()
{
    return (this->_userName);
}

std::string	Client::getRealName()
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