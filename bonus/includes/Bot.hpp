#ifndef BOT_HPP
#define BOT_HPP
#include "../../includes/irc.hpp"

class Bot
{
    private:
        std::string     _nickname;
        std::string     _channel;
        std::string     _password;
        pollfd          _pollfd;
        sockaddr_in     _serverAddress;
        unsigned short	_port;
		static bool		_isBotUp;

    public:
        Bot(char* port, std::string password);
        ~Bot(void);
        void    					serverConnection(void);
        void    					receiveMessage(void);
		std::vector<std::string>	makeGigaChatVector();
		std::string 				createGigaChatSentence();
		void    					sendMessage(int fd, std::string msg);
		static void					stopBot(int signum);
};

#endif