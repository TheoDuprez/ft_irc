#include <sys/socket.h>
#include "../includes/irc.hpp"

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string>    result;
    std::string                 token;
    std::istringstream          tokenSource(str);

    while (std::getline(tokenSource, token, delim)) {
        result.push_back(token);
    }
    return result;
}

void    sendMessage(int fd, std::string msg)
{
    msg += "\r\n";
    std::cout << "Message send to client is : " << msg << std::endl;
    send(fd, msg.c_str(), msg.size(), 0);
}

// bool    checkExcludedChar(std::string name) {
//     std::string excluded = "#$&: ";

//     if (name.find_first_of(excluded) != std::string::npos)
//         return false;
//     return true;
// }