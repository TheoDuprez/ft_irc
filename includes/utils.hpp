#include <iostream>
#include <vector>

std::vector<std::string>    split(const std::string& str, char delim);
void                        sendMessage(int fd, std::string msg);
size_t						strtost(const std::string& str);

std::string					ullToString(size_t valueToConvert);
std::string                 iToString(int valueToConvert);
// bool                        checkExcludedChar(std::string name);