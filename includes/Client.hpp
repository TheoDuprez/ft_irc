/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/17 17:33:19 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <netinet/in.h> // For socket data struct
#include <errno.h>
#include <cstring> // For memset function


#define MESSAGE_SIZE 512
#define NO_FLAG 0

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
		std::string 				_tempBuffer;
		// Client(const Client& obj);
		Client& operator=(const Client& obj);
	public:
		Client(int clientFd);
		Client(void);
		~Client(void);
		
		void								clearTempBuffer(void);
		bool								isValidBuffer(const std::string tmpBuffer);
		std::string const&					receiveMessage(int fd);
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


class ContinueException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class BreakException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

#endif