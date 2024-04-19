/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lusersCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 01:37:48 by shellks           #+#    #+#             */
/*   Updated: 2024/04/19 11:42:37 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::lusersCommand(Client const *currentClient) const
{
    if (currentClient->getIsRegister() == false)
        return ;
    int         unknownConnectionNb = 0;
    std::string userNb = iToString(this->_clients.size());
    std::string channelNb = iToString(this->_channelsMap.size());

    for (clientMap::const_iterator clientIt = this->_clients.begin(); clientIt != this->_clients.end(); clientIt++) {
        if (clientIt->second->getIsRegister() == false)
            unknownConnectionNb++;
    }
    sendMessage(currentClient->getClientFd(), RPL_LUSERCLIENT(currentClient->getNickName(), userNb));
    sendMessage(currentClient->getClientFd(), RPL_LUSEROP(currentClient->getNickName()));
    sendMessage(currentClient->getClientFd(), RPL_LUSERUNKNOWN(currentClient->getNickName(), iToString(unknownConnectionNb)));
    sendMessage(currentClient->getClientFd(), RPL_LUSERCHANNELS(currentClient->getNickName(), channelNb));
    sendMessage(currentClient->getClientFd(), RPL_LUSERME(currentClient->getNickName(), userNb));
    sendMessage(currentClient->getClientFd(), RPL_LOCALUSERS(currentClient->getNickName(), userNb, iToString(this->_maxClientConnected)));
    sendMessage(currentClient->getClientFd(), RPL_GLOBALUSERS(currentClient->getNickName(), userNb, iToString(this->_maxClientConnected)));
}