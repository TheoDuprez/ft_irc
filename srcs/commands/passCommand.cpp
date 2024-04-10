/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:09 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 21:48:00 by acarlott         ###   ########lyon.fr   */
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

    if (currentClient->getIsRegister() == true) {
        this->printLogMessage("ERR_ALREADYREGISTERED (462)\n", ERROR);
        return;
    }
    else if (cmd.size() != 2 || cmd[1].empty()) {
        this->printLogMessage("ERR_NEEDMOREPARAMS (461)\n", ERROR);
        return;
    }
    if (this->_password.compare(cmd[1].c_str())) {
        this->errorCommand(currentClient->getClientFd(), ":server 464 ", ":Password incorrect");
        throw (std::runtime_error(strerror(errno)));
    }
    currentClient->setServerPassword(cmd[1]);
    // --- Debug message ---
    // std::cout << "passcmd test: pswd set as: " << currentClient->getServerPassword() << std::endl;
    // ---------------------
}
