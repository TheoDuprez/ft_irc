#include "../includes/Bot.hpp"

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cout << "Usage : ./ircserv 'port' 'password'" << std::endl;
		return 1;
	}
	try {
		Bot bot(av[1], av[2]);

		bot.serverConnection();
		bot.receiveMessage();
	} catch (const std::runtime_error &e) {
		std::cout << "Error : " << e.what() << std::endl;
		return 1;
	}
}

