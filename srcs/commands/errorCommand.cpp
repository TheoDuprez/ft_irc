/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:38:44 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 19:42:23 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::errorCommand(int clientFd, std::string error, std::string reason)
{
    int fd = clientFd;
    sendMessage(clientFd, error + " ERROR :" + reason);
    close(clientFd);
    delete(this->_clients.find(clientFd)->second);
    this->_clients.erase(this->_clients.find(clientFd));
    throw (std::runtime_error("Client '" + iToString(fd) + "' has been closed: Incorrect password\n"));
}