/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:44 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 00:24:21 by shellks          ###   ########lyon.fr   */
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
    for (pollVector::iterator it = this->_pollFds.begin(); it != this->_pollFds.end();) {
		(it->fd == fd) ? it = this->_pollFds.erase(it) : it++;
		if (it == this->_pollFds.end()) {
            this->printLogMessage("Client '" + iToString(fd) + "' has been closed: Incorrect password\n", ERROR);
			throw QuitClientException();
		}
	}
    throw (std::runtime_error("Client '" + iToString(fd) + "' has been closed: Incorrect password\n"));
}