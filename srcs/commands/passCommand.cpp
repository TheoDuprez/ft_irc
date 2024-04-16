/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:09 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/14 11:51:06 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::passCommand(std::vector<std::string> cmd, int fd)
{
    // --- Debug message ---
    std::cout << " ----- Input of passCommand ----- " << std::endl;
    for (size_t i = 0; i < cmd.size(); i++)
        std::cout << cmd[i] << " ";
    std::cout << std::endl;
    // ---------------------
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
    // --- Debug message ---
    // std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
    // ---------------------
}
