#include "../includes/Bot.hpp"

Bot::Bot(char* port, std::string password) : _nickname("GIGACHAT"), _channel("#gigachan"), _password(password)
{
	/* Check port number */
	long double		tempPort;
	
	for (size_t i = 0; port[i]; ++i) {
		if (!isdigit(port[i]))
			throw (std::runtime_error("Bad port"));
	}
	tempPort = std::strtold(port, NULL);
	if (tempPort > std::numeric_limits<unsigned short>::max() || tempPort <= 1024)
		throw (std::runtime_error("Bad port"));
	this->_port = atoi(port);

	/* Create client socket */
	int	clientSocket = socket(AF_INET, SOCK_STREAM, IP);
	if (clientSocket == -1)
		throw (std::runtime_error(strerror(errno)));

	/* Defining Server Address */
	memset(&this->_serverAddress, 0, sizeof(this->_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(this->_port);

	/* Create pollfd */
    this->_pollfd.events = POLLIN;
    this->_pollfd.fd = clientSocket;
} 

Bot::~Bot(void){}

void	Bot::serverConnection()
{
	if (connect(this->_pollfd.fd, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1) {
		throw (std::runtime_error(strerror(errno)));
	} 
	std::string message = "PASS " + this->_password + "\r\nNICK GIGACHAT\r\nUSER GIGACHAT 0 * :realname\r\nJOIN #gigachat\r\n";
    if (send(this->_pollfd.fd, message.c_str(), message.length(), 0) == -1)
		throw std::runtime_error(strerror(errno));
	std::cout << "bot connection done" << std::endl;
}

void	Bot::receiveMessage()
{
	while (true)
	{
		if (poll(&this->_pollfd, 1, POLL_NO_TIMEOUT) == -1) {
				throw (std::runtime_error(strerror(errno)));
		}
		else if (this->_pollfd.revents & POLLIN) {
			char	buffer[MESSAGE_SIZE] = {0};
			int		recvReturn = 0;
			recvReturn = recv(this->_pollfd.fd, &buffer, MESSAGE_SIZE, NO_FLAG);
			if (recvReturn == -1)
        		throw (std::runtime_error(strerror(errno)));
    		else if (recvReturn == 0) {
				close(this->_pollfd.fd);
				std::cout << "Connection to server is closed" << std::endl;
				break;
			}
			else {
				std::cout << "From gigachat" << std::endl;
			}
		}
	}
}