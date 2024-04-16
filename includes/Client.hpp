/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shellks <shellks@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/16 23:23:29 by shellks          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <algorithm>

class Client
{
	private:
		bool						_isRegister;
		bool						_isOperator;
		int							_cliendFd;
		std::string					_nickName;
		std::string					_userName;
		std::string					_realName;
		std::string					_serverPassword;
		std::vector<Client *>		_contactedClients;
		// Client(const Client& obj);
		Client& operator=(const Client& obj);
	public:
		Client(int clientFd);
		Client(void);
		~Client(void);
		
        const int&          				getClientFd(void) const;
		const std::string&					getNickName(void) const;
		const std::string&					getUserName(void) const;
		const std::string&					getRealName(void) const;
		const std::string&					getServerPassword(void) const;
		const std::vector<Client*>&			getContactedClients(void) const;
		Client* 							getContactedClientByNick(const std::string& nickName) const;
		void								setContactedClient(Client* targetClient, bool setClient);
		bool								getIsRegister(void) const;
		void								setIsRegister(bool reg);
		void								setServerPassword(std::string pswd);
		void								setnickName(std::string nickName);
		void								setuserName(std::string userName);
		void								setrealName(std::string realName);
};

#endif