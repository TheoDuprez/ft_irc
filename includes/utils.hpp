#include <iostream>
#include <vector>

std::vector<std::string>    split(const std::string& str, char delim);
void                        sendMessage(int fd, std::string msg);