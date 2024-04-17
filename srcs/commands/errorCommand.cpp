/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:44 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 17:43:06 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::errorCommand(Client *currentClient)
{
    int fd = currentClient->getClientFd();
    sendMessage(fd, ERR_PASSWDMISMATCH(this->getServerName()));
    this->_clients.erase(this->_clients.find(fd));
    close(fd);
    delete(currentClient);
    for (pollVector::iterator it = this->_pollFds.begin(); it != this->_pollFds.end();)
		(it->fd == fd) ? it = this->_pollFds.erase(it) : it++;
    this->printLogMessage("Client '" + iToString(fd) + "' has been closed: Incorrect password\n", ERROR);
	throw QuitClientException();
}