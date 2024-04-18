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

std::string	const	getCurrentTimeStamp(void)
{
	std::string retTime;
	std::time_t currentTime = std::time(NULL);
	std::tm *localTime = std::localtime(&currentTime);

	retTime = SSTR(localTime->tm_year + 1900) + "/" + SSTR(localTime->tm_mon) + "/" + SSTR(localTime->tm_mday) + " ";
	if (localTime->tm_mon < 10)
		retTime.insert(5, "0");
	if (localTime->tm_mday < 10)
		retTime.insert(8, "0");
	retTime = retTime + SSTR(localTime->tm_hour) + ":" + SSTR(localTime->tm_min) + ":" + SSTR(localTime->tm_sec);
	if (localTime->tm_hour < 10)
		retTime.insert(11, "0");
	if (localTime->tm_min < 10)
		retTime.insert(14, "0");
	if (localTime->tm_sec < 10)
		retTime.insert(17, "0");
	return retTime;
}