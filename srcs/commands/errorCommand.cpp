/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:44 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/11 22:12:21 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::errorCommand(Client *currentClient)
{
    sendMessage(currentClient->getClientFd(), ERR_PASSWDMISMATCH(currentClient->getNickName()));
    close(currentClient->getClientFd());
    delete(this->_clients.find(currentClient->getClientFd())->second);
    this->_clients.erase(this->_clients.find(currentClient->getClientFd()));
    throw (std::runtime_error("Client '" + iToString(currentClient->getClientFd()) + "' has been closed: Incorrect password\n"));
}