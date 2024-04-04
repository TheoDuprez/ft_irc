/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 22:25:29 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/04 23:03:39 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(pollfd& fd) : _nickName("NA"), _userName("NA"), _fd(fd)
{
    this->_isRegister = false;
    this->_isOperator = false;
}

Client::~Client(void)
{
}