/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:44 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/14 11:42:45 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::errorCommand(Client *currentClient)
{
    sendMessage(currentClient->getClientFd(), ERR_PASSWDMISMATCH(this->getServerName()));
    this->_clients.erase(this->_clients.find(currentClient->getClientFd()));
    close(currentClient->getClientFd());
    delete(currentClient);
    throw (std::runtime_error("Client '" + iToString(currentClient->getClientFd()) + "' has been closed: Incorrect password\n"));
}