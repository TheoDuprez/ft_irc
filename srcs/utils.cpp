#include <sys/socket.h>
#include "../includes/irc.hpp"

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

void    sendMessage(int fd, std::string msg)
{
    msg += "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

bool	isStringContainOnlyNumbers(const std::string& str)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (!isdigit(*it))
			return false;
	}
	return true;
}

size_t strtost(const std::string& str)
{
	std::istringstream iss(str);
	size_t retNumber;

	if (!isStringContainOnlyNumbers(str) || !(iss >> retNumber))
		throw std::runtime_error("Error converting std::string into size_t");

	return retNumber;
}

std::string	ullToString(size_t valueToConvert)
{
	std::string			convertValue;
	std::ostringstream	osStr;

	osStr << valueToConvert;
	convertValue = osStr.str();
	return (convertValue);
}

std::string iToString(int valueToConvert)
{
    std::string			convertedValue;
    std::ostringstream	osStr;

    osStr << valueToConvert;
    convertedValue = osStr.str();
    return (convertedValue);
}

// bool    checkExcludedChar(std::string name) {
//     std::string excluded = "#$&: ";

//     if (name.find_first_of(excluded) != std::string::npos)
//         return false;
//     return true;
// }