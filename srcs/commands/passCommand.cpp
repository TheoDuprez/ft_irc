/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:09 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/17 17:46:29 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
    Client	*currentClient = this->_clients[fd];

    if (cmd.size() != 2 || cmd[1].empty()) {
        sendMessage(currentClient->getClientFd(), ERR_NEEDMOREPARAMS(currentClient->getNickName(), "PASS"));
        return;
    }
    else if (currentClient->getIsRegister() == true) {
        //check here is an ambigitue with irc doc an antrochat use
        sendMessage(currentClient->getClientFd(), ERR_ALREADYREGISTERED(currentClient->getNickName()));
        return;
    }
    if (this->_password.compare(cmd[1].c_str())) {
        this->errorCommand(currentClient);
        throw (std::runtime_error(strerror(errno)));
    }
    currentClient->setServerPassword(cmd[1]);
}
