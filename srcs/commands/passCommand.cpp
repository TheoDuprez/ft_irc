/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:09 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/18 20:37:50 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
    Client	*currentClient = this->_clients[fd];

    if (cmd.size() != 2 || cmd[1].empty()) {
        sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(this->getServerName(), "PASS"));
        return;
    }
    else if (currentClient->getIsRegister() == true) {
        sendMessage(currentClient->getClientFd(), ERR_ALREADYREGISTERED(this->getServerName()));
        return;
    }
    if (this->_password.compare(cmd[1].c_str())) {
        this->errorCommand(currentClient);
        throw (std::runtime_error(strerror(errno)));
    }
    currentClient->setServerPassword(cmd[1]);
}
