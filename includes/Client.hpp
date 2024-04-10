/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/10 01:50:24 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		bool					_isRegister;
		bool					_isOperator;
		int						_cliendFd;
		std::string				_nickName;
		std::string				_userName;
		std::string				_realName;
		std::string				_serverPassword;
		// Client(const Client& obj);
		Client& operator=(const Client& obj);
	public:
		Client(int clientFd);
		Client(void);
		~Client(void);
		
        const int&	getClientFd(void) const;
		std::string	getNickName(void) const;
		std::string	getUserName(void) const;
		std::string	getRealName(void) const;
		std::string	getServerPassword(void) const;
		bool		getIsRegister(void) const;
		void		setIsRegister(bool reg);
		void		setServerPassword(std::string pswd);
		void		setnickName(std::string nickName);
		void		setuserName(std::string userName);
		void		setrealName(std::string realName);
		void		setNewActiveChannels(std::string realName);
};

#endif