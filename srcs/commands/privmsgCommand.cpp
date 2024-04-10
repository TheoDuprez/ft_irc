/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:03:06 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/10 13:03:07 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void                Server::privmsgCommand(commandTokensVector cmd, Client* client)
{
    if (cmd.at(1).find("#") != std::string::npos || cmd.at(1).find("&")) { // Is a channel target
        this->_channelsList.find(cmd.at(1))->second->privmsg(cmd, client);
    }
}