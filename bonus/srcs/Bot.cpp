#include "../includes/Bot.hpp"

std::vector<std::string> split(const std::string& str, char delim);

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
	std::string message = "PASS " + this->_password + "\r\nNICK GIGACHAT\r\nUSER GIGACHAT 0 * :realname\r\nJOIN #gigachan\r\n";
    if (send(this->_pollfd.fd, message.c_str(), message.length(), 0) == -1)
		throw std::runtime_error(strerror(errno));
	std::cout << "bot connection done" << std::endl;
}

void	Bot::receiveMessage()
{
	std::vector<std::string>	cmd;
	Bot::_isBotUp = true;

	std::signal(SIGINT, Bot::stopBot);
	while (this->_isBotUp)
	{
		if (poll(&this->_pollfd, 1, POLL_NO_TIMEOUT) == -1 && this->_isBotUp) {
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
				if (std::string(buffer).find("#gigachan") != std::string::npos) {
					if (std::string(buffer).find("!GIGACHAT") != std::string::npos)
						sendMessage(this->_pollfd.fd, "PRIVMSG " + this->_channel + " :" + createGigaChatSentence());
				}
				else if (std::string(buffer).find("!GIGACHAT") != std::string::npos) {
					cmd = split(std::string(buffer), ' ');
					sendMessage(this->_pollfd.fd, "PRIVMSG " + std::string(cmd[0].begin() + 1, cmd[0].end()) + " :" + createGigaChatSentence());
				}
			}
		}
	}
}

std::vector<std::string>	Bot::makeGigaChatVector()
{
	std::vector<std::string>	gigaChatVector;
	std::string 				line;
	std::ifstream				motivationFile("motivation.csv");

	if (!motivationFile.is_open()) {
		throw std::runtime_error("motivation.csv file cannot be open");
	}
	while (std::getline(motivationFile, line)) {
		gigaChatVector.push_back(line);
	}
	return gigaChatVector;
}

std::string Bot::createGigaChatSentence()
{
	std::vector<std::string>	gigaChat;
	int	randomSentence;
	srand(time(NULL));

	gigaChat = makeGigaChatVector();
	randomSentence = rand() % gigaChat.size();

	return gigaChat[randomSentence];
}

void    Bot::sendMessage(int fd, std::string msg)
{
	msg += "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

std::vector<std::string> split(const std::string& str, char delim)
{
	std::vector<std::string>    result;
	std::string                 token;
	std::istringstream          tokenSource(str);

	while (std::getline(tokenSource, token, delim)) {
		if (token[0] != 0)
			result.push_back(token);
	}
	return result;
}

void	Bot::stopBot(int signum)
{
	static_cast<void>(signum);
	Bot::_isBotUp = false;
}

bool	Bot::_isBotUp;