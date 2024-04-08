/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/08 10:20:22 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class Client
{
	private:
		bool			_isRegister;
		bool			_isOperator;
		std::string		_nickName;
		std::string		_userName;
		std::string		_realName;
		std::string		_serverPassword;
		// Client(const Client& obj);
		Client& operator=(const Client& obj);
	public:
		Client(void);
		~Client(void);
		
		std::string	getNickName(void);
		std::string	getUserName(void);
		std::string	getRealName(void);
		std::string	getServerPassword(void);
		bool		getIsRegister(void);
		void		setIsRegister(bool reg);
		void		setServerPassword(std::string pswd);
		void		setnickName(std::string nickName);
		void		setuserName(std::string userName);
		void		setrealName(std::string realName);
};
