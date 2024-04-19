/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motdCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 03:01:03 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/19 11:55:33 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::motdCommand(Client const *currentClient) const
{
    if (currentClient->getIsRegister() == false)
        return ;
    sendMessage(currentClient->getClientFd(), RPL_MOTDSTART(currentClient->getNickName(), this->getServerName()));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), " /=====================================================\\"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(),  " ||  ______   ______      __     ______     ______    ||"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), " || \\  ___\\ /\\__  _\\    /\\ \\   /\\  == \\   /\\  ___\\    ||"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), " || \\ \\  __\\ \\/_/\\ \\    \\ \\ \\  \\ \\  __<   \\ \\ \\____   ||"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), " ||  \\ \\_\\      \\ \\_\\     \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\ ||"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), " ||   \\/_/       \\/_/      \\/_/   \\/_/ /_/   \\/_____/ ||"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), "\\=====================================================/"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), "          ! Welcome to ft_irc !"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), "  42 Lyon - Auvergne-Rhone-Alpes - France"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), "- designer: Tduprez - Hleung - Acarlott"));
    sendMessage(currentClient->getClientFd(), RPL_MOTD(currentClient->getNickName(), ("- SSL port: " + iToString(this->_port))));
    sendMessage(currentClient->getClientFd(), RPL_ENDOFMOTD(currentClient->getNickName()));
}
