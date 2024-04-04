/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tduprez <tduprez@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:08:02 by tduprez           #+#    #+#             */
/*   Updated: 2024/04/04 13:11:55 by tduprez          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <poll.h>

class Client
{
private:
	pollfd	fd[1];
	Client(const Client& obj);
	Client& operator=(const Client& obj);
public:
	Client(void);
	~Client(void);
};

Client::Client(/* args */)
{
}

Client::~Client()
{
}
