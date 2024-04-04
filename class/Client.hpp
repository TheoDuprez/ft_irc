/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/04 23:08:58 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <poll.h>

class Client
{
	private:
		unsigned int	_isRegister;
		bool			_isOperator;
		std::string		_nickName;
		std::string		_userName;
		pollfd&			_fd;
		Client(void);
		// Client(const Client& obj);
		Client& operator=(const Client& obj);
	public:
		Client(pollfd& fd);
		~Client(void);
};
