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