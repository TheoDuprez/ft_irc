#ifndef BOT_HPP
#define BOT_HPP
#include "../includes/irc.hpp"

class Bot
{
    private:
        std::string     _nickname;
        std::string     _channel;
        std::string     _password;
        pollfd          _pollfd;
        sockaddr_in     _serverAddress;
        unsigned short	_port;

    public:
        Bot(char* port, std::string password);
        ~Bot(void);
        void    serverConnection(void);
        void    receiveMessage(void);
};

#endif