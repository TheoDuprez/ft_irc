/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:43:23 by acarlott          #+#    #+#             */
/*   Updated: 2024/04/05 13:51:47 by acarlott         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	main(int ac, char **av)
{
	if (ac != 3) {
		std::cout << "Usage : ./ircserv 'port' 'password'" << std::endl;
		return 1;
	}
	try {
		Server server(av[1], av[2]);

		server.initServer();
		server.launchServer();
	} catch (const std::runtime_error &e) {
		std::cout << "Error : " << e.what() << std::endl;
		return 1;
	}
}
